#include "../mouse_dongle.X/mouse.h"
#include <stdio.h>
#include <string.h>
//#include "system/debug/sys_debug.h"
static spi_data_t spi_data;
static mouse_data_t mouse_data;
static mouse_report_t mouse_report USB_ALIGN;
static mouse_spi_data mouse_spi;
void usb_device_hid_event_handler(
    USB_DEVICE_HID_INDEX hid_instance,
    USB_DEVICE_HID_EVENT event,
    void *event_data,
    uintptr_t user_data) {
    mouse_data_t * mouse_data_obj = (mouse_data_t *)user_data;

    switch(event) {
        case USB_DEVICE_HID_EVENT_REPORT_SENT:
            mouse_data_obj->is_mouse_report_send_busy = false;
            break;
        case USB_DEVICE_HID_EVENT_REPORT_RECEIVED:
            break;
        case USB_DEVICE_HID_EVENT_SET_IDLE:
            USB_DEVICE_ControlStatus(mouse_data_obj->device_handle, USB_DEVICE_CONTROL_STATUS_OK);
            mouse_data_obj->idle_rate = ((USB_DEVICE_HID_EVENT_DATA_SET_IDLE*)event_data)->duration;
            break;
        case USB_DEVICE_HID_EVENT_GET_IDLE:
            USB_DEVICE_ControlSend(mouse_data_obj->device_handle, &(mouse_data_obj->idle_rate), 1);
            break;
        case USB_DEVICE_HID_EVENT_SET_PROTOCOL:
            mouse_data_obj->active_protocol = *(USB_HID_PROTOCOL_CODE *)event_data;
            USB_DEVICE_ControlStatus(mouse_data_obj->device_handle, USB_DEVICE_CONTROL_STATUS_OK);
            break;
        case USB_DEVICE_HID_EVENT_GET_PROTOCOL:
            USB_DEVICE_ControlSend(mouse_data_obj->device_handle, &(mouse_data_obj->active_protocol), 1);
            break;
        case USB_DEVICE_HID_EVENT_CONTROL_TRANSFER_DATA_SENT:
            break;
        default:
            break;
    }
}
void usb_device_event_handler(USB_DEVICE_EVENT event, void * event_data, uintptr_t context) {
    USB_DEVICE_EVENT_DATA_CONFIGURED * configuration_value;
    
    switch(event) {
        case USB_DEVICE_EVENT_SOF:
            mouse_data.sof_event_has_occured = true;
            break;
        case USB_DEVICE_EVENT_RESET:
        case USB_DEVICE_EVENT_DECONFIGURED:

            mouse_data.is_configured = false;
            mouse_data.is_mouse_report_send_busy = false;
            break;
        case USB_DEVICE_EVENT_CONFIGURED:
            configuration_value = (USB_DEVICE_EVENT_DATA_CONFIGURED *)event_data;
            if(configuration_value->configurationValue == 1) {
                mouse_data.is_configured = true;
                USB_DEVICE_HID_EventHandlerSet(mouse_data.hid_instance,
                        usb_device_hid_event_handler, (uintptr_t)&mouse_data);
            }
            break;
        case USB_DEVICE_EVENT_POWER_DETECTED:
            USB_DEVICE_Attach(mouse_data.device_handle);
            break;

        case USB_DEVICE_EVENT_POWER_REMOVED:
            USB_DEVICE_Detach(mouse_data.device_handle);
            break;

        case USB_DEVICE_EVENT_SUSPENDED:
            break;

        case USB_DEVICE_EVENT_RESUMED:
            if(mouse_data.is_configured == true) {

            }
            break;
        case USB_DEVICE_EVENT_ERROR:
        default:
            break;
            
    }
}
void spi_event_handler(uintptr_t context) {
    if (SPI2_ErrorGet() == SPI_SLAVE_ERROR_NONE) {
//        SPI2_Write(data, 6);
        SPI2_Read(spi_data.rx_data, SPI2_ReadCountGet());
//        if (spi_data.rx_data[3] != 0x0) {
//            printf("temp");
//        }
        unpack_spi_message(spi_data.rx_data, &mouse_spi);
//        if (mouse_spi.motion.dx != 0 || mouse_spi.motion.dy != 0) {
//            SYS_DEBUG_PRINT(SYS_ERROR_INFO, "Button Bitmask: 0x%02X\n", mouse_spi.button_bm);
//            SYS_DEBUG_PRINT(SYS_ERROR_INFO, "Wheel Rotation: %d\n", mouse_spi.wheel.rotation);
//            SYS_DEBUG_PRINT(SYS_ERROR_INFO, "X Motion: %d\n", mouse_spi.motion.dx);
//            SYS_DEBUG_PRINT(SYS_ERROR_INFO, "Y Motion: %d\n", mouse_spi.motion.dy);
//        }

//        memcpy(tmp, spi_data.rx_data, 6);
        if (mouse_data.is_configured && !mouse_data.is_mouse_report_send_busy) {
//            mouse_data.is_mouse_report_send_busy = true;
//            create_mouse_report(spi_data.rx_data, &mouse_report);
            create_mouse_report_new(&mouse_spi, &mouse_report);

            USB_DEVICE_HID_ReportSend(
                    mouse_data.hid_instance,
                    &mouse_data.report_transfer_handle,
                    (uint8_t*)&mouse_report,
                    sizeof(mouse_report_t)
            );
        }
    }
}

