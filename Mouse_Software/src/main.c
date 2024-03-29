#include "mouse.h"
#include <zephyr/kernel.h>
#include <zephyr/sys/byteorder.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(main, CONFIG_LOG_DEFAULT_LEVEL);

void main(void) {
    LOG_INF("Starting mouse");
    init_mouse();
    LOG_INF("Mouse Initialized");
    set_wireless(true);
    k_sleep(K_MSEC(1000));
    for (;;) {
        handle_mouse_transmission();
    }
}