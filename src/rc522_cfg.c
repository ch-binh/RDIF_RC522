// example of init ""
/*

void rc522_init(void) {
  rc522_spi_ops_t ops = {
      .spi_write = stm32_spi_write, // replace this
      .spi_read = stm32_spi_read  // replace this
  };
  rc522_set_spi_spec(&ops);
}

*/

#include "../inc/rc522_cfg.h"

static rc522_ops_t spi_ops;

void rc522_set_spi_spec(rc522_ops_t *ops)
{
    spi_ops = *ops; // Copy the function pointers
}

int rc522_spi_read_reg(uint8_t reg, uint8_t *data, uint8_t size)
{
    if (spi_ops.spi_read)
    {
        return spi_ops.spi_read(reg, data, size);
    }
    return -1; // Error: function not set
}

int rc522_spi_write_reg(uint8_t reg, uint8_t *data, uint8_t size)
{
    if (spi_ops.spi_write)
    {
        spi_ops.spi_write(reg, data, size);
        return 0;
    }
    return -1; // Error: function not set
}

int rc522_rst_pin_write(uint8_t val)
{
    if (spi_ops.rst_pin_write)
    {
        spi_ops.rst_pin_write(val);
        return 0;
    }
    return -1; // Error: function not set
}

int rc522_rst_pin_read(void)
{
    if (spi_ops.rst_pin_read)
    {
        return spi_ops.rst_pin_read();
    }
    return -1; // Error: function not set
}
