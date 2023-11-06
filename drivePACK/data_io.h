// *****************************************************************
// ****                       drivePACK                         ****
// ****                  www.tolaemon.com/dpack                 ****
// ****                       Source code                       ****
// ****                       21/08/2020                        ****
// ****                     Jordi Bartolome                     ****
// ****                                                         ****
// **** IMPORTANT:                                              ****
// **** Using this code or any part of it means accepting all   ****
// **** conditions exposed in: http://www.tolaemon.com/dpack    ****
// *****************************************************************

#include "sam.h"
#include "sys_defs.h"
#include "file_sys.h"
#include "usart.h"
#include "timing.h"

#ifndef _DATA_IO_H_
#define _DATA_IO_H_

#define FILE_DATA_BLOCK_TITLE            0x01
#define FILE_DATA_BLOCK_SONGS_INFO       0x02
#define FILE_DATA_BLOCK_SONGS_ROM        0x03

/*********************************************************************************************
* @brief Procedure that reads and loads to RAM specified ROM file.
* @param[in] pui8_file_name: name of the file to load
* @return  >=0 the command has been succesfully executed, <0 an error occurred while trying 
* to execute the command:
*    -1: File open error,
*    -2: Error in ROM PACK file format / version,
*    -3: Error while reading from file
*********************************************************************************************/
int16_t DATA_IO_file_rom_load(uint8_t * pui8_file_name );

/*********************************************************************************************
* @brief Procedure that writes to the specified file the content of RAM
* @param[in] pui8_file_name: name of the file to write
* @return >=0 the command has been succesfully executed, <0 an error occurred while trying to
* execute the command: 
*    -1: File open error,
*    -2: Error in ROM PACK file format / version
*    -3: Error while reading from file
*********************************************************************************************/
int16_t DATA_IO_file_rom_write(uint8_t * pui8_file_name );

/*********************************************************************************************
* @brief  Procedure that loads to RAM the ROM file received through 1KXModem. It uses the
* 1kXmodem connection to receive and load to RAM a ROM file.
* @return >=0 if no error, <0 if error:
*    USART_1KX_EOT: End Of Transmission char code received from sender
*    USART_1KX_PACKET_OK: current packet has been completely and successfully received
*    USART_1KX_TOUT_ERR: timeout receiving the packet content ( no bytes received during the configured timeout period ).
*    USART_1KX_HEADER_ERR: error on received header ( wrong packet number )
*    USART_1KX_CRC_ERR: received wrong packet CRC
*********************************************************************************************/
int16_t DATA_IO_1KXmodem_rom_receive();

/*********************************************************************************************
* @brief Procedure that sends through 1KXModem the ROM loaded into RAM. It uses the 1kXmodem 
* connection to send the bytes of the ROM loaded into RAM.
* @return >=0 if no error, <0 if error:
*	 USART_1KX_EOT: End Of Transmission char code received from sender
*	 USART_1KX_PACKET_OK: current packet has been completely and successfully received
*	 USART_1KX_TOUT_ERR: timeout receiving the packet content ( no bytes received during the configured timeout period ).
*	 USART_1KX_HEADER_ERR: error on received header ( wrong packet number )
*	 USART_1KX_CRC_ERR: received wrong packet CRC
*********************************************************************************************/
int16_t DATA_IO_1KXmodem_rom_send();

#endif // _DATA_IO_H_
