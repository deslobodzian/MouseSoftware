#ifndef MOUSE_MOTION_SENSOR_H
#define MOUSE_MOTION_SENSOR_H

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include "pmw3360.h"
// #include <zephyr/drivers/sensor.h>
// #include <sensor/pmw3360.h>

typedef struct {
    int16_t dx;
    int16_t dy;
} motion_info_t;

int init_pmw3360_sensor(void);
bool is_motion_sensor_ready(void);

bool configure_pmw3360(void);

motion_info_t read_motion(void);
void reset_current_motion_info(void);
motion_info_t get_current_motion_info(void);

bool set_cpi(uint16_t cpi);
uint16_t get_cpi(void);

#endif //MOUSE_MOTION_SENSOR_H