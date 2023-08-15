#include "mouse.h"
#include "usb.h"
#include "transmitter.h"
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(mouse, CONFIG_LOG_DEFAULT_LEVEL);

K_FIFO_DEFINE(mouse_fifo);

static mouse_t mouse_data;
static mouse_config_t config;

int init_mouse() {
    config.cpi = 400;
    while (!is_pmw3360_ready()) {
        LOG_DBG("PMW3360 not ready");
        k_sleep(K_MSEC(1));
    }
    LOG_INF("PMW3360 is ready");

    while (!init_buttons()) {
        LOG_DBG("Buttons not ready");
        k_sleep(K_MSEC(1));
    }
    LOG_INF("Buttons are ready");

    while (init_wheel() != 0) {
        LOG_DBG("Wheel not ready");
        k_sleep(K_MSEC(1));
    }
    LOG_INF("Wheel Encoder is ready");
    while (!configure_pmw3360()) {
        LOG_DBG("Configuring PMW3360 is not ready");
        k_sleep(K_MSEC(1));
    }

    while(init_usb() != 0) {
        LOG_DBG("Configuring USB is not ready");
        k_sleep(K_MSEC(1));
    }
    LOG_INF("USB is ready");

    while (init_transceiver() != 0) {
        LOG_DBG("Configuring transceiver is not ready");
        k_sleep(K_MSEC(1));
    }
    LOG_INF("Transceiver is ready");
    k_fifo_init(&mouse_fifo);
    return 0;
}

void update_config(void) {
    set_cpi(config.cpi);
    use_wireless(config.is_wireless);
}

void use_wireless(bool use_wireless) {
    config.is_wireless = use_wireless;
}

void update_mouse(mouse_t* mouse) {
    mouse->wheel_data = fetch_wheel_data();
    mouse->motion_info = read_motion();
    fetch_buttons(&(mouse->button_states));
}

void handle_mouse_transmission(void) {
    static message_t messages[2];
    static int current_message_index = 0;

    mouse_t new_data;
    update_mouse(&new_data);

    if (!k_fifo_is_empty(&mouse_fifo)) {
        message_t *prev_message = k_fifo_get(&mouse_fifo, K_NO_WAIT);
        prev_message->data.motion_info.dx += new_data.motion_info.dx;
        prev_message->data.motion_info.dy += new_data.motion_info.dy;
        prev_message->data.wheel_data.rotation += new_data.wheel_data.rotation;
        k_fifo_put(&mouse_fifo, prev_message);
    } else {
        messages[current_message_index].data = new_data;
        k_fifo_put(&mouse_fifo, &messages[current_message_index]);
        current_message_index = (current_message_index + 1) % 2;
    }

    if (!k_fifo_is_empty(&mouse_fifo)) {
        message_t *message_to_send = k_fifo_get(&mouse_fifo, K_NO_WAIT);
        if (config.is_wireless) {
            if (!esb_line_busy()) {}
                esb_create_message(&message_to_send->data);
                write_message();
            }
        } else {
            if (!line_busy()) {
                hid_write(&message_to_send->data);
            }
        }
    }
}

