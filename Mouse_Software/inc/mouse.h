#ifndef MOUSE_H
#define MOUSE_H

#include "wheel.h"
#include "motion_sensor.h"
#include "buttons.h"

typedef struct {
    wheel_data_t wheel_data;
    motion_info_t motion_info;
    uint8_t button_states[5];
} mouse_t;

typedef struct {
    void *fifo_reserved;
    mouse_t data;
} message_t; 

int init_mouse();
void update_mouse(mouse_t* mouse);
void send_hid_message(void);


#endif