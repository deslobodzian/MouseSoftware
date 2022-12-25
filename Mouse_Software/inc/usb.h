#ifndef MOUSE_USB_H
#define MOUSE_USB_H

#include <zephyr/usb/usb_device.h>
#include <zephyr/usb/class/usb_hid.h>
#include <zephyr/sys/byteorder.h>
#include "hid_config.h"


typedef struct {
    const struct device *device;
    // uint32_t report_bit_map;
    // uint8_t protocol;
    enum usb_dc_status_code status;
    // bool enabled;
} USB_Data;

void status_callback(enum usb_dc_status_code status, const uint8_t *param);

int init_usb(USB_Data *data);

int hid_write(USB_Data *data, uint8_t msg);

// these are used for HID_OPS of Zephyr lib.
// static void report_sent(const struct device *dev, bool error);
// static void report_sent_callback(const struct device *dev);

// void send_report(void);

#endif // MOUSE_USB_H