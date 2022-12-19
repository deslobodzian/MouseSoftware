#ifndef MOUSE_BUTTONS_H
#define MOUSE_BUTTONS_H

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/util.h>
#include <zephyr/sys/printk.h>
#include <zephyr/usb/usb_device.h>
#include <inttypes.h>
#include "structs.h"

#define SLEEP_TIME_MS 1

#define MOUSE_BUTTON_M1 BIT(0);
#define MOUSE_BUTTON_M2 BIT(1);
#define MOUSE_BUTTON_M3 BIT(2);
#define MOUSE_BUTTON_M4 BIT(3);
#define MOUSE_BUTTON_M5 BIT(4);

int init_buttons(void);

void button_pressed(const struct device *dev, struct gpio_callback *cb, uint32_t pins);

void configure_callback(const struct gpio_dt_spec *spec, gpio_callback_handler_t handler, struct gpio_callback *callback);
void m1_button(const struct device *dev, struct gpio_callback *cb, uint32_t pins);
void m2_button(const struct device *dev, struct gpio_callback *cb, uint32_t pins);
void m3_button(const struct device *dev, struct gpio_callback *cb, uint32_t pins);
void m4_button(const struct device *dev, struct gpio_callback *cb, uint32_t pins);
void m5_button(const struct device *dev, struct gpio_callback *cb, uint32_t pins);

void fetch_buttons(int *array_values);

const struct gpio_dt_spec get_button_from_idx(int idx);


#endif // MOUSE_BUTTONS_H