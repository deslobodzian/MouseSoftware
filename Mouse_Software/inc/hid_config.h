#ifndef MOUSE_HID_CONFIG
#define MOUSE_HID_CONFIG

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>

/* report size of the mouse
    8 bits: button bit mask
    8 bits: wheel rotation
    12 bits: x movement
    12 bits: y movement
 */
#define REPORT_SIZE                     6 /* 1 byte id + 5 bytes data*/ 

#define USAGE_PAGE_MOUSE_XY             0x01
#define USAGE_PAGE_MOUSE_WHEEL          0x01
#define USAGE_PAGE_MOUSE_BUTTONS        0x09

#define MOUSE_REPORT_ID             0x01

#define HID_DEVICE_ID               "HID_0"
#define NUM_MOUSE_BUTTONS           5
#define NUM_AXIS                    3 /* X, Y, Mouse Wheel */
#define NUM_ITEMS                   5

#define MOUSE_REPORT_WHEEL_MAX      (0x7F)
#define MOUSE_REPORT_WHEEL_MIN      (-0x7F)
#define MOUSE_REPORT_XY_MAX         (0x07FF)
#define MOUSE_REPORT_XY_MIN         (-0x07FF)
#define MOUSE_REPORT_BUTTONS_NUM_MAX    8

enum {
	MOUSE_AXIS_X,
	MOUSE_AXIS_Y,
	MOUSE_AXIS_WHEEL,
	MOUSE_AXIS_NUM,
};

// for usage pages https://www.usb.org/sites/default/files/documents/hut1_12v2.pdf
#define REPORT_MAP                                              \
    /* Report: Mouse */                                         \
    0x09, 0x01, /* Usage (pointer) */                           \
    0xA1, 0x00, /* Collection (physical)*/                      \
                                                                \
    0x85, MOUSE_REPORT_ID, /* Report ID */                   \
                                                                \
    0x05, USAGE_PAGE_MOUSE_BUTTONS,                             \
    0x19, 0x01,       /* Usage Minimum (1) */                   \
    0x29, 0x08,       /* Usage Maximum (8) */                   \
    0x15, 0x00,       /* Logical Minimum (0) */                 \
    0x25, 0x01,       /* Logical Maximum (1) */                 \
    0x75, 0x01,       /* Report Size (1) */                     \
    0x95, MOUSE_REPORT_BUTTONS_NUM_MAX, /* Report Count */      \
    0x81, 0x02,       /* Input (data, variable, absolute) */    \
                                                                \
    0x05, USAGE_PAGE_MOUSE_WHEEL,                                \
    0x09, 0x38,       /* Usage (Wheel) */                       \
    0x15, 0x81,       /* Logical Minimum (-127) */              \
    0x25, 0x7F,       /* Logical Maximum (127) */               \
    0x75, 0x08,       /* Report Size (8) */                     \
    0x95, 0x01,       /* Report Count */                        \
    0x81, 0x06,       /* Input (data, variable, relative) */    \
                                                                \
    0x05, USAGE_PAGE_MOUSE_XY,                                   \
    0x09, 0x30,       /* Usage (X) */                           \
    0x09, 0x31,       /* Usage (Y) */                           \
    0x16, 0x01, 0xF8,  /* Logical Maximum (2047) */              \
    0x26, 0xFF, 0x07,  /* Logical Minimum (-2047) */             \
    0x75, 0x0C,       /* Report Size (12) */                    \
    0x95, 0x02,       /* Report Count (2) */                    \
    0x81, 0x06,       /* Input (data, variable, relative) */    \
                                                                \
    0xC0              /* End Collection (Physical) */


static const uint8_t hid_report_desc[] = {
    0x05, 0x01, /* Usage Page (Generic Desktop) */ 
    0x09, 0x02, /* Usage (Mouse) */
    0xA1, 0x01, /* Collection (Application)*/
    REPORT_MAP,
    0xC0,
};

#endif // MOUSE_HID_CONFIG