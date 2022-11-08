/*
 * Copyright (c) 2012-2014 Wind River Systems, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */


#include <zephyr.h>
#include <device.h>
#include <devicetree.h>
#include <sys/printk.h>
// #include "pmw3360.h"
#include "buttons.h"
// #include "transmitter.h"
#include <drivers/gpio.h>
#include <logging/log.h>

LOG_MODULE_REGISTER(app, CONFIG_LOG_DEFAULT_LEVEL);

void main(void)
{
	const struct device *dev = get_pmw3360_device();
	int button_vals[5];
	init_buttons();


	k_sleep(K_MSEC(1000));
	init_pmw3360(dev);
	int rc = 0;
	for (;;) {
		fetch_pmw3360_data(dev);
		// LOG_INF("CPI: %d", get_cpi(dev));
		fetch_buttons(&button_vals);
		// LOG_INF("Val: %d", val);
		// LOG_INF("x: %03d; y: %03d\n", get_dx(dev), get_dy(dev));
		LOG_INF("x: %03d; y: %03d; M1: %d, M2: %d, M3: %d, M4: %d, M5: %d\n",
		 	get_dx(dev),
			get_dy(dev),
			button_vals[0],
			button_vals[1],
			button_vals[2],
			button_vals[3],
			button_vals[4]
		);
		// LOG_INF("CPI: %d", get_cpi(dev));
		
		// struct esb_payload test = ESB_CREATE_PAYLOAD(0, get_dx(dev), get_dy(dev));
		// write_message(test);
		k_sleep(K_MSEC(1000));
	}
}
