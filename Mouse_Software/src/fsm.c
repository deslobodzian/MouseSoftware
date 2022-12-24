#include "fsm.h"
#include "structs.h"

#include <zephyr/logging/log.h>
#include <zephyr/usb/usb_device.h>

LOG_MODULE_REGISTER(FSM, CONFIG_LOG_DEFAULT_LEVEL);

void run_fsm(FSM_Data *fsm_data) {
    switch(fsm_data->state) {
        case INIT_MODE:
            // LOG_ERR("USB err %i", init_usb(&usb));
            k_sleep(K_MSEC(1000));
            init_pmw3360();
            // init_buttons();
            LOG_DBG("Current State: %s", "INIT_MODE");

            // init_esb();
            // init_transciever();

            enter_state(fsm_data, SEARCHING_MODE);
        case SEARCHING_MODE:
            // TODO: add logic for looking if USB or ESB detected
            enter_state(fsm_data, USB_MODE);
            LOG_DBG("Current State: %s", "SEARCHING_MODE");
        case USB_MODE:
            LOG_DBG("Current State: %s", "USB_MODE");
            for(;;) {
                // k_sem_take(&sem, K_FOREVER);
                fetch_pmw3360_data(pmw3360_dev.device);
                int16_t dx = get_dx(pmw3360_dev.device);
                int16_t dy = get_dy(pmw3360_dev.device);
                LOG_ERR("dx: %i, dy: %i", dx, dy);
                // mouse.state[MOUSE_X_REPORT_POS] = get_dx(pmw3360_struct.device);
                // mouse.state[MOUSE_Y_REPORT_POS] = get_dy(pmw3360_struct.device);
               
                // int ret = hid_write(&usb);
                // if (ret) {
                //     LOG_ERR("usb err: %i", ret);
                // }
                // LOG_DBG("message: %");
                // k_sleep(K_MSEC(1));
            }
        case WIRELESS_MODE:
            for (;;) {

            }
    }
}

void enter_state(FSM_Data *fsm_data, uint8_t state) {
    fsm_data->state = state;
}