#include "pmw3360.h"
#include <zephyr/logging/log.h>
#include <zephyr/sys/byteorder.h>
#include "hw_config.h"

#define PMW3360_CPI                         800

#define PMW3360_RUN_DOWNSHIFT_TIME_MS       500     /* ms */
#define PMW3360_REST1_DOWNSHIFT_TIME_MS     9220    /* ms */
#define PMW3360_REST2_DOWNSHIFT_TIME_MS     150000  /* ms */

LOG_MODULE_REGISTER(pmw3360, 4);//CONFIG_LOG_DEFAULT_LEVEL);

static pmw3360_data_t data;
static pmw3360_config_t cfg;
static const nrfx_spim_t spim_instance = NRFX_SPIM_INSTANCE(SPI_INSTANCE);
static const struct gpio_dt_spec irq_gpio = GPIO_SPEC(PMW3360_IRQ_NODE);

static const struct gpio_dt_spec cs_gpio = GPIO_DT_SPEC_GET(SPI_NODE, cs_gpios);

static const int32_t async_init_delay[ASYNC_INIT_STEP_COUNT] = {
	[ASYNC_INIT_STEP_POWER_UP]         = 1,
	[ASYNC_INIT_STEP_FW_LOAD_START]    = 50,
	[ASYNC_INIT_STEP_FW_LOAD_CONTINUE] = 10,
	[ASYNC_INIT_STEP_FW_LOAD_VERIFY]   = 1,
	[ASYNC_INIT_STEP_CONFIGURE]        = 0,
};

static int pmw3360_async_init_power_up();
static int pmw3360_async_init_configure();
static int pmw3360_async_init_fw_load_verify();
static int pmw3360_async_init_fw_load_continue();
static int pmw3360_async_init_fw_load_start();

static int (* const async_init_fn[ASYNC_INIT_STEP_COUNT])() = {
	[ASYNC_INIT_STEP_POWER_UP] = pmw3360_async_init_power_up,
	[ASYNC_INIT_STEP_FW_LOAD_START] = pmw3360_async_init_fw_load_start,
	[ASYNC_INIT_STEP_FW_LOAD_CONTINUE] = pmw3360_async_init_fw_load_continue,
	[ASYNC_INIT_STEP_FW_LOAD_VERIFY] = pmw3360_async_init_fw_load_verify,
	[ASYNC_INIT_STEP_CONFIGURE] = pmw3360_async_init_configure,
};

static nrfx_spim_config_t spim_config = NRFX_SPIM_DEFAULT_CONFIG(
	SPI_SCK,
	SPI_MOSI,
	SPI_MISO,
    NRF_SPIM_PIN_NOT_CONNECTED
);

static int spi_cs_ctrl(bool enable) {
	int err;

	if (!enable) {
		k_busy_wait(T_NCS_SCLK);
	}

	err = gpio_pin_set_dt(&cs_gpio, (int)enable);
	if (err) {
		LOG_ERR("SPI CS ctrl failed");
	}

	if (enable) {
		k_busy_wait(T_NCS_SCLK);
	}

	return err;
}

static int reg_read(uint8_t reg, uint8_t *buf) {
    nrfx_err_t err;
    __ASSERT_NO_MSG((reg & SPI_WRITE_BIT) == 0);

    err = spi_cs_ctrl(true);
    if (err) {
        return err;
    }

    nrfx_spim_xfer_desc_t xfer_desc_tx = NRFX_SPIM_XFER_TX(&reg, 1);

    err = nrfx_spim_xfer(&spim_instance, &xfer_desc_tx, 0);
    if (err != NRFX_SUCCESS) {
        LOG_ERR("Reg read failed on SPI write");
        return err;
    }

    k_busy_wait(T_SRAD);

    nrfx_spim_xfer_desc_t xfer_desc_rx = NRFX_SPIM_XFER_RX(buf, 1);
    // Read the received data
    err = nrfx_spim_xfer(&spim_instance, &xfer_desc_rx, 0);
    if (err != NRFX_SUCCESS) {
        LOG_ERR("Reg read failed on SPI read");
        return err;
    }

    err = spi_cs_ctrl(false);
    if (err) {
        return err;
    }

    k_busy_wait(T_SRX);

    data.last_read_burst = false;

    return 0;
}

