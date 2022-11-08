#include "transmitter.h"

LOG_MODULE_REGISTER(esb_transmitter, CONFIG_LOG_DEFAULT_LEVEL);

void event_handler(struct esb_evt const *event) {
    ready = true;

    switch(event -> evt_id) {
        case ESB_EVENT_TX_SUCCESS:
            LOG_DBG("TX SUCCESS EVENT");
            break;
        case ESB_EVENT_TX_FAILED:
            LOG_DBG("TX FAILED EVENT");
            break;
        case ESB_EVENT_RX_RECEIVED:
            while (esb_read_rx_payload(&rx_payload) == 0) {
                LOG_DBG("Packet received");
            }
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

    return 0;
}


int init_transciever(void) {
    int err;

    LOG_INF("Staring mouse board transciever");

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
    return 0;
}

int write_message(struct esb_payload payload) {
    int err;
    payload.noack = false;
    if (ready) {
        ready = false;
        esb_flush_tx();
        err = esb_write_payload(&payload);
        
        if (err) {
            LOG_ERR("Failed to write, err: %d", err);
            return -1;
        }
        return 0;
    }
    return -1;
}

