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

bool is_sensor_ready(void);

bool configure_sensor(void);

motion_info_t read_motion(void);

bool set_cpi(uint16_t cpi);




#endif //MOUSE_MOTION_SENSOR_H
