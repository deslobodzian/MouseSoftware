#ifndef MOUSE_HID_H
#define MOUSE_HID_H

#include <zephyr/kernel.h>
#include "hw_config.h"
#include "hid_config.h"
#include "event_types.h"
#include "event_manager.h"
#include <zephyr/sys/slist.h>
#include <zephyr/sys/byteorder.h>


typedef enum {
    HID_DISCONNECTED,
    HID_CONNECTED_IDLE,
    HID_CONNECTED_BUSY
} enum_state_t;

typedef struct {
    uint16_t usage_id;
    int16_t value;
} item_t;

/* Struct keeping state of a single hid target */
typedef struct {
    item_t item[NUM_ITEMS];
    size_t item_size;
} items_t;

typedef struct {
    int16_t axis[NUM_AXIS];
} axis_data_t;

typedef struct {
    items_t items;
    axis_data_t axes;
    // struct report state *linked_rs;
} report_data_t;

typedef struct {
    enum_state_t state;
    uint8_t num;
    // report_data_t *linked_rd;
    bool update_needed;
} report_state_t;

typedef struct {
    report_data_t report_data;
    // hid_device_t device;
} hid_state_t;

typedef struct {
    hid_state_t hid_state;
    report_state_t report_state;
} hid_device_t;




void init_hid(hid_device_t* hid_device);

report_data_t *get_report_data(hid_device_t* hid_device);
// static bool report_send(report_state_t *rs, report_data_t *rd, bool check_state, bool send_always);
static void send_report(report_data_t *rd);

bool handle_motion_event(motion_event_t* motion_event);

bool handle_button_event(button_event_t* button_event);

bool handle_wheel_event(wheel_event_t* wheel_event);

#endif // MOUSE_HID_H