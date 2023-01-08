#ifndef MOUSE_PERIODIC_TASK_H
#define MOUSE_PERIODIC_TASK_H

#include <zephyr/kernel.h>

void set_period_us(uint32_t us);

void set_period_ms(uint32_t ms);

void init_task(void (*f)(struct k_timer*));

#endif // MOUSE_PERIODIC_TASK_H