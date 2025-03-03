#ifndef RC522_H
#define RC522_H

#include <stdint.h>
#include <stdbool.h>
#include <string.h> // for memset

#include "../inc/rc522_reg.h"
#include "../inc/rc522_hw.h"

#define FIFO_SIZE 64

/* Recommended value */
/* MFRC522 Commands */
#define CMD_IDLE 0x00       // NO action; Cancel the current command
#define CMD_AUTHENT 0x0E    // Authentication Key
#define CMD_RECEIVE 0x08    // Receive Data
#define CMD_TRANSMIT 0x04   // Transmit data
#define CMD_TRANSCEIVE 0x0C // Transmit and receive dyata,
#define CMD_RESETPHASE 0x0F // Reset
#define CMD_CALCCRC 0x03    // CRC Calculate

typedef enum
{
    BITRATE_106KBITS = 0x00,
    BITRATE_212KBITS = 0x01,
    BITRATE_424KBITS = 0x02,
    BITRATE_848KBITS = 0x03
} tag_bitrate_e;

typedef enum
{
    // The commands used by the PCD to manage communication with several PICCs (ISO 14443-3, Type A, section 6.4)
    PICC_CMD_REQA = 0x26,    // REQuest command, Type A. Invites PICCs in state IDLE to go to READY and prepare for anticollision or selection. 7 bit frame.
    PICC_CMD_WUPA = 0x52,    // Wake-UP command, Type A. Invites PICCs in state IDLE and HALT to go to READY(*) and prepare for anticollision or selection. 7 bit frame.
    PICC_CMD_CT = 0x88,      // Cascade Tag. Not really a command, but used during anti collision.
    PICC_CMD_SEL_CL1 = 0x93, // Anti collision/Select, Cascade Level 1
    PICC_CMD_SEL_CL2 = 0x95, // Anti collision/Select, Cascade Level 2
    PICC_CMD_SEL_CL3 = 0x97, // Anti collision/Select, Cascade Level 3
    PICC_CMD_HLTA = 0x50,    // HaLT command, Type A. Instructs an ACTIVE PICC to go to state HALT.
    PICC_CMD_RATS = 0xE0,    // Request command for Answer To Reset.
    // The commands used for MIFARE Classic (from http://www.mouser.com/ds/2/302/MF1S503x-89574.pdf, Section 9)
    // Use PCD_MFAuthent to authenticate access to a sector, then use these commands to read/write/modify the blocks on the sector.
    // The read/write commands can also be used for MIFARE Ultralight.
    PICC_CMD_MF_AUTH_KEY_A = 0x60, // Perform authentication with Key A
    PICC_CMD_MF_AUTH_KEY_B = 0x61, // Perform authentication with Key B
    PICC_CMD_MF_READ = 0x30,       // Reads one 16 byte block from the authenticated sector of the PICC. Also used for MIFARE Ultralight.
    PICC_CMD_MF_WRITE = 0xA0,      // Writes one 16 byte block to the authenticated sector of the PICC. Called "COMPATIBILITY WRITE" for MIFARE Ultralight.
    PICC_CMD_MF_DECREMENT = 0xC0,  // Decrements the contents of a block and stores the result in the internal data register.
    PICC_CMD_MF_INCREMENT = 0xC1,  // Increments the contents of a block and stores the result in the internal data register.
    PICC_CMD_MF_RESTORE = 0xC2,    // Reads the contents of a block into the internal data register.
    PICC_CMD_MF_TRANSFER = 0xB0,   // Writes the contents of the internal data register to a block.
    // The commands used for MIFARE Ultralight (from http://www.nxp.com/documents/data_sheet/MF0ICU1.pdf, Section 8.6)
    // The PICC_CMD_MF_READ and PICC_CMD_MF_WRITE can also be used for MIFARE Ultralight.
    PICC_CMD_UL_WRITE = 0xA2 // Writes one 4 byte page to the PICC.
} PICC_cmd_e;

typedef enum
{
    STATUS_OK,                // Success
    STATUS_ERROR,             // Error in communication
    STATUS_COLLISION,         // Collission detected
    STATUS_TIMEOUT,           // Timeout in communication.
    STATUS_NO_ROOM,           // A buffer is not big enough.
    STATUS_INTERNAL_ERROR,    // Internal error in the code. Should not happen ;-)
    STATUS_INVALID,           // Invalid argument.
    STATUS_CRC_WRONG,         // The CRC_A does not match
    STATUS_MIFARE_NACK = 0xff // A MIFARE PICC responded with NAK.
} sts_e;

typedef struct
{
    uint8_t size;
    uint8_t fsc; // Frame size for proximity card

    struct
    {
        bool is_transmitted;
        bool is_same_dir;  // Only the same D for both directions supported
        tag_bitrate_e ds; // Send D
        tag_bitrate_e dr; // Receive D
    } ta1;

    struct
    {
        bool is_transmitted;
        uint8_t fwi;  // Frame waiting time integer
        uint8_t sfgi; // Start-up frame guard time integerq
    } tb1;

    struct
    {
        bool is_transmitted;
        bool is_support_CID;
        bool is_support_NAD;
    } tc1;

    // Raw data from ATS
    uint8_t data[FIFO_SIZE - 2]; // ATS cannot be bigger than FSD - 2 uint8_ts (CRC), according to ISO 14443-4 5.2.2
} Ats;

typedef struct
{
    uint8_t size; // Number of bytes in the UID. 4, 7 or 10.
    uint8_t uidByte[10];
    uint8_t sak; // The SAK (Select acknowledge) byte returned from the PICC after successful selection.
} Uid;

// A struct used for passing the PICC information
typedef struct
{
    uint16_t atqa;
    Uid uid;
    Ats ats;

    // For Block PCB
    bool blockNumber;
} tag_inf_t;

/*======================== HANDY FUNCTIONS ===========================*/
void rc522_init_reg(void);
void rc522_pcd_init(void);
int rc522_antenna_on(void);

/*======================== SETUP FUNCTIONS ===========================*/
uint8_t rc522_cfg_cmd(uint8_t val);

/*======================== GET DATA FUNCTIONS ===========================*/

/* REG 0x75 */
uint8_t rc522_get_chip_id(void);
uint8_t rc522_get_chip_sts(void);
#endif // RC522_H