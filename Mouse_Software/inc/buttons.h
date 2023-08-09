#ifndef MOUSE_BUTTONS_H
#define MOUSE_BUTTONS_H

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>

#include "hw_config.h"

#define DEBOUNCE_COUNT 5

bool init_buttons(void);

int button_debounce(void);

static void configure_callback(const struct gpio_dt_spec *spec, gpio_callback_handler_t handler, struct gpio_callback *cb);

void button_callback(const struct device *dev, struct gpio_callback *cb, uint32_t pins);

void fetch_buttons(uint8_t* button_array);
void get_state(uint8_t *states);

#endif // MOUSE_BUTTONS_H