// *****************************************************************
// ****                       drivePACK                         ****
// ****                  www.tolaemon.com/dpack                 ****
// ****                       Source code                       ****
// ****                       01/04/2020                        ****
// ****                     Jordi Bartolome                     ****
// ****                                                         ****
// **** IMPORTANT:                                              ****
// **** Using this code or any part of it means accepting all   ****
// **** conditions exposed in: http://www.tolaemon.com/dpack    ****
// **** This code is part of the bareSAM platform. All info at: ****
// **** http://www.tolaemon.com/baresam                         ****
// *****************************************************************
#ifndef SDMMC_H_
#define SDMMC_H_

#include "sam.h"
#include "gpio.h"
#include "timing.h"

#define SDMMC_SECTOR_SIZE 512

#define SDMMC_MAX_UNITS  3    // maximum number of SD cards that can be managed by the driver
#define SDMMC_IDX_0      0

#define SDMMC_MAX_RETRIES 65535

#define SET_SPI_nCS_0       PORT->Group[0].OUTSET.reg = P0_SPI_SD_nCS_MASK; // disable SD_CARD 0 nCS=1
#define CLR_SPI_nCS_0       PORT->Group[0].OUTCLR.reg = P0_SPI_SD_nCS_MASK; // enable  SD_CARD 0 nCS=0

typedef enum{
	INVALID=0,
	SDv1=1,
	SDv2=2,       // SDv2 or later Standard Capacity
	SDv2_HC_XC=3, // SDv2 or later High Capacity or Xtended Capacity
	MMCv3=4
}SDMMC_type_t;

typedef struct{
	SDMMC_type_t st_type;
	uint16_t     sector_length;
}SDMMC_unit_t;


/*********************************************************************************************
* @brief Initializes the SDMMC module, preparing all the data structures to work with an SD MMC Card
*********************************************************************************************/
void SDMMC_Init();

/*********************************************************************************************
* @brief Calculate and returns the CRC-7 for a message of "length" bytes
* @param[in] message the array of uint8_t whose CRC we want to calculate.
* @param[in] length the number of uint8_t of the received array used to calucate the CRC.
* @return the CRC-7 for a message of "length" bytes
* @note
* Before using the CRC function, the CRC table should have been initialized with the corresponding
* CRC polynomy
* From: https://github.com/hazelnusse/crc7/blob/master/crc7.cc
*    Example of use:
*         uint8_t bytes[5]={0x48,0x00,0x00,0x01,0xAA};//>>0x87
*         uint8_t crc=0x00;
*         SDMMC_GenerateCRCTable();
*         crc = SDMMC_GetCRC(bytes, 5);
*********************************************************************************************/
uint8_t SDMMC_GetCRC(uint8_t message[], int length);

/*********************************************************************************************
* @brief Adds a message byte to the current CRC-7 to get a the new CRC-7
* @param[in] CRC
* @param[in] message_byte
* @return Calculate the CRC-7 for a message of "length" bytes
* @note before using the CRC function, the CRC table should have been initialized with the 
* corresponding CRC polynomy
*    From: https://github.com/hazelnusse/crc7/blob/master/crc7.cc
*********************************************************************************************/
uint8_t SDMMC_CRCAdd(uint8_t CRC, uint8_t message_byte);

/*********************************************************************************************
* @brief Initializes the SDMMC module, preparing all the microcontroller hardware ( SPI ) to work
* with an SD MMC Card
* @note From: https://github.com/hazelnusse/crc7/blob/master/crc7.cc
*********************************************************************************************/
void SDMMC_GenerateCRCTable();

/*********************************************************************************************
* @brief Sends the received byte "value" to the SD MMC Card through the SPI corresponding 
* to the specified uint_idx. Different SDMMC units may be controlled by the same
* or different SPIs (with different !CS)
* @param[in] unit_idx the index of the unit to send the byte
* @param[in] ui8_to_send with the byte we want to send to the card
* @return  the value received into the SPI data register after the transmission
* @note to use that function SPI interface should have been previously configured.
*********************************************************************************************/
uint8_t SDMMC_send_byte(uint8_t unit_idx, uint8_t value);

