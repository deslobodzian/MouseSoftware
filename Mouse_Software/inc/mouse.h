#ifndef MOUSE_MOUSE_H
#define MOUES_MOUSE_H

#include <zephyr/kernel.h>
#include "hw_config.h"

typedef struct {
    int16_t dx;
    int16_t dy;
} motion_info;

typedef struct {
    int16_t val;
} wheel_info;

typedef struct {
    motion_info motion;
    uint8_t data[6];
    uint8_t state[5 + 2]; // 2 for dx and dy
    struct gpio_callback callback[5 + 1]; // 1 for the pmw3360 interupt
} MOUSE_Data;



void init_mouse(void);

#endif // MOUSE_MOUSE_H