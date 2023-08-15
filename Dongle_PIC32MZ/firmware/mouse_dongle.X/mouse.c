#include "../mouse_dongle.X/mouse.h"
#include <stdio.h>
#include <string.h>
#include "system/debug/sys_debug.h"
static spi_data_t spi_data;
static mouse_data_t mouse_data;

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
        SPI2_Read(spi_data.rx_data, SPI_BUFFER_SIZE);
//        if (mouse_data.is_configured && !mouse_data.is_mouse_report_send_busy) {
//            // Send the SPI data as a HID report
//            USB_DEVICE_HID_ReportSend(
//                    mouse_data.hid_instance,
//                    &mouse_data.report_transfer_handle,
//                    spi_data.rx_data,
//                    SPI_BUFFER_SIZE
//            );
//        }
    }
}

void init_mouse(void){
    mouse_data.device_handle = USB_DEVICE_HANDLE_INVALID;
    mouse_data.is_configured = false;
    mouse_data.hid_instance = 0;
    mouse_data.is_mouse_report_send_busy = false;
}
void enable_usb(void){
    mouse_data.device_handle = USB_DEVICE_Open(USB_DEVICE_INDEX_0, DRV_IO_INTENT_READWRITE);
    if (mouse_data.device_handle != USB_DEVICE_HANDLE_INVALID) {
        USB_DEVICE_EventHandlerSet(mouse_data.device_handle, usb_device_event_handler, 0);
    }
}

void create_mouse_report(uint8_t *spi_rx_data, mouse_report_t *mouse_report) {
    
}
