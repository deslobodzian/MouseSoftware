
#include <zephyr/logging/log.h>
#include "receiver.h"

LOG_MODULE_REGISTER(main, CONFIG_LOG_DEFAULT_LEVEL);

void main(void) {
	init_receiver();
	init_pic32_spi();
	k_sleep(K_MSEC(1000));
	uint8_t i = 0;
	for (;;) {
		LOG_INF("Main");
		k_sleep(K_MSEC(100));
	}
}
