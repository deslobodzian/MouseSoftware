#include "usb.h"
#include <zephyr/sys/byteorder.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(usb, CONFIG_LOG_DEFAULT_LEVEL);
static usb_device_t usb_device;

void status_callback(enum usb_dc_status_code status, const uint8_t *param) {
    usb_device.status = status;
}

int init_usb(void) {
    usb_device.dev = device_get_binding(HID_DEVICE_ID); 
    if (usb_device.dev == NULL) {
        return -1;
    }

    usb_hid_register_device(
        usb_device.dev,
        hid_report_desc,
        sizeof(hid_report_desc),
        NULL
    );

    usb_hid_init(usb_device.dev);

    return usb_enable(status_callback);
}

int hid_write(uint8_t* button_states, int16_t wheel, int16_t dx, int16_t dy) {
    uint8_t button_bm = 0;
	for (size_t i = 0; i < 5; i++) {
        if (button_states[i]) {
		    uint8_t mask = 1 << (i);
		    button_bm |= mask;
        }
	}
    uint8_t x_buf[sizeof(dx)];
    uint8_t y_buf[sizeof(dy)];
    sys_put_le16(dx, x_buf);
    sys_put_le16(dy, y_buf);
	uint8_t message[6];
	message[0] = 0x01;
    message[1] = button_bm; //button_bit_mask;
    message[2] = wheel; // wheel;
    message[3] = x_buf[0];
    message[4] = (y_buf[0] << 4) | (x_buf[1] & 0x0f);
    message[5] = (y_buf[1] << 4) | (y_buf[0] >> 4);
    return hid_int_ep_write(usb_device.dev, message, 6, NULL);
}