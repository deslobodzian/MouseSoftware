#include "hid.h"
#include <zephyr/sys/util.h>
#include <limits.h>
#include <sys/types.h>
#include <zephyr/types.h>
#include "structs.h"

LOG_MODULE_REGISTER(hid, CONFIG_LOG_DEFAULT_LEVEL);

report_data_t *get_report_data(hid_device_t* hid_device) {
    return &hid_device->hid_state.report_data;
}

void init_hid(hid_device_t* hid_device) {
}

// static bool report_send(report_state *rs, report_data *rd, bool check_state, bool send_always) {
//     bool report_sent = false;
//     if (!check_state || (rs->state != STATE_DISCONNECTED)) {
//         while(send_always) {
//             send_report(rs, rd);
//         }
//     }
// }

static void send_report(report_data_t *rd) {
    int16_t dx = MAX(
        MIN(
            rd->axes.axis[MOUSE_AXIS_X],
            MOUSE_REPORT_XY_MAX),
        MOUSE_REPORT_XY_MIN
    );
    int16_t dy = MAX(
        MIN(
            rd->axes.axis[MOUSE_AXIS_Y],
            MOUSE_REPORT_XY_MAX),
        MOUSE_REPORT_XY_MIN
    );

    // int16_t wheel = MAX(
    //     MIN(
    //         rd->axes.axis[MOUSE_AXIS_WHEEL],
    //         MOUSE_REPORT_WHEEL_MAX),
    //     MOUSE_REPORT_WHEEL_MIN
    // );

    // uint8_t button_bit_mask = 0;
    // for (size_t i = 0; i < ARRAY_SIZE(rd->items.item); i++) {
    //     item item = rd->items.item[i];
    //     if (item.usage_id) {
    //         uint8_t mask = 1 << (item.usage_id - 1);
    //         button_bit_mask |= mask;
    //     }
    // }
    /* Encode */

    /* Little-endian */
    uint8_t x_buf[sizeof(dx)];
    uint8_t y_buf[sizeof(dy)];
    sys_put_le16(dx, x_buf);
    sys_put_le16(dy, y_buf);
    // uint8_t buffer[6];

    hid_event_t hid_event;
    hid_event.type = 0x01;

    hid_event.message[0] = 0x01;
    hid_event.message[1] = 0x0; //button_bit_mask;
    hid_event.message[2] = 0x0; // wheel;
    hid_event.message[3] = x_buf[0];
    hid_event.message[4] = (y_buf[0] << 4) | (x_buf[1] & 0x0f);
    hid_event.message[5] = (y_buf[1] << 4) | (y_buf[0] >> 4);

    event_t event = create_hid_event(&hid_event);
    enqueue_event(&manager.event_queue, event);
    k_sem_give(&manager.event_sem);
}


bool handle_motion_event(motion_event_t* motion_event) {
    report_data_t* report_data = get_report_data(&hid_device);
    LOG_DBG("Motion event{dx: %i, dy: %i}", motion_event->dx, motion_event->dy);
    report_data->axes.axis[0] = motion_event->dx;
    report_data->axes.axis[1] = motion_event->dy;
    send_report(report_data);
    return true;
}