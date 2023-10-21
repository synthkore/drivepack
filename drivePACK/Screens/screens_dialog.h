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
#ifndef _SCREENS_DIALOG_H_
#define _SCREENS_DIALOG_H_

#include "screens.h" 

#define SCREENS_DIALOG_BACKGROUND_CHAR       152

// list of available dialog screens IDs to load to
#define SCREEN_DIALOG_ID_DEFAULT            0
#define SCREEN_DIALOG_ID_INTRO			    1
#define SCREEN_DIALOG_ID_LOAD_ROM_FILE		2
#define SCREEN_DIALOG_ID_LOAD_RUN_ROM_FILE	3
#define SCREEN_DIALOG_ID_RUN_RAM	        4
#define SCREEN_DIALOG_ID_SHOW_RAM_INFO      5
#define SCREEN_DIALOG_ID_DUMP_ROM           6
#define SCREEN_DIALOG_ID_SAVE_RAM_AS_FILE   7
#define SCREEN_DIALOG_ID_SAVE_RAM_FILE      8
#define SCREEN_DIALOG_ID_RECEIVE_RUN_RAM    9
#define SCREEN_DIALOG_ID_RECEIVE_RAM        10
#define SCREEN_DIALOG_ID_SEND_RAM           11
#define SCREEN_DIALOG_ID_TEST               12

// First state of all dialogs
#define SCREEN_DIALOG_INITIAL_STATE         0

// Dialog Screen LOAD_FILE states
#define SCREEN_DIALOG_LOAD_ROM_FILE_MOUNTING              1
#define SCREEN_DIALOG_LOAD_ROM_FILE_LOADING_FILE          2
#define SCREEN_DIALOG_LOAD_ROM_FILE_LOADED                4
#define SCREEN_DIALOG_LOAD_ROM_FILE_ACCESS_ERROR          5
#define SCREEN_DIALOG_LOAD_ROM_FILE_FORMAT_VER_ERROR      6
#define SCREEN_DIALOG_LOAD_ROM_FILE_CANCELLED             7

// Dialog Screen LOAD_RUN_ROM_FILE states
#define SCREEN_DIALOG_LOAD_RUN_ROM_FILE_MOUNTING          1
#define SCREEN_DIALOG_LOAD_RUN_ROM_FILE_LOADED            2
#define SCREEN_DIALOG_LOAD_RUN_ROM_FILE_RUNNING_FILE      3
#define SCREEN_DIALOG_LOAD_RUN_ROM_FILE_ACCESS_ERROR      4
#define SCREEN_DIALOG_LOAD_RUN_ROM_FILE_FORMAT_VER_ERROR  5
#define SCREEN_DIALOG_LOAD_RUN_ROM_FILE_CANCELLED         6

// Dialog Screen RUN_RAM states
#define SCREEN_DIALOG_RUN_RAM_CHECK                       1
#define SCREEN_DIALOG_RUN_RAM_RUNNING                     2
#define SCREEN_DIALOG_RUN_RAM_NO_LOADED_ERROR             3

// Dialog Screen SHOW_RAM states
#define SCREEN_DIALOG_SHOW_RAM_INFO_CHECK                 1
#define SCREEN_DIALOG_SHOW_RAM_INFO_SHOWING_1             2
#define SCREEN_DIALOG_SHOW_RAM_INFO_SHOWING_2             3
#define SCREEN_DIALOG_SHOW_RAM_INFO_NO_LOADED_ERROR       4
#define SCREEN_DIALOG_SHOW_RAM_INFO_NO_FILE_ERROR               5

// Dialog Screen DUMP ROM states
#define SCREEN_DIALOG_DUMP_ROM_PREPARE                    1
#define SCREEN_DIALOG_DUMP_ROM_DUMPING                    2
#define SCREEN_DIALOG_DUMP_ROM_DUMPED                     3
#define SCREEN_DIALOG_DUMP_ROM_CHECK_ERROR                4
#define SCREEN_DIALOG_DUMP_ROM_CANCELLED                  5