static int reg_write(uint8_t reg, uint8_t val) {
    nrfx_err_t err;
    uint8_t tx_data[] = {
        SPI_WRITE_BIT | reg,
        val
    };

    err = spi_cs_ctrl(true);
    if (err) {
        return err;
    }

    nrfx_spim_xfer_desc_t xfer_desc = NRFX_SPIM_XFER_TX(tx_data, sizeof(tx_data));

    err = nrfx_spim_xfer(&spim_instance, &xfer_desc, 0);
    if (err != NRFX_SUCCESS) {
        LOG_ERR("Reg write failed on SPI write");
        spi_cs_ctrl(false); 
        return err;
    }

    k_busy_wait(T_SCLK_NCS_WR);

    err = spi_cs_ctrl(false); 
    if (err) {
        return err;
    }

    k_busy_wait(T_SWX);

    data.last_read_burst = false;
    return 0;
}

static int motion_burst_read(uint8_t *buf, size_t burst_size) {
    nrfx_err_t err;
    uint8_t reg_buf = PMW3360_REG_MOTION_BURST;

    __ASSERT_NO_MSG(burst_size <= PMW3360_MAX_BURST_SIZE);

    if (!data.last_read_burst) {
        err = reg_write(PMW3360_REG_MOTION_BURST, 0x00);
        if (err) {
            return err;
        }
    }

    err = spi_cs_ctrl(true); 
    if (err) {
        return err;
    }

    nrfx_spim_xfer_desc_t xfer_desc_tx = NRFX_SPIM_XFER_TX(&reg_buf, 1);

    data.last_read_burst = true;

    err = nrfx_spim_xfer(&spim_instance, &xfer_desc_tx, 0);
    if (err != NRFX_SUCCESS) {
        LOG_ERR("Motion burst failed on SPI write");
        return err;
    }

    k_busy_wait(T_SRAD_MOTBR);

    nrfx_spim_xfer_desc_t xfer_desc_rx = NRFX_SPIM_XFER_RX(buf, burst_size);

    err = nrfx_spim_xfer(&spim_instance, &xfer_desc_rx, 0);
    if (err != NRFX_SUCCESS) {
        LOG_ERR("Motion burst failed on SPI read");
        return err;
    }

    err = spi_cs_ctrl(false); 
    if (err) {
        return err;
    }

    k_busy_wait(T_BEXIT);
    data.last_read_burst = true;
    return 0;
}

static int burst_write(uint8_t reg, const uint8_t *buf, size_t size) {
    nrfx_err_t err;
    uint8_t write_buf;

    write_buf = reg | SPI_WRITE_BIT;

    nrfx_spim_xfer_desc_t xfer_desc_tx = NRFX_SPIM_XFER_TX(&write_buf, 1);

    err = spi_cs_ctrl(true); 
    if (err) {
        return err;
    }

    err = nrfx_spim_xfer(&spim_instance, &xfer_desc_tx, 0);
    if (err != NRFX_SUCCESS) {
        LOG_ERR("Burst write failed on SPI write");
        spi_cs_ctrl(false); 
        return err;
    }

    for (size_t i = 0; i < size; i++) {
        write_buf = buf[i];
        xfer_desc_tx.p_tx_buffer = &write_buf;

        err = nrfx_spim_xfer(&spim_instance, &xfer_desc_tx, 0);
        if (err != NRFX_SUCCESS) {
            LOG_ERR("Burst write failed on SPI write (data)");
            // spi_cs_ctrl(false); 
            return err;
        }

        k_busy_wait(T_BRSEP);
    }

    err = spi_cs_ctrl(false);
    if (err) {
        return err;
    }

    k_busy_wait(T_BEXIT);

    data.last_read_burst = false;

    return 0;
}

static int update_cpi(uint32_t cpi) {
    if ((cpi > PMW3360_MAX_CPI) || (cpi < PMW3360_MIN_CPI)) {
        LOG_ERR("CPI value %u out of range", cpi);
        return -EINVAL;
    }

    // Convert CPI to register value
    uint8_t value = (cpi / 100) - 1;

    LOG_DBG("Setting CPI to %u (reg value 0x%x)", cpi, value);

    uint8_t reg = PMW3360_REG_CONFIG1 | SPI_WRITE_BIT;
    uint8_t tx_data[] = {reg, value};

    nrfx_spim_xfer_desc_t xfer_desc = NRFX_SPIM_XFER_TX(tx_data, sizeof(tx_data));

    nrfx_err_t err = spi_cs_ctrl(true); 
    if (err) {
        return err;
    }

    err = nrfx_spim_xfer(&spim_instance, &xfer_desc, 0);
    if (err != NRFX_SUCCESS) {
        LOG_ERR("Failed to change CPI");
        spi_cs_ctrl(false); 
        return err;
    }

    err = spi_cs_ctrl(false); 
    return err;
}

