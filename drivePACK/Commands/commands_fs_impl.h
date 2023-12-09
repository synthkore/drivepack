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

#ifndef _COMMANDS_FS_IMPL_H_
#define _COMMANDS_FS_IMPL_H_

#include "sam.h"
#include "../gpio.h"
#include "../aux_funcs.h"
#include "../file_sys.h"
#include "../usart.h"
#include "commands_defs.h"


/*********************************************************************************************
* @brief Procedure that prints the text associated to the received FRESULT error code
*********************************************************************************************/
void COMMANDS_FS_IMPL_print_file_error(int16_t i16_file_result);

/*********************************************************************************************
* @brief Executes the actions related to the disk mount command
* @param[in] p_command the command structure with the parsed tokens
* @return >=0 the command has been succesfully executed
* -1 an error occurred while trying to execute the command
*********************************************************************************************/
int16_t COMMANDS_FS_IMPL_execute_mnt(t_command * p_command);

/*********************************************************************************************
* @brief executes the actions related to the disk dir (list
* available directories) command.
* @param[in] the command structure with the parsed tokens
* @return >=0 the command has been succesfully executed
* <0 an error occurred while trying to execute the command
*********************************************************************************************/
int16_t COMMANDS_FS_IMPL_execute_dir(t_command * p_command);


/*********************************************************************************************
* @brief executes the actions related to the mkdir ( create directory )
* command.
* @param[in] p_command: the command structure with the parsed tokens
* @return >=0 the command has been succesfully executed
* -1 an error occurred while trying to execute the command
*********************************************************************************************/
int16_t COMMANDS_FS_IMPL_execute_make_dir(t_command * p_command);

/*********************************************************************************************
* @brief executes the actions related to the directory change command
* @param[in] the command structure with the parsed tokens
* @return >=0 the command has been succesfully executed
* -1 an error occurred while trying to execute the command
*********************************************************************************************/
int16_t COMMANDS_FS_IMPL_execute_change_dir(t_command * p_command);

/*********************************************************************************************
* @brief executes the actions related to the remove ( delete specified
* element ) command.
* @param[in] the command structure with the parsed tokens
* @return >=0 the command has been succesfully executed
* -1 an error occurred while trying to execute the command
*********************************************************************************************/
int16_t COMMANDS_FS_IMPL_execute_remove(t_command * p_command);

/*********************************************************************************************
* @brief executes the TYPE command and shows the content of the specified file name
* as text
* @param[in] name of the file to show
* @return >=0 the command has been succesfully executed
* -1 an error occurred while trying to execute the command
*********************************************************************************************/
int16_t COMMANDS_FS_IMPL_execute_command_type_txt(uint8_t * pui8_file_name);

/*********************************************************************************************
* @brief execeutes the TYPE command and shows the content of the specified file name
* byte by byte in 0x00 format
* @param[in] pui8_file_name name of the file to show
* @return  >=0 the command has been succesfully executed
*-1 an error occurred while trying to execute the command
*********************************************************************************************/
int16_t COMMANDS_FS_IMPL_execute_command_type_hex(uint8_t * pui8_file_name);

/*********************************************************************************************
* @brief procedure that processes the type command and calls the corresponding
* hex or text type function depending on the received parameters
* @param[in] p_command the command structure with the parsed tokens
* @return >=0 the command has been succesfully executed
* -1 an error occurred while trying to execute the command
*********************************************************************************************/
int16_t COMMANDS_FS_IMPL_execute_command_type(t_command * p_command);

/*********************************************************************************************
* @brief procedure that processes the write command wich creates a file with the
* specified name and with the specified text string content
* @param[in] p_command the command structure with the parsed tokens
* @return  >=0 the command has been executed
* -1 an error occurred while trying to execute the command
*********************************************************************************************/
int16_t COMMANDS_FS_IMPL_execute_command_write(t_command * p_command);

#endif // _COMMANDS_FS_IMPL_H_