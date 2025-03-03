#include "../inc/rc522.h"

static tag_inf_t tag;

void rc522_init_reg(void)
{

    rc522_cfg_cmd(CMD_RESETPHASE);

    // hw_spi_write_reg(REG_T_MODE, (void *)0x8D, 1);
    // hw_spi_write_reg(REG_T_PRESCALER, (void *)0x3E, 1);
    // hw_spi_write_reg(REG_T_RELOAD_L, (void *)30, 1);
    // hw_spi_write_reg(REG_T_RELOAD_H, (void *)0, 1);

    // /* 48dB gain */
    // hw_spi_write_reg(REG_RF_CFG, (void *)0x70, 1);

    // hw_spi_write_reg(REG_TX_AUTO, (void *)0x40, 1);
    // hw_spi_write_reg(REG_MODE, (void *)0x3D, 1);
}

void rc522_pcd_init(void)
{
    uint8_t is_hard_reset = 0;

    if (hw_rst_pin_read() == 0)
    {
        hw_rst_pin_write(0);
        hw_rst_pin_write(1);
        is_hard_reset = 1;
    }
    if (!is_hard_reset)
    {
        rc522_cfg_cmd(CMD_RESETPHASE);
    }

    hw_spi_write_reg(REG_TX_MODE, (void *)0x00, 1);
    hw_spi_write_reg(REG_RX_MODE, (void *)0x00, 1);
    // Reset ModWidthReg
    hw_spi_write_reg(REG_MOD_WIDTH, (void *)0x26, 1);

    // When communicating with a PICC we need a timeout if something goes wrong.
    // f_timer = 13.56 MHz / (2*TPreScaler+1) where TPreScaler = [TPrescaler_Hi:TPrescaler_Lo].
    // TPrescaler_Hi are the four low bits in TModeReg. TPrescaler_Lo is TPrescalerReg.
    hw_spi_write_reg(REG_T_MODE, (void *)0x80, 1);      // TAuto=1; timer starts automatically at the end of the transmission in all communication modes at all speeds
    hw_spi_write_reg(REG_T_PRESCALER, (void *)0xA9, 1); // TPreScaler = TModeReg[3..0]:TPrescalerReg, ie 0x0A9 = 169 => f_timer=40kHz, ie a timer period of 25μs.
    hw_spi_write_reg(REG_T_RELOAD_H, (void *)0x03, 1);  // Reload timer with 0x3E8 = 1000, ie 25ms before timeout.
    hw_spi_write_reg(REG_T_RELOAD_L, (void *)0xE8, 1);

    hw_spi_write_reg(REG_TX_AUTO, (void *)0x40, 1); // Default 0x00. Force a 100 % ASK modulation independent of the ModGsPReg register setting
    hw_spi_write_reg(REG_MODE, (void *)0x3D, 1);    // Default 0x3F. Set the preset value for the CRC coprocessor for the CalcCRC command to 0x6363 (ISO 14443-3 part 6.2.4)
    rc522_antenna_on();
}

int rc522_antenna_on(void)
{
    uint8_t size = 1;
    uint8_t data[size];
    hw_spi_read_reg(REG_TX_CONTROL, data, size);
    if ((data[0] & 0x03) != 0x03)
    {
        hw_spi_write_reg(REG_TX_CONTROL, (void *)(data[0] | 0x03), size);
    }
    return 0;
}

uint8_t rc522_cfg_cmd(uint8_t val)
{
    uint8_t size = 1;
    uint8_t data[size];
    hw_spi_write_reg(REG_COMMAND, data, size);
    return data[0];
}

uint8_t rc522_get_chip_id(void)
{
    uint8_t size = 1;
    uint8_t data[size];
    hw_spi_read_reg(REG_VERSION, data, size);
    return data[0];
}

uint8_t rc522_get_chip_sts(void)
{
    uint8_t size = 1;
    uint8_t data[size];
    hw_spi_read_reg(REG_COMMAND, data, size);
    return data[0];
}

bool rc522_get_new_card()
{
    uint8_t bufferATQA[2];
    uint8_t bufferSize = sizeof(bufferATQA);

    // Reset baud rates
    hw_spi_write_reg(REG_TX_MODE, (void *)0x00, 1);
    hw_spi_write_reg(REG_RX_MODE, (void *)0x00, 1);
    // Reset ModWidthReg
    hw_spi_write_reg(REG_MOD_WIDTH, (void *)0x26, 1);

    sts_e result = PICC_RequestA(bufferATQA, &bufferSize);

    if (result == STATUS_OK || result == STATUS_COLLISION)
    {
        tag.atqa = ((uint16_t)bufferATQA[1] << 8) | bufferATQA[0];
        tag.ats.size = 0;
        tag.ats.fsc = 32; // default FSC value

        // Defaults for TA1
        tag.ats.ta1.is_transmitted = false;
        tag.ats.ta1.is_same_dir = false;
        tag.ats.ta1.ds = BITRATE_106KBITS;
        tag.ats.ta1.dr = BITRATE_106KBITS;

        // Defaults for TB1
        tag.ats.tb1.is_transmitted = false;
        tag.ats.tb1.fwi = 0;  // TODO: Don't know the default for this!
        tag.ats.tb1.sfgi = 0; // The default value of SFGI is 0 (meaning that the card does not need any particular SFGT)

        // Defaults for TC1
        tag.ats.tc1.is_transmitted = false;
        tag.ats.tc1.is_support_CID = true;
        tag.ats.tc1.is_support_NAD = false;

        memset(tag.ats.data, 0, FIFO_SIZE - 2);

        tag.blockNumber = false;
        return true;
    }
    return false;
} // End PICC_IsNewCardPresent()

sts_e rc522_picc_request_a(uint8_t *bufferATQA, uint8_t *bufferSize)
{
    return rc522_PICC_REQA_or_WUPA(PICC_CMD_REQA, bufferATQA, bufferSize);
}

sts_e rc522_PICC_REQA_or_WUPA(PICC_cmd_e command,     ///< The command to send - PICC_CMD_REQA or PICC_CMD_WUPA
                              uint8_t *bufferATQA, ///< The buffer to store the ATQA (Answer to request) in
                              uint8_t *bufferSize  ///< Buffer size, at least two bytes. Also number of bytes returned if STATUS_OK.
)
{
    uint8_t validBits;
    sts_e status;

    if (bufferATQA == NULL || *bufferSize < 2)
    { // The ATQA response is 2 bytes long.
        return STATUS_NO_ROOM;
    }
    hw_spi_cls_reg_bitmask(REG_COLL, 0x80); // ValuesAfterColl=1 => Bits received after collision are cleared.
    validBits = 7;                           // For REQA and WUPA we need the short frame format - transmit only 7 bits of the last (and only) byte. TxLastBits = BitFramingReg[2..0]
    status = PCD_TransceiveData(&command, 1, bufferATQA, bufferSize, &validBits);
    if (status != STATUS_OK)
    {
        return status;
    }
    if (*bufferSize != 2 || validBits != 0)
    { // ATQA must be exactly 16 bits.
        return STATUS_ERROR;
    }
    return STATUS_OK;
} // End PICC_REQA_or_WUPA()