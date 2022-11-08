#include "pmw3360.h"
#include <logging/log.h>

LOG_MODULE_REGISTER(pmw3360_custom, CONFIG_LOG_DEFAULT_LEVEL);

const struct device *get_pmw3360_device(void) {
    
	const struct device *dev;
	dev = device_get_binding(DT_LABEL(DT_INST(0, pixart_pmw3360)));
	if (dev == NULL) {
		LOG_ERR("Could not get %s device\n", DT_LABEL(DT_INST(0, pixart_pmw3360)));
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

void init_pmw3360(const struct device *dev) {
    int rc = sensor_sample_fetch(dev);
    LOG_DBG("RC val: %d", rc);
    int err = 0;
	const struct sensor_value enable_rest = {
        .val1 = DISABLE_REST,
    };

	err = sensor_attr_set(dev, SENSOR_CHAN_ALL, PMW3360_ATTR_REST_ENABLE, &enable_rest);
    LOG_DBG("Error val: %d", err);

    set_cpi(dev, INIT_CPI);
}

int fetch_pmw3360_data(const struct device *dev) {
    int rc = sensor_sample_fetch(dev);
    return rc;
}

int get_dx(const struct device *dev) {
    struct sensor_value x;
	sensor_channel_get(dev, SENSOR_CHAN_POS_DX, &x);
    return x.val1;
}

int get_dy(const struct device *dev) {
    struct sensor_value y;
	sensor_channel_get(dev, SENSOR_CHAN_POS_DY, &y);
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
void set_cpi(const struct device *dev, int cpi) {
    const struct sensor_value val = {
        .val1 = cpi,
    };
    int rc = sensor_attr_set(dev, SENSOR_CHAN_ALL, PMW3360_ATTR_CPI, &val);
    if (rc == 0) {
        LOG_INF("Set CPI to: %d\n", val.val1);
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
int get_cpi(const struct device *dev) {
    struct sensor_value cpi;
    int rc = 0;
    rc = sensor_channel_get(dev, PMW3360_ATTR_CPI, &cpi);
    return cpi.val1;
}