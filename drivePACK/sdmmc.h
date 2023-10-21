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


//	Description:
//	  Initializes the SDMMC module, preparing all the data structures to work with an SD MMC Card
//  Receives:
//  Returns:
void SDMMC_Init();

//	Description:
//	  Calculate and returns the CRC-7 for a message of "length" bytes
//  Receives:
//  Returns:
//	  Calculate the CRC-7 for a message of "length" bytes
//  Note:
//    Before using the CRC function, the CRC table should have been initialized with the corresponding CRC polynomy
//    From: https://github.com/hazelnusse/crc7/blob/master/crc7.cc
//    Example of use:
//         uint8_t bytes[5]={0x48,0x00,0x00,0x01,0xAA};//>>0x87
//         uint8_t crc=0x00;
//         SDMMC_GenerateCRCTable();
//         crc = SDMMC_GetCRC(bytes, 5);
uint8_t SDMMC_GetCRC(uint8_t message[], int length);

//	Description:
//	  Adds a message byte to the current CRC-7 to get a the new CRC-7
//  Receives:
//    CRC:
//    message_byte
//  Returns:
//	  Calculate the CRC-7 for a message of "length" bytes
//  Note:
//    Before using the CRC function, the CRC table should have been initialized with the corresponding CRC polynomy
//    From: https://github.com/hazelnusse/crc7/blob/master/crc7.cc
uint8_t SDMMC_CRCAdd(uint8_t CRC, uint8_t message_byte);

//	Description:
//	  Initializes the SDMMC module, preparing all the microcontroller hardware ( SPI ) to work with an SD MMC Card
//  Receives:
//  Returns:
//  Note:
//    From: https://github.com/hazelnusse/crc7/blob/master/crc7.cc
void SDMMC_GenerateCRCTable();

// Description:
//   Sends the received byte "value" to the SD MMC Card through the SPI corresponding 
//  to the specified uint_idx. Different SDMMC units may be controlled by the same
//  or different SPIs (with different !CS)
// Receives:
//   unit_idx: the index of the unit to send the byte
//   ui8_to_send: with the byte we want to send to the card
// Returns:
//   By value:
//    Returns the value received into the SPI data register after the transmission
// Note:
//  To use that function SPI interface should have been previously configured.
uint8_t SDMMC_send_byte(uint8_t unit_idx, uint8_t value);

// Description:
//   Clears the CS ( nCS = 0 ) to select the SDMMC card unit. Different SDMMC units may
//  be controlled by the same or different SPIs (with different !CS)
// Receives:
//   unit_idx: the index of the unit to select
// Returns:
void SDMMC_select_card(uint8_t unit_idx);

// Description:
//   Sets the CS ( nCS = 1 ) to UNselect the SDMMC card.Different SDMMC units may
//  be controlled by the same or different SPIs (with different !CS)
// Receives:
//   unit_idx: the index of the unit to unselect
// Returns:
void SDMMC_unselect_card(uint8_t unit_idx);

// Description:
//   Lower level procedure which wakes-up the SD MMC Card
// Receives:
//   unit_idx: the index of the unit to wake up
// Returns:
// Note:
//  To use that function SPI interface should have been previously configured.
void SDMMC_wake_up(uint8_t unit_idx);

//	Description:
//	Sends the command received in the array p_bytes_tx and waits for and returns the R1 answer
//  to the command sent
//	Receives:
//     unit_idx: the index of the unit to send the specified command
//     p_bytes_tx[6]: pointing to the array with the bytes of the command to send: 
//   1 byte (CMD bits) + 4 bytes (params) + 1 byte (CRC7 byte)
//	Returns:
//    By reference:
//      p_bytes_rx[1]: pointing to the R1 if the answer has been received.
//    By value:
//      the R1 command response ( 1 byte )
//      0xff something has gone wrong or any error
//  Note:
//    To use that function SPI interface should have been previously configured.
uint8_t SDMMC_send_command_R1(uint8_t unit_idx, uint8_t *p_bytes_tx, uint8_t *p_bytes_rx);

//	Description:
//	Sends the command received in the array p_bytes_tx and waits for and returns the R2 answer
//  to the sent command. The R2 response toke is 2 byte long and sent as response to the 
//  SEND_STATUS command.
//	Receives:
//     unit_idx: the index of the unit to send the specified command
//     p_bytes_tx[6]: pointing to the array with the bytes of the command to send:
//   1 byte (CMD bits) + 4 bytes (params) + 1 byte (CRC7 byte)
//	Returns:
//    By reference:
//      p_bytes_rx[1]: pointing to the R2 if the answer has been received ( 2 bytes )
//    By value:
//      the first byte of the R2 command response. First byte of the R2 response is identical
//    to the R1 response.
//      0xff something has gone wrong or any error
//  Note:
//    To use that function SPI interface should have been previously configured. 
uint8_t SDMMC_send_command_R2(uint8_t unit_idx, uint8_t *p_bytes_tx, uint8_t *p_bytes_rx);

