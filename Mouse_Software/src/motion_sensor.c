#include "motion_sensor.h"
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(motion_sensor, CONFIG_LOG_DEFAULT_LEVEL);

// static const struct device *const device = DEVICE_DT_GET_ANY(pixart_pmw3360);

static motion_info_t motion_data;

int init_pmw3360_sensor(void) {
    int err;
    err = pmw3360_init();
    return err;
}
bool is_motion_sensor_ready(void) {
    return is_pmw3360_ready();
}

bool set_cpi(uint16_t cpi) {
    return set_pmw3360_cpi(cpi);
}

uint16_t get_cpi(void) {
    return get_pmw3360_cpi();
}

bool configure_pmw3360(void) {
    if (!is_motion_sensor_ready()) {
        return false;
    }

    if (!set_cpi(800)) {
        LOG_ERR("Failed to set CPI");
        return false;
    }

    uint16_t cpi = get_cpi();
    if (cpi == -1) {
        LOG_ERR("Failed to get CPI");
        return false;
    }

    return true;
}

motion_info_t read_motion(void) {
    pmw3360_fetch();
    motion_info_t info;
    info.dx = get_pmw3360_dx(); //dx.val1;
    info.dy = -get_pmw3360_dy(); //-dy.val1;
    return info;
}

void reset_current_motion_info(void) {
    motion_data.dx = 0;
    motion_data.dy = 0;
}

motion_info_t get_current_motion_info(void) {
    motion_info_t motion = motion_data;
    reset_current_motion_info();
    return motion;
}