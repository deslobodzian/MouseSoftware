#include "timing_utils.h"
#include <zephyr/logging/log.h>

uint32_t get_time_us(void) {
    return (uint32_t)k_ticks_to_us_floor64(k_uptime_ticks());
}

uint32_t measure_function_time(void (*func)(void)) {
    uint32_t start_time, end_time, elapsed_time;

    start_time = get_time_us();
    func();
    end_time = get_time_us();

    elapsed_time = end_time - start_time;
    return elapsed_time;
}
