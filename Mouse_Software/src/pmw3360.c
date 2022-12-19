#include "pmw3360.h"
#include <zephyr/logging/log.h>
#include "structs.h"

LOG_MODULE_REGISTER(pmw3360_custom, CONFIG_LOG_DEFAULT_LEVEL);

const struct device *get_pmw3360_device(void) {
	const struct device *const dev = DEVICE_DT_GET_ANY(pixart_pmw3360); 
	if (dev == NULL) {
		LOG_ERR("Could not get %s device\n", DEVICE_DT_GET_ANY(pixart_pmw3360));
		return NULL;
	}
	
	if (!device_is_ready(dev)) {
		LOG_ERR("\nError: Device \"%s\" is not ready; "
		       "check the driver initialization logs for errors.\n",
		       dev->name);
		return NULL;
	}
	LOG_INF("Found device \"%s\", getting sensor data\n", dev->name);
	return dev;
}

void trigger_handler(const struct device *pmw3360, const struct sensor_trigger *trig) {
    int rc;
    rc = fetch_pmw3360_data(pmw3360);
    mouse.state[MOUSE_X_REPORT_POS] = get_dx(pmw3360);
    mouse.state[MOUSE_Y_REPORT_POS] = get_dy(pmw3360);
    // LOG_DBG("Pos x: %d, y: %d", get_dx(pmw3360), get_dy(pmw3360));
    k_sem_give(&sem);
}

void init_pmw3360(PMW3360_Data *pmw3360) {
    // set up pmw3360 device;
    pmw3360->device = get_pmw3360_device();
    // set up sensor trigger
    struct sensor_trigger trig;
    trig.type = SENSOR_TRIG_DATA_READY; 
    trig.chan = SENSOR_CHAN_ALL; 

    pmw3360->trigger = &trig;

    // wait for setup to finish.
	// k_sleep(K_MSEC(500));
    
    // init other wanted values.
    int rc = sensor_sample_fetch(pmw3360->device);
    LOG_DBG("Failed to fetch -> RC val: %d", rc);
    int err = 0;
	const struct sensor_value enable_rest = {
        .val1 = DISABLE_REST,
    };

	err = sensor_attr_set(pmw3360->device, SENSOR_CHAN_ALL, PMW3360_ATTR_REST_ENABLE, &enable_rest);
    LOG_DBG("Error val: %d", err);

    set_cpi(pmw3360->device, INIT_CPI);
    // LOG_DBG("Setting up sensor trigger");
    rc = sensor_trigger_set(pmw3360->device, pmw3360->trigger, trigger_handler);
    if (rc != 0) {
        LOG_ERR("Trigger set failed");
    }
}

int fetch_pmw3360_data(const struct device *pmw3360) {
    int rc = sensor_sample_fetch(pmw3360);
    return rc;
}

int get_dx(const struct device *pmw3360) {
    struct sensor_value x;
	sensor_channel_get(pmw3360, SENSOR_CHAN_POS_DX, &x);
    // pmw3360->dx = x.val1;
    return x.val1;
}

int get_dy(const struct device *pmw3360) {
    struct sensor_value y;
	sensor_channel_get(pmw3360, SENSOR_CHAN_POS_DY, &y);
    // pmw3360->dy = y.val1;
    return y.val1;
}

/**
 * @brief Set the cpi object
 * Minimum = 100
 * Maximum = 12000
 * Increments of 100
 * @param dev 
 * @param cpi 
 */
void set_cpi(const struct device *pmw3360, int cpi) {
    const struct sensor_value val = {
        .val1 = cpi,
    };
    int rc = sensor_attr_set(pmw3360, SENSOR_CHAN_ALL, PMW3360_ATTR_CPI, &val);
    if (rc == 0) {
        LOG_INF("Set CPI to: %d\n", val.val1);
        // pmw3360->cpi = val.val1;
    } else {
        LOG_INF("Error setting CPI, error %d\n", rc);
    }
}

/**
 * @brief Get the cpi object
 * 
 * @param dev 
 * @return int 
 */
int get_cpi(const struct device *pmw3360) {
    struct sensor_value cpi;
    int rc = 0;
    rc = sensor_attr_get(pmw3360, SENSOR_CHAN_ALL, PMW3360_ATTR_CPI, &cpi);
    LOG_INF("CPI rc = %d\n", rc);
    return cpi.val1;
}