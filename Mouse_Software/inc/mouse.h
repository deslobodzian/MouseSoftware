#ifndef MOUSE_MOUSE_H
#define MOUES_MOUSE_H

#include <zephyr/kernel.h>
#include "hw_config.h"

typedef struct {
    uint8_t message[NUM_MOUSE_BUTTONS + 2];
    uint8_t state[NUM_MOUSE_BUTTONS + 2]; // 2 for dx and dy
    struct gpio_callback callback[NUM_MOUSE_BUTTONS + 1]; // 1 for the pmw3360 interupt
} MOUSE_Data;

#endif // MOUSE_MOUSE_H