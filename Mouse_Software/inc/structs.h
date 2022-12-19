#ifndef MOUSE_STRUCTS_H
#define MOUSE_STRUCTS_H

#include "fsm.h"
#include "transmitter.h"
#include "buttons.h"
#include "pmw3360.h"
#include "usb.h"
#include "mouse.h"

/* Global Data Structurs */ 
extern struct k_sem sem;
extern USB_Data usb;
extern ESB_Data transmitter;
extern MOUSE_Data mouse;
extern PMW3360_Data pmw3360_struct;
extern FSM_Data fsm;

#endif // MOUSE_STRUCTS_H