static int read_cpi(uint32_t *cpi_value) {
    uint8_t reg = PMW3360_REG_CONFIG1 & ~SPI_WRITE_BIT; 
    uint8_t value;

    int err = reg_read(reg, &value);
    if (err) {
        LOG_ERR("Failed to read CPI register");
        return err;
    }

    // Convert register value to CPI
    *cpi_value = (value + 1) * 100;

    LOG_DBG("Read CPI value: %u", *cpi_value);

    return 0;
}


static int update_downshift_time(uint8_t reg_addr, uint32_t time) {
    uint32_t maxtime;
    uint32_t mintime;

    switch (reg_addr) {
	case PMW3360_REG_RUN_DOWNSHIFT:
		/*
		 * Run downshift time = PMW3360_REG_RUN_DOWNSHIFT * 10 ms
		 */
		maxtime = 2550;
		mintime = 10;
		break;

	case PMW3360_REG_REST1_DOWNSHIFT:
		/*
		 * Rest1 downshift time = PMW3360_REG_RUN_DOWNSHIFT
		 *                        * 320 * Rest1 rate (default 1 ms)
		 */
		maxtime = 81600;
		mintime = 320;
		break;

	case PMW3360_REG_REST2_DOWNSHIFT:
		/*
		 * Rest2 downshift time = PMW3360_REG_REST2_DOWNSHIFT
		 *                        * 32 * Rest2 rate (default 100 ms)
		 */
		maxtime = 816000;
		mintime = 3200;
		break;

	default:
		LOG_ERR("Not supported");
		return -ENOTSUP;
	}

    if ((time > maxtime) || (time < mintime)) {
        LOG_WRN("Downshift time %u out of range", time);
        return -EINVAL;
    }

    __ASSERT_NO_MSG((mintime > 0) && (maxtime/mintime <= UINT8_MAX));

    // Convert time to register value
    uint8_t value = time / mintime;

    LOG_DBG("Set downshift time to %u ms (reg value 0x%x)", time, value);

    // Write the value to the specified register
    uint8_t reg = reg_addr | SPI_WRITE_BIT; // Include write bit
    uint8_t tx_data[] = {reg, value};

    nrfx_spim_xfer_desc_t xfer_desc = NRFX_SPIM_XFER_TX(tx_data, sizeof(tx_data));

    nrfx_err_t err = spi_cs_ctrl(true); // Enable CS
    if (err) {
        return err;
    }

    err = nrfx_spim_xfer(&spim_instance, &xfer_desc, 0);
    if (err != NRFX_SUCCESS) {
        LOG_ERR("Failed to change downshift time");
        spi_cs_ctrl(false); // Disable CS on error
        return err;
    }

    err = spi_cs_ctrl(false); // Disable CS
    return err;
}

static int update_sample_time(uint8_t reg_addr_lower,
                              uint8_t reg_addr_upper,
                              uint32_t sample_time) {
    uint32_t maxtime = 0x9B0;
    uint32_t mintime = 1;

    if ((sample_time > maxtime) || (sample_time < mintime)) {
        LOG_WRN("Sample time %u out of range", sample_time);
        return -EINVAL;
    }

    LOG_DBG("Set sample time to %u ms", sample_time);

    /* The sample time is (reg_value + 1) ms. */
    sample_time--;
    uint8_t buf[2];
    sys_put_le16((uint16_t)sample_time, buf);

    nrfx_err_t err = spi_cs_ctrl(true);
    if (err) {
        return err;
    }

    // Write lower register value
    uint8_t tx_data_lower[] = {reg_addr_lower | SPI_WRITE_BIT, buf[0]};
    nrfx_spim_xfer_desc_t xfer_desc_lower = NRFX_SPIM_XFER_TX(tx_data_lower, sizeof(tx_data_lower));
    err = nrfx_spim_xfer(&spim_instance, &xfer_desc_lower, 0);
    if (err != NRFX_SUCCESS) {
        LOG_ERR("Failed to change sample time");
        spi_cs_ctrl(false); 
        return err;
    }

    // Write upper register value
    uint8_t tx_data_upper[] = {reg_addr_upper | SPI_WRITE_BIT, buf[1]};
    nrfx_spim_xfer_desc_t xfer_desc_upper = NRFX_SPIM_XFER_TX(tx_data_upper, sizeof(tx_data_upper));
    err = nrfx_spim_xfer(&spim_instance, &xfer_desc_upper, 0);
    if (err != NRFX_SUCCESS) {
        LOG_ERR("Failed to change sample time");
        spi_cs_ctrl(false);
        return err;
    }

    err = spi_cs_ctrl(false);
    return err;
}

