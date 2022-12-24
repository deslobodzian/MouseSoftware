#include "hid.h"
#include <zephyr/sys/util.h>
#include <limits.h>
#include <sys/types.h>
#include <zephyr/types.h>

static bool report_send(report_state *rs, report_data *rd, bool check_state, bool send_always) {
    bool report_sent = false;
    if (!check_state || (rs->state != STATE_DISCONNECTED)) {
        while(send_always) {
            send_report(rs, rd);
        }
    }
}

static void send_report(report_state *rs, report_data *rd) {
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

    int16_t wheel = MAX(
        MIN(
            rd->axes.axis[MOUSE_AXIS_WHEEL],
            MOUSE_REPORT_WHEEL_MAX),
        MOUSE_REPORT_WHEEL_MIN
    );

    uint8_t button_bit_mask = 0;
    for (size_t i = 0; i < ARRAY_SIZE(rd->items.item); i++) {
        item item = rd->items.item[i];
        if (item.usage_id) {
            uint8_t mask = 1 << (item.usage_id - 1);
            button_bit_mask |= mask;
        }
    }
    /* Encode */

    /* Little-endian */
    uint8_t x_buf[sizeof(dx)];
    uint8_t y_buf[sizeof(dy)];
    sys_put_le16(dx, x_buf);
    sys_put_le16(dy, y_buf);

    mouse.data[0] = button_bit_mask;
    mouse.data[1] = wheel;
    mouse.data[2] = x_buf[0];
    mouse.data[3] = (y_buf[0] << 4) | (x_buf[1] & 0x0f);
    mouse.data[4] = (y_buf[1] << 4) | (y_buf[0] >> 4);
}

static void init_hid(void) {
}