//	Description:
//	  Sends the command received in the array p_bytes_tx and waits for and returns the R1 and 
//  then R3 answer to the sent command.
//	Receives:
//     unit_idx: the index of the unit to send the specified command
//     p_bytes_tx[6]: pointing to the array with the bytes of the command to send:
//   1 byte (CMD bits) + 4 bytes (params) + 1 byte (CRC7 byte)
//	Returns:
//    By reference:
//      p_bytes_rx[4]: pointing to the R3 if the answer has been received.
//    By value:
//      the R1 command response at the beginning of the sequence ( 1 byte )
//      0xff something has gone wrong or any error
//  Note:
//    To use that function SPI interface should have been previously configured.
uint8_t SDMMC_send_command_R3(uint8_t unit_idx, uint8_t *p_bytes_tx, uint8_t *p_bytes_rx);

//	Description:
//	  Sends the command received in the array p_bytes_tx and waits for and returns the R1 and 
//  then R7 answer to the sent command.
//	Receives:
//     unit_idx: the index of the unit to send the specified command
//     p_bytes_tx[6]: pointing to the array with the bytes of the command to send:
//   1 byte (CMD bits) + 4 bytes (params) + 1 byte (CRC7 byte)
//	Returns:
//    By reference:
//      p_bytes_rx[4]: pointing to the R7 if the answer has been received.
//    By value:
//      the R1 command response at the beginning of the sequence ( 1 byte )
//      0xff something has gone wrong or any error
//  Note:
//    To use that function SPI interface should have been previously configured.
uint8_t SDMMC_send_command_R7(uint8_t unit_idx, uint8_t *p_bytes_tx, uint8_t *p_bytes_rx);

// Description:
//   Returns the state of the SD or MMC card
// Receives:
//     unit_idx: the index of the unit to send the specified command
// Returns:
//   By value:
//      int8_t: with the current state of the SD or MMC card.
int8_t SDMMC_card_status(uint8_t unit_idx);

// Description:
//   Executes initialization sequence on the SD or MMC card and returns the type of card.
// Receives:
//     unit_idx: the index of the unit to initialize
// Returns:
//   By value:
//      The type of the SD or MMC card
// Note: to understand that function and SD initialization check "Figure 7-2 SPI Mode Initialization Flow"
// page 211 of document "Physical Layer Simplified Specification Version 6.00" from SD Card Association.
// In the other hand, MMC Initialization consists in CMD0 until R1=0x01 followed by CMD1 until R1=0x00
SDMMC_type_t SDMMC_card_initialize(uint8_t unit_idx);

// Description:
//   Receives the answer to the command sent to MMCard.
// Receives:
//     unit_idx: the index of the unit to wait from the FEDataTokenFrom
// Returns:
//   By value:
//      value returned by the card
uint8_t SDMMC_WaitFEDataToken(uint8_t unit_idx);

// Description:
//   Every data block written to the card should be acknowledged by a data response token
//  referred as Data Response token, that is 1 byte long with following format ( check 
//  "7.3.3 Control Tokens" section of "Physical Layer Simplified Specification":
//          7 6 5 4 3 2 1  0
//          x x x 0 Status 1
//  where Status bits correspond to:
//     0 1 0  Data accepted
//     1 0 1  Data rejected due to CRC error
//     1 1 0  Data rejected due to Write error
//
// Receives:
//     unit_idx: the index of the unit to wait from the FEDataTokenFrom
// Returns:
//   By value:
//    the value of the DataResponse
uint8_t SDMMC_WaitDataResponse(uint8_t unit_idx);


// Description:

// Receives:
//    Waits until R1b response indicates that the SD card is not busy. If the card
//  sends a '0x00' it means it is busy (i.e. storing data) and if it sends a value 
//  different of '0x00' it means that card is ready for next command. 
// Returns:
//   By reference:
//      unit_idx: the index of the unit to read the busy command from
//	 By value:
// Note: to understand how busy response works read "7.3.2.2 Format R1b" in page 225 of
// document "Physical Layer Simplified Specification Version 6.00" from SD Card Association.
// "... this response token is identical to the R1 format with the optional addition of 
// the bus signal. The busy signal toke can be any number of bytes. A zero value indcates
//  card is busy. A non-zero value indicates the card is ready for the next command..."
uint8_t SDMMC_WaitR1bResponseNotBusy(uint8_t unit_idx);