/*********************************************************************************************
* @brief clears the CS ( nCS = 0 ) to select the SDMMC card unit. Different SDMMC units may
* be controlled by the same or different SPIs (with different !CS)
* @param[in] unit_idx the index of the unit to select
*********************************************************************************************/
void SDMMC_select_card(uint8_t unit_idx);

/*********************************************************************************************
* @brief sets the CS ( nCS = 1 ) to UNselect the SDMMC card.Different SDMMC units may
* be controlled by the same or different SPIs (with different !CS)
* @param[in]  unit_idx the index of the unit to unselect
*********************************************************************************************/
void SDMMC_unselect_card(uint8_t unit_idx);

/*********************************************************************************************
* @brief lower level procedure which wakes-up the SD MMC Card
* @param[in] unit_idx the index of the unit to wake up
* @note to use that function SPI interface should have been previously configured.
*********************************************************************************************/
void SDMMC_wake_up(uint8_t unit_idx);

/*********************************************************************************************
* @brief sends the command received in the array p_bytes_tx and waits for and returns the R1 
* answer to the command sent
* @param[in]  unit_idx the index of the unit to send the specified command
* @param[in] p_bytes_tx[6] pointing to the array with the bytes of the command to send:
*       1 byte (CMD bits) + 4 bytes (params) + 1 byte (CRC7 byte)
* @param[out] p_bytes_rx pointing to the R1 if the answer has been received.
* @return the R1 command response ( 1 byte ), 0xff something has gone wrong or any error
* @note to use that function SPI interface should have been previously configured.
*********************************************************************************************/
uint8_t SDMMC_send_command_R1(uint8_t unit_idx, uint8_t *p_bytes_tx, uint8_t *p_bytes_rx);

/*********************************************************************************************
* @brief sends the command received in the array p_bytes_tx and waits for and returns the R2 answer
* to the sent command. The R2 response toke is 2 byte long and sent as response to the 
* SEND_STATUS command.
* @param[in]  unit_idx the index of the unit to send the specified command
* @param[in] p_bytes_tx[6] pointing to the array with the bytes of the command to send:
*      1 byte (CMD bits) + 4 bytes (params) + 1 byte (CRC7 byte)
* @param[out] p_bytes_rx[1] pointing to the R2 if the answer has been received ( 2 bytes )
* @return the first byte of the R2 command response. First byte of the R2 response is identical
* to the R1 response, 0xff something has gone wrong or any error
* @note To use that function SPI interface should have been previously configured. 
*********************************************************************************************/
uint8_t SDMMC_send_command_R2(uint8_t unit_idx, uint8_t *p_bytes_tx, uint8_t *p_bytes_rx);

/*********************************************************************************************
* @brief sends the command received in the array p_bytes_tx and waits for and returns the R1 and 
* then R3 answer to the sent command.
* @param[in] unit_idx the index of the unit to send the specified command
* @param[in] p_bytes_tx[6] pointing to the array with the bytes of the command to send:
*        1 byte (CMD bits) + 4 bytes (params) + 1 byte (CRC7 byte)
* @param[out]  p_bytes_rx[4] pointing to the R3 if the answer has been received.
* @return the R1 command response at the beginning of the sequence ( 1 byte )
* 0xff something has gone wrong or any error
* @note To use that function SPI interface should have been previously configured.
*********************************************************************************************/
uint8_t SDMMC_send_command_R3(uint8_t unit_idx, uint8_t *p_bytes_tx, uint8_t *p_bytes_rx);

