
#include <zephyr/logging/log.h>
#include "receiver.h"

LOG_MODULE_REGISTER(main, CONFIG_LOG_DEFAULT_LEVEL);

void main(void) {
	// struct esb_payload dummy;
	init_receiver();
	init_pic32_spi();
	// message_t msg = {
	// 	.data =  {0x1, 0x2, 0x3, 0x4, 0x5, 0x6},
	// 	.len = 6,
	// };

	k_sleep(K_MSEC(1000));
	// uint8_t rx_data[6];
	for (;;) {
		// LOG_INF("Main Running");
		// pic32_spi_write(&msg);
		// pic32_spi_read(&rx_data);
		// LOG_INF("Data: {%u}" ,rx_data[0]);

		// dummy = get_payload();
		// len = dummy.length;
		// x = dummy.data[0];
		// y = dummy.data[1];

		// LOG_DBG("Data: {%u}, {%u}, {%u}, {%u}, {%u}, {%u}", dummy.data[0], dummy.data[1], dummy.data[2], dummy.data[3], dummy.data[4], dummy.data[5]);
		k_sleep(K_MSEC(100));
	}
}
