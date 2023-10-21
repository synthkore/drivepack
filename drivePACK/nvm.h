// *****************************************************************
// ****                       drivePACK                         ****
// ****                  www.tolaemon.com/dpack                 ****
// ****                       Source code                       ****
// ****                       21/04/2023                        ****
// ****                     Jordi Bartolome                     ****
// ****                                                         ****
// **** IMPORTANT:                                              ****
// **** Using this code or any part of it means accepting all   ****
// **** conditions exposed in: http://www.tolaemon.com/dpack    ****
// *****************************************************************
#ifndef NVM_H_
#define NVM_H_

#include "sam.h"
#include "gpio.h"
#include "sys_defs.h"
#include "file_sys.h"

// BOOTPROT [3:0] Rows Protected by BOOTPROT Boot Loader Size in KBytes
// [1] BOOTPROT [3:0]
// [2] Rows Protected by BOOTPROT
// [3] Boot Loader Size in KBytes
// [1] [2]    [3]
// 15  None    0
// 14    1     8
// 13    2    16
// 12    3    24
// 11    4    32
// 10    5    40
// 09    6    48
// 08    7    56
// 07    8    64
// 06    9    72
// 05    10   80
// 04    11   88
// 03    12   96
// 02    13  104
// 01    14  112
// 00    15  120
#define NVMCTRL_BOOTLOADER_SIZE 0x6000; // 0x6000 = 24576 = 24 KBs

// Page size: to know the page size of the micro controller the PSZ[2:0] bits must be read 
// PSZ bits   Size
// 0x00       8 bytes
// 0x01      16 bytes
// 0x02      32 bytes
// 0x03      64 bytes
// 0x04     128 bytes
// 0x05     256 bytes
// 0x06     512 bytes
// 0x07    1024 bytes

// The NVM is organized into two banks, each bank is organized into blocks, where each block contains sixteen pages.
#define NVMCTRL_MAX_PAGE_SIZE_BY      1024// this is only the maximum page possible page size in bytes. To know the page size of the micro controller the PSZ[2:0] bits must be read first.
#define NVMCTRL_MAX_PAGE_SIZE_DW      (NVMCTRL_MAX_PAGE_SIZE_BY/4)  //  this is only the maximum page possible size in double words (32bits , 4 bytes). To know the page size of the micro controller the PSZ[2:0] bits must be read first.

// Region size: the region size depends on flash memory size!! The region size is dependent on the flash memory 
// size, and is given in the table below. Each region has a dedicated lock bit preventing writing and erasing 
// pages in the region. After production, all regions will be unlocked.
// Models with 1024KB Flash   32Kb per region
// Models with  512KB Flash   16Kb per region
// Models with  256KB Flash    8Kb per region
// So the region size is calculated by software deppending on the size of the flash memory

#define NVMCTRL_FLASH_START_ADDRESS        (0x00000000U) //  0x00000000 to 0x0007FFFF (0 to (2^19)-1) > 512KBs ( 3 bytes )    o    0x00000000 to 0x000FFFFF  (0 to (2^20)-1)  > 1024KBs ( 3 bytes ) 

#define NVMCTRL_INVALID_32BIT_ADDR         0xFFFFFFFF // code to identify the invalid address ( in fact 0xFFFFFFFF is a valid value, but as it will never be reached it is used as the invalid address code )

#define NVMCTRL_DEFAULT_HEX_BYTE           0xFF

#define NVMCTR_CMD_EXEC 0xA500 // CTRLB bit group should be written with the key value 0xA5 to execute the command. The key value must be written at the same time as CMD.

