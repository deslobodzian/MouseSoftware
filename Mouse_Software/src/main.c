#include "mouse.h"
#include <zephyr/kernel.h>
#include <zephyr/sys/byteorder.h>
#include <zephyr/logging/log.h>
#include "timing_utils.h"

LOG_MODULE_REGISTER(main, CONFIG_LOG_DEFAULT_LEVEL);

void test_func(void) {
    motion_info_t motion = read_motion();
    // LOG_INF("dx: %i, dy: %i", motion.dx, motion.dy);
}

void main(void) {
    LOG_INF("Starting mouse");
    // init_pmw3360_sensor();
    // while(!is_pmw3360_ready()){
    //     k_sleep(K_MSEC(1));
    // }
    init_mouse();
    LOG_INF("Mouse Initialized");

    // uint32_t time = 0;
    set_wireless(true);
    k_sleep(K_MSEC(1000));
    for (;;) {
        // time = measure_function_time(test_func);
        // LOG_INF("Time taken by function: %u microseconds", time);
        // k_sleep(K_MSEC(100));
        handle_mouse_transmission();
    }
}