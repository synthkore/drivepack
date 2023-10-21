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
#ifndef _SCREENS_MENUS_H_
#define _SCREENS_MENUS_H_

#include "sam.h"
#include "screens_params_edition.h"
#include "screens_dialog.h"

 #define SCREENS_MENUS_MAX_VISIBLE_OPTION_ROWS      12 // ideally it should be the same number of rows set when initializing the "number of rows" of the GRAPHIX_text_buffer_init in GRAPHIX_text_buffer_init(...)

#define SCREENS_MENU_MAX_OPTIONS		25 // the max number of choice options on a single menu
#define SCREENS_MENU_MAX_DEPTH           3 // with the max number of menu "branches"

#define SCREENS_MENU_BACKGROUND_CHAR   152

// list of available menus IDs to jump to
#define SCREEN_MENU_PREVIOUS           -1  // the previously selected menu Id option
#define SCREEN_MENU_CURRENT             0  // the current selected menu Id option
#define SCREEN_MENU_MAIN_MENU		    1
#define SCREEN_MENU_SYSTEM			    2


// with the complete information of a menu option
typedef struct{	
	int8_t i8_screen_type; // type of the option, that indicates if that menu option points to another menu, to a dialog screen or to a file explorer.
	uint8_t * pui8_description; // pointer to the string with the option name
	int16_t i16_screen_item_id; // stores the parameter to use with the load function or the ID of the menu to jump to in case the option points to another menu
	uint8_t * pui8_help[2]; // pointer to the menu option contextual help
}t_menu_option;

// last menu state. It is used to store the current state of the menu before jumping to another menu, and it is used to restore the menu state when going back from the new menu to the previous menu.
typedef struct{
	int16_t i16_selected_index; // with the current selected menu option
	int16_t i16_top_index; // with the first option shown in the menu
	int16_t i16_bottom_index; // with the last option shown in the menu
	int16_t i16_menu_id; // with the identifier of the menu at specified depth
}t_last_menu_state;

struct ref_screens_menus{
	int16_t i16_num_options;// the number of options stored in the current menu
	t_menu_option option[SCREENS_MENU_MAX_OPTIONS];
	uint8_t ui8_depth;// the current menu depth in the menu tree, used as index in the "last_menu_state" stack structure that stores the top, bottom and cursor indexes on each depth
	t_last_menu_state stack[SCREENS_MENU_MAX_DEPTH];// stores the state of the menu before jumping to the next menu. This stored information will be used when going back to the previous menu
	uint8_t * pui8_title;// title of the current menu screen
};

// Synthkore  30-09-2014  Tolaemon
// Clears all the information of the current menu
// Receives:
// Returns:
//  By value:
void SCREENS_menus_clear();

// Synthkore  15-03-2022  Tolaemon
// Initializes the menus structures
// Receives:
// Returns:
//  By value:
void SCREENS_menus_init();

// Synthkore  02-10-2014  Tolaemon
// Loads the options of the specified menu into the current menu structure
// Receives:
//    int16_t i16_menu_id : with the identifier of the menu we want to load into
//   the current menu structure.
// Returns:
//  By value:
//    >=0 if has been possible to load the specified menu screen
//   -1: if has not been possible to load the specified menu screen
int8_t SCREENS_menus_load(int16_t i16_menu_id);

// Synthkore  02-10-2014  Tolaemon
// Shows the content of the current menu in the LCD
// Receives:
// Returns:
void SCREENS_menus_show();

// Synthkore  30-09-2014  Tolaemon
// Manages the menus. Checks if has to jump to another menu, if has to change the selected option
// or call to another function.
// Receives:
//    int16_t * encoders_var_value: pointing to the array with the variation of value of the encoders
//    uint8_t * pushbutton_values: pointing to the array with the state of all the push buttons
// Returns:
//  By value:
void SCREENS_menus_ev_manager(int16_t * pi16_encoders_var_value, uint8_t * pui8_pushbutton_values);

#endif /* _SCREENS_MENUS_H_ */