// NVM COMMAND & CONTROL COMMANDS
#define NVM_CMD_EP       0x00 //  EP Erase Page - Only supported in the User page in the auxiliary space.
#define NVM_CMD_EB       0x01 //  EB Erase Block - Erases the block addressed by the ADDR register, not supported in the user page
// 0x02 Reserved
#define NVM_CMD_WP       0x03 // WP Write Page - Writes the contents of the page buffer to the page addressed by the ADDR register, not supported in the user page
#define NVM_CMD_WQW      0x04 // WQW Write Quad Word - Writes a 128-bit word at the location addressed by the ADDR register.
// 0x5-0xF Reserved
#define NVM_CMD_SWRS     0x10 // SWRST Software Reset - Power-Cycle the NVM memory and replay the device automatic calibration procedure and resets the module configuration registers
#define NVM_CMD_LR       0x11 // LR Lock Region - Locks the region containing the address location in the ADDR register until next reset.
#define NVM_CMD_UR       0x12 // UR Unlock Region - Unlocks the region containing the address location in the ADDR register until next reset.
#define NVM_CMD_SPRM     0x13 // SPRM Sets the power reduction mode.
#define NVM_CMD_CPRM     0x14 // CPRM Clears the power reduction mode.
#define NVM_CMD_PBC      0x15 // PBC Page Buffer Clear - Clears the page buffer.
#define NVM_CMD_SSB      0x16 // SSB Set Security Bit
#define NVM_CMD_BKSWRST  0x17 // BKSWRST Bank swap and system reset, if SmartEEPROM is used also reallocate its data into the opposite BANK
#define NVM_CMD_CELCK    0x18 // CELCK Chip Erase Lock - DSU.CTRL.CE command is not available
#define NVM_CMD_CEULCK   0x19 // CEULCK Chip Erase Unlock - DSU.CTRL.CE command is available
#define NVM_CMD_SPBDIS   0x1A // SBPDIS Sets STATUS.BPDIS, Boot loader protection is discarded until CBPDIS is issued or next start-up sequence
#define NVM_CMD_CBPDIS   0x1B // CBPDIS Clears STATUS.BPDIS, Boot loader protection is not discarded
//0x1C-0x2F Reserved
#define NVM_CMD_ASEES0   0x30 // ASEES0 Configure SmartEEPROM to use Sector 0
#define NVM_CMD_ASEES1   0x31 // ASEES1 Configure SmartEEPROM to use Sector 1
#define NVM_CMD_SEERALOC 0x32 // SEERALOC Starts SmartEEPROM sector reallocation algorithm
#define NVM_CMD_SEEFLUSH 0x33 // SEEFLUSH Flush SmartEEPROM data when in buffered mode
#define NVM_CMD_LSEE     0x34 // LSEE Lock access to SmartEEPROM data from any means
#define NVM_CMD_USEE     0x35 // USEE Unlock access to SmartEEPROM data
#define NVM_CMD_LSEER    0x36 // LSEER Lock access to the SmartEEPROM Register Address Space (above 64KB)
#define NVM_CMD_USEER    0x37 // USEER Unock access to the SmartEEPROM Register Address Space (above 64KB)
// 0x38-0x7F Reserved

// HEX file record types:
// 00 Data
// 01 End Of File
// 02 Extended Segment Address
// 03 Start Segment Address
// 04 Extended Linear address
// 05 Start Linear Address
//
// EXTENDED SEGMENT ADDRESS:
// "...add the extended segment address to the current absolute address. The extended segment address data 
// field contains a 16-bit segment base address. This is multiplied by 16 ( 4bits shift ) and added to each
// subsequent data  record address to form the address for the data. This allows addressing up to one
// megabyte (1048576 bytes) of address space..."
//
// EXTENDED LINEAR ADDRESS:
// "...add the extended linear address to the current absolute address. The extended linear address allows for
// 32 bit addressing (up to 4 GiB). The byte count is always 02 and the address field is ignored (typically 0000).
// The two data bytes (big endian) specify the upper 16 bits of the 32 bit absolute address for all subsequent
// type 00 records; these upper address bits apply until the next 04 record. The absolute address for a type 00
// record is formed by combining the upper 16 address bits of the most recent 04 record with the low 16 address
// bits of the 00 record. If a type 00 record is not preceded by any type 04 records then its upper 16 address
// bits default to 0000..."
#define NVM_HEX_REC_TY_DATA            0x00   // DATA
#define NVM_HEX_REC_TY_EOF             0x01   // END OF FILE
#define NVM_HEX_REC_TY_EXT_SEG_ADDR    0x02   // EXTENDED SEGMENT ADDRESS
#define NVM_HEX_REC_TY_START_SEG_ADDR  0x03   // START SEGMENT ADDRESS
#define NVM_HEX_REC_TY_EXT_LIN_ADDR    0x04   // EXTENDED LINEAR ADDRESS
#define NVM_HEX_REC_TY_START_LIN_ADDR  0x05   // START LINEAR ADDRESS
#define NVM_HEX_REC_TY_NONE            0xFF  

#define NVM_HEX_REC_START_MARK         ':'