/*********************************************************************************************
* @brief sends the command received in the array p_bytes_tx and waits for and returns the R1 and 
*  then R7 answer to the sent command.
* @param[in] unit_idx the index of the unit to send the specified command
* @param[in] p_bytes_tx[6] pointing to the array with the bytes of the command to send:
*         1 byte (CMD bits) + 4 bytes (params) + 1 byte (CRC7 byte)
* @param[out] p_bytes_rx[4] pointing to the R7 if the answer has been received.
* @return the R1 command response at the beginning of the sequence ( 1 byte ), 0xff something
* has gone wrong or any error.
* @note To use that function SPI interface should have been previously configured.
*********************************************************************************************/
uint8_t SDMMC_send_command_R7(uint8_t unit_idx, uint8_t *p_bytes_tx, uint8_t *p_bytes_rx);

/*********************************************************************************************
* @brief returns the state of the SD or MMC card
* @param[in] unit_idx the index of the unit to send the specified command
* @return int8_t with the current state of the SD or MMC card.
*********************************************************************************************/
int8_t SDMMC_card_status(uint8_t unit_idx);

/*********************************************************************************************
* @brief executes initialization sequence on the SD or MMC card and returns the type of card.
* @param[in] unit_idx the index of the unit to initialize.
* @return the type of the SD or MMC card.
* @note to understand that function and SD initialization check "Figure 7-2 SPI Mode Initialization Flow"
* page 211 of document "Physical Layer Simplified Specification Version 6.00" from SD Card Association.
* In the other hand, MMC Initialization consists in CMD0 until R1=0x01 followed by CMD1 until R1=0x00
*********************************************************************************************/
SDMMC_type_t SDMMC_card_initialize(uint8_t unit_idx);

/*********************************************************************************************
* @brief receives the answer to the command sent to MMCard.
* @param[in] unit_idx the index of the unit to wait from the FEDataTokenFrom
* @return value returned by the card
*********************************************************************************************/
uint8_t SDMMC_WaitFEDataToken(uint8_t unit_idx);

/*********************************************************************************************
* @brief Every data block written to the card should be acknowledged by a data response token
*  referred as Data Response token, that is 1 byte long with following format ( check 
*  "7.3.3 Control Tokens" section of "Physical Layer Simplified Specification":
*          7 6 5 4 3 2 1  0
*          x x x 0 Status 1
*  where Status bits correspond to:
*     0 1 0  Data accepted
*     1 0 1  Data rejected due to CRC error
*     1 1 0  Data rejected due to Write error
* @param[in] unit_idx the index of the unit to wait from the FEDataTokenFrom
* @return the value of the DataResponse
*********************************************************************************************/
uint8_t SDMMC_WaitDataResponse(uint8_t unit_idx);

/*********************************************************************************************
* @brief waits until R1b response indicates that the SD card is not busy. If the card
* sends a '0x00' it means it is busy (i.e. storing data) and if it sends a value 
* different of '0x00' it means that card is ready for next command. 
* @param[out] unit_idx the index of the unit to read the busy command from
* @return
* @note to understand how busy response works read "7.3.2.2 Format R1b" in page 225 of
* document "Physical Layer Simplified Specification Version 6.00" from SD Card Association.
* "... this response token is identical to the R1 format with the optional addition of 
* the bus signal. The busy signal toke can be any number of bytes. A zero value indicates
*  card is busy. A non-zero value indicates the card is ready for the next command..."
*********************************************************************************************/
uint8_t SDMMC_WaitR1bResponseNotBusy(uint8_t unit_idx);

/*********************************************************************************************
* @brief reads and returns the number of sectors specified in "count", starting by the sector specified in "sector"
* @param[in] unit_idx the index of the unit to read from
* @param[out] buff pointing to the data buffer with the read data bytes
* @param[in] sector with the start sector number to start reading from
* @param[in] count number of sectors to read and store into the specified buffer
* @return
* @note to understand that function and SD data read check section "7.2.3 Data Read" in page 212 of
* document "Physical Layer Simplified Specification Version 6.00" from SD Card Association.
*********************************************************************************************/
int8_t SDMMC_card_read (uint8_t unit_idx, uint8_t *buff, uint64_t sector, uint16_t count);

