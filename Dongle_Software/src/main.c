/*
 * Copyright (c) 2012-2014 Wind River Systems, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr.h>

void main(void)
{
	char c = 0;
for (;;)
{
	c = SEGGER_RTT_WaitKey(); // will block until data is available
	if(c == 'r'){
		SEGGER_RTT_WriteString(0, "Resetting..\n");
		nrf_delay_ms(1000);
		sd_nvic_SystemReset();
	}
	//power_manage();
}
}
