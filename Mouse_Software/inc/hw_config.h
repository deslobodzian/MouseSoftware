#ifndef MOUSE_HW_INFO_H
#define MOUSE_HW_INFO_H

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>

#define GPIO_SPEC(node_id) GPIO_DT_SPEC_GET_OR(node_id, gpios, {0})

#define HID_DEVICE_ID               "HID_0"
#define NUM_MOUSE_BUTTONS           5

#define MOUSE_BUTTON_REPORT_POS     0
#define MOUSE_X_REPORT_POS          1
#define MOUSE_Y_REPORT_POS          2

// Buttons
#define M1_NODE DT_ALIAS(m1)
#define M2_NODE DT_ALIAS(m2)
#define M3_NODE DT_ALIAS(m3)
#define M4_NODE DT_ALIAS(m4)
#define M5_NODE DT_ALIAS(m5)

static const uint8_t hid_report_desc[] = HID_MOUSE_REPORT_DESC(NUM_MOUSE_BUTTONS);

static const struct gpio_dt_spec mouse_one = GPIO_SPEC(M1_NODE),
    mouse_two = GPIO_SPEC(M2_NODE),
    mouse_three = GPIO_SPEC(M3_NODE),
    mouse_four = GPIO_SPEC(M4_NODE),
    mouse_five = GPIO_SPEC(M5_NODE);

#endif // MOUSE_HW_INFO_H