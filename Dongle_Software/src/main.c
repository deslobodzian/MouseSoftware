
#include <zephyr/logging/log.h>
#include "receiver.h"

LOG_MODULE_REGISTER(main, CONFIG_LOG_DEFAULT_LEVEL);

void main(void) {
	// struct esb_payload dummy;
	init_receiver();
	int len;
	int x;
	int y;
	for (;;) {
		// dummy = get_payload();
		// len = dummy.length;
		// x = dummy.data[0];
		// y = dummy.data[1];

		// LOG_DBG("Data: {%u}, {%u}, {%u}, {%u}, {%u}, {%u}", dummy.data[0], dummy.data[1], dummy.data[2], dummy.data[3], dummy.data[4], dummy.data[5]);
		k_sleep(K_MSEC(100));
	}
}