// Dialog Screen SAVE RAM AS states
#define SCREEN_DIALOG_SAVE_RAM_AS_FILE_CHECK              1
#define SCREEN_DIALOG_SAVE_RAM_AS_FILE_MOUNTING           2
#define SCREEN_DIALOG_SAVE_RAM_AS_FILE_SAVING_FILE        3
#define SCREEN_DIALOG_SAVE_RAM_AS_FILE_SAVED              4
#define SCREEN_DIALOG_SAVE_RAM_AS_FILE_ACCESS_ERROR       5
#define SCREEN_DIALOG_SAVE_RAM_AS_FILE_SAVING_ERROR       6
#define SCREEN_DIALOG_SAVE_RAM_AS_FILE_CANCELLED          7
#define SCREEN_DIALOG_SAVE_RAM_AS_NO_LOADED_ERROR         8

// Dialog Screen SAVE RAM states
#define SCREEN_DIALOG_SAVE_RAM_FILE_CHECK             1
#define SCREEN_DIALOG_SAVE_RAM_FILE_MOUNTING          2
#define SCREEN_DIALOG_SAVE_RAM_FILE_SAVING_FILE       3
#define SCREEN_DIALOG_SAVE_RAM_FILE_SAVED             4
#define SCREEN_DIALOG_SAVE_RAM_FILE_ACCESS_ERROR      5
#define SCREEN_DIALOG_SAVE_RAM_FILE_SAVING_ERROR      6
#define SCREEN_DIALOG_SAVE_RAM_FILE_CANCELLED         7
#define SCREEN_DIALOG_SAVE_RAM_FILE_NO_FILE_ERROR     8
#define SCREEN_DIALOG_SAVE_RAM_FILE_NO_LOADED_ERROR   9

// Dialog Screen RECEIVE RAM buffer states
#define SCREEN_DIALOG_RECEIVE_RUN_RAM_WAITING             1
#define SCREEN_DIALOG_RECEIVE_RUN_RAM_RECEIVING           2
#define SCREEN_DIALOG_RECEIVE_RUN_RAM_RECEIVED            3
#define SCREEN_DIALOG_RECEIVE_RUN_RAM_TIMEOUT_ERROR       4
#define SCREEN_DIALOG_RECEIVE_RUN_RAM_PROCESS_ERROR       5
#define SCREEN_DIALOG_RECEIVE_RUN_RAM_CANCELLED           6
#define SCREEN_DIALOG_RECEIVE_RUN_RAM_RUNNING             7

// Dialog Screen RECEIVE RAM buffer states
#define SCREEN_DIALOG_RECEIVE_RAM_WAITING             1
#define SCREEN_DIALOG_RECEIVE_RAM_RECEIVING           2
#define SCREEN_DIALOG_RECEIVE_RAM_RECEIVED            3
#define SCREEN_DIALOG_RECEIVE_RAM_TIMEOUT_ERROR       4
#define SCREEN_DIALOG_RECEIVE_RAM_PROCESS_ERROR       5
#define SCREEN_DIALOG_RECEIVE_RAM_CANCELLED           6

// Dialog Screen SEND RAM buffer states
#define SCREEN_DIALOG_SEND_RAM_CHECK                  1
#define SCREEN_DIALOG_SEND_RAM_WAITING                2
#define SCREEN_DIALOG_SEND_RAM_SENDING                3
#define SCREEN_DIALOG_SEND_RAM_SENT                   4
#define SCREEN_DIALOG_SEND_RAM_TIMEOUT_ERROR          5
#define SCREEN_DIALOG_SEND_RAM_PROCESS_ERROR          6
#define SCREEN_DIALOG_SEND_RAM_NO_LOADED_ERROR        7
#define SCREEN_DIALOG_SEND_RAM_CANCELLED              8