static int toggle_rest_modes(uint8_t reg_addr, bool enable) {
    uint8_t value;
    nrfx_err_t err;

    err = spi_cs_ctrl(true);
    if (err) {
        return err;
    }

    // Read register value
    uint8_t tx_data_read = reg_addr & ~SPI_WRITE_BIT; // Ensure write bit is clear
    nrfx_spim_xfer_desc_t xfer_desc_read = NRFX_SPIM_XFER_TRX(&tx_data_read, 1, &value, 1);
    err = nrfx_spim_xfer(&spim_instance, &xfer_desc_read, 0);
    if (err != NRFX_SUCCESS) {
        LOG_ERR("Failed to read register");
        spi_cs_ctrl(false); 
        return err;
    }

    // Modify register value
    WRITE_BIT(value, PMW3360_REST_EN_POS, enable);
    LOG_DBG("%sable rest modes", (enable) ? ("En") : ("Dis"));

    // Write modified value
    uint8_t tx_data_write[] = {reg_addr | SPI_WRITE_BIT, value};
    nrfx_spim_xfer_desc_t xfer_desc_write = NRFX_SPIM_XFER_TX(tx_data_write, sizeof(tx_data_write));
    err = nrfx_spim_xfer(&spim_instance, &xfer_desc_write, 0);
    if (err != NRFX_SUCCESS) {
        LOG_ERR("Failed to set rest mode");
        spi_cs_ctrl(false); 
        return err;
    }

    err = spi_cs_ctrl(false);
    return err;
}

static int pmw3360_async_init_fw_load_start(void) {
    int err = 0;

    for (uint8_t reg = 0x02; (reg <= 0x06) && !err; reg++) {
        uint8_t buf[1];
        err = reg_read(reg, buf);
    }

    if (err) {
        LOG_ERR("Cannot read from data registers");
        return err;
    }

    err = reg_write(PMW3360_REG_CONFIG2, 0x00);
    if (err) {
        LOG_ERR("Cannot disable REST mode");
        return err;
    }

    err = reg_write(PMW3360_REG_SROM_ENABLE, 0x1D);
    if (err) {
        LOG_ERR("Cannot initialize SROM");
        return err;
    }

    return err;
}

static int pmw3360_async_init_fw_load_continue(void)
{
    int err;

    LOG_INF("Uploading optical sensor firmware...");

    err = reg_write(PMW3360_REG_SROM_ENABLE, 0x18);
    if (err) {
        LOG_ERR("Cannot start SROM download");
        return err;
    }

    err = burst_write(PMW3360_REG_SROM_LOAD_BURST,
                      pmw3360_firmware_data, pmw3360_firmware_length);
    if (err) {
        LOG_ERR("Cannot write firmware to sensor");
    }

    return err;
}

static int pmw3360_async_init_fw_load_verify()
{
	int err;

	/* Read the SROM_ID register to verify the firmware ID before any
	 * other register reads or writes
	 */

	uint8_t fw_id;
	err = reg_read(PMW3360_REG_SROM_ID, &fw_id);
	if (err) {
		LOG_ERR("Cannot obtain firmware id");
		return err;
	}

	LOG_DBG("Optical chip firmware ID: 0x%x", fw_id);
	if (fw_id != PMW3360_FIRMWARE_ID) {
		LOG_ERR("Chip is not running from SROM!");
		return -EIO;
	}

	uint8_t product_id;
	err = reg_read(PMW3360_REG_PRODUCT_ID, &product_id);
	if (err) {
		LOG_ERR("Cannot obtain product id");
		return err;
	}

	if (product_id != PMW3360_PRODUCT_ID) {
		LOG_ERR("Invalid product id!");
		return -EIO;
	}

	/* Write 0x20 to Config2 register for wireless mouse design.
	 * This enables entering rest modes.
	 */
	err = reg_write(PMW3360_REG_CONFIG2, 0x20);
	if (err) {
		LOG_ERR("Cannot enable REST modes");
	}

	return err;
}

