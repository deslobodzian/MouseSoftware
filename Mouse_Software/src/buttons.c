#include "buttons.h"
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(buttons, CONFIG_LOG_DEFAULT_LEVEL);

void button_pressed(const struct device *dev, struct gpio_callback *cb, uint32_t pins) {
    // LOG_DBG("Button %d pressed at %" PRIu32 "", 0, k_cycle_get_32());
    // for (int idx = 0; idx < (sizeof(buttons)/sizeof(*buttons)); idx++) {
        // if (BIT(buttons[idx].pin) == pins) {
            // LOG_DBG("Button %d pressed at %" PRIu32 "", idx, k_cycle_get_32());
        // }
    // }
}


int init_buttons() {
    configure_callback(&mouse_one, &m1_button, &mouse.callback[0]);
    configure_callback(&mouse_two, &m2_button, &mouse.callback[1]);
    configure_callback(&mouse_three, &m3_button, &mouse.callback[2]);
    configure_callback(&mouse_four, &m4_button, &mouse.callback[3]);
    configure_callback(&mouse_five, &m5_button, &mouse.callback[4]);
}

void configure_callback(const struct gpio_dt_spec *spec, gpio_callback_handler_t handler, struct gpio_callback *callback) {
    const struct device *gpio = spec->port;
	gpio_pin_t pin = spec->pin;
	int ret;
	if (gpio == NULL) {
		/* Optional GPIO is missing. */
		// return 0;
	}
	if (!device_is_ready(gpio)) {
		LOG_ERR("GPIO port %s is not ready", gpio->name);
		// return -ENODEV;
	}
	ret = gpio_pin_configure_dt(spec, GPIO_INPUT);
	if (ret < 0) {
		LOG_ERR("Failed to configure port %s pin %u, error: %d",
			gpio->name, pin, ret);
		// return ret;
	}
	ret = gpio_pin_get(gpio, pin);
	if (ret < 0) {
		LOG_ERR("Failed to get the state of port %s pin %u, error: %d",
			gpio->name, pin, ret);
		// return ret;
	}

	gpio_init_callback(callback, m1_button, BIT(pin));
	ret = gpio_add_callback(gpio, callback);
	
	ret = gpio_pin_interrupt_configure_dt(spec, GPIO_INT_EDGE_BOTH);
	if (ret < 0) {
		LOG_ERR("Failed to configure interrupt for port %s pin %u, "
			"error: %d",
			gpio->name, pin, ret);
		// return ret;
	}

    LOG_DBG("Set up button at %s pin %d", gpio->name, pin);
	// return 0;
}
void m1_button(const struct device *dev, struct gpio_callback *cb, uint32_t pins) {
    int ret;
    uint8_t state = mouse.state[MOUSE_BUTTON_REPORT_POS];
    if (IS_ENABLED(CONFIG_USB_DEVICE_REMOTE_WAKEUP)) {
        if (usb.status == USB_DC_SUSPEND) {
            usb_wakeup_request();
        }
    }

    ret = gpio_pin_get(dev, mouse_one.pin);
    if (ret < 0) {
        LOG_ERR("Failed to get m1 button");
    }

    if (0 != (uint8_t)ret) {
        state |= MOUSE_BUTTON_M1;
    } else {
        state &= ~MOUSE_BUTTON_M1;
    }

    if (mouse.state[MOUSE_BUTTON_REPORT_POS] != state) {
        mouse.state[MOUSE_BUTTON_REPORT_POS] = state;
        k_sem_give(&sem);
    }
}

