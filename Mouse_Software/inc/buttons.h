#ifndef MOUSE_BUTTONS_H
#define MOUSE_BUTTONS_H

#include <zephyr.h>
#include <device.h>
#include <drivers/gpio.h>
#include <sys/util.h>
#include <sys/printk.h>
#include <inttypes.h>

#define SLEEP_TIME_MS 1

#define M1_NODE DT_ALIAS(m1)
#define M2_NODE DT_ALIAS(m2)
#define M3_NODE DT_ALIAS(m3)
#define M4_NODE DT_ALIAS(m4)
#define M5_NODE DT_ALIAS(m5)

static const struct gpio_dt_spec buttons[5] = {
    GPIO_DT_SPEC_GET_OR(M1_NODE, gpios, {0}),
    GPIO_DT_SPEC_GET_OR(M2_NODE, gpios, {0}),
    GPIO_DT_SPEC_GET_OR(M3_NODE, gpios, {0}),
    GPIO_DT_SPEC_GET_OR(M4_NODE, gpios, {0}),
    GPIO_DT_SPEC_GET_OR(M5_NODE, gpios, {0}),
};

static struct gpio_callback cb_datas[5];

int init_buttons(void);

void button_pressed(const struct device *dev, struct gpio_callback *cb, uint32_t pins);

void fetch_buttons(int *array_values);

const struct gpio_dt_spec get_button_from_idx(int idx);


#endif // MOUSE_BUTTONS_H