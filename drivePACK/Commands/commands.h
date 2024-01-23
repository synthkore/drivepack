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
// **** This code is part of the bareSAM platform. All info at: ****
// **** http://www.tolaemon.com/baresam                         ****
// *****************************************************************

#ifndef _COMMANDS_H_
#define _COMMANDS_H_

#include "sam.h"
#include "../gpio.h"
#include "../aux_funcs.h"
#include "commands_defs.h"
#include "commands_impl.h"
#include "commands_fs_impl.h"


/*********************************************************************************************
* @brief receives a command, and executes it
* @param[in] the command structure with all the parameters
* @return 2 a command cycle has been executed but not completed yet
*  1 the command execution has been completed
*  0 there is nothing to execute
* -1 empty command to execute has been received
* -2 the command has not been identified
* -3 if any error in the command has ocurred
* -4 quit application
*********************************************************************************************/
int16_t COMMANDS_execute(t_command * p_command);

/*********************************************************************************************
* @brief clears the content ( all the tokens ) and variables of the received command
* @param[in|out] command pointing to the command structure to clear
* @return
* 1 the command has been cleared
* 0 it has not been possible to clear the command
*********************************************************************************************/
int16_t COMMANDS_clear_command(t_command * command);

/*********************************************************************************************
* @brief inserts a token into a command structure
* @param[in] token_string a string with the token to insert into the command structure
* @param[in|out] command the command structure where the toke will be inserted
* @return 1 if the token is inserted to the command
* 0 if the token can't be inserted to the command
*********************************************************************************************/
int16_t COMMANDS_insert_token_in_command(t_command * command, uint8_t * token_string);

/*********************************************************************************************
* @brief prints all the tokens contained in the received command structure
* @param[in] command the command with the tokens to print
*********************************************************************************************/
void COMMANDS_print_tokens(t_command command);

/*********************************************************************************************
* @brief Procedure which takes each of the tokens contained in the received command string and inserts
* them into a command structure. Then the command structure is stored into the commands Lifo
* @param[in] command_string with the buffer of pressed keys
* @param[in] command_string_index the first free position in the buffer
* @param[in] ush_MAX_LEN the maximum allowed length of the command string.
* @param[out] command struct with the command tokens of the processed command string
* @return 1 if the tokens contained in the buffer have been inserted succesfully
*  into the command structure
* -1 the buffer is empty.
* -2 if the buffer contains an invalid token sequence
* -3 there is no space for new commands
*********************************************************************************************/
int16_t COMMANDS_parse_command_string(uint8_t * command_string,  uint16_t ush_MAX_LEN, t_command * command);

/*********************************************************************************************
* @brief Receives a command, and executes it
* @param[in] the command structure with all the parameters
* @return 2 a command cycle has been executed but not completed yet
*  1 the command execution has been completed
*  0 there is nothing to execute
* -1 empty command to execute has been received
* -2 the command has not been identified
* -3 if any error in the command has ocurred
* -4 quit application
*********************************************************************************************/
int16_t COMMANDS_execute(t_command * p_command);

#endif // _COMMANDS_H_