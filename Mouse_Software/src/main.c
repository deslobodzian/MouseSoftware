#include "mouse.h"
#include <zephyr/kernel.h>
#include <zephyr/sys/byteorder.h>
#include <zephyr/logging/log.h>
#include "timing_utils.h"

LOG_MODULE_REGISTER(main, CONFIG_LOG_DEFAULT_LEVEL);

void test_func(void) {
    read_motion();
}

void main(void) {
    LOG_INF("Starting mouse");
    init_pmw3360_sensor();
    // init_mouse();
    LOG_INF("Mouse Initialized");

    // set_wireless(true);
    k_sleep(K_MSEC(2000));
    for (;;) {
        // time = measure_function_time(test_func);
        // LOG_INF("Time taken by function: %u microseconds", time);
        // k_sleep(K_MSEC(100));
        // handle_mouse_transmission();
    }
}