void init_mouse(void){
    mouse_data.device_handle = USB_DEVICE_HANDLE_INVALID;
    mouse_data.is_configured = false;
    mouse_data.hid_instance = 0;
    mouse_data.is_mouse_report_send_busy = false;
}
int enable_usb(void){
    mouse_data.device_handle = USB_DEVICE_Open(USB_DEVICE_INDEX_0, DRV_IO_INTENT_READWRITE);
    if (mouse_data.device_handle != USB_DEVICE_HANDLE_INVALID) {
        USB_DEVICE_EventHandlerSet(mouse_data.device_handle, usb_device_event_handler, 0);
        return 0;
    } else {
        printf("Error Failed to start mouse");
        return -1;
    }
    return 0;
}

void create_mouse_report(uint8_t *spi_rx_data, mouse_report_t *mouse_report) {
    if (spi_rx_data == NULL || mouse_report == NULL) {
        return;
    }
    memcpy(mouse_report->data, spi_rx_data, SPI_BUFFER_SIZE);
}
void create_mouse_report_new(mouse_spi_data *mouse_spi, mouse_report_t *hid_report) {
    // Button bitmask (8 bits)
    hid_report->data[0] = mouse_spi->button_bm;

    // Wheel rotation (8 bits, signed)
    hid_report->data[1] = mouse_spi->wheel.rotation;

    // X motion (12 bits, signed)
    hid_report->data[2] = mouse_spi->motion.dx & 0xFF;
    hid_report->data[3] = (mouse_spi->motion.dx >> 8) & 0x0F;

    // Y motion (12 bits, signed)
    hid_report->data[3] |= (mouse_spi->motion.dy & 0x0F) << 4;
    hid_report->data[4] = (mouse_spi->motion.dy >> 4) & 0xFF;
}

void unpack_spi_message(uint8_t *msg, mouse_spi_data *mouse_spi) {
    mouse_spi->button_bm = msg[0];

    mouse_spi->wheel.rotation = (int8_t)msg[1]; // Interpret as signed 8-bit value

    int16_t x = (msg[2] & 0xFF) | ((msg[3] & 0x0F) << 8);
    int16_t y = ((msg[3] & 0xF0) >> 4) | ((msg[4] & 0xFF) << 4);

    // If the most significant bit is set, the value is negative
    if (x & 0x0800) x |= 0xF000;
    if (y & 0x0800) y |= 0xF000;
    mouse_spi->motion.dx = x;
    mouse_spi->motion.dy = y;
}

void create_dummy_mouse_report(mouse_report_t *mouse_report) {
    if (mouse_report == NULL) {
        return;
    }
    mouse_report->data[0] = 0x00;
    mouse_report->data[1] = 0x10;
    mouse_report->data[2] = 0x00; // Least significant byte
    mouse_report->data[3] = 0x00; // Least significant byte
    mouse_report->data[4] = 0x00; // Least significant byte
}
void test_mouse(void) {
    if (mouse_data.is_configured && !mouse_data.is_mouse_report_send_busy) {
            mouse_data.is_mouse_report_send_busy = true;
            create_dummy_mouse_report(&mouse_report);

//            create_mouse_report(spi_data.rx_data, &mouse_report);
            // Send the SPI data as a HID report
            uint8_t result = USB_DEVICE_HID_ReportSend(
                    mouse_data.hid_instance,
                    &mouse_data.report_transfer_handle,
                    (uint8_t*)&mouse_report,
                    sizeof(mouse_report_t)
            );
            if (result != USB_DEVICE_HID_RESULT_OK) {
                printf("Failed");
            }
        }
}