/* 
    I'm not using interrupt but look into using the nrfx interrupt 
*/

// static void irq_handler(const struct device *gpiob, struct gpio_callback *cb,
// 			uint32_t pins)
// {
// 	int err;
// 	pmw3360_data_t *data = CONTAINER_OF(cb, pmw3360_data_t, irq_gpio_cb);

// 	err = gpio_pin_interrupt_configure_dt(&irq_gpio, GPIO_INT_DISABLE);
// 	if (unlikely(err)) {
// 		LOG_ERR("Cannot disable IRQ");
// 		k_panic();
// 	}

// 	k_work_submit(&data->trigger_handler_work);
// }

// static void trigger_handler(struct k_work *work) {
// 	sensor_trigger_handler_t handler;
// 	int err = 0;
// 	pmw3360_data_t *data = CONTAINER_OF(work, pmw3360_data_t, trigger_handler_work);
// 	k_spinlock_key_t key = k_spin_lock(&data->lock);

// 	handler = data->data_ready_handler;
// 	k_spin_unlock(&data->lock, key);

// 	if (!handler) {
// 		return;
// 	}

// 	struct sensor_trigger trig = {
// 		.type = SENSOR_TRIG_DATA_READY,
// 		.chan = SENSOR_CHAN_ALL,
// 	};

// 	handler(dev, &trig);

// 	key = k_spin_lock(&data->lock);
// 	if (data->data_ready_handler) {
// 		err = gpio_pin_interrupt_configure_dt(&config->irq_gpio,
// 						      GPIO_INT_LEVEL_ACTIVE);
// 	}
// 	k_spin_unlock(&data->lock, key);

// 	if (unlikely(err)) {
// 		LOG_ERR("Cannot re-enable IRQ");
// 		k_panic();
// 	}
// }
static int pmw3360_async_init_power_up()
{
	/* Reset sensor */
    LOG_INF("Power Up");
	return reg_write(PMW3360_REG_POWER_UP_RESET, 0x5A);
}

static int pmw3360_async_init_configure() {
	int err;
    LOG_INF("Configuring");

	err = update_cpi(PMW3360_CPI);

	if (!err) {
		err = update_downshift_time(
					    PMW3360_REG_RUN_DOWNSHIFT,
					    PMW3360_RUN_DOWNSHIFT_TIME_MS);
	}

	if (!err) {
		err = update_downshift_time(
					    PMW3360_REG_REST1_DOWNSHIFT,
					    PMW3360_REST1_DOWNSHIFT_TIME_MS);
	}

	if (!err) {
		err = update_downshift_time(
					    PMW3360_REG_REST2_DOWNSHIFT,
					    PMW3360_REST2_DOWNSHIFT_TIME_MS);
	}

	return err;
}
static void pmw3360_async_init(struct k_work *work) {
	pmw3360_data_t *data = CONTAINER_OF(work, pmw3360_data_t, init_work);

	LOG_DBG("PMW3360 async init step %d", data->async_init_step);

	data->err = async_init_fn[data->async_init_step]();
	if (data->err) {
		LOG_ERR("PMW3360 initialization failed");
	} else {
		data->async_init_step++;

		if (data->async_init_step == ASYNC_INIT_STEP_COUNT) {
			data->ready = true;
			LOG_INF("PMW3360 initialized");
		} else {
			k_work_schedule(&data->init_work,
					K_MSEC(async_init_delay[
						data->async_init_step]));
		}
	}
}
// static int pmw3360_init_irq() {
// 	int err;

// 	if (!device_is_ready(irq_gpio.port)) {
// 		LOG_ERR("IRQ GPIO device not ready");
// 		return -ENODEV;
// 	}

