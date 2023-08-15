#include <zephyr/kernel.h>
#include <zephyr/drivers/spi.h>
#include <zephyr/sys/byteorder.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/gpio.h>

#define SPI_DEV                 DT_NODELABEL(pic32mz)
#define SPI_OP                  SPI_OP_MODE_MASTER | SPI_WORD_SET(8) | SPI_TRANSFER_MSB | SPI_MODE_CPOL | SPI_MODE_CPHA


typedef struct {
    uint8_t data[6];
    size_t len;
} message_t;

void init_pic32_spi(void);

void pic32_spi_write(const message_t *msg);
void pic32_spi_read(uint8_t *data);