/*
#define NVMCTRL_FLASH_ROWSIZE              (256U)

#define NVMCTRL_RWWEEPROM_START_ADDRESS    (0x00400000U)
#define NVMCTRL_RWWEEPROM_SIZE             (0x800U)
#define NVMCTRL_RWWEEPROM_PAGESIZE         (64U)
#define NVMCTRL_RWWEEPROM_ROWSIZE          (256U)

#define NVMCTRL_USERROW_START_ADDRESS     (0x00804000U)
#define NVMCTRL_USERROW_SIZE              (0x100U)
#define NVMCTRL_USERROW_PAGESIZE          (64U)

#define NVMCTRL_ERROR_NONE 0x0U
#define NVMCTRL_ERROR_PROG 0x4U
#define NVMCTRL_ERROR_LOCK 0x8U
#define NVMCTRL_ERROR_NVM 0x10U
*/

//	Description:
//	  Initializes the NVM module used to update the device flash content
//  Receives:
//  Returns:
void NVM_Init();

// Synthkore  05-05-2023  Tolaemon
// Description:
//  Returns the size of the microcontroller flash memory in bytes
// Receives:
// Returns:
//  By reference:
//  By value:
//   the size of the microcontroller flash memory in bytes.
uint32_t NVM_GetFlashSize();

// Synthkore  21-04-2023  Tolaemon
// Description:
//   Reads and stores in the array pointed by *data the ui32_n_dw_to_read number of bytes
// at the address specified by address.
// Receives:
//   *data: pointer to the array where the values read from flash will be stored
//   ui32_n_dw_to_read: number of 32 bit memory positions to read from flash and store in *data
//   address: the address of flash whose content we want to read and store in *data
// Returns:
//  By reference:
//   *data: pointer to the array filled with the ui32_n_dw_to_read values at the address specified by address
//   By value:
void NVM_ReadFromFlash(uint32_t *data, uint32_t ui32_n_dw_to_read, const uint32_t address );

// Synthkore  22-04-2023  Tolaemon
// Description:

// Receives:
//   *data:
//   ui32_n_dw_to_write:
//   address:
// Returns:
//  By reference:
//   By value:
//    >=0: operation has been succesfully executed.
//    <0: somehting failed while executing operation.
int8_t NVM_WriteInPageBuffer( uint32_t *data, uint32_t ui32_n_dw_to_write, const uint32_t ui32_address);

// Synthkore  22-04-2023  Tolaemon
// Description:
//   Saves the current content in the page buffer to the flash page the received address 
// belongs to. Before calling this commando the page buffer should have been intialized 
// with the content to save in flash.
// Receives:
//   address:
// Returns:
//   By reference:
//   By value:
//    >=0: operation has been succesfully executed.
//    <0: somehting failed while executing operation.
// Note:
// Before writing the page to memory, the accessed block must be erased 
// with the corresponding Erase Block command
int8_t NVM_WritePageBufferToFlash(const uint32_t ui32_address);

// Synthkore  22-04-2023  Tolaemon
// Description:
//   Erases the flash address the received address belongs to. Before writing
// a page to memory, the accessed block must be erased.
// Receives:
//   address:
// Returns:
//   By reference:
//   By value:
//     >=0: operation has been succesfully executed.
//     <0: somehting failed while executing operation.
int8_t NVM_EraseFlashBlock(const uint32_t ui32_address);

// Synthkore  23-04-2023  Tolaemon
// Description:
// Receives an address and erases all blocks, starting from the block that contains
// the received address until the last block in the NVM Flash.
// Receives:
//   ui32_address: the address of the first block to start erasing flash. Blocks before
// the one the address belongs to are not erased.
// Returns:
//   By reference:
//   By value:
//     >=0: operation has been succesfully executed.
//     <0: somehting failed while executing operation.
int8_t NVM_EraseFlashFromAddressToEnd(const uint32_t ui32_address);

// Synthkore  23-04-2023  Tolaemon
// Description:
// Receives an address and unlocks all regions, starting from the region that contains
// the received address until the last region in the NVM Flash.
// Receives:
//   ui32_address: the address of the first region to start unlocking flash. The locked/unlocked
// state of the regions before the one the address belongs to are not modified.
// Returns:
//   By reference:
//   By value:
//     >=0: operation has been succesfully executed.
//     <0: somehting failed while executing operation.
int8_t NVM_UnlockFlashFromAddressToEnd(const uint32_t ui32_address);

