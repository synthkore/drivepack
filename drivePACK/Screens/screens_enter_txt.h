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

#ifndef _SCREENS_ENTER_TXT_H_
#define _SCREENS_ENTER_TXT_H_

#include "screens.h"

#define MAX_ENTER_TEXT_LEN 22

#define SCREENS_ENTER_TXT_BACKGROUND_CHAR     152

#define SCREEN_ENTER_TXT_BLINK_TIMEOUT        TIMMING_TICKS_400_MS

// list of enter txt operations
#define SCREEN_ENTER_TXT_STANDARD              0
#define SCREEN_ENTER_TXT_FILE_TO_SAVE          1

// First state of all enter txt screens
#define SCREEN_ENTER_TXT_INITIAL_STATE             0

// SAVE FILE enter name txt screen states
#define SCREEN_ENTER_TXT_FILE_TO_SAVE_EDITING      1


struct ref_screens_enter_txt{
	int8_t (*load_cur_function)(); // pointer to the function that loads the specific information of the current text enter screen
	int8_t (*show_cur_function)(uint8_t *); // pointer to the function that shows the specific information of the current text enter screen
	int8_t (*ev_process_cur_function)(int16_t * , uint8_t * ); // pointer to the function that processes the events specific of the current text enter screen	
	int8_t i8_state;// indicates the state of the enter text screen
	int8_t  i8_cursor_blink_flag;// indicates the state of the edition cursor '_' or char
	uint8_t ui8_max_allowed_len; // maximum characters that the user can enter
	uint8_t ui8_text[MAX_ENTER_TEXT_LEN];
	TIMING_TICKS timer_blink;
};


// Synthkore  18-02-2022  Tolaemon
// Receives:
// Returns:
//  By value:
void SCREENS_enter_txt_clear();

// Synthkore  27-04-2022  Tolaemon
// Initializes the module variables on program start.
// Receives:
// Returns:
//  By value:
void SCREENS_enter_txt_init();

// Synthkore  18-02-2022  Tolaemon
// Loads and initializes the enter text screen structure that corresponds to the received
// i16_enter_txt_id
// Receives:
//   i16_enter_txt_id: indicates the enter text screen to load
//   i16_enter_text_state:  indicates the substate of the specified enter text screen
// Returns:
//  By value:
int8_t SCREENS_enter_txt_load(int16_t i16_enter_text_id,int16_t i16_enter_text_state);

// Synthkore  18-02-2022  Tolaemon
// Shows the current enter text screen.
// Receives:
//   i16_f_explorer_id: indicates the type of fire explorer to load
// Returns:
//  By value:
void SCREENS_enter_txt_show();

// Synthkore  18-02-2022  Tolaemon
// Manages the keys and knobs while in the enter text screens.
// Receives:
//   * pi16_encoders_var_value: pointing to the array with the encoders values.
//   * pui8_pushbutton_values:  pointing to the array with the push button states.
// Receives:
// Returns:
//  By value:
void SCREENS_enter_txt_ev_manager(int16_t * pi16_encoders_var_value, uint8_t * pui8_pushbutton_values);

// SAVE_RAM enter file name text screen
int8_t SCREENS_enter_txt_load_SAVE_RAM();
int8_t SCREENS_enter_txt_show_SAVE_RAM(uint8_t * ui8_message);
int8_t SCREENS_enter_txt_ev_manager_SAVE_RAM(int16_t * pi16_encoders_var_value, uint8_t * pui8_pushbutton_values);


#endif /* _SCREENS_ENTER_TXT_H_ */