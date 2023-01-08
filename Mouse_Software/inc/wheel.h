#ifndef MOUSE_WHEEL_H
#define MOUSE_WHEEL_H

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>

typedef struct {
    int16_t rotation;
} wheel_data_t;

void init_wheel(void);

wheel_data_t fetch_wheel_data(void);
#endif // MOUSE_WHEEL_
