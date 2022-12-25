#include "usb.h"
#include "structs.h"
#include <zephyr/logging/log.h>
#include <zephyr/usb/usb_device.h>

#include "hw_config.h"

LOG_MODULE_REGISTER(usb, CONFIG_LOG_DEFAULT_LEVEL);

void status_callback(enum usb_dc_status_code status, const uint8_t *param) {
    usb.status = status;
}

int init_usb(USB_Data *data) {
    // static const struct hid_ops hid_ops = {
	// 	.get_report = get_report,
	// 	.set_report = set_report,
	// 	.int_in_ready = report_sent_cb,
	// 	.protocol_change = protocol_change,
	// };

    data->device = device_get_binding(HID_DEVICE_ID);
    if (data->device == NULL) {
        LOG_ERR("HID Device null");
    }
    usb_hid_register_device(
        data->device,
        hid_report_desc,
        sizeof(hid_report_desc),
        NULL
    );
    usb_hid_init(data->device);
    if (usb_enable(status_callback) != 0) {
        LOG_ERR("USB failed to start");
        return -1;
    }
    return 0;
    // return usb_enable(status_callback);
}

int hid_write(USB_Data *data, uint8_t msg) {
    int ret = 0;
    ret = hid_int_ep_write(data->device, mouse.data, 6, NULL);
    return ret;
}

// static void report_sent(const struct device *dev, bool error) {
// }

// static void report_sent_callback(const struct device *dev) {
//     report_sent(dev, false);
// }

// void send_report() {
//     const uint8_t *report_buffer = mouse.data;
//     int err = hid_int_ep_write(
//         usb.device,
//         report_buffer,
//         sizeof(report_buffer),
//         NULL
//     );

//     if (err) {
//         LOG_ERR("Cannot send report (&d)", err);
//     }
// }