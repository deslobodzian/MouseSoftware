#ifndef MOUSE_USB_H
#define MOUSE_USB_H

#include "hid_config.h"
#include <zephyr/kernel.h>
#include <zephyr/usb/usb_device.h>
#include <zephyr/usb/class/usb_hid.h>

typedef struct {
    const struct device *dev;
    enum usb_dc_status_code status;
} usb_device_t;

void status_callback(enum usb_dc_status_code status, const uint8_t *param);

int init_usb(void);

int hid_write(uint8_t* button_states, int16_t wheel, int16_t dx, int16_t dy);

#endif // MOUSE_USB_H