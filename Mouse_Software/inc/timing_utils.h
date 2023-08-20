#ifndef MOUSE_TIMEING_UTILS_H
#define MOUSE_TIMEING_UTILS_H
#include <zephyr/kernel.h>
#include <zephyr/sys_clock.h>

uint32_t get_time_us(void);

uint32_t measure_function_time(void (*func)(void));
 

#endif /*MOUSE_TIMEING_UTILS_H */