// *****************************************************************
// ****                       drivePACK                         ****
// ****                  www.tolaemon.com/dpack                 ****
// ****                       Source code                       ****
// ****                       8/05/2022                        ****
// ****                     Jordi Bartolome                     ****
// ****                                                         ****
// **** IMPORTANT:                                              ****
// **** Using this code or any part of it means accepting all   ****
// **** conditions exposed in: http://www.tolaemon.com/dpack    ****
// *****************************************************************

#ifndef _COMMANDS_IMPL_H_
#define _COMMANDS_IMPL_H_

#include "sam.h"
#include "../gpio.h"
#include "../sys_defs.h"
#include "../usart.h"
#include "../FatFsSrc/ff.h"
#include "../aux_funcs.h"
#include "../dpack_ctrl.h"
#include "../data_io.h"
#include "commands_defs.h"


// Executes the help command: shows information of available commands
// Recives:
//	p_command: the command structure with the parsed tokens
// Returns:
//	By value:
//   >=0 the command has been succesfully executed
//	 -1 an error occurred while trying to execute the command
int16_t COMMANDS_IMPL_execute_help(t_command * p_command);

// Executes the gpio command which allows testing the system I/O lines
// Receives:
//	p_command: the command structure with the parsed tokens
// Returns:
//	By value:
//   >=0 the command has been succesfully executed
//	 -1 an error occurred while trying to execute the command
int16_t COMMANDS_IMPL_execute_gpio(t_command * p_command);

// Executes the info command: shows system general information
//   SW_NAME, SW_VERSION ...
// Receives:
//	p_command: the command structure with the parsed tokens
// Returns:
//	By value:
//   >=0 the command has been succesfully executed
//	 -1 an error occurred while trying to execute the command
int16_t COMMANDS_IMPL_execute_info(t_command * p_command);

// Executes the steps to receive a rom file from the USB-UART connection
// from a remote computer using the 1KXModem protocol
// Receives:
//	p_command: the command structure with the parsed tokens
// Returns:
//	By value:
//   >=0 the command has been succesfully executed
//	 -1 an error occurred while trying to execute the command
int16_t COMMANDS_IMPL_execute_command_rom_rx1k(t_command * p_command);

// Executes the steps to send the rom file in memory through the USB-UART 
// connection to remote computer using the 1KXModem protocol
// Receives:
//	p_command: the command structure with the parsed tokens
// Returns:
//	By value:
//   >=0 the command has been succesfully executed
//	 -1 an error occurred while trying to execute the command
int16_t COMMANDS_IMPL_execute_command_rom_tx1k(t_command * p_command);

// Procedure that processes the rom write command: writes the
// content of the memory buffer to the specified file.
// Receives:
//	p_command: the command structure with the parsed tokens
// Returns:
//	By value:
//   >=0 the command has been succesfully executed
//	 -1 an error occurred while trying to execute the command
//
// File structure:
// 11 bytes: TAG : ROMPACKv00+'\0' : format and version identification
// 04 bytes: ROM_START: uint32 : content start file offset
// n bytes: FILE INFO: text line until '\0' is found: file text data
// n bytes: METADATA
// n bytes: ROM_PACK_CONTENT: rompack content starts at @ROM_START, each byte correspond to 2 data nibbles
int16_t COMMANDS_IMPL_execute_command_rom_write(t_command * p_command);

// Procedure that processes the rom dump command: calls the assembly procedure that
// reads the ROM PACK content and stores it into the RAM buffer.
// Receives:
//	p_command: the command structure with the parsed tokens
// Returns:
//	By value:
//   >=0 the command has been succesfully executed
//	 -1 an error occurred while trying to execute the command
int16_t COMMANDS_IMPL_execute_command_rom_dump(t_command * p_command);

// Procedure that processes the rom load command from command line
// Receives:
//	p_command: the command structure with the parsed tokens
// Returns:
//	By value:
//   >=0 the command has been succesfully executed
//	 <0 an error occurred while trying to execute the command
int16_t COMMANDS_IMPL_execute_command_rom_load(t_command * p_command);

// Procedure that processes the rom type command: shows the content
// of the memory ROM PACK buffer.
// Receives:
//	p_command: the command structure with the parsed tokens
// Returns:
//	By value:
//   >=0 the command has been succesfully executed
//	 -1 an error occurred while trying to execute the command
int16_t COMMANDS_IMPL_execute_command_rom_type(t_command * p_command);

// Procedure that processes the rom run command: the run command
// sets the system in the ROM PACK emulation mode. The content of
// the ROM PACK memory buffer should have been previously initialized
// before calling this command.
// Receives:
//	p_command: the command structure with the parsed tokens
// Returns:
//	By value:
//   >=0 the command has been succesfully executed
//	 -1 an error occurred while trying to execute the command
int16_t COMMANDS_IMPL_execute_command_rom_run(t_command * p_command);

// Procedure that compares the first bytes in the ROM PACK memory buffer that
// contain the ROM PACK header bytes with the expected header bytes and prints
// the result of the comparison.
// Receives:
//	p_command: the command structure with the parsed tokens
// Returns:
//	By value:
//   >=0 the command has been succesfully executed
//	 -1 an error occurred while trying to execute the command
int16_t COMMANDS_IMPL_execute_command_check(t_command * p_command);

// Procedure that processes the rom info command: shows the content
// of the TITLE and SONG INFO rom fields stored in memory. 
// Receives:
//	p_command: the command structure with the parsed tokens
// Returns:
//	By value:
//   >=0 the command has been succesfully executed
//	 -1 an error occurred while trying to execute the command
int16_t COMMANDS_IMPL_execute_command_rom_info(t_command * p_command);

// Procedure that processes the rom command and calls the corresponding
// rom function depending on the received parameters
// Receives:
//	p_command: the command structure with the parsed tokens
// Returns:
//	By value:
//   >=0 the command has been succesfully executed
//	 -1 an error occurred while trying to execute the command
int16_t COMMANDS_IMPL_execute_rom(t_command * p_command);

#endif // _COMMANDS_IMPL_H_ 