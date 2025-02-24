#include "../inc/rc522.h"
#include "../inc/rc522_reg.h"
#include "../inc/rc522_cfg.h"

void rc522_init_reg(void)
{

    rc522_cfg_cmd(CMD_RESETPHASE);

    // rc522_spi_write_reg(REG_T_MODE, (void *)0x8D, 1);
    // rc522_spi_write_reg(REG_T_PRESCALER, (void *)0x3E, 1);
    // rc522_spi_write_reg(REG_T_RELOAD_L, (void *)30, 1);
    // rc522_spi_write_reg(REG_T_RELOAD_H, (void *)0, 1);

    // /* 48dB gain */
    // rc522_spi_write_reg(REG_RF_CFG, (void *)0x70, 1);

    // rc522_spi_write_reg(REG_TX_AUTO, (void *)0x40, 1);
    // rc522_spi_write_reg(REG_MODE, (void *)0x3D, 1);
}

void rc522_pcd_init(void)
{
    uint8_t is_hard_reset = 0;

    if (rc522_rst_pin_read() == 0)
    {
        rc522_rst_pin_write(0);
        rc522_rst_pin_write(1);
        is_hard_reset = 1;
    }
    if (!is_hard_reset)
    {
        rc522_cfg_cmd(CMD_RESETPHASE);
    }

    rc522_spi_write_reg(REG_TX_MODE, (void *)0x00, 1);
    rc522_spi_write_reg(REG_RX_MODE, (void *)0x00, 1);
    // Reset ModWidthReg
    rc522_spi_write_reg(REG_MOD_WIDTH, (void *)0x26, 1);

    // When communicating with a PICC we need a timeout if something goes wrong.
    // f_timer = 13.56 MHz / (2*TPreScaler+1) where TPreScaler = [TPrescaler_Hi:TPrescaler_Lo].
    // TPrescaler_Hi are the four low bits in TModeReg. TPrescaler_Lo is TPrescalerReg.
    rc522_spi_write_reg(REG_T_MODE, (void *)0x80, 1);      // TAuto=1; timer starts automatically at the end of the transmission in all communication modes at all speeds
    rc522_spi_write_reg(REG_T_PRESCALER, (void *)0xA9, 1); // TPreScaler = TModeReg[3..0]:TPrescalerReg, ie 0x0A9 = 169 => f_timer=40kHz, ie a timer period of 25μs.
    rc522_spi_write_reg(REG_T_RELOAD_H, (void *)0x03, 1);  // Reload timer with 0x3E8 = 1000, ie 25ms before timeout.
    rc522_spi_write_reg(REG_T_RELOAD_L, (void *)0xE8, 1);

    rc522_spi_write_reg(REG_TX_AUTO, (void *)0x40, 1); // Default 0x00. Force a 100 % ASK modulation independent of the ModGsPReg register setting
    rc522_spi_write_reg(REG_MODE, (void *)0x3D, 1);    // Default 0x3F. Set the preset value for the CRC coprocessor for the CalcCRC command to 0x6363 (ISO 14443-3 part 6.2.4)
    // PCD_AntennaOn();
}

uint8_t rc522_cfg_cmd(uint8_t val)
{
    uint8_t size = 1;
    uint8_t data[size];
    rc522_spi_write_reg(REG_COMMAND, data, size);
    return data[0];
}

uint8_t rc522_get_chip_id(void)
{
    uint8_t size = 1;
    uint8_t data[size];
    rc522_spi_read_reg(REG_VERSION, data, size);
    return data[0];
}

uint8_t rc522_get_chip_sts(void)
{
    uint8_t size = 1;
    uint8_t data[size];
    rc522_spi_read_reg(REG_COMMAND, data, size);
    return data[0];
}