// Dialog Screen TEST states
#define SCREEN_DIALOG_TEST_DEAD_PIXELS                1
#define SCREEN_DIALOG_TEST_COLORS                     2
#define SCREEN_DIALOG_TEST_CTRLS                      3
#define SCREEN_DIALOG_TEST_DISK_UNIT_START            4
#define SCREEN_DIALOG_TEST_DISK_UNIT                  5
#define SCREEN_DIALOG_TEST_DISK_UNIT_OK               6
#define SCREEN_DIALOG_TEST_DISK_UNIT_ERROR            7
#define SCREEN_DIALOG_TEST_LED_GREEN                  8
#define SCREEN_DIALOG_TEST_LED_YELLOW                 9
#define SCREEN_DIALOG_TEST_LED_RED                   10
#define SCREEN_DIALOG_TEST_USB_COMS                  11

// Dialog Screen INTRO states // JBR 2022-12-27 Esta esto bien ?
#define SCREEN_DIALOG_INTRO_INIT                1
#define SCREEN_DIALOG_INTRO_NOT_CONTINUE        2
#define SCREEN_DIALOG_INTRO_CONTINUE            3
#define SCREEN_DIALOG_INTRO_FINISHED            4

#define SCREEN_DIALOG_SHOW_FAST_MESSAGE_TIMEOUT       TIMMING_TICKS_500_MS// Fast message show timeout
#define SCREEN_DIALOG_SHOW_SLOW_MESSAGE_TIMEOUT       TIMMING_TICKS_2_S// Slow message show timeout
#define SCREEN_DIALOG_SHOW_SLOWEST_MESSAGE_TIMEOUT    TIMMING_TICKS_4_S// Slowest message show timeout

struct ref_screens_dialog{
	int8_t (*load_cur_function)(); // pointer to the function that loads the information of the current dialog screen
	int8_t (*show_cur_function)(uint8_t *); // pointer to the function that shows the information corresponding to the current dialog screen
	int8_t (*ev_process_cur_function)(int16_t * , uint8_t * ); // pointer to the function that processes the events on the current dialog screen
	int8_t i8_state; // with the current state of the screen dialog
	TIMING_TICKS timer;
};


// Synthkore  11-10-2014  Tolaemon
// Clears all the information of the current parameters edition screen
// Receives:
// Returns:
//  By value:
void SCREENS_dialog_clear();

// Synthkore  27-04-2022  Tolaemon
// Initializes the module variables on program start.
// Receives:
// Returns:
//  By value:
void SCREENS_dialog_init();

// Synthkore  11-10-2022  Tolaemon
// Receives:
// Returns:
//  By value:
int8_t SCREENS_dialog_load(int16_t i16_received_dialog_id,int16_t i16_dialog_state);

// Synthkore  24-10-2014  Tolaemon
// Receives:
//    portSHORT * encoders_var_value: pointing to the array with the variation of value of the encoders
//    portCHAR * pushbutton_values: pointing to the array with the state of all the push buttons
// Returns:
//  By value:
void SCREENS_dialog_ev_manager(int16_t * pi16_encoders_var_value, uint8_t * pui8_pushbutton_values);

// Synthkore  31-10-2022  Tolaemon
// Prints in the screen the information of the file that is loaded in RAM buffer
// Receives:
// Returns:
//  By value:
void SCREENS_print_current_rom_file_info();

// Dialog screen specific callback functions

// LOAD_FILE dialog
int8_t SCREENS_dialog_load_LOAD_ROM_FILE();
int8_t SCREENS_dialog_show_LOAD_ROM_FILE(uint8_t * ui8_message);
int8_t SCREENS_dialog_ev_manager_LOAD_ROM_FILE(int16_t * pi16_encoders_var_value, uint8_t * pui8_pushbutton_values);

// LOAD_RUN_FILE dialog
int8_t SCREENS_dialog_load_LOAD_RUN_ROM_FILE();
int8_t SCREENS_dialog_show_LOAD_RUN_ROM_FILE(uint8_t * ui8_message);
int8_t SCREENS_dialog_ev_manager_LOAD_RUN_ROM_FILE(int16_t * pi16_encoders_var_value, uint8_t * pui8_pushbutton_values);

