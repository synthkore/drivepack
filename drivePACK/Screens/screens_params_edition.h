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
#ifndef _SCREENS_PARAMS_EDITION_H_
#define _SCREENS_PARAMS_EDITION_H_

#include "sam.h"
#include "../timing.h"
#include "../user_iface.h"
#include "../graphix.h"
#include "screens_menus.h"
#include "screens.h"


#define NULL_INDEX		0xFFFF // an invalid INDEX. Remember that the ID of a patch is NOT its index !!! When a Patch is unloaded all the patches abover it are moved on postion in the patches array, but they keep their ID!!

#define SCREEN_MAX_CHARS_PER_ROW  25 // JBR 2022-02-01 Revisar esta constante

#define SCREENS_PARAM_EDIT_BACKGROUND_CHAR 119

// defines of the parameter edition screen
#define SCREENS_PARAM_EDIT_1_COLS_WIDTH 20 // the max number of LCD characters of a param edition screen column
#define SCREENS_PARAM_EDIT_2_COLS_WIDTH 10 // the max number of LCD characters of a param edition screen column
#define SCREENS_PARAM_EDIT_3_COLS_WIDTH 6
#define SCREENS_PARAM_EDIT_4_COLS_WIDTH 3
// the max number of LCD characters of a param edition screen column
#define SCREENS_PARAM_EDIT_1_COLS		1
#define SCREENS_PARAM_EDIT_2_COLS		2
#define SCREENS_PARAM_EDIT_3_COLS		3
#define SCREENS_PARAM_EDIT_4_COLS		3
#define SCREENS_PARAM_EDIT_1_ROWS		1 // the number of parameters rows int the LCD. Each parameter row takes two text rows in the LCD: the upper for the tittle and the lower for the value.
#define SCREENS_PARAM_EDIT_2_ROWS		2 // the number of parameters rows int the LCD. If the LCD had only 2 text rows then the LCD would have only one parameter row.
#define SCREENS_PARAM_EDIT_3_ROWS		3
#define SCREENS_PARAM_EDIT_4_ROWS		4
#define SCREENS_PARAM_EDIT_5_ROWS		5
#define SCREENS_PARAM_EDIT_6_ROWS		6
#define SCREENS_PARAM_EDIT_7_ROWS		7
#define SCREENS_PARAM_EDIT_8_ROWS       8
#define SCREENS_PARAM_EDIT_ROWS			2 // the number of rows used to show and edit a parameter on the LCD: one for the title, and the other to edit the value
#define SCREENS_PARAM_EDIT_MAX_LEN      SCREENS_PARAM_EDIT_1_COLS_WIDTH// the max number of chars that a parameter edition can have
#define SCREENS_PARAM_EDIT_MAX_PARAMS	18 // the max number of paramaters that a parameter edition screen will have.

// Defines to control the incremental speed variation of the encoder
#define SCREEN_EDITION_CONT_PRESSED_MS		TIMMING_TICKS_200_MS  // max milliseconds to consider that the pusbuttons or the encoder have been continuoysly pressed or turned.
#define SCREEN_EDITION_CONT_PR_INT2_MS		1000 // milliseconds where starts the 2nd increase/decrease adjustable speed interval
#define SCREEN_EDITION_CONT_PR_INT2_FACT	2    // factor applied in the 2nd increase/decrease adjustable speed interval
#define SCREEN_EDITION_CONT_PR_INT3_MS		TIMMING_TICKS_1_S // milliseconds where starts the 3th increase/decrease adjustable speed interval
#define SCREEN_EDITION_CONT_PR_INT3_FACT	4    // factor applied in the 3th increase/decrease adjustable speed interval
#define SCREEN_EDITION_CONT_PR_INT4_MS		TIMMING_TICKS_2_S // milliseconds where starts the 4th increase/decrease adjustable speed interval
#define SCREEN_EDITION_CONT_PR_INT4_FACT	7    // factor applied in the 4th increase/decrease adjustable speed interval
#define SCREEN_EDITION_CONT_PR_INT5_MS		TIMMING_TICKS_3_S // milliseconds where starts the 5th increase/decrease adjustable speed interval
#define SCREEN_EDITION_CONT_PR_INT5_FACT	10   // factor applied in the 5th increase/decrease adjustable speed interval

#define SCREEN_PARAM_EDIT_DEFAULT_ID     0

// list of available parameter editon screens IDs to load to
#define SCREEN_PARAM_EDIT_ID_EDIT_A		1

