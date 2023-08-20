#include "motion_sensor.h"
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(motion_sensor, CONFIG_LOG_DEFAULT_LEVEL);

static const struct device *const device = DEVICE_DT_GET_ANY(pixart_pmw3360);

K_MUTEX_DEFINE(motion_mutex);

static motion_info_t motion_data;

bool is_pmw3360_ready(void) {
    return device_is_ready(device);
}

bool set_cpi(uint16_t cpi) {
    const struct sensor_value val = {
        .val1 = cpi,
        .val2 = 0,
    };

    int err = sensor_attr_set(device, SENSOR_CHAN_ALL, PMW3360_ATTR_CPI, &val);
    return err == 0;
}

uint16_t get_cpi(void) {
    struct sensor_value cpi;
    int err = sensor_sample_fetch(device);
    
    if (err) {
        LOG_ERR("Failed to fetch sensor sample: %d", err);
        return -1;
    }

    sensor_channel_get(device, PMW3360_ATTR_CPI, &cpi);
    LOG_DBG("Sensor CPI.val1: %u", cpi.val1);
    LOG_DBG("Sensor CPI.val2: %u", cpi.val2);
    return cpi.val1;
}

void set_sensor_trigger(void (*handler)(const struct device *dev, const struct sensor_trigger *trig)) {
    struct sensor_trigger trig = {
        .type = SENSOR_TRIG_DATA_READY,
        .chan = SENSOR_CHAN_ALL,
    };

    sensor_trigger_set(device, &trig, handler);
}

void enable_data_interrupt(void) {
    set_sensor_trigger(interrupt_handler);
}

void disable_data_interrupt(void) {
    set_sensor_trigger(NULL);
}

void interrupt_handler(const struct device *dev, const struct sensor_trigger *trig) {
    motion_info_t new_motion_data = read_motion();
    
    k_mutex_lock(&motion_mutex, K_FOREVER);
    motion_data.dx += new_motion_data.dx;
    motion_data.dy += new_motion_data.dy;
    k_mutex_unlock(&motion_mutex);
}

bool configure_pmw3360(void) {
    if (sensor_sample_fetch(device) == -EBUSY) {
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

    LOG_INF("CPI is now %u", cpi);
    LOG_INF("Enabling Interrupts");
    // enable_data_interrupt();
    return true;
}

motion_info_t read_motion(void) {
    struct sensor_value dx, dy;
    int err = sensor_sample_fetch(device);

    if (err) {
        LOG_ERR("Failed to fetch sensor sample: %d", err);
        return (motion_info_t){ .dx = 0, .dy = 0 };
    }

    sensor_channel_get(device, SENSOR_CHAN_POS_DX, &dx);
    sensor_channel_get(device, SENSOR_CHAN_POS_DY, &dy);

    // dy is negative due to sensor
    motion_info_t info;
    info.dx = dx.val1;
    info.dy = -dy.val1;
    return info;
}

void reset_current_motion_info(void) {
    k_mutex_lock(&motion_mutex, K_FOREVER);
    motion_data.dx = 0;
    motion_data.dy = 0;
    k_mutex_unlock(&motion_mutex);
}

motion_info_t get_current_motion_info(void) {
    k_mutex_lock(&motion_mutex, K_FOREVER);
    motion_info_t motion = motion_data;
    reset_current_motion_info();
    k_mutex_unlock(&motion_mutex);
    return motion;
}