// INTRO dialog
int8_t SCREENS_dialog_load_INTRO();
int8_t SCREENS_dialog_show_INTRO(uint8_t * ui8_message);
int8_t SCREENS_dialog_ev_manager_INTRO(int16_t * pi16_encoders_var_value, uint8_t * pui8_pushbutton_values);

// RUN_RAM buffer content dialog
int8_t SCREENS_dialog_ev_manager_RUN_RAM(int16_t * pi16_encoders_var_value, uint8_t * pui8_pushbutton_values);
int8_t SCREENS_dialog_show_RUN_RAM(uint8_t * ui8_message);
int8_t SCREENS_dialog_load_RUN_RAM();

// SHOW_RAM_INFO in memory dialog
int8_t SCREENS_dialog_load_SHOW_RAM_INFO();
int8_t SCREENS_dialog_show_SHOW_RAM_INFO(uint8_t * ui8_message);
int8_t SCREENS_dialog_ev_manager_SHOW_RAM_INFO(int16_t * pi16_encoders_var_value, uint8_t * pui8_pushbutton_values);

// DUMP ROM to memory dialog
int8_t SCREENS_dialog_load_DUMP_ROM();
int8_t SCREENS_dialog_ev_manager_DUMP_ROM(int16_t * pi16_encoders_var_value, uint8_t * pui8_pushbutton_values);
int8_t SCREENS_dialog_show_DUMP_ROM(uint8_t * ui8_message);

// SAVE RAM AS to FILE dialog
int8_t SCREENS_dialog_load_SAVE_RAM_AS_FILE();
int8_t SCREENS_dialog_ev_manager_SAVE_RAM_AS_FILE(int16_t * pi16_encoders_var_value, uint8_t * pui8_pushbutton_values);
int8_t SCREENS_dialog_show_SAVE_RAM_AS_FILE(uint8_t * ui8_message);

// SAVE RAM to FILE dialog
int8_t SCREENS_dialog_load_SAVE_RAM_FILE();
int8_t SCREENS_dialog_ev_manager_SAVE_RAM_FILE(int16_t * pi16_encoders_var_value, uint8_t * pui8_pushbutton_values);
int8_t SCREENS_dialog_show_SAVE_RAM_FILE(uint8_t * ui8_message);

// RECEIVE RUN RAM dialog
int8_t SCREENS_dialog_load_RECEIVE_RUN_RAM();
int8_t SCREENS_dialog_ev_manager_RECEIVE_RUN_RAM(int16_t * pi16_encoders_var_value, uint8_t * pui8_pushbutton_values);
int8_t SCREENS_dialog_show_RECEIVE_RUN_RAM(uint8_t * ui8_message);

// RECEIVE RAM dialog
int8_t SCREENS_dialog_load_RECEIVE_RAM();
int8_t SCREENS_dialog_ev_manager_RECEIVE_RAM(int16_t * pi16_encoders_var_value, uint8_t * pui8_pushbutton_values);
int8_t SCREENS_dialog_show_RECEIVE_RAM(uint8_t * ui8_message);

// SEND RAM dialog
int8_t SCREENS_dialog_load_SEND_RAM();
int8_t SCREENS_dialog_show_SEND_RAM(uint8_t * ui8_message);
int8_t SCREENS_dialog_ev_manager_SEND_RAM(int16_t * pi16_encoders_var_value, uint8_t * pui8_pushbutton_values);

// TEST dialog
int8_t SCREENS_dialog_load_TEST();
int8_t SCREENS_dialog_show_TEST(uint8_t * ui8_message);
int8_t SCREENS_dialog_ev_manager_TEST(int16_t * pi16_encoders_var_value, uint8_t * pui8_pushbutton_values);

#endif /* _SCREENS_DIALOG_H_ */