#include "buttons.h"
#include <logging/log.h>

LOG_MODULE_REGISTER(buttons, CONFIG_LOG_DEFAULT_LEVEL);

void button_pressed(const struct device *dev, struct gpio_callback *cb, uint32_t pins) {
    // LOG_DBG("Button %d pressed at %" PRIu32 "", 0, k_cycle_get_32());
    // for (int idx = 0; idx < (sizeof(buttons)/sizeof(*buttons)); idx++) {
        // if (BIT(buttons[idx].pin) == pins) {
            // LOG_DBG("Button %d pressed at %" PRIu32 "", idx, k_cycle_get_32());
        // }
    // }
}

int init_buttons(void) {
    int ret;

    for (int idx = 0; idx < (sizeof(buttons)/sizeof(*buttons)); idx++) {
        if (!device_is_ready(buttons[idx].port)) {
            LOG_ERR("Button device %s is not ready", buttons[idx].port->name);
            return -1;
        }
        ret = gpio_pin_configure_dt(&buttons[idx], GPIO_INPUT);
        if (ret != 0) {
            LOG_ERR("Error %d: failed to configure %s pin %d", ret, buttons[idx].port->name, buttons[idx].pin);
            return;
        }
        ret = gpio_pin_interrupt_configure_dt(&buttons[idx], GPIO_INT_EDGE_TO_ACTIVE);
        if (ret != 0) {
            LOG_ERR("Error %d: failed to configure interrupt on %s pin %d", ret, buttons[idx].port->name, buttons[idx].pin);
            return;
        }
        gpio_init_callback(&cb_datas[idx], button_pressed, BIT(buttons[idx].pin));
        gpio_add_callback(buttons[idx].port, &cb_datas[idx]);
        LOG_DBG("Set up button at %s pin %d", buttons[idx].port->name, buttons[idx].pin);
    }
}

void fetch_buttons(int *array_values) {
    array_values[0] = gpio_pin_get_dt(&(buttons[0])); // Mouse button 1
    array_values[1] = gpio_pin_get_dt(&(buttons[1])); // Mouse button 2
    array_values[2] = gpio_pin_get_dt(&(buttons[2])); // Mouse button 3
    array_values[3] = gpio_pin_get_dt(&(buttons[3])); // Mouse button 4
    array_values[4] = gpio_pin_get_dt(&(buttons[4])); // Mouse button 5
}

const struct gpio_dt_spec get_button_from_idx(int idx) {
    return buttons[idx];
}