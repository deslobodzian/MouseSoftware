/*
 * Copyright (c) 2012-2014 Wind River Systems, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "motion_sensor.h"
#include "usb.h"
#include "buttons.h"
#include "wheel.h"
// #include "periodic_task.h"
#include <zephyr/sys/byteorder.h>

#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(main, CONFIG_LOG_DEFAULT_LEVEL);

static struct k_timer timer;

void main(void) {

	uint32_t start;
	uint32_t stop;
	uint32_t dur;
	uint32_t nano;
	LOG_DBG("Starting mouse");
	init_buttons();
	if (!is_sensor_ready()) {
		// sleep for 1 second to wait for sensor.
		k_sleep(K_MSEC(1000));
	}
	if (!configure_sensor()) {
		return;
	}
	init_usb();
	k_sleep(K_MSEC(1000));
	uint8_t button_states[5];

	// k_timer_init(&timer, mouse_function, NULL);
	// k_timer_start(&timer, K_MSEC(1), K_MSEC(1));
	// LOG_DBG("initializing task");
	// init_task(mouse_function);
	// set_period_ms(1);
	for (;;) {
		k_sleep(K_USEC(700));
		start = k_cycle_get_32();
		motion_info_t motion = read_motion();
		wheel_data_t wheel = fetch_wheel_data();
		fetch_buttons(&button_states);
		// LOG_DBG("Button States: %i, %i, %i, %i, %i",
		//  	button_states[0],
		//  	button_states[1],
		//  	button_states[2],
		//  	button_states[3],
		//  	button_states[4]
		// );
		// get_state(&button_states);
		// LOG_DBG("Button state: %i", button_states);
		wheel.rotation = 0;
		// hid_write(&button_states, wheel.rotation, motion.dx, -motion.dy);
		// hid_write(0, 0);
		stop = k_cycle_get_32();
		dur = stop - start;
		// nano = (dur * 1000000000) / 32768;
		nano = (uint32_t) k_cyc_to_ns_floor64(dur);
		// LOG_DBG("Time took %i nano secs", nano);
		// LOG_DBG("dx: %i, dy: %i", motion.dx, motion.dy);
	}
}
