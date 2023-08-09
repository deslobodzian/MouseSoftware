#ifndef DONGLE_RECEIVER_H
#define DONGLE_RECEIVER_H

#include <zephyr/drivers/clock_control.h>
#include <zephyr/drivers/clock_control/nrf_clock_control.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/irq.h>
#include <zephyr/logging/log.h>
#include <nrf.h>
#include <esb.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/kernel.h>
#include <zephyr/types.h>


void event_handler(struct esb_evt const *event);

int clocks_start(void);

int init_esb(void);
int init_receiver(void);

struct esb_payload get_payload();

#endif // DONGLE_RECEIVER_H