#ifndef MOUSE_USB_H
#define MOUSE_USB_H

#include <zephyr/usb/usb_device.h>
#include <zephyr/usb/class/usb_hid.h>


typedef struct {
    const struct device *device;
    enum usb_dc_status_code status;
} USB_Data;

void status_callback(enum usb_dc_status_code status, const uint8_t *param);
int init_usb(USB_Data *data);

int hid_write(USB_Data *data);

#endif // MOUSE_USB_H