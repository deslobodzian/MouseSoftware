#include "buttons.h"
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(buttons, CONFIG_LOG_DEFAULT_LEVEL);
static struct k_sem button_sem;

static const struct gpio_dt_spec mouse_one = GPIO_SPEC(M1_NODE),
    mouse_two = GPIO_SPEC(M2_NODE),
    mouse_three = GPIO_SPEC(M3_NODE),
    mouse_four = GPIO_SPEC(M4_NODE),
    mouse_five = GPIO_SPEC(M5_NODE);


static uint8_t button_states;
static struct gpio_callback callbacks[5];

bool init_buttons(void) {
    k_sem_init(&button_sem, 1, 1);
    configure_callback(&mouse_one, button_callback, &callbacks[0]);
    configure_callback(&mouse_two, button_callback, &callbacks[1]);
    configure_callback(&mouse_three, button_callback, &callbacks[2]);
    configure_callback(&mouse_four, button_callback, &callbacks[3]);
    configure_callback(&mouse_five, button_callback, &callbacks[4]);
}

static uint8_t get_bit(uint32_t pin) {
    if (pin == BIT(mouse_one.pin)) {
        // LOG_DBG("Found mouse 1");
        return BIT(0);
    }
    if (pin == BIT(mouse_two.pin)) {
        // LOG_DBG("Found mouse 2");
        return BIT(1);
    }

    if (pin == BIT(mouse_three.pin)) {
        return BIT(2);
    }

    if (pin == BIT(mouse_four.pin)) {
        return BIT(3);
    }

    if (pin == BIT(mouse_five.pin)) {
        return BIT(4);
    }

    return -1;
}

void button_callback(const struct device *dev, struct gpio_callback *cb, uint32_t pins) {
    // static uint8_t prev_state = 0;
    // int count = 0;
    // uint8_t button_state;
    // k_sem_take(&button_sem, K_FOREVER);
    // uint8_t states = button_states;
    // // LOG_DBG("Pins val: %i", pins);
    // // while (count < DEBOUNCE_COUNT) {
    // //     button_state = gpio_pin_get(dev, pins);
    // //     if (button_state == prev_state) {
    // //         count++;
    // //     } else {
    // //         count = 0;
    // //         prev_state = button_state;
    // //     }
    // // }
    // uint8_t bit = get_bit(pins);
    // // LOG_DBG("Found bit %i", bit);
    // if (bit == -1) {
    //     LOG_ERR("Not a valid pin!");
    // }
    // // LOG_DBG("button state: %i", button_state);
    // if (0 == (uint8_t)button_state) {
    //     states |= bit;
    // } else {
    //     states &= ~bit;
    // }
    // // LOG_DBG("States: %i", states);
    // if (button_states != states) {
    //     button_states = states;
    // }
    // k_sem_give(&button_sem);
}

static void configure_callback(const struct gpio_dt_spec *spec, gpio_callback_handler_t handler, struct gpio_callback *cb) {
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

	gpio_init_callback(cb, button_callback, BIT(pin));
	ret = gpio_add_callback(gpio, cb);
	
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

void fetch_buttons(uint8_t* button_array) {
    button_array[0] = gpio_pin_get_dt(&mouse_one);
    button_array[1] = gpio_pin_get_dt(&mouse_two);
    button_array[2] = gpio_pin_get_dt(&mouse_three);
    button_array[3] = gpio_pin_get_dt(&mouse_four);
    button_array[4] = gpio_pin_get_dt(&mouse_five);
}

void get_state(uint8_t *states) {
    // k_sem_take(&button_sem, K_FOREVER);
    // *states = button_states;
    // k_sem_give(&button_sem);
}
