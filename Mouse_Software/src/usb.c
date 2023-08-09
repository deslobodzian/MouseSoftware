#include "usb.h"
#include <zephyr/sys/byteorder.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(usb, CONFIG_LOG_DEFAULT_LEVEL);

static usb_device_t usb_device;
static uint8_t message_buffer[MESSAGE_SIZE];
static bool transmission_busy = false;

void int_in_ready_cb(const struct device *dev) {
    ARG_UNUSED(dev);
    transmission_busy = false;
}

void status_callback(enum usb_dc_status_code status, const uint8_t *param) {
    usb_device.status = status;
}

static const struct hid_ops ops = {
    .int_in_ready = int_in_ready_cb
};

int init_usb(void) {
    usb_device.dev = device_get_binding(HID_DEVICE_ID); 
    if (!usb_device.dev) {
        LOG_ERR("Failed to get USB device binding");
        return -ENODEV;
    }

    usb_hid_register_device(
        usb_device.dev,
        hid_report_desc,
        sizeof(hid_report_desc),
        &ops
    );
    
    int err = usb_hid_init(usb_device.dev);
    if (err) {
        LOG_ERR("Failed to initialize USB HID");
        return err;
    }

    return usb_enable(status_callback);
}

int hid_write(const mouse_t* mouse) {
    uint8_t button_bm = 0;
    for (size_t i = 0; i < 5; i++) {
        if (mouse->button_states[i]) {
            button_bm |= (1 << i);
        }
    }

    uint8_t x_buf[sizeof(mouse->motion_info.dx)];
    uint8_t y_buf[sizeof(mouse->motion_info.dy)];

    sys_put_le16(mouse->motion_info.dx, x_buf);
    sys_put_le16(mouse->motion_info.dy, y_buf);

    message_buffer[0] = 0x01;
    message_buffer[1] = button_bm;
    message_buffer[2] = mouse->wheel_data.rotation;
    message_buffer[3] = x_buf[0];
    message_buffer[4] = (y_buf[0] << 4) | (x_buf[1] & 0x0f);
    message_buffer[5] = (y_buf[1] << 4) | (y_buf[0] >> 4);

    int err = hid_int_ep_write(usb_device.dev, message_buffer, MESSAGE_SIZE, NULL);
    if (err == 0) {
        transmission_busy = true;
    }
    return err;
}

bool line_busy(void) {
    return transmission_busy;
}
