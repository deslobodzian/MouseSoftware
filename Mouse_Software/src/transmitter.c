#include "transmitter.h"

#define _RADIO_SHORTS_COMMON                                            \
        (RADIO_SHORTS_READY_START_Msk | RADIO_SHORTS_END_DISABLE_Msk |  \
         RADIO_SHORTS_ADDRESS_RSSISTART_Msk |                           \
         RADIO_SHORTS_DISABLED_RSSISTOP_Msk)                            
LOG_MODULE_REGISTER(esb_transmitter, CONFIG_LOG_DEFAULT_LEVEL);

static esb_data_t esb_data = INIT_ESB_DATA(0, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08);

void event_handler(struct esb_evt const *event) {
    esb_data.ready = true;

    switch(event -> evt_id) {
        case ESB_EVENT_TX_SUCCESS:
            LOG_DBG("TX SUCCESS EVENT");
            break;
        case ESB_EVENT_TX_FAILED:
            LOG_DBG("TX FAILED EVENT");
            break;
        case ESB_EVENT_RX_RECEIVED:
            while (esb_read_rx_payload(&esb_data.received_message) == 0) {
                LOG_DBG("Packet received");
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
        LOG_ERR("Unable to get the Clock manager");
        return -ENXIO;
    }

    sys_notify_init_spinwait(&clk_cli.notify);

    err = onoff_request(clk_mgr, &clk_cli);
    if (err < 0) {
        LOG_ERR("Clock request fialed: %d", err);
        return err;
    }

    do {
        err = sys_notify_fetch_result(&clk_cli.notify, &res);
        if (!err && res) {
            LOG_ERR("Clock could not be started: %d", res);
            return res;
        }
    } while (err);
    LOG_DBG("High Frequency clock started");
    return 0;
}

int init_esb(void) {
    
    int err;
    uint8_t base_addr_0[4] = {0xE7, 0xE7, 0xE7, 0xE7};
    uint8_t base_addr_1[4] = {0xC2, 0xC2, 0xC2, 0xC2};
    uint8_t addr_prefix[8] = {0xE7, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7, 0xC8};
    struct esb_config config = ESB_DEFAULT_CONFIG;

    config.protocol = ESB_PROTOCOL_ESB_DPL;
    config.retransmit_delay = 600; //ms
    config.bitrate = ESB_BITRATE_2MBPS;
    config.event_handler = event_handler;
    config.mode = ESB_MODE_PTX;
    config.selective_auto_ack = true;

    err = esb_init(&config);

    if (err) {
        LOG_ERR("Error initializing esb");
        return err;
    }   

    err = esb_set_base_address_0(base_addr_0);
    if (err) {
        LOG_ERR("Error setting base address 0");
        return err;
    }   

    err = esb_set_base_address_1(base_addr_1);
    if (err) {
        LOG_ERR("Error setting base address 1");
        return err;
    }   

    err = esb_set_prefixes(addr_prefix, ARRAY_SIZE(addr_prefix));
    if (err) {
        LOG_ERR("Error setting address prefix");
        return err;
    }   
    esb_data.ready = true;

    return 0;
}

int init_transceiver(void) {
    int err;
    LOG_INF("Staring mouse board transceiver");
    err = clocks_start();
    if (err) {
        return -1;
    }
    err = init_esb();
    if (err) {
        LOG_ERR("ESB init failed, err %d", err);
        return -1;
    }
    LOG_INF("Initialization complete");
    esb_data.message.noack = false;
    return 0;
}

int esb_create_message(motion_info_t *motion, wheel_data_t *wheel, uint8_t* button_states) {
    uint8_t button_bm = 0;
	for (size_t i = 0; i < 5; i++) {
        if (button_states[i]) {
		    uint8_t mask = 1 << (i);
		    button_bm |= mask;
        }
	}
    uint8_t x_buf[sizeof(motion->dx)];
    uint8_t y_buf[sizeof(motion->dy)];
    sys_put_le16(motion->dx, x_buf);
    sys_put_le16(motion->dy, y_buf);
	esb_data.message.data[0] = 0x01;
    esb_data.message.data[1] = button_bm; //button_bit_mask;
    esb_data.message.data[2] = wheel->rotation; // wheel;
    esb_data.message.data[3] = x_buf[0];
    esb_data.message.data[4] = (y_buf[0] << 4) | (x_buf[1] & 0x0f);
    esb_data.message.data[5] = (y_buf[1] << 4) | (y_buf[0] >> 4);
    return 0;
}

int write_message() {
    int err;
    // LOG_DBG("Esb_data ready {%u}", esb_data.ready);
    if (esb_data.ready) {
        esb_data.ready = false;
        esb_flush_tx();
        err = esb_write_payload(&esb_data.message);
        
        if (err) {
            LOG_ERR("Failed to write, err: %d", err);
            return -1;
        }
        return 0;
    }
    return -1;
}