void m2_button(const struct device *dev, struct gpio_callback *cb, uint32_t pins) {
    int ret;
    uint8_t state = mouse.state[MOUSE_BUTTON_REPORT_POS];
    if (IS_ENABLED(CONFIG_USB_DEVICE_REMOTE_WAKEUP)) {
        if (usb.status == USB_DC_SUSPEND) {
            usb_wakeup_request();
        }
    }

    ret = gpio_pin_get(dev, mouse_two.pin);
    if (ret < 0) {
        LOG_ERR("Failed to get m2 button");
    }

    if (0 != (uint8_t)ret) {
        state |= MOUSE_BUTTON_M2;
    } else {
        state &= ~MOUSE_BUTTON_M2;
    }

    if (mouse.state[MOUSE_BUTTON_REPORT_POS] != state) {
        mouse.state[MOUSE_BUTTON_REPORT_POS] = state;
        k_sem_give(&sem);
    }
}

void m3_button(const struct device *dev, struct gpio_callback *cb, uint32_t pins) {
    int ret;
    uint8_t state = mouse.state[MOUSE_BUTTON_REPORT_POS];
    if (IS_ENABLED(CONFIG_USB_DEVICE_REMOTE_WAKEUP)) {
        if (usb.status == USB_DC_SUSPEND) {
            usb_wakeup_request();
        }
    }

    ret = gpio_pin_get(dev, mouse_three.pin);
    if (ret < 0) {
        LOG_ERR("Failed to get m3 button");
    }

    if (0 != (uint8_t)ret) {
        state |= MOUSE_BUTTON_M3;
    } else {
        state &= ~MOUSE_BUTTON_M3;
    }

    if (mouse.state[MOUSE_BUTTON_REPORT_POS] != state) {
        mouse.state[MOUSE_BUTTON_REPORT_POS] = state;
        k_sem_give(&sem);
    }
}

void m4_button(const struct device *dev, struct gpio_callback *cb, uint32_t pins) {
    int ret;
    uint8_t state = mouse.state[MOUSE_BUTTON_REPORT_POS];
    if (IS_ENABLED(CONFIG_USB_DEVICE_REMOTE_WAKEUP)) {
        if (usb.status == USB_DC_SUSPEND) {
            usb_wakeup_request();
        }
    }

    ret = gpio_pin_get(dev, mouse_four.pin);
    if (ret < 0) {
        LOG_ERR("Failed to get m4 button");
    }

    if (0 != (uint8_t)ret) {
        state |= MOUSE_BUTTON_M4;
    } else {
        state &= ~MOUSE_BUTTON_M4;
    }

    if (mouse.state[MOUSE_BUTTON_REPORT_POS] != state) {
        mouse.state[MOUSE_BUTTON_REPORT_POS] = state;
        k_sem_give(&sem);
    }
}

void m5_button(const struct device *dev, struct gpio_callback *cb, uint32_t pins) {
    int ret;
    uint8_t state = mouse.state[MOUSE_BUTTON_REPORT_POS];
    if (IS_ENABLED(CONFIG_USB_DEVICE_REMOTE_WAKEUP)) {
        if (usb.status == USB_DC_SUSPEND) {
            usb_wakeup_request();
        }
    }

    ret = gpio_pin_get(dev, mouse_five.pin);
    if (ret < 0) {
        LOG_ERR("Failed to get m5 button");
    }

    if (0 != (uint8_t)ret) {
        state |= MOUSE_BUTTON_M5;
    } else {
        state &= ~MOUSE_BUTTON_M5;
    }

    if (mouse.state[MOUSE_BUTTON_REPORT_POS] != state) {
        mouse.state[MOUSE_BUTTON_REPORT_POS] = state;
        k_sem_give(&sem);
    }
}


void fetch_buttons(int *array_values) {
    // array_values[0] = gpio_pin_get_dt(&(buttons[0])); // Mouse button 1
    // array_values[1] = gpio_pin_get_dt(&(buttons[1])); // Mouse button 2
    // array_values[2] = gpio_pin_get_dt(&(buttons[2])); // Mouse button 3
    // array_values[3] = gpio_pin_get_dt(&(buttons[3])); // Mouse button 4
    // array_values[4] = gpio_pin_get_dt(&(buttons[4])); // Mouse button 5
}

const struct gpio_dt_spec get_button_from_idx(int idx) {
    // return buttons[idx];
    return;
}