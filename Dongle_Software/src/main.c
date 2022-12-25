/*
 * Copyright (c) 2012-2014 Wind River Systems, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr.h>
#include <logging/log.h>
#include "receiver.h"

LOG_MODULE_REGISTER(main, CONFIG_LOG_DEFAULT_LEVEL);

void main(void) {
	struct esb_payload dummy;
	init_esb();
	int len;
	int x;
	int y;
	for (;;) {
		dummy = get_payload();
		len = dummy.length;
		x = dummy.data[0];
		y = dummy.data[1];
		// LOG_INF("Test");
	}
}
