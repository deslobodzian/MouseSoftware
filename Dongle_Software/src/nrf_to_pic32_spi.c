#include "nrf_to_pic32_spi.h"
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(nrf_to_pic32, CONFIG_LOG_DEFAULT_LEVEL);

struct spi_dt_spec spi_com = SPI_DT_SPEC_GET(SPI_DEV, SPI_OP, 0);

void init_pic32_spi(void) {
    while(!spi_is_ready_dt(&spi_com)) {
        LOG_ERR("SPI NOT READY");
    }
    LOG_INF("SPI is ready");
} 

void pic32_spi_write(const message_t *msg) {
    struct spi_buf tx_buf = {
        .buf = msg->data,
        .len = msg->len
    };

    const struct spi_buf_set tx_bufs = {
        .buffers = &tx_buf,
        .count = 1
    };

    if (spi_write_dt(&spi_com, &tx_bufs) != 0) {
        LOG_ERR("Failed to write message");
    }
}

void pic32_spi_read(uint8_t *data) {
    const struct spi_buf rx_buf =  {
        .buf = data,
        .len = sizeof(data),
    };
    if (!spi_read_dt(&spi_com, &rx_buf)) {
        LOG_ERR("Failed to write message");
    }
}

