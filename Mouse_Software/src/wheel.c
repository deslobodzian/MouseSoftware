#include "wheel.h"
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(wheel, CONFIG_LOG_DEFAULT_LEVEL);

static const struct device *qdec_dev = DEVICE_DT_GET(DT_NODELABEL(qdec));

void init_wheel(void) {
    if (!device_is_ready(qdec_dev)) {
        LOG_ERR("QDEC not ready!");
    }
}

wheel_data_t fetch_wheel_data(void) {
    struct sensor_value value;

    int err = sensor_sample_fetch(qdec_dev);
    err = sensor_channel_get(qdec_dev, SENSOR_CHAN_ROTATION, &value);

    int16_t wheel = value.val1;
    LOG_DBG("Wheel rot: %i", wheel);
    wheel_data_t data;
    data.rotation = wheel;
    return data;
}
