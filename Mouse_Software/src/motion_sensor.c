#include "motion_sensor.h"
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(motion_sensor, CONFIG_LOG_DEFAULT_LEVEL);

static const struct device *const device = DEVICE_DT_GET_ANY(pixart_pmw3360);

bool is_sensor_ready(void) {
    return device_is_ready(device);
}

bool set_cpi(uint16_t cpi) {
    const struct sensor_value val = {
        .val1 = cpi,
        .val2 = 0,
    };

    int err = sensor_attr_set(device, SENSOR_CHAN_ALL, PMW3360_ATTR_CPI, &val);
    return err;
}

bool configure_sensor(void) {
    set_cpi(800);
}

motion_info_t read_motion(void) {
	uint32_t start;
	uint32_t stop;
	uint32_t dur;
	uint32_t nano;

    struct sensor_value dx;
    struct sensor_value dy;

	start = k_cycle_get_32();
    int err = sensor_sample_fetch(device);
	stop = k_cycle_get_32();
	dur = stop - start;
	nano = (uint32_t) k_cyc_to_ns_floor64(dur);
	// LOG_DBG("Sample took %i nano secs", nano);


    if (!err) {
        sensor_channel_get(device, SENSOR_CHAN_POS_DX, &dx);
        sensor_channel_get(device, SENSOR_CHAN_POS_DY, &dy);
    }

    motion_info_t info;
    info.dx = dx.val1;
    info.dy = dy.val1;
    return info;
}

