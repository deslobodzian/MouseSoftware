#ifndef MOUSE_FSM_H
#define MOUSE_FSM_H

#include <zephyr/kernel.h>
// #include "buttons.h"

// States will be defined a byte to save space

#define INIT_MODE           0
#define SEARCHING_MODE      1
#define USB_MODE            2
#define WIRELESS_MODE       3

typedef struct {
    uint8_t state;
    uint8_t prev_state;

    uint8_t message[5];
} FSM_Data;

void run_fsm(FSM_Data *fsm_data);

void enter_state(FSM_Data *fsm_data, uint8_t state);


#endif // MOUSE_FSM_H