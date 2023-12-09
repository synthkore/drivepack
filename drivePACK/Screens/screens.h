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

#ifndef SCREENS_H_
#define SCREENS_H_

#include "sam.h"
#include "stdio.h"
#include "../sys_defs.h"
#include "../user_iface.h"
#include "../graphix.h"
#include "../aux_funcs.h"
#include "../file_sys.h"
#include "../data_io.h"
#include "screens_menus.h"
#include "screens_params_edition.h"
#include "screens_f_explorer.h"
#include "screens_enter_txt.h"


#define SCREEN_EMPTY_MESSAGE ""

#define SCREEN_UP_CHAR      165
#define SCREEN_DOWN_CHAR    166
#define SCREEN_END_CHAR     174
#define SCREEN_BAR_CHAR     172
#define SCREEN_CURSOR_CHAR  147
#define SCREEN_CANCEL_CHAR  175
#define SCREEN_ACCEPT_CHAR  176

// user graphic interface modes
#define SCREENS_INVALID_SCREEN	       0 // there is no any screen active
#define SCREENS_MENU_SCREEN		       1 // the menu screen is being shown
#define SCREENS_PARAMS_EDITION_SCREEN  2 // a parameters edition is being shown
#define SCREENS_DIALOG_SCREEN          3 // a dialog screen is being shown
#define SCREENS_F_EXPLORER_SCREEN      4 // a file explorer screen is being shown
#define SCREENS_ENTER_TXT_SCREEN       5 // a enter text (alphanumeric value) screen is being shown

#define SCREENS_INVALID_ID   	       0 // there is no any valid parameter Id

struct ref_screens_control{
	uint8_t ui8_on_load_event;// to set the file explorer screen load event	
	int8_t i8_current_type;// indicates the state of the screens user interface, if has to show a menu screen, a parameters edition screen, a dialog screen ..
	int16_t i16_current_ID;// indicates the ID of the current screen of the specified type
};

/*********************************************************************************************
* @brief initializes the SCREENs module internal structures and variables.
* @note Synthkore  15-03-2022  Tolaemon
*********************************************************************************************/
void SCREENS_Init();

/*********************************************************************************************
* @brief procedure that manages the events on the different types of screens. It receives the
* state of the encoders and pushbuttons and depending on the current configured screen it calls
* the specific event manager configured for the current screen.
* @param[in] pi16_encoders_var_value pointing to the array with the variation of value of the encoders
* param[in] pui8_buttons_state current state of the push buttons
* @note Synthkore  15-03-2022  Tolaemon
*********************************************************************************************/
void SCREENS_ev_manager(int16_t * pi16_encoders_var_value, uint8_t * pui8_buttons_state);

/*********************************************************************************************
* @brief shows the symbols that indicates to the user if there are more items to view in the 
* list above or below.
* param[in] i16_bottom_index with the first element of the list of items ( menus, parameter, 
* files ) shown in the LCD.
* param[in] i16_top_index with the last element of the menu or list of items ( menus, parameter, 
* files ) shown in the LCD.
* param[in] i16_max_options with the total number of elements of the menu or list
* @note Synthkore  26-03-2022  Tolaemon
*********************************************************************************************/
void SCREENS_print_up_down_symbols(int16_t i16_bottom_index, int16_t i16_top_index, int16_t i16_max_options );

/*********************************************************************************************
* @brief shows the title of the current screen
* param[in] pui8_title with the title to print in the current screen
* files ) shown in the LCD.
* param[in]  ui8_line_idx the index of the title line to print
* @note Synthkore  26-03-2022  Tolaemon
*********************************************************************************************/
void SCREENS_print_title(uint8_t * pui8_title, uint8_t ui8_line_idx);

/*********************************************************************************************
* @brief shows the help / informative text of the current screen
* param[in] pui8_help with the help / informative text to print in the current screen
* param[in] ui8_line_idx the index of the help line to print
* @note Synthkore  26-03-2022  Tolaemon
*********************************************************************************************/
void SCREENS_print_help(uint8_t * pui8_help, uint8_t ui8_line_idx);

#endif // SCREENS_H_ 