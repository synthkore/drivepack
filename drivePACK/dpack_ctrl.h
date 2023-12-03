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
#ifndef _DPACK_CTRL_H
#define _DPACK_CTRL_H

#include <stdio.h>
#include "sam.h"
#include "gpio.h"
#include "sys_defs.h"
#include "./FatFsSrc/ff.h"
#include "aux_funcs.h"


// each ROMPACK address corresponds to 1 nibble, so in 1 memroy byte there 
// will be space for 2 nibbles, for thar reason the size of teh buffer is  
// the highest address read dividied by / 2
#define DPACK_DUMPER_MAX_ADDRESS     0xFFFF // 0x1FFF = 8192 nibbles so 4012 bytes in RAM,  REMEMBER TO ALSO UPDATE UPDATE THE .equ DPACK_DUMPER_MAX_ADDRESS with the same value in dpack_dumper.s
#define MAX_ROM_NIBBLES_BUFFER         0x8000 // 0x1FFF / 2 = 0xFFF ( 2 nibbles stored per byte) that is from byte 0x0000 to 0x0FFF > that is 0x1000 elements. REMEMBER TO ALSO UPDATE THE .equ MAX_DATA_NIBBLE_BUFFER with the same value in dpack_dumper.s
#define MAX_ROM_TITLE_BUFFER           65  // maximum number of characters that can be stored into the ROM TITLE buffer
#define MAX_ROM_SONGS_INFO_BUFFER      360 // maximum number of characters that can be stored into the ROM SONGS INFO buffer
#define MAX_ROM_FILE_NAME              13  // The older MS-DOS FAT file system allows for a maximum of 8 characters in the basic file name and 3 characters in the extension, for a total of 12 characters with the dot separator.
#define MAX_ROM_FILE_PATH              64  // size of the buffer used to store the string with current song path

/*********************************************************************************************
* @brief Initializes the dpack module main data structures
* @note Tolaemon 09-07-2022
*********************************************************************************************/
void DPACK_CTRL_Init();

/*********************************************************************************************
* @brief Checks that the first ROMPACK_HEADER_NUM_BYTES in the nibbles buffer corresponds to
* the expected nibbles for a valid ROM pack. If that bytes are not the expected then it means
* that the nibbles buffer contains invalid data.
* @return  >=0 if the first ROMPACK_HEADER_NUM_BYTES in the nibbles buffer are the expected
* and the content of the nibbles buffer corresponds to a valid ROM pack. <0 the nibbles buffer
* does not correspond to a valid ROM pack.
* @note 03-08-2022  Tolaemon
*********************************************************************************************/
int8_t DPACK_CTRL_check_buffer();

#endif // _DPACK_CTRL_H_
