// *****************************************************************
// ****                       drivePACK                         ****
// ****                  www.tolaemon.com/dpack                 ****
// ****                       Source code                       ****
// ****                       10/07/2020                        ****
// ****                     Jordi Bartolome                     ****
// ****                                                         ****
// **** IMPORTANT:                                              ****
// **** Using this code or any part of it means accepting all   ****
// **** conditions exposed in: http://www.tolaemon.com/dpack    ****
// *****************************************************************

#ifndef _COMMANDS_H_
#define _COMMANDS_H_

#include "sam.h"
#include "../gpio.h"
#include "../aux_funcs.h"
#include "commands_defs.h"
#include "commands_impl.h"
#include "commands_fs_impl.h"


int16_t COMMANDS_insert_token_in_command(t_command * command,uint8_t * token_string);
void COMMANDS_print_tokens(t_command command);
int16_t COMMANDS_parse_command_string(uint8_t * command_string,  uint16_t ush_MAX_LEN, t_command * command);

// Receives a command, and executes it
// Receives:
//		By reference:
//			the command structure with all the parameters
// Returns:
//		By value:
//		 2 a command cycle has been executed but not completed yet
//		 1 the command execution has been completed
//       0 there is nothing to execute
//		-1 empty command to execute has been received
//		-2 the command has not been identified
//		-3 if any error in the command has ocurred
//		-4 quit application
int16_t COMMANDS_execute(t_command * p_command);


#endif // _COMMANDS_H_