/*********************************************************************************************
* @brief reads and returns the the bytes of a single sector using command CMD17 READ_SINGLE_BLOCK 
* @param[in] unit_idx the index of the unit to read from
* @param[out] buff pointing to the data buffer with the read data bytes
* @param[in] sector with the start sector number to start reading from
* @return
* @note Read single block sequence is:
*   - send CMD17 READ SINGLE block
*   - receive R1
*   - wait for 'Data token start' 0xFE
*   - receive all block data bytes
*   - receive the 2 CRC bytes
*********************************************************************************************/
int8_t SDMMC_card_read_single_block(uint8_t unit_idx, uint8_t *buff, uint64_t sector);

/*********************************************************************************************
* @brief reads and returns the number of sectors specified in "count", starting by the sector specified in "sector"
*  using command CMD18 READ_MULTIPLE_BLOCKs
* @param[in] unit_idx the index of the unit to read from
* @param[out] buff pointing to the data buffer with the read data bytes
* @param[in] sector with the start sector number to start reading from
* @param[in] count number of blocks to read and store into the specified buffer
* @return
* @note Read multiple block sequence consists on:
*    - send CMD18 READ MULTIPLE blocks
*    - receive R1
*    - wait for 'Data token start' 0xFE
*    - receive all block 0 data bytes
*    - receive the 2 CRC bytes
*    - wait for 'Data token start' 0xFE
*    - receive all block 1 bytes
*    - receive the 2 CRC bytes
*    ...
*    - wait for 'Data token start' 0xFE
*    - receive last block n-1 data bytes
*    - receive the 2 CRC bytes
*    - send CMD12 STOP_TRANSMISSION
*********************************************************************************************/
int8_t SDMMC_card_read_multiple_blocks(uint8_t unit_idx, uint8_t *buff, uint64_t sector, uint16_t count);

/*********************************************************************************************
* @brief Writes the number of sectors received in buff specified in "count", starting 
* by the sector specified in "sector"
* @param[in] unit_idx the index of the unit to write to
* @param[in] buff pointing to the data buffer with the data to write
* @param[in] sector with the start sector number to start writing from
* @param[in] count number of sectors to write to the disk
* @return
* @note to understand that function and SD data read check section "7.2.4 Data Write" in page 213
* ofdocument "Physical Layer Simplified Specification Version 6.00" from SD Card Association.
*********************************************************************************************/
int8_t SDMMC_card_write(uint8_t unit_idx, uint8_t *buff, uint64_t sector, uint16_t count);

/*********************************************************************************************
* @brief writes and returns the the bytes of a single sector using command CMD24 xxxxxxxxxxxx
* @param[in] unit_idx the index of the unit to write to
* @param[in] buff pointing to the data buffer that contains the data to write to disk
* sector with the number of sector to start writing to
* @param[out] buff pointing to the data buffer with the data bytes
* @return
* @note Write single block sequence is:
*    - send CMD24 WRITE SINGLE block
*    - receive R1
*    - wait for 'Data token start' 0xFE
*    - receive all block data bytes
*    - receive the 2 CRC bytes
*********************************************************************************************/
int8_t SDMMC_card_write_single_block(uint8_t unit_idx, uint8_t *buff, uint64_t sector);

/*********************************************************************************************
* @brief writes the number of sectors specified in "count", starting by the sector specified in 
* "sector" using command CMD25 WRITE_MULTIPLE_BLOCKs
* @param[in] unit_idx the index of the unit to write to
* @param[in] buff pointing to the data buffer that contains the data to write to disk
* @param[in] sector with the number of sector to start writing to
* @param[in] count number of blocks to write to disk from the buffer
* @return
*********************************************************************************************/
int8_t SDMMC_card_write_multiple_blocks(uint8_t unit_idx, uint8_t *buff, uint64_t sector, uint16_t count);

#endif /* SDMMC_H_ */