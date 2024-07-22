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

// states of the state machine used to read the INFO DATA BLOCK with the 
// ROM TITLE, THEMES and other information
#define FSM_LOAD_DRP_XML_UNDEFINED          0
#define FSM_LOAD_DRP_XML_TAG_MARK           1 // a TAG mark '<' has been read
#define FSM_LOAD_DRP_XML_START_TAG          2 // the read TAG is a start TAG <xxx> ( not an END </xxx> TAG )
#define FSM_LOAD_DRP_XML_END_TAG            3 // the read TAG is an END TAG </xxx> ( not a START <xxx> TAG )
#define FSM_LOAD_DRP_XML_ROM_TITLE          4 // reading the content between the DRP_TAG_ROM_TITLE tags
#define FSM_LOAD_DRP_XML_LI_THEMES_TITLES   6 // reading the content between the DRP_TAG_LIST_TITLES tags
#define FSM_LOAD_DRP_XML_THEME_TITLE        7 // reading the content between the DRP_TAG_TITLE tags
#define FSM_LOAD_DRP_XML_ROM_INFO           8 // reading the content between the DRP_TAG_ROM_INFO tags
#define FSM_LOAD_DRP_XML_INFO               9 // reading the content between the DRP_TAG_INFO tags

// tags that delimit the content in the INFO DATA BLOCK
#define DRP_TAG_ROM_INFO     "rom_info"
#define DRP_TAG_ROM_TITLE    "ro_ti"
#define DRP_TAG_LIST_TITLES  "li_ti"
#define DRP_TAG_TITLE        "ti"
#define DRP_TAG_INFO         "info"

#define DRP_DEFAULT_INFO_BLOCK     "<rom_info><ro_ti>Info not initialized</ro_ti><li_ti></li_ti><info></info></rom_info>"
#define DRP_DEFAULT_INFO_BLOCK_LEN 84

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
