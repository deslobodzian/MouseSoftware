#ifndef MOUSE_EVENT_TYPES_H
#define MOUSE_EVENT_TYPES_H

#include <zephyr/kernel.h>
#include "hid_config.h"

typedef enum {
    MOTION_EVENT,
    BUTTON_EVENT,
    WHEEL_EVENT,
    HID_EVENT,
    INVALID_EVENT
} event_type_t;

typedef struct {
    event_type_t type;
    void *data;
    size_t data_size;
} event_t;

typedef struct {
    event_type_t type;
    int16_t dx;
    int16_t dy;
} motion_event_t;


typedef struct {
    event_type_t type;
    uint8_t button_num;
} button_event_t;

typedef struct {
    event_type_t type;
    int16_t motion;
} wheel_event_t;

typedef struct {
    event_type_t type;
    uint8_t message[REPORT_SIZE];
}  hid_event_t;


#endif //MOUSE_EVENT_TYPES_H