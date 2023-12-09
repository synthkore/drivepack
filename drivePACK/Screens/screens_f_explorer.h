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

#ifndef _SCREENS_F_EXPLORER_H_
#define _SCREENS_F_EXPLORER_H_

#include "screens.h"

#define SCREENS_F_EXPLORER_VISIBLE_OPTIONS          12 // ideally it should be the same number of rows set when initializing the "number of rows" of the GRAPHIX_text_buffer_init in GRAPHIX_text_buffer_init(...)

#define SCREENS_F_EXPLORER_BACKGROUND_CHAR         120

#define SCREENS_F_EXPLORER_MAX_FOLDERS_DEPTH         4 // the max allowed folders depth on the file explorer

#define SCREENS_F_EXPLORER_FILE_EXT_DEFAULT     ".txt"
#define SCREENS_F_EXPLORER_FILE_EXT_ROM_FILE    ".drp"

#define SCREENS_F_EXPLORER_FILE_EXT_LEN		        4 // JBR TBD 2022-04-01 Modify to work with array size instead with LENGTH
#define SCREENS_F_EXPLORER_FILE_NAME_MAX_LEN	   13 // JBR TBD 2022-04-01 Modify to work with array size instead with LENGTH
#define SCREENS_F_EXPLORER_FILE_NAME_N_EXT_LEN   SCREENS_F_EXPLORER_FILE_NAME_MAX_LEN + SCREENS_F_EXPLORER_FILE_EXT_LEN  // JBR TBD 2022-04-01 Modify to work with array size instead with LENGTH

// list of file explorer load operations
#define SCREEN_F_EXPLORER_STANDARD             0
#define SCREEN_F_EXPLORER_LOAD_ROM_FILE        1
#define SCREEN_F_EXPLORER_LOAD_RUN_ROM_FILE    2
#define SCREEN_F_EXPLORER_SAVE_RAM             3

// First state of all dialogs
#define SCREEN_F_EXPLORER_INITIAL_STATE                 0

// File Explorer Screen LOAD_FILE states
#define SCREEN_F_EXPLORER_LOAD_ROM_FILE_NAVIGATING      1

// File Explorer Screen LOAD_FILE states
#define SCREEN_F_EXPLORER_LOAD_RUN_ROM_FILE_NAVIGATING  1

// File Explorer Screen SAVE_RAM states
#define SCREEN_F_EXPLORER_SAVE_RAM_NAVIGATING           1


#define SCREEN_F_EXPLORER_LOAD_TIMEOUT          90000

typedef struct{
	int16_t i16_selected_index; // stores the selected index at path  depth level to restore it when going back
	int16_t i16_top_index; // stores the top index at path  depth level to restore it when going back to the previous folder
	int16_t i16_bottom_index;// stores the bottom index at path depth level to restore it when going back to the previous folder
	int16_t i16_elements_number;// stores the elements number at path  depth level to restore it when going back to the previous folder
}t_last_f_explorer_state;

struct ref_screens_f_explorer{
	int8_t (*load_cur_function)(); // pointer to the function that loads the specific information of the current file explorer screen
	int8_t (*show_cur_function)(uint8_t *); // pointer to the function that shows the specific information of the current file explorer screen
	int8_t (*ev_process_cur_function)(int16_t * , uint8_t * ); // pointer to the function that processes the events specific of the current file explorer screen
    int8_t i8_state;// indicates the state of the navigation interface
	uint8_t ui8_depth; // the current path depth in the file explorer folders tree, used as index in sthe tack structure that stores the top, bottom and cursor indexes on each depth
	t_last_f_explorer_state stack[SCREENS_F_EXPLORER_MAX_FOLDERS_DEPTH];
	TIMING_TICKS timer; // to control the duration of some disk navigation tasks
	uint8_t ui8_consider_folders_files;// bitset used to indicate if it must be considered files, folders or both when navigating through the folders
};


/*********************************************************************************************
* @brief clears the variables and pointers ( to NULL ) used to manage the current configured
* FILE EXPLORER SCREEN.
* @note Synthkore 18-02-2022  Tolaemon
*********************************************************************************************/
void SCREENS_f_explorer_clear();

/*********************************************************************************************
* @brief initializes the variables and structures used for the FILE EXPLORER SCREEN to its 
* default values. It may be used to initialize the module variables on program start.
* @note Synthkore  27-04-2022 Tolaemon
*********************************************************************************************/
void SCREENS_f_explorer_init();

/*********************************************************************************************
* @brief Loads and initializes the FILE EXPLORER SCREEN variables, structures and pointers with
* the values that corresponds to the received i16_f_explorer_id.
* @param[in] i16_f_explorer_id indicates the file explorer screen to load
* @param[in] i16_f_explorer_state indicates the substate of the specified FILE EXPLORER SCREEN 
* to load.
* @return >=0 the specified FILE EXPLORER SCREEN has been succesfully loaded
* <0 the specified screen could not be loaded.
* @note Synthkore 18-02-2022  Tolaemon
*********************************************************************************************/
int8_t SCREENS_f_explorer_load(int16_t i16_f_explorer_id,int16_t i16_f_explorer_state);

/*********************************************************************************************
* @brief shows the current configured FILE EXPLORER SCREEN.
* @note Synthkore 18-02-2022  Tolaemon
*********************************************************************************************/
void SCREENS_f_explorer_show();

/*********************************************************************************************
* @brief Manages the keys and knobs of the current configured FILE EXPLORER SCREEN.
* @param[in] pi16_encoders_var_value pointing to the array with the variation of value of the encoders.
* @param[in] pui8_pushbutton_values pointing to the array with the push button states.
* @note Synthkore 18-02-2022  Tolaemon
*********************************************************************************************/
void SCREENS_f_explorer_ev_manager(int16_t * pi16_encoders_var_value, uint8_t * pui8_pushbutton_values);

// LOAD_ROM_FILE file selection screen
int8_t SCREENS_f_explorer_load_LOAD_ROM_FILE();
int8_t SCREENS_f_explorer_show_LOAD_ROM_FILE(uint8_t * ui8_message);
int8_t SCREENS_f_explorer_ev_manager_LOAD_ROM_FILE(int16_t * pi16_encoders_var_value, uint8_t * pui8_pushbutton_values);

// LOAD_RUN_ROM_FILE file selection screen
int8_t SCREENS_f_explorer_load_LOAD_RUN_ROM_FILE();
int8_t SCREENS_f_explorer_show_LOAD_RUN_ROM_FILE(uint8_t * ui8_message);
int8_t SCREENS_f_explorer_ev_manager_LOAD_RUN_ROM_FILE(int16_t * pi16_encoders_var_value, uint8_t * pui8_pushbutton_values);

// SAVE_RAM folder selection screen
int8_t SCREENS_f_explorer_load_SAVE_RAM();
int8_t SCREENS_f_explorer_show_SAVE_RAM(uint8_t * ui8_message);
int8_t SCREENS_f_explorer_ev_manager_SAVE_RAM(int16_t * pi16_encoders_var_value, uint8_t * pui8_pushbutton_values);

#endif /* _SCREENS_F_EXPLORER_H_ */