// Description:
//   Reads and returns the number of sectors specified in "count", starting by the sector specified in "sector"
// Receives:
//      unit_idx: the index of the unit to read from
//		buff: pointing to the data buffer where read data will be stored
//      sector: with the start sector number to start reading from
//      count: number of sectors to read and store into the specified buffer
// Returns:
//   By reference:
//	    buff: pointing to the data buffer with the data bytes
//	 By value:
//
// Note: to understand that function and SD data read check section "7.2.3 Data Read" in page 212 of
// document "Physical Layer Simplified Specification Version 6.00" from SD Card Association.
int8_t SDMMC_card_read (uint8_t unit_idx, uint8_t *buff, uint64_t sector, uint16_t count);

// Description:
//   Reads and returns the the bytes of a single sector using command CMD17 READ_SINGLE_BLOCK 
// Receives:
//      unit_idx: the index of the unit to read from
//		buff: pointing to the data buffer where read data will be stored and returned
//      sector: with the start sector number to start reading from
// Returns:
//   By reference:
//		buff: pointing to the data buffer with the data bytes
//	 By value:
// Note: Read single block sequence is:
//    - send CMD17 READ SINGLE block
//    - receive R1
//    - wait for 'Data token start' 0xFE
//    - receive all block data bytes
//    - receive the 2 CRC bytes
int8_t SDMMC_card_read_single_block(uint8_t unit_idx, uint8_t *buff, uint64_t sector);

// Description:
//   Reads and returns the number of sectors specified in "count", starting by the sector specified in "sector"
//  using command CMD18 READ_MULTIPLE_BLOCKs
// Receives:
//      unit_idx: the index of the unit to read from
//		buff: pointing to the data buffer where read data will be stored
//      sector: with the start sector number to start reading from
//      count: number of blocks to read and store into the specified buffer
// Returns:
//   By reference:
//	    buff: pointing to the data buffer with the data bytes
//	 By value:
// Note: Read multiple block sequence consists on:
//    - send CMD18 READ MULTIPLE blocks
//    - receive R1
//    - wait for 'Data token start' 0xFE
//    - receive all block 0 data bytes
//    - receive the 2 CRC bytes
//    - wait for 'Data token start' 0xFE
//    - receive all block 1 bytes
//    - receive the 2 CRC bytes
//    ...
//    - wait for 'Data token start' 0xFE
//    - receive last block n-1 data bytes
//    - receive the 2 CRC bytes
//    - send CMD12 STOP_TRANSMISSION
int8_t SDMMC_card_read_multiple_blocks(uint8_t unit_idx, uint8_t *buff, uint64_t sector, uint16_t count);


// Description:
//   Writes the number of sectors received in buff specified in "count", starting 
//   by the sector specified in "sector"
// Receives:
//      unit_idx: the index of the unit to write to
//		buff: pointing to the data buffer with the data to write
//      sector: with the start sector number to start writing from
//      count: number of sectors to write to the disk
// Returns:
//   By reference:
//	    buff: pointing to the data buffer with the data bytes
//	 By value:
//
// Note: to understand that function and SD data read check section "7.2.4 Data Write" in page 213 of
// document "Physical Layer Simplified Specification Version 6.00" from SD Card Association.
int8_t SDMMC_card_write(uint8_t unit_idx, uint8_t *buff, uint64_t sector, uint16_t count);


// Description:
//   Writes and returns the the bytes of a single sector using command CMD24 xxxxxxxxxxxx
// Receives:
//      unit_idx: the index of the unit to write to
//		buff: pointing to the data buffer that contains the data to write to disk
//      sector: with the number of sector to start writing to
// Returns:
//   By reference:
//		buff: pointing to the data buffer with the data bytes
//	 By value:
// Note: Write single block sequence is:
//    - send CMD24 WRITE SINGLE block
//    - receive R1
//    - wait for 'Data token start' 0xFE
//    - receive all block data bytes
//    - receive the 2 CRC bytes
int8_t SDMMC_card_write_single_block(uint8_t unit_idx, uint8_t *buff, uint64_t sector);

// Description:
//   Writs the number of sectors specified in "count", starting by the sector specified in "sector"
//  using command CMD25 WRITE_MULTIPLE_BLOCKs
// Receives:
//      unit_idx: the index of the unit to write to
//		buff: pointing to the data buffer that contains the data to write to disk
//      sector: with the number of sector to start writing to
//      count: number of blocks to write to disk from the buffer
// Returns:
//   By reference:
//	 By value:
// Note: 
int8_t SDMMC_card_write_multiple_blocks(uint8_t unit_idx, uint8_t *buff, uint64_t sector, uint16_t count);

#endif /* SDMMC_H_ */