// *****************************************************************
// ****                       drivePACK                         ****
// ****                  www.tolaemon.com/dpack                 ****
// ****                       Source code                       ****
// ****                       10/11/2021                        ****
// ****                     Jordi Bartolome                     ****
// ****                                                         ****
// **** IMPORTANT:                                              ****
// **** Using this code or any part of it means accepting all   ****
// **** conditions exposed in: http://www.tolaemon.com/dpack    ****
// **** This code is part of the bareSAM platform. All info at: ****
// **** http://www.tolaemon.com/baresam                         ****
// *****************************************************************
#ifndef __CONSOLE_H__
#define __CONSOLE_H__

#include "sam.h"
#include "aux_funcs.h"
#include "usart.h"
#include "timing.h"
#include "./Commands/commands.h"


#define MAX_LINE_CONSOLE_BUFFER   64 //maximum length of keys buffer
#define MAX_ESCAPE_SEQUENCE_CHARS 8  //maximum allowed characters in a special escape sequence
#define EMPTY_STRING ""
#define MAX_COMMANDS_QUEUE 4 // number of commands that can be stored into the last commands queue.
#define CONSOLE_PROMPT_STRING "\r\n>>"

#define CONSOLE_SCAPE_SEQUENCE_TOUT 2000

// console characters types
// key code types
#define CONSOLE_KEY_TYPE_UNKNOWN           0 // ¿?¿?¿?¿?
#define CONSOLE_KEY_TYPE_TEXT              1 // i.e: 'a' 'b' '1' '2' '?' ...
#define CONSOLE_KEY_TYPE_CTRL              2 // i.e: RETURN, BACK, DEL
#define CONSOLE_KEY_TYPE_ESCAPE_SEQUENCE   3 // i.e: RIGHT KEY, LEFT KEY... ( special keys received through escape sequence ).

// CONSOLE_KEY_TYPE_TEXT keys
#define CONSOLE_KEY_UNKNWON                1

// CONSOLE_KEY_TYPE_CTRL keys
#define CONSOLE_KEY_CTRL_RETURN            1
#define CONSOLE_KEY_CTRL_BACK              2
#define CONSOLE_KEY_CTRL_DEL               3

// ESCAPE SEQUENCE keys
#define CONSOLE_KEY_ESC_SEQ_UNKNWON        1
#define CONSOLE_KEY_ESC_SEQ_LEFT           2
#define CONSOLE_KEY_ESC_SEQ_RIGHT          3
#define CONSOLE_KEY_ESC_SEQ_UP             4
#define CONSOLE_KEY_ESC_SEQ_DOWN           5
#define CONSOLE_KEY_ESC_SEQ_F1             6
#define CONSOLE_KEY_ESC_SEQ_HOME           7

// KEY CODES
#define CONSOLE_KEY_CODE_BACK                0x08
#define CONSOLE_KEY_CODE_RETURN              0x0D
#define CONSOLE_KEY_CODE_DELETE              0x1F
#define CONSOLE_KEY_CODE_ESC_SEQUENCE_START  0x1B

/*********************************************************************************************
* @brief Initializes parser and console variables
* @note Tolaemon 21/08/2020
*********************************************************************************************/
void CONSOLE_Init();

/*********************************************************************************************
* @brief Enables the CONSOLE module. When the CONSOLE module is enabled the module functions
* are executed on each cycle. When the CONSOLE module is not enabled the module functions
* are not executed on each cycle.
* @note Tolaemon 21/08/2020
*********************************************************************************************/
void CONSOLE_enable();

/*********************************************************************************************
* @brief Disables the CONSOLE module. When the CONSOLE module is enabled the module functions
* are executed on each cycle. When the CONSOLE module is NOT enabled the module functions
* are not executed on each cycle, so the console is not executed.
* @note Tolaemon 21/08/2020
*********************************************************************************************/
void CONSOLE_disable();

/*********************************************************************************************
* @brief Initializes parser and console variables
* @note Tolaemon 21/08/2020
*********************************************************************************************/
void CONSOLE_reset();

/*********************************************************************************************
* @brief Takes the received char code and finds out the associated key and key type
* @param[out] pui8_received_key_type pointing to the type of the decoded key ( normal character
* key, control key ...)
* @param[out] pui8_received_key pointing to the decoded key identifier
* @return >0 a key has been decoded, <=0 no key has been decoded
* @note Tolaemon 21/08/2020
*********************************************************************************************/
int16_t CONSOLE_decode_key_from_char(uint8_t ui8_usart_char, uint8_t * pui8_received_key_type, uint8_t * pui8_received_key);

/*********************************************************************************************
* @brief It uses ANSI Escape Codes to delete all characters from end of line to current cursor
* position in the same line, and then rewrites the line from the current cursor position
* to the end of the line.
* @note Tolaemon 21/08/2020
*********************************************************************************************/
void CONSOLE_rewrite_from_cursor_to_end();

/*********************************************************************************************
* @brief Processes the received keys and prepares the command corresponding to the received keys
* @param[in] ui8_received_char_type the type of the key to process ( normal character key, control
* key ...)
* @param[in] ui8_received_char the character or code of the key to process
* @param[out] command_string: when the whole command string has been received, a string with the
* whole contentread from the keyboard.
* @return 1 a command string has been received and processed, !=1 no command string completed
*********************************************************************************************/
int16_t CONSOLE_process_received_keys(uint8_t ui8_received_key_type, uint8_t ui8_received_key, uint8_t * command_string);

/*********************************************************************************************
* @brief Is the console main function. Each time a character is received it is stored into the
* KEYBOARD BUFFER, and when then CONSOLE_END_KEY key is pressed, the KEYBOARD BUFFER content is
* stored into a string and returned to the main program in order to let it parse it into a command.
* This routines distinguishes between single byte characters ( normal ASCII characters ) and
* multiple byte characters special sequences ( escape sequence characters ).
* @param[out] When the whole command string has been received, a string with the whole content
* read from the keyboard
* @return 1 a complete command string has been received ( CONSOLE_KEY_CTRL_RETURN character ),
* -1  no complete command string has been received yet, -2 when the QUIT key has been detected
* @note Tolaemon  06-11-2021
*********************************************************************************************/
int16_t CONSOLE_check_input(uint8_t * command_string);

/*********************************************************************************************
* @brief Stores a new command string into the the console command strings queue to allow the
* user to use that command string again.
* @return 1 the command string has been enqueued,-1 the command string  has not been enqueued
* @note Tolaemon 21/08/2020
*********************************************************************************************/
int16_t CONSOLE_enqueue_command_string(uint8_t * command_to_store);

/*********************************************************************************************
* @brief  Returns the next command string in the queue of entered command strings
* @param[out]  The string with the next command in the queue
* @return 1 if the next command has been delivered,  -1 if there are no commands in the queue
* @note Tolaemon 21/08/2020
*********************************************************************************************/
int16_t CONSOLE_get_next_queue_command_string(uint8_t* next_command);

/*********************************************************************************************
* @brief Returns the previous command string in the queue of entered command strings
* @param[out] The string with the previous command in the queue
* @return 1 if the prev command has been delivered, -1 if there are no commands in the queue
* @note Tolaemon 21/08/2020
*********************************************************************************************/
int16_t CONSOLE_get_prev_queue_command_string(uint8_t* prev_command);

#endif //__CONSOLE_H__