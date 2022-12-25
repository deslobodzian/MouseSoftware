#ifndef MOUSE_STRUCTS_H
#define MOUSE_STRUCTS_H

#include "event_manager.h"
#include "fsm.h"
#include "transmitter.h"
#include "buttons.h"
#include "pmw3360.h"
#include "usb.h"
#include "mouse.h"
#include "hid.h"

/* Global Data Structurs */ 
extern struct k_sem sem;
extern event_manager_t manager;

extern hid_device_t hid_device;
extern USB_Data usb;
// extern hid_state hid_state;
extern ESB_Data transmitter;
extern MOUSE_Data mouse;
extern pmw3360_device pmw3360_dev;
extern FSM_Data fsm;

#endif // MOUSE_STRUCTS_H