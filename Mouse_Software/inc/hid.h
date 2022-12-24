#ifndef MOUSE_HID_H
#define MOUSE_HID_H

#include <zephyr/kernel.h>
#include "hw_config.h"
#include "hid_config.h"
#include "structs.h"
#include <zephyr/sys/slist.h>
#include <zephyr/sys/byteorder.h>


enum state {
    STATE_DISCONNECTED,
    STATE_CONNECTED_IDLE,
    STATE_CONNECTED_BUSY
};

typedef struct {
    uint16_t usage_id;
    int16_t value;
} item;

/* Struct keeping state of a single hid target */
typedef struct {
    item item[NUM_ITEMS];
} items;

typedef struct {
    int16_t axis[NUM_AXIS];
} axis_data;

typedef struct {
    items items;
    axis_data axes;
    // struct report state *linked_rs;
} report_data;

typedef struct {
    enum state state;
    uint8_t num;
    report_data *linked_rd;
    bool update_needed;
} report_state;

typedef struct {
    report_state state;
} hid_device;

typedef struct {
    report_data report_data;
    hid_device device;
} hid_state;

static bool report_send(report_state *rs, report_data *rd, bool check_state, bool send_always);
static void send_report(report_state *rs, report_data *rd);
static void init_hid(void);

#endif // MOUSE_HID_H