#ifndef MOUSE_HW_INFO_H
#define MOUSE_HW_INFO_H

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/usb/usb_device.h>
#include <zephyr/usb/class/usb_hid.h>

#define GPIO_SPEC(node_id) GPIO_DT_SPEC_GET_OR(node_id, gpios, {0})

#define MOUSE_BUTTON_REPORT_POS     0
#define MOUSE_X_REPORT_POS          1
#define MOUSE_Y_REPORT_POS          2

// Buttons
#define M1_NODE DT_ALIAS(m1)
#define M2_NODE DT_ALIAS(m2)
#define M3_NODE DT_ALIAS(m3)
#define M4_NODE DT_ALIAS(m4)
#define M5_NODE DT_ALIAS(m5)

#define SPI_INSTANCE                        0
#define SPI_NODE                            DT_NODELABEL(spi0)
#define SPI_FREQUENCY                       SPI_FREQUENCY_FREQUENCY_M2
#define SPI_SCK                             7
#define SPI_MISO                            14
#define SPI_MOSI                            15
// #define PMW3360_IRQ_NODE                    DT_ALIAS(irq)
// static const uint8_t hid_report_desc[] = HID_MOUSE_REPORT_DESC(5);

#endif // MOUSE_HW_INFO_H