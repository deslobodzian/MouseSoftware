#include "wheel.h"
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(wheel, CONFIG_LOG_DEFAULT_LEVEL);

static const struct device *qdec_dev = DEVICE_DT_GET(DT_NODELABEL(qdec));

int init_wheel(void) {
    if (!device_is_ready(qdec_dev)) {
        LOG_ERR("QDEC not ready!");
        return -1;
    }
    return 0;
}

wheel_data_t fetch_wheel_data() {
    struct sensor_value value;

    int err = 0;
    err = sensor_sample_fetch(qdec_dev);
    err = sensor_channel_get(qdec_dev, SENSOR_CHAN_ROTATION, &value);

    int16_t wheel = value.val1;
    #ifdef WHEEL_DEBUG
    LOG_DBG("Wheel rot: %i", wheel);
    #endif
    wheel_data_t data;
    /* Negative as thats what USB expects */
    data.rotation = -wheel;

    return data;
}
