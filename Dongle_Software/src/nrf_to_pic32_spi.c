#include "nrf_to_pic32_spi.h"
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(nrf_to_pic32, CONFIG_LOG_DEFAULT_LEVEL);

static struct spi_dt_spec spi_com = SPI_DT_SPEC_GET(SPI_DEV, SPI_OP, 0);
static struct gpio_dt_spec cs_pin = SPI_CS_GPIOS_DT_SPEC_GET(SPI_DEV);

K_FIFO_DEFINE(spi_fifo);
K_THREAD_STACK_DEFINE(spi_stack_area, 1024); // Adjust stack size as needed
K_MEM_SLAB_DEFINE(spi_message_slab, sizeof(message_fifo_t), MAX_MESSAGES, 4);

static struct k_thread spi_thread_data;

void spi_thread(void *arg1, void *arg2, void *arg3) {
    for (;;) {
        message_fifo_t *msg = k_fifo_get(&spi_fifo, K_FOREVER);
        if (msg) {
            LOG_INF("SENDING SPI");
            pic32_spi_write(&msg->message);
            LOG_INF("SPI SENT");
            k_mem_slab_free(&spi_message_slab, (void **)&msg);
        }
    }
}

void init_pic32_spi(void) {
    while(!spi_is_ready_dt(&spi_com)) {
        LOG_ERR("SPI NOT READY");
    }
    LOG_INF("SPI is ready");
    LOG_INF("Starting SPI thread");

    k_tid_t tid = k_thread_create(
        &spi_thread_data,
        spi_stack_area,
        K_THREAD_STACK_SIZEOF(spi_stack_area), 
        spi_thread,
        NULL, NULL, NULL,
        K_PRIO_PREEMPT(0), 0, K_NO_WAIT
    );

    if (!tid) {
        LOG_ERR("Failed to spi start thread");
        return;
    }

    LOG_INF("SPI Thread Started");
} 

void pic32_cs_ctrl(bool enable) {
    if (gpio_pin_set_dt(&cs_pin, (int) enable)) {
        LOG_ERR("SPI CS Failed");
    }
}

void pic32_enable_cs(void) {
    pic32_cs_ctrl(true);
}

void pic32_disable_cs(void) {
    pic32_cs_ctrl(false);
}
void pic32_spi_write(const message_t *msg) {
    struct spi_buf tx_buf = {
        .buf = msg->data,
        .len = msg->len
    };
    const struct spi_buf_set tx_bufs = {
        .buffers = &tx_buf,
        .count = 1
    };
    // pic32_enable_cs();
    if (spi_write_dt(&spi_com, &tx_bufs) != 0) {
        LOG_ERR("Failed to write message");
    }
    // pic32_disable_cs();
    LOG_INF("SPI Success");
}

void pic32_spi_read(uint8_t *data) {
    const struct spi_buf rx_buf =  {
        .buf = data,
        .len = sizeof(data),
    };
    pic32_enable_cs();
    if (!spi_read_dt(&spi_com, &rx_buf)) {
        LOG_ERR("Failed to write message");
    }
    pic32_disable_cs();
}

void add_message_to_fifo(const message_t *msg) {
    message_fifo_t *msg_fifo;
    if (k_mem_slab_alloc(&spi_message_slab, (void **)&msg_fifo, K_NO_WAIT) == 0) {
        memcpy(&msg_fifo->message, msg, sizeof(message_t));
        k_fifo_put(&spi_fifo, msg_fifo);
        LOG_INF("Creating message");
    } else { 
        LOG_ERR("Failed to allocate memory for message");
    }
}
void print_message_content(const message_t *msg) {
    LOG_INF("Message content (length: %zu):", msg->len);
    for (size_t i = 0; i < msg->len; i++) {
        LOG_INF("%02x ", msg->data[i]);
    }
}