// 	err = gpio_pin_configure_dt(&irq_gpio, GPIO_INPUT);
// 	if (err) {
// 		LOG_ERR("Cannot configure IRQ GPIO");
// 		return err;
// 	}

// 	gpio_init_callback(&data.irq_gpio_cb, irq_handler,
// 			   BIT(irq_gpio.pin));

// 	err = gpio_add_callback(irq_gpio.port, &data.irq_gpio_cb);
// 	if (err) {
// 		LOG_ERR("Cannot add IRQ GPIO callback");
// 	}

// 	return err;
// }

bool is_pmw3360_ready(void) {
    return data.ready;
}

int pmw3360_init(void) {
    data.ready = false;
	int err;

    nrfx_err_t spi_err;
    spim_config.ss_active_high = false;
    spim_config.frequency = SPI_FREQUENCY;
    spim_config.mode = NRF_SPIM_MODE_3;
    spim_config.bit_order = NRF_SPIM_BIT_ORDER_MSB_FIRST;

    // spi_err = nrfx_spim_init(&spim_instance, &spim_config, spim_event_handler, NULL);
    spi_err = nrfx_spim_init(&spim_instance, &spim_config, NULL, NULL);
    if (spi_err != NRFX_SUCCESS) {
        return -1;
    }
    // config data
    if (!device_is_ready(cs_gpio.port)) {
		LOG_ERR("SPI CS device not ready");
		return -ENODEV;
	}

	err = gpio_pin_configure_dt(&cs_gpio, GPIO_OUTPUT_INACTIVE);
	if (err) {
		LOG_ERR("Cannot configure SPI CS GPIO");
		return err;
	}
    data.last_read_burst = false;
    // err = pmw3360_async_init_power_up();
    // k_sleep(K_USEC(1));
    // pmw3360_async_init_fw_load_start();
    // k_sleep(K_USEC(50));
    // pmw3360_async_init_fw_load_continue();
    // k_sleep(K_USEC(10));
    // pmw3360_async_init_fw_load_verify();
    // k_sleep(K_USEC(1));
    // pmw3360_async_init_configure();
	// // k_work_init(&data.trigger_handler_work, trigger_handler);
    // data.ready = true;

    // data.ready = true;
	k_work_init_delayable(&data.init_work, pmw3360_async_init);

	k_work_schedule(
        &data.init_work,
		K_MSEC(async_init_delay[data.async_init_step])
    );

	return err;
}

int pmw3360_fetch(void) {
	uint8_t buf[PMW3360_BURST_SIZE];

	if (unlikely(!data.ready)) {
		LOG_DBG("Device is not initialized yet");
		return -EBUSY;
	}

	int err = motion_burst_read(buf, sizeof(buf));
	
	int16_t x = sys_get_le16(&buf[PMW3360_DX_POS]);
	int16_t y = sys_get_le16(&buf[PMW3360_DY_POS]);

	data.x = -x;
	data.y = y;

	return err;
}

int set_pmw3360_cpi(uint32_t cpi) {
    cfg.cpi = cpi;
    return update_cpi(cpi);
}

uint32_t get_pmw3360_cpi(void) {
    int err;
    err = read_cpi(&cfg.cpi);
    if (err != 0) {
        LOG_ERR("Failed to read CPI");
        cfg.cpi = -1;
    }
    return cfg.cpi;
}

int16_t get_pmw3360_dx(void) {
    return data.x;
}

int16_t get_pmw3360_dy(void) {
    return data.y;
}

int update_rest1_sample_time(uint32_t time) {
    int err;
    err = update_sample_time(
		PMW3360_REG_REST1_RATE_LOWER,
		PMW3360_REG_REST1_RATE_UPPER,
		time
    );
    return err;
}
int update_rest2_sample_time(uint32_t time) {
    int err;
    err = update_sample_time(
		PMW3360_REG_REST2_RATE_LOWER,
		PMW3360_REG_REST2_RATE_UPPER,
		time
    );
    return err;
}
int update_rest3_sample_time(uint32_t time) {
    int err;
    err = update_sample_time(
		PMW3360_REG_REST3_RATE_LOWER,
		PMW3360_REG_REST3_RATE_UPPER,
		time
    );
    return err;
}

int pmw3360_enable_rest(bool rest) {
    int err;
    err = toggle_rest_modes(
        PMW3360_REG_CONFIG2,
        rest
    );
    return err;
}
