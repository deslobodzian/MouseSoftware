#ifndef MOUSE_TRANSMITTER_H
#define MOUSE_TRANSMITTER_H

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
#include <zephyr/sys/byteorder.h>
#include "wheel.h"
#include "motion_sensor.h"


typedef struct {
        struct esb_payload received_message;
        struct esb_payload message;
        bool ready;
} esb_data_t;

#define INIT_ESB_DATA(_pipe, ...)  \
{ \
        .message = ESB_CREATE_PAYLOAD(_pipe, __VA_ARGS__), \
        .ready = true \
}

void event_handler(struct esb_evt const *event);

int clocks_start(void);

int init_esb(void);
int init_transceiver(void);

int esb_create_message(motion_info_t *motion, wheel_data_t *wheel, uint8_t* button_states);
int write_message();

#endif // MOUSE_TRANSMITTER_H