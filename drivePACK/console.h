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

void CONSOLE_Init();
void CONSOLE_enable();
void CONSOLE_disable();
void CONSOLE_reset();
int16_t CONSOLE_check_input(uint8_t * command_string);
int16_t CONSOLE_enqueue_command_string(uint8_t * command_to_store);
int16_t CONSOLE_get_next_queue_command_string(uint8_t* next_command);
int16_t CONSOLE_get_prev_queue_command_string(uint8_t* prev_command);

#endif //__CONSOLE_H__