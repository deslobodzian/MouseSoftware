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
#include "buttons.h"
#include "transmitter.h"
#include <zephyr/drivers/gpio.h>
#include <zephyr/logging/log.h>

#include <zephyr/usb/usb_device.h>
#include <zephyr/usb/class/usb_hid.h>

LOG_MODULE_REGISTER(app, CONFIG_LOG_DEFAULT_LEVEL);

K_SEM_DEFINE(sem, 0, 1);
USB_Data usb;
ESB_Data transmitter;
MOUSE_Data mouse;
PMW3360_Data pmw3360_struct;
FSM_Data fsm;


void main(void)
{
	k_sleep(K_MSEC(1000));
	fsm.state = INIT_MODE;
	run_fsm(&fsm);
	for (;;) {
		// k_sem_take(&sem, K_FOREVER);
		// LOG_DBG("FSM state: ", fsm.state);
		k_sleep(K_MSEC(1));
	}
}
