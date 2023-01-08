#include "periodic_task.h"

static struct k_timer timer;

void set_period_us(uint32_t us) {
    k_timer_stop(&timer);
    k_timer_start(&timer, K_USEC(us), K_USEC(us));
}

void set_period_ms(uint32_t ms) {
    k_timer_stop(&timer);
    k_timer_start(&timer, K_MSEC(ms), K_MSEC(ms));
}

void init_task(void (*f)(struct k_timer*)) {
    k_timer_init(&timer, f, NULL);
    k_timer_start(&timer, K_MSEC(1), K_MSEC(1));
}

