#ifndef MOUSE_USB_H
#define MOUSE_USB_H

#include "hid_config.h"
#include "mouse.h"
#include <zephyr/kernel.h>
#include <zephyr/usb/usb_device.h>
#include <zephyr/usb/class/usb_hid.h>
    
#define MESSAGE_SIZE 6

// Structure to hold USB device information
typedef struct {
    const struct device *dev;
    enum usb_dc_status_code status;
} usb_device_t;

// Callback function for USB device status
void status_callback(enum usb_dc_status_code status, const uint8_t *param);

// Function to initialize the USB device
int init_usb(void);

// Function to write to the HID
int hid_write(const mouse_t* mouse);
bool usb_line_busy(void);

#endif // MOUSE_USB_H