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

#define _RADIO_SHORTS_COMMON                                            \
        (RADIO_SHORTS_READY_START_Msk | RADIO_SHORTS_END_DISABLE_Msk |  \
         RADIO_SHORTS_ADDRESS_RSSISTART_Msk |                           \
         RADIO_SHORTS_DISABLED_RSSISTOP_Msk)                            

typedef struct {
        struct esb_payload received_message;
        struct esb_payload message;
        bool ready;
} ESB_Data;

void event_handler(ESB_Data *data, struct esb_evt const *event);

int clocks_start(void);

int init_esb(void);
int init_transciever(void);

int write_message(ESB_Data *esb_data);

#endif // MOUSE_TRANSMITTER_H