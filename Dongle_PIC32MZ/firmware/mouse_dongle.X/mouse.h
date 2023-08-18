#ifndef _MOUSE_H   /* Guard against multiple inclusion */
#define _MOUSE_H
#include "definitions.h"
#include "system/system.h"
#include "usb/usb_common.h"
#include "usb/usb_chapter_9.h"
#include "usb/usb_device.h"
#include "usb/usb_device_hid.h"


#define NUM_MOUSE_BUTTONS 5
#define SPI_BUFFER_SIZE   5

typedef int16_t mouse_coordinate_t;

typedef enum {
    RELEASED,
    PRESSED
} button_state;

typedef struct {
    mouse_coordinate_t dx;
    mouse_coordinate_t dy;
} motion_data_t;

typedef struct {
    int16_t rotation;
} wheel_data_t;

typedef struct {
    uint8_t data[5];
} mouse_report_t;

typedef struct {
    uint8_t button_bm;
    wheel_data_t wheel;
    motion_data_t motion;
}mouse_spi_data;

typedef struct {
    USB_DEVICE_HANDLE device_handle;
    SYS_MODULE_INDEX hid_instance;
    USB_DEVICE_HID_TRANSFER_HANDLE report_transfer_handle;
    SYS_MODULE_OBJ devic_layer_object;
    bool sof_event_has_occured;
    bool is_mouse_report_send_busy;
    uint8_t active_protocol;
    uint8_t idle_rate;
    
    bool is_configured;
}mouse_data_t;
typedef struct {
    uint8_t rx_data[SPI_BUFFER_SIZE];
    uint8_t tx_data[SPI_BUFFER_SIZE];
} spi_data_t;


void init_mouse(void);
void spi_event_handler(uintptr_t context);
void create_mouse_report(uint8_t *spi_rx_data, mouse_report_t *mouse_report);
void create_mouse_report_new(mouse_spi_data *mouse_spi, mouse_report_t *mouse_report);

void unpack_spi_message(uint8_t *msg, mouse_spi_data *mouse_spi);
void init_mouse(void);
int enable_usb(void);
void test_mouse(void);
#endif /* _MOUSE_H */

