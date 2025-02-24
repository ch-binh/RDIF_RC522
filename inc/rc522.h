#ifndef RC522_H
#define RC522_H

#include <stdint.h>

/* Recommended value */
/* MFRC522 Commands */
#define CMD_IDLE 0x00       // NO action; Cancel the current command
#define CMD_AUTHENT 0x0E    // Authentication Key
#define CMD_RECEIVE 0x08    // Receive Data
#define CMD_TRANSMIT 0x04   // Transmit data
#define CMD_TRANSCEIVE 0x0C // Transmit and receive data,
#define CMD_RESETPHASE 0x0F // Reset
#define CMD_CALCCRC 0x03    // CRC Calculate

/*======================== HANDY FUNCTIONS ===========================*/
void rc522_init_reg(void);
void rc522_pcd_init(void);

/*======================== SETUP FUNCTIONS ===========================*/
uint8_t rc522_cfg_cmd(uint8_t val);

/*======================== GET DATA FUNCTIONS ===========================*/

/* REG 0x75 */
uint8_t rc522_get_chip_id(void);
uint8_t rc522_get_chip_sts(void);
#endif // RC522_H