// PARAMETERS of the EDIT PARAM param edition screen
#define PARAM_EDIT_A_PARAM_01		0
#define PARAM_EDIT_A_PARAM_02		1
#define PARAM_EDIT_A_PARAM_03		2
#define PARAM_EDIT_A_PARAM_04		3
#define PARAM_EDIT_A_PARAM_05		4
#define PARAM_EDIT_A_PARAM_06		5
#define PARAM_EDIT_A_PARAM_07		6
#define PARAM_EDIT_A_PARAM_08		7
#define PARAM_EDIT_A_PARAM_09		8
#define PARAM_EDIT_A_PARAM_10		9
#define PARAM_EDIT_A_PARAM_11		10
#define PARAM_EDIT_A_PARAM_12		11
#define PARAM_EDIT_A_PARAM_13		12
#define PARAM_EDIT_A_PARAM_14		13
#define PARAM_EDIT_A_PARAM_15		14
#define PARAM_EDIT_A_PARAM_16		15
#define PARAM_EDIT_A_PARAM_17		16
#define PARAM_EDIT_A_PARAM_TOTAL	17



// with all the information of a parameter available on the param edition screen
typedef struct{
	uint8_t * pui8_description;// pointing to the name of the parameter ( parameter label )
	uint8_t value[SCREENS_PARAM_EDIT_MAX_LEN];// used to print the value of the parameter in text format
	uint8_t * pui8_help[2]; // pointer to the parameter  contextual help
}t_param_edit_option;

struct ref_screens_params_edition{
	int8_t (*load_cur_function)(); // pointer to the function that loads the information of the current param edition screen
	int8_t (*show_cur_function)(uint8_t *); // pointer to the current function that shows the information corresponding to the current param edition screen	
	int8_t (*ev_process_cur_function)(int16_t * , uint8_t * ); // pointer to the function that processes the events on the current param edition screen
	uint8_t * pui8_title; // pointer to the string with the title of the current screen
	int16_t i16_num_parameters;// the number of parameters stored in the parameters edition screen
	t_param_edit_option parameter[SCREENS_PARAM_EDIT_MAX_PARAMS];
	int16_t i16_selected_index; // with the current selected parameter
	int16_t i16_top_index; // with the first parameter shown in the parameter edition screen
	int16_t i16_bottom_index; // with the last parameter shown in the parameter edition screen
	int16_t i16_params_cols_numb; // the number of columns on which the parameters are shown in the current edition screen
	int16_t i16_params_rows_numb; // the number of rows on which the parameters are shown in the current edition screen
	int16_t i16_params_cols_width; // the number of chars occupied per column on that screen	
};

// Synthkore  11-10-2014  Tolaemon
// Clears all the information of the current parameters edition screen
// Receives:
// Returns:
//  By value:
void SCREENS_params_edition_clear();

// Synthkore  27-04-2022  Tolaemon
// Initializes the module variables on program start.
// Receives:
// Returns:
//  By value:
void SCREENS_params_edition_init();

// Synthkore  26-10-2014  Tolaemon
// Receives:
// Returns:
int8_t SCREENS_params_edition_load(int16_t i16_params_edition_id);

// Synthkore  26-10-2014  Tolaemon
// Standard call back routine that shows most part of param edition screens. It reads the screen_edition parameters structure
// and shows all the parameter names and values organized on the LCD screen with the specified rows and columns.
// Receives:
// Returns:
void SCREENS_params_edition_show();

// Synthkore  09-11-2014  Tolaemon
// Standard call back routine that processes the keys pressed by the user on the current screen
// Receives:
//    int16_t * encoders_var_value: pointing to the array with the variation of value of the encoders
//    uint8_t * pushbutton_values: pointing to the array with the state of all the push buttons
// Returns:
void SCREENS_params_edition_ev_manager(int16_t * pi16_encoders_var_value, uint8_t * pui8_pushbutton_values);

// Synthkore  03-03-2015  Tolaemon
// Function that adds or sustracts the received variation to the received value, checking that the new value is between the min and max value. It also applies
// the adjustable amount of variation ( modification speed ) depending on the time that any of the pushbuttons or the encoder has been continuously pressed.
// Receives:
//	 encoders_var_value: pointing to the array with the last variation of the encoders since last read
//   pushbutton_values: pointing to the array with the pushbutton values
//   value: pointing to the current value to modify
//   step_variation: the value to add or subtract to the current value when the buttons is pressed or with each encoder step
//   min_value: the lower allowed limit for the new value
//   max_value: the upper allowed limit for the new value
// Returns:
//  By ref:
//    value: pointing to the new value of the modified value
//  By value:
//    TRUE: if the received value has been modified
//    FALSE: if the received value has not been modified
// NOTE:
//    This function should only be called when detected a change in the state of the pushbutton or the encoders.
uint8_t SCREENS_params_edition_ev_manager_update_param_val(int16_t * pi16_encoders_var_value, uint8_t * pui8_pushbutton_values, int16_t * pi16_value, int16_t pi16_step_variation, int16_t min_value, int16_t max_value);

// Param Edition specific callback functions
int8_t SCREENS_params_edition_load_PARAMS_EDIT_A();
int8_t SCREENS_params_edition_show_PARAMS_EDIT_A(uint8_t * ui8_message);
int8_t SCREENS_params_edition_ev_manager_PARAMS_EDIT_A(int16_t * encoders_var_value, uint8_t * pushbutton_values);

#endif /* _SCREENS_PARAMS_EDITION_H_ */