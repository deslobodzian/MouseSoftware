/*
 * Copyright (c) 2012-2014 Wind River Systems, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */


#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/logging/log.h>
#include <zephyr/usb/usb_device.h>
#include <zephyr/usb/class/usb_hid.h>

#include "structs.h"
#include "pmw3360.h"
#include "event_manager.h"
#include "buttons.h"
#include "hid.h"
#include "transmitter.h"
#include <zephyr/drivers/gpio.h>
#include <zephyr/logging/log.h>

#include <zephyr/usb/usb_device.h>
#include <zephyr/usb/class/usb_hid.h>

LOG_MODULE_REGISTER(app, CONFIG_LOG_DEFAULT_LEVEL);

K_SEM_DEFINE(sem, 0, 1);
event_manager_t manager;
pmw3360_device pmw3360_dev;
USB_Data usb;
hid_device_t hid_device;
ESB_Data transmitter;
MOUSE_Data mouse;
FSM_Data fsm;


void main(void)
{
	k_sleep(K_MSEC(1000));
	// fsm.state = INIT_MODE;
	init_usb(&usb);
	// init_pmw3360();
	// run_fsm(&fsm);
	k_sleep(K_MSEC(1000));
	init_event_manager(&manager);
	start_event_manager(&manager);

	motion_thread();

	for (;;) {
		// k_sleep(K_MSEC(1));
	}
}
