#include "receiver.h"

LOG_MODULE_REGISTER(esb_receiver, CONFIG_LOG_DEFAULT_LEVEL);

void event_handler(struct esb_evt const *event) {
    switch (event->evt_id) {
        case ESB_EVENT_TX_SUCCESS:
            printk("TX SUCCESS EVENT");
            break;
        case ESB_EVENT_TX_FAILED:
            printk("TX FAILED EVENT");
            break;
        case ESB_EVENT_RX_RECEIVED:
            if (esb_read_rx_payload(&rx_payload) == 0) {
                printk("Packet received, len %d : "
				"0x%02x, 0x%02x, 0x%02x, 0x%02x, "
				"0x%02x, 0x%02x, 0x%02x, 0x%02x",
				rx_payload.length, rx_payload.data[0],
				rx_payload.data[1], rx_payload.data[2],
				rx_payload.data[3], rx_payload.data[4],
				rx_payload.data[5], rx_payload.data[6],
				rx_payload.data[7]);
            } else {
                printk("Error while reading rx packet");
            }
            break;
    }

}

int clocks_start(void) {
    int err;
    int res;
    struct onoff_manager *clk_mgr;
    struct onoff_client clk_cli;

    clk_mgr = z_nrf_clock_control_get_onoff(CLOCK_CONTROL_NRF_SUBSYS_HF);
    if (!clk_mgr) {
        printk("Unable to get the Clock manager");
        return -ENXIO;
    }

    sys_notify_init_spinwait(&clk_cli.notify);

    err = onoff_request(clk_mgr, &clk_cli);
    if (err < 0) {
        printk("Clock request failed: %d", err);
        return err;
    }

    do {
        err = sys_notify_fetch_result(&clk_cli.notify, &res);
        if (!err && res) {
            printk("Clock could not be started: %d", res);
            return res;
        }
    } while (err);

    printk("HF clock started");
    return 0;
}

int init_esb(void) {
    int err;
    /* These are arbitrary default addresses. In end user products
	 * different addresses should be used for each set of devices.
	 */
	uint8_t base_addr_0[4] = {0xE7, 0xE7, 0xE7, 0xE7};
	uint8_t base_addr_1[4] = {0xC2, 0xC2, 0xC2, 0xC2};
	uint8_t addr_prefix[8] = {0xE7, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7, 0xC8};

    struct esb_config config = ESB_DEFAULT_CONFIG;

    config.protocol = ESB_PROTOCOL_ESB_DPL;
    config.bitrate = ESB_BITRATE_2MBPS;
    config.mode = ESB_MODE_PRX;
    config.event_handler = event_handler;
    config.selective_auto_ack = true;

    err = esb_init(&config);

    if (err) {
        printk("Error initializing esb");
        return err;
    }   

    err = esb_set_base_address_0(base_addr_0);
    if (err) {
        printk("Error setting base address 0");
        return err;
    }   

    err = esb_set_base_address_1(base_addr_1);
    if (err) {
        printk("Error setting base address 1");
        return err;
    }   

    err = esb_set_prefixes(addr_prefix, ARRAY_SIZE(addr_prefix));
    if (err) {
        printk("Error setting address prefix");
        return err;
    }   

    return 0;

}

int init_receiver(void) {
    int err;

    printk("Staring mouse board transciever");
    err = clocks_start();
    if (err < 0) {
        return err;
    }

    err = init_esb();
    if (err < 0) {
        printk("ESB init failed, err %d", err);
        return err;
    }

    err = esb_start_rx();
    if (err < 0) {
        printk("RX setup failed, err %d", err);
        return err;
    }

    printk("Initialization complete");
    return 0;
}

struct esb_payload get_payload() {
    return rx_payload;
}
