#ifndef RC522_SPI_H
#define RC522_SPI_H

#include <stdint.h>

typedef struct
{
    int (*spi_write)(uint8_t reg, uint8_t *data, uint8_t size);
    int (*spi_read)(uint8_t reg, uint8_t *data, uint8_t size);
    int (*rst_pin_write)(uint8_t val);
    int (*rst_pin_read)(void);
} rc522_ops_t;

int rc522_rst_pin_write(uint8_t val);
int rc522_rst_pin_read(void);

int rc522_spi_read_reg(uint8_t reg, uint8_t *data, uint8_t size);
int rc522_spi_write_reg(uint8_t reg, uint8_t *data, uint8_t size);

#endif // RC522_SPI_H