// Synthkore  2-05-2023  Tolaemon
// Description:
//    Opens the received hex file and initializes the internal structures used to parse it to
//  begin processing the file.
// Receives:
//     pui8_file_name: pointing to the string with the hex file to open
// Returns:
//   By reference:
//   By value:
//   >=0 the hex file with the received name has been succesfully open
//   <0 something failed when trying to open the hex file with received name
int16_t NVM_HEXFileOpen(uint8_t * pui8_file_name );

// Synthkore  2-05-2023  Tolaemon
// Description:
//    Closes the currently open hex file.
// Receives:
// Returns:
//   By reference:
//   By value:
//   >=0 the currently open hex file has been succesfully closed.
//   <0 something failed when trying to close the current file.
int16_t NVM_HEXFileClose();

// Synthkore  2-05-2023  Tolaemon
// Description:
//    Parses the currently open hex file until it locates the last address 
// defined on it. It helps to know the size of the content defined in the
// currently open hex file.
// Receives:
// Returns:
//  By reference:
//    pui32_lat_abs_addr: the last address defined in the file. 0 if the last
//  address could not be reached.
//  By value:
//   >=0 the last address of the hex file has been found and returned by reference
//   <0 the last address of the hex file could not be reached
int16_t NVM_HEXGetHexLastAddress(uint32_t * pui32_lat_abs_addr);

// Synthkore  2-05-2023  Tolaemon
// Description:
//   Reads and returns the content of the address currently pointed by the internal read file 
// cursor, and then moves that internal read file cursor to point to the next address defined
// in  the file. It also returns the address of the read byte and the position where the read 
// file cursor has been placed. 
//   Before using this function the hex file must be open with the NVM_HEXFileOpen(...) function.
// The function NVM_HEXSeekToByteAddr(...) can also be used to place the file read cursor
// in the desired file address. After having processed the hex file the NVM_HEXFileClose(...)
// must be called.
//
// Receives:
// Returns:
//   By reference:
//     pui8_read_bye: the value of the byte pointed by the internal read file cursor.
//     pui32_read_abs_address: the absolute address of the returned read data byte. 
//     pui32_curr_abs_address: the absolute address where the internal file cursor is
//   pointing after reading and returning the following byte. The position of the read
//   cursor in the file after reading does not necessarily have to be consecutive to 
//   the memory address read, since the file may contain jumps or
// undeclared area.
//   By value:
//    >=0 the currently pointed byte has been succesfully read.
//    -1 despite requested address is in the hex file range it does not exist
//    -2 reached end of file, requested address is out of hex range
// Note:
// The position of the read cursor in the file after reading does not necessarily have 
// to be consecutive to the memory address read, since the file may contain jumps or 
// undeclared areas.
int16_t NVM_HEXReadNextByte(uint8_t * pui8_read_bye,uint32_t * pui32_read_abs_address, uint32_t * pui32_curr_abs_address);

// Synthkore  2-05-2023  Tolaemon
// Description:
//   Moves the internal file cursor used to read hex file content, to point to the data 
//  of the address specified in the ui32_abs_addr_to_seek parameter. 
// Receives:
//   ui32_abs_addr_to_seek: with the address to which the internal read file cursor must 
//  point to.
// Returns:
//  By reference:
//  By value:
//  >=0 the read file cursor has been set pointing to the received address
//  -1 despite requested address is in the hex file address range there is no value defined for that address
//  -2 reached end of file, requested address is out of hex range
int16_t NVM_HEXSeekToByteAddr(uint32_t ui32_abs_addr_to_seek);

// Synthkore  2-05-2023  Tolaemon
//   Reads from the specified hex file the content of the address range defined in the received start and
// end parameters and burns that content in the corresponding address range of the microprocessor memory.
// The routine works page by page, reading the page content of the hex file, then erasing the page in the
// flash memory and then writting that content to flash.
// Description:
//   pui8_file_name: pointing to a string with the name of the hex file to burn to flash.
//   ui32_start_abs_addr:
//   ui32_end_abs_addr:
// Receives:
// Returns:
//   By reference:
//   By value:
int16_t NVM_HEXFileBurnToFlash(uint8_t * pui8_file_name,uint32_t ui32_start_abs_addr, uint32_t ui32_end_abs_addr );

#endif /* NVM_H_ */