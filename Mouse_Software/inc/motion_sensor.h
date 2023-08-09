#ifndef MOUSE_MOTION_SENSOR_H
#define MOUSE_MOTION_SENSOR_H

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <sensor/pmw3360.h>

typedef struct {
    int16_t dx;
    int16_t dy;
} motion_info_t;

void enable_data_interrupt(void);

void disable_data_interrupt(void);

void interrupt_handler(const struct device *dev, const struct sensor_trigger *trig);

bool is_pmw3360_ready(void);

bool configure_pmw3360(void);

motion_info_t read_motion(void);
void reset_current_motion_info(void);
motion_info_t get_current_motion_info(void);

bool set_cpi(uint16_t cpi);
uint16_t get_cpi(void);

#endif //MOUSE_MOTION_SENSOR_H