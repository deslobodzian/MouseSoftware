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
#include "event_manager.h"
#include "pmw3360.h"
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
USB_Data usb;
hid_device_t hid_device;
ESB_Data transmitter;
MOUSE_Data mouse;
pmw3360_device pmw3360_dev;
FSM_Data fsm;


void main(void)
{
	k_sleep(K_MSEC(1000));
	// fsm.state = INIT_MODE;
	// init_usb(&usb);
	// init_pmw3360();
	// run_fsm(&fsm);
	k_sleep(K_MSEC(1000));
	init_event_manager(&manager);
	start_event_manager(&manager);

	motion_thread();

	for (;;) {
        // k_sem_take(&sem, K_FOREVER);
		// fetch_pmw3360_data(pmw3360_dev.device);
		// int16_t dx = get_dx(pmw3360_dev.device);
    	// int16_t dy = get_dy(pmw3360_dev.device);
		// int16_t dx = 0x01;
		// int16_t dy = 0x01;


        // uint8_t x_buf[sizeof(dx)];
        // uint8_t y_buf[sizeof(dy)];
        // sys_put_le16(dx, x_buf);
        // sys_put_le16(dy, y_buf);

        // mouse.data[0] = MOUSE_REPORT_ID;
        // mouse.data[1] = 0x00;
        // mouse.data[2] = 0x00;
        // mouse.data[3] = x_buf[0];
        // mouse.data[4] = (y_buf[0] << 4) | (x_buf[1] & 0x0f);
        // mouse.data[5] = (y_buf[1] << 4) | (y_buf[0] >> 4);
        // mouse.data[3] = 0x01;
        // mouse.data[4] = 0x01;
        // mouse.data[5] = 0x01;
		// LOG_INF("dx: %d, dy: %d", get_dx(pmw3360_dev.device), get_dy(pmw3360_struct.device)); 
		// k_sem_take(&sem, K_FOREVER);
		// LOG_DBG("FSM state: ", fsm.state);
		// int rc = hid_write(&usb);
        // LOG_ERR("usb err: %i", rc);
		k_sleep(K_MSEC(1));
	}
}
