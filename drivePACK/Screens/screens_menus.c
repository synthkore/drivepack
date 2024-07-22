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

#include "screens_menus.h"

// reference to external structures
extern struct ref_screens_control screens_control;

struct ref_screens_menus screens_menus;

// menus literals in flash
// menu default literals in flash
const uint8_t SCREEN_MENU_DEFAULT_TITLE[]  = "Invalid title";
const uint8_t SCREEN_MENU_DEFAULT_TXT[]  = "Invalid option";
const uint8_t SCREEN_MENU_DEFAULT_HLP1[] = "No defined help1";
const uint8_t SCREEN_MENU_DEFAULT_HLP2[] = "No defined help2";

// "Main menu" menu literals in flash
const uint8_t SCREEN_MENU_MAIN_TITLE[]  = "Main menu";

const uint8_t SCREEN_MENU_MAIN_LOAD_RUN_TXT[]  = "Load & run";
const uint8_t SCREEN_MENU_MAIN_LOAD_RUN_HLP1[] = "Load ROM file from disk";
const uint8_t SCREEN_MENU_MAIN_LOAD_RUN_HLP2[] = "to RAM and then run it.";

const uint8_t SCREEN_MENU_MAIN_LOAD_ROM_TXT[]  = "Load";
const uint8_t SCREEN_MENU_MAIN_LOAD_ROM_HLP1[] = "Load ROM file from disk";
const uint8_t SCREEN_MENU_MAIN_LOAD_ROM_HLP2[] = "to RAM buffer.";

const uint8_t SCREEN_MENU_MAIN_RUN_RAM_TXT[]   = "Run";
const uint8_t SCREEN_MENU_MAIN_RUN_RAM_HLP1[]  = "Run current RAM buffer";
const uint8_t SCREEN_MENU_MAIN_RUN_RAM_HLP2[]  = "content.              ";

const uint8_t SCREEN_MENU_MAIN_INFO_RAM_TXT[]   = "Info";
const uint8_t SCREEN_MENU_MAIN_INFO_RAM_HLP1[]  = "Show current RAM buf-";
const uint8_t SCREEN_MENU_MAIN_INFO_RAM_HLP2[]  = "fer content info.";

const uint8_t SCREEN_MENU_MAIN_DUMP_TXT[]   = "Dump";
const uint8_t SCREEN_MENU_MAIN_DUMP_HLP1[]  = "Read and store in RAM ";
const uint8_t SCREEN_MENU_MAIN_DUMP_HLP2[]  = "buffer a ROM cart.";

const uint8_t SCREEN_MENU_MAIN_SAVE_AS_TXT[]  = "Save as";
const uint8_t SCREEN_MENU_MAIN_SAVE_AS_HLP1[] = "Save current RAM buffer";
const uint8_t SCREEN_MENU_MAIN_SAVE_AS_HLP2[] = "to a ROM file in disk.";

const uint8_t SCREEN_MENU_MAIN_SAVE_TXT[]  = "Save";
const uint8_t SCREEN_MENU_MAIN_SAVE_HLP1[] = "Save current RAM buffer";
const uint8_t SCREEN_MENU_MAIN_SAVE_HLP2[] = "to a ROM file in disk.";

const uint8_t SCREEN_MENU_MAIN_SEND_TXT[]   = "Send";
const uint8_t SCREEN_MENU_MAIN_SEND_HLP1[]  = "Send current RAM buffer";
const uint8_t SCREEN_MENU_MAIN_SEND_HLP2[]  = "to a computer.";

const uint8_t SCREEN_MENU_MAIN_RECEIVE_RUN_TXT[]   = "Receive & run";
const uint8_t SCREEN_MENU_MAIN_RECEIVE_RUN_HLP1[]  = "Receive RAM buffer from";
const uint8_t SCREEN_MENU_MAIN_RECEIVE_RUN_HLP2[]  = "a computer and run it.";

const uint8_t SCREEN_MENU_MAIN_RECEIVE_TXT[]  = "Receive";
const uint8_t SCREEN_MENU_MAIN_RECEIVE_HLP1[] = "Receive RAM buffer ";
const uint8_t SCREEN_MENU_MAIN_RECEIVE_HLP2[] = "from a computer.";

const uint8_t SCREEN_MENU_MAIN_SYSTEM_TXT[]  = "System";
const uint8_t SCREEN_MENU_MAIN_SYSTEM_HLP1[] = "Check and configure";
const uint8_t SCREEN_MENU_MAIN_SYSTEM_HLP2[] = "different sys options.";

// "System" menu literals in flash
const uint8_t SCREEN_MENU_SYSTEM_TITLE[]= "System";

const uint8_t SCREEN_MENU_SYSTEM_ABOUT_TXT[]       = "About drivePACK ";
const uint8_t SCREEN_MENU_SYSTEM_ABOUT_HLP1[]      = "General information";
const uint8_t SCREEN_MENU_SYSTEM_ABOUT_HLP2[]      = "of the device.";

const uint8_t SCREEN_MENU_SYSTEM_TEST_TXT[]        = "Test system";
const uint8_t SCREEN_MENU_SYSTEM_TEST_HLP1[]       = "Test the state of peri-";
const uint8_t SCREEN_MENU_SYSTEM_TEST_HLP2[]       = "pherals.";

void SCREENS_menus_clear(){
	int16_t i16_aux;


	// clear the structure used to store the menus
	for (i16_aux=0;i16_aux<SCREENS_MENU_MAX_OPTIONS;i16_aux++){
		memset(&screens_menus.option[i16_aux],0,sizeof(t_menu_option));
		screens_menus.option[i16_aux].i8_screen_type=SCREENS_INVALID_SCREEN;
		screens_menus.option[i16_aux].i16_screen_item_id=SCREENS_INVALID_ID;
		screens_menus.option[i16_aux].pui8_description = SCREEN_MENU_DEFAULT_TXT;
		screens_menus.option[i16_aux].pui8_help[0]=SCREEN_MENU_DEFAULT_HLP1;
		screens_menus.option[i16_aux].pui8_help[1]=SCREEN_MENU_DEFAULT_HLP2;
	}//for
	screens_menus.pui8_title=SCREEN_MENU_DEFAULT_TITLE;
	screens_menus.i16_num_options=0;
	
}//SCREENS_menus_clear



void SCREENS_menus_init(){


	// clear the structure used to store the menus
	SCREENS_menus_clear();

	// initializes the navigation indexes, the indexes stack ...
	SCREENS_menus_load(SCREEN_MENU_MAIN_MENU);
	screens_menus.ui8_depth=0;
	screens_menus.stack[0].i16_menu_id=SCREEN_MENU_MAIN_MENU;
	screens_menus.stack[0].i16_selected_index=0;
	screens_menus.stack[0].i16_bottom_index=0;
	if (screens_menus.i16_num_options>SCREENS_MENUS_MAX_VISIBLE_OPTION_ROWS){
		screens_menus.stack[0].i16_top_index=SCREENS_MENUS_MAX_VISIBLE_OPTION_ROWS-1;
	}else{
		screens_menus.stack[0].i16_top_index=screens_menus.i16_num_options-1;
	}//if

}//SCREENS_menus_init



int8_t SCREENS_menus_load(int16_t i16_received_menu_id){
	int8_t  i8_ret_value= 1;
	int16_t i16_menu_id = 0;
	int16_t i;
	
	
	// check if the received menu id corresponds to a special menu ID code or to a normal
	// menu ID. Special menu Ids are used to excute special operations like jump to 
	// PREVIOUS, to CURRENT, ... ) 
	if (i16_received_menu_id==SCREEN_MENU_PREVIOUS){
		
		// as the received menu Id is SCREEN_MENU_PREVIOUS then point to the previous 
		// navigation data and the indexes into the menu navigation stack
		if (screens_menus.ui8_depth==0){
			// there is no previous navigation data to point to
			i8_ret_value = -1;
		}else{
			 screens_menus.ui8_depth--;
			// get the Id to load the in menu options structure that corresponds to the previous menu
			i16_menu_id = screens_menus.stack[screens_menus.ui8_depth].i16_menu_id;
		}
	
	}else if (i16_received_menu_id==SCREEN_MENU_CURRENT){
		
		// get the Id of current menu to load the options structure of the current menu
		i16_menu_id = screens_menus.stack[screens_menus.ui8_depth].i16_menu_id;
		
	}else{
	
		// as the received menu Id is >=0 it corresponds to a new menu to load. Before loading
		// the new menu check that there is still space in the menu navigation stack
	    if (screens_menus.ui8_depth>=(SCREENS_MENU_MAX_DEPTH-1)){
			// there is no space to add a new menu option in the menu navigation stack
			i8_ret_value = -1;
		}else{
		    i16_menu_id = i16_received_menu_id;	
		}
		
	}//if
	
	// if no error then load the menu options structure with the data of the specified menu
	if (i8_ret_value>=0){
				
		switch (i16_menu_id){

			case SCREEN_MENU_MAIN_MENU:				
				SCREENS_menus_clear();
				// initialize the strings used to print the title and the context help
				screens_menus.pui8_title = SCREEN_MENU_MAIN_TITLE;
				// initialize the structure with the specified menu options
				i=0;
				screens_menus.option[i].pui8_description = SCREEN_MENU_MAIN_LOAD_RUN_TXT;
				screens_menus.option[i].pui8_help[0] = SCREEN_MENU_MAIN_LOAD_RUN_HLP1;
				screens_menus.option[i].pui8_help[1] = SCREEN_MENU_MAIN_LOAD_RUN_HLP2;
				screens_menus.option[i].i8_screen_type = SCREENS_DIALOG_SCREEN;
				screens_menus.option[i].i16_screen_item_id = SCREEN_DIALOG_ID_LOAD_RUN_ROM_FILE;
				i++;						
				screens_menus.option[i].pui8_description = SCREEN_MENU_MAIN_LOAD_ROM_TXT;
				screens_menus.option[i].pui8_help[0] = SCREEN_MENU_MAIN_LOAD_ROM_HLP1;
				screens_menus.option[i].pui8_help[1] = SCREEN_MENU_MAIN_LOAD_ROM_HLP2;
				screens_menus.option[i].i8_screen_type = SCREENS_DIALOG_SCREEN;
				screens_menus.option[i].i16_screen_item_id = SCREEN_DIALOG_ID_LOAD_ROM_FILE;
				i++;			
				screens_menus.option[i].pui8_description = SCREEN_MENU_MAIN_RUN_RAM_TXT;
				screens_menus.option[i].pui8_help[0] = SCREEN_MENU_MAIN_RUN_RAM_HLP1;
				screens_menus.option[i].pui8_help[1] = SCREEN_MENU_MAIN_RUN_RAM_HLP2;
				screens_menus.option[i].i8_screen_type = SCREENS_DIALOG_SCREEN;
				screens_menus.option[i].i16_screen_item_id = SCREEN_DIALOG_ID_RUN_RAM;
				i++;
				screens_menus.option[i].pui8_description = SCREEN_MENU_MAIN_INFO_RAM_TXT;
				screens_menus.option[i].pui8_help[0] = SCREEN_MENU_MAIN_INFO_RAM_HLP1;
				screens_menus.option[i].pui8_help[1] = SCREEN_MENU_MAIN_INFO_RAM_HLP2;
				screens_menus.option[i].i8_screen_type = SCREENS_DIALOG_SCREEN;
				screens_menus.option[i].i16_screen_item_id = SCREEN_DIALOG_ID_SHOW_RAM_INFO;
				i++;												
				screens_menus.option[i].pui8_description = SCREEN_MENU_MAIN_DUMP_TXT;
				screens_menus.option[i].pui8_help[0] = SCREEN_MENU_MAIN_DUMP_HLP1;
				screens_menus.option[i].pui8_help[1] = SCREEN_MENU_MAIN_DUMP_HLP2;				
				screens_menus.option[i].i8_screen_type = SCREENS_DIALOG_SCREEN;
				screens_menus.option[i].i16_screen_item_id = SCREEN_DIALOG_ID_DUMP_ROM;
				i++;
				screens_menus.option[i].pui8_description = SCREEN_MENU_MAIN_SAVE_AS_TXT;
				screens_menus.option[i].pui8_help[0] = SCREEN_MENU_MAIN_SAVE_AS_HLP1;
				screens_menus.option[i].pui8_help[1] = SCREEN_MENU_MAIN_SAVE_AS_HLP2;
				screens_menus.option[i].i8_screen_type = SCREENS_DIALOG_SCREEN;
				screens_menus.option[i].i16_screen_item_id = SCREEN_DIALOG_ID_SAVE_RAM_AS_FILE;
				i++;
				screens_menus.option[i].pui8_description = SCREEN_MENU_MAIN_SAVE_TXT;
				screens_menus.option[i].pui8_help[0] = SCREEN_MENU_MAIN_SAVE_HLP1;
				screens_menus.option[i].pui8_help[1] = SCREEN_MENU_MAIN_SAVE_HLP2;
				screens_menus.option[i].i8_screen_type = SCREENS_DIALOG_SCREEN;
				screens_menus.option[i].i16_screen_item_id=SCREEN_DIALOG_ID_SAVE_RAM_FILE;				
				i++;
				screens_menus.option[i].pui8_description = SCREEN_MENU_MAIN_RECEIVE_RUN_TXT;
				screens_menus.option[i].pui8_help[0] = SCREEN_MENU_MAIN_RECEIVE_RUN_HLP1;
				screens_menus.option[i].pui8_help[1] = SCREEN_MENU_MAIN_RECEIVE_RUN_HLP2;
				screens_menus.option[i].i8_screen_type = SCREENS_DIALOG_SCREEN;
				screens_menus.option[i].i16_screen_item_id = SCREEN_DIALOG_ID_RECEIVE_RUN_RAM;								
				i++;
				screens_menus.option[i].pui8_description = SCREEN_MENU_MAIN_RECEIVE_TXT;
				screens_menus.option[i].pui8_help[0] = SCREEN_MENU_MAIN_RECEIVE_HLP1;
				screens_menus.option[i].pui8_help[1] = SCREEN_MENU_MAIN_RECEIVE_HLP2;
				screens_menus.option[i].i8_screen_type = SCREENS_DIALOG_SCREEN;
				screens_menus.option[i].i16_screen_item_id = SCREEN_DIALOG_ID_RECEIVE_RAM;
				i++;				
				screens_menus.option[i].pui8_description = SCREEN_MENU_MAIN_SEND_TXT;
				screens_menus.option[i].pui8_help[0] = SCREEN_MENU_MAIN_SEND_HLP1;
				screens_menus.option[i].pui8_help[1] = SCREEN_MENU_MAIN_SEND_HLP2;
				screens_menus.option[i].i8_screen_type = SCREENS_DIALOG_SCREEN;
				screens_menus.option[i].i16_screen_item_id = SCREEN_DIALOG_ID_SEND_RAM;
				i++;
				screens_menus.option[i].pui8_description = SCREEN_MENU_MAIN_SYSTEM_TXT;
				screens_menus.option[i].pui8_help[0] = SCREEN_MENU_MAIN_SYSTEM_HLP1;
				screens_menus.option[i].pui8_help[1] = SCREEN_MENU_MAIN_SYSTEM_HLP2;
				screens_menus.option[i].i8_screen_type = SCREENS_MENU_SCREEN;
				screens_menus.option[i].i16_screen_item_id = SCREEN_MENU_SYSTEM;				
				i++;
				screens_menus.i16_num_options=i;
				break;
			
			case SCREEN_MENU_SYSTEM:				
				SCREENS_menus_clear();
				// initialize the strings used to print the title and the context help
				screens_menus.pui8_title = SCREEN_MENU_SYSTEM_TITLE;				
				// initialize the structure with the specified menu options
				i=0;
				screens_menus.option[i].pui8_description = SCREEN_MENU_SYSTEM_ABOUT_TXT;
				screens_menus.option[i].pui8_help[0] = SCREEN_MENU_SYSTEM_ABOUT_HLP1;
				screens_menus.option[i].pui8_help[1] = SCREEN_MENU_SYSTEM_ABOUT_HLP2;
				screens_menus.option[i].i8_screen_type = SCREENS_DIALOG_SCREEN;
				screens_menus.option[i].i16_screen_item_id = SCREEN_DIALOG_ID_INTRO;
				i++;
				screens_menus.option[i].pui8_description = SCREEN_MENU_SYSTEM_TEST_TXT;
				screens_menus.option[i].pui8_help[0] = SCREEN_MENU_SYSTEM_TEST_HLP1;
				screens_menus.option[i].pui8_help[1] = SCREEN_MENU_SYSTEM_TEST_HLP2;
				screens_menus.option[i].i8_screen_type = SCREENS_DIALOG_SCREEN;
				screens_menus.option[i].i16_screen_item_id=SCREEN_DIALOG_ID_TEST;
				i++;
				screens_menus.i16_num_options=i;
				break;

			default:
				i8_ret_value=-1;			
				break;
		}//switch
	
	}//if ( (i8_ret_value>=0)
	
    
	if (i8_ret_value>=0){
			
	    // if a new menu has been loaded then store menu navigation data and indexes into the navigation stack
		if (i16_received_menu_id>0){

			// as the received menu corresponds to a new menu to load ( Id is >=0 ) then store the menu 
			// navigation data and the indexes into the menu navigation stack 
			screens_menus.ui8_depth++;
			screens_menus.stack[screens_menus.ui8_depth].i16_menu_id=i16_menu_id;					
			// after loading the new menu some vars are initialized to show it properly in the LCD
			screens_menus.stack[screens_menus.ui8_depth].i16_selected_index=0;
			screens_menus.stack[screens_menus.ui8_depth].i16_bottom_index=0;
		
			if (screens_menus.i16_num_options<SCREENS_MENUS_MAX_VISIBLE_OPTION_ROWS){
				screens_menus.stack[screens_menus.ui8_depth].i16_top_index=screens_menus.i16_num_options-1;
			}else{
				screens_menus.stack[screens_menus.ui8_depth].i16_top_index=SCREENS_MENUS_MAX_VISIBLE_OPTION_ROWS-1;
			}//if
			
		}//if
			
		// set the SCREENS control MENUs SCREEN mode
		screens_control.i8_current_type=SCREENS_MENU_SCREEN;
		screens_control.i16_current_ID=i16_received_menu_id;
		// set the OnLoad event to show in screen the new loaded menu
		screens_control.ui8_on_load_event=TRUE;
		
	}//if
		
	return i8_ret_value;
	
}//SCREENS_menus_load



void SCREENS_menus_show(){
	int16_t i16_aux;
	uint8_t ui8_aux_string1_64[AUX_FUNCS_F_P_MAX_STR_SIZE_64];	
	int16_t i16_bot_row_index;
	int16_t i16_top_row_index;
	int16_t i16_curr_row_index;

	
	i16_bot_row_index = screens_menus.stack[screens_menus.ui8_depth].i16_bottom_index;
	i16_top_row_index = screens_menus.stack[screens_menus.ui8_depth].i16_top_index;
	i16_curr_row_index=0;
	for (i16_aux=i16_bot_row_index;i16_aux<=i16_top_row_index;i16_aux++){
		
		// check if the current option is the selected one
		AUX_FUNCS_lstrcpy(ui8_aux_string1_64,"",AUX_FUNCS_F_P_MAX_STR_SIZE_64);
		if (i16_aux==screens_menus.stack[screens_menus.ui8_depth].i16_selected_index){
			// add the brackets to mark the selected option
			AUX_FUNCS_strcat(ui8_aux_string1_64,"[",AUX_FUNCS_F_P_MAX_STR_SIZE_64);
			AUX_FUNCS_strcat(ui8_aux_string1_64,screens_menus.option[i16_aux].pui8_description, AUX_FUNCS_F_P_MAX_STR_SIZE_64);
			AUX_FUNCS_strcat(ui8_aux_string1_64,"]",AUX_FUNCS_F_P_MAX_STR_SIZE_64);
			// print the currently selected option in the string
			GRAPHIX_text_buffer_set_string(0,i16_curr_row_index,(uint8_t*)ui8_aux_string1_64,ATTR_NO_ATTRIBS,GRAPHIX_TEXT_COL_IDX_BLACK,GRAPHIX_TEXT_COL_NEUTRAL,GRAPHIX_TEXT_COL_IDX_GREY);			
		}else{
			AUX_FUNCS_strcat(ui8_aux_string1_64,"-",AUX_FUNCS_F_P_MAX_STR_SIZE_64);
			AUX_FUNCS_strcat(ui8_aux_string1_64,screens_menus.option[i16_aux].pui8_description, AUX_FUNCS_F_P_MAX_STR_SIZE_64);
			AUX_FUNCS_strcat(ui8_aux_string1_64," ",AUX_FUNCS_F_P_MAX_STR_SIZE_64);// " " blank space to clear the bracket "]"
			// print the option in screen
			GRAPHIX_text_buffer_set_string(0,i16_curr_row_index,(uint8_t*)ui8_aux_string1_64,ATTR_SPACE_BACKSYMBOL,GRAPHIX_TEXT_COL_NEUTRAL,GRAPHIX_TEXT_COL_IDX_BLACK,GRAPHIX_TEXT_COL_IDX_BLACK);			
		}//
		
		i16_curr_row_index++;
		
	}//for
	
	// prints the 'up', 'down' or 'score' indicating the user if there are more elements available
	i16_bot_row_index = screens_menus.stack[screens_menus.ui8_depth].i16_bottom_index;
	i16_top_row_index = screens_menus.stack[screens_menus.ui8_depth].i16_top_index;    
	SCREENS_print_up_down_symbols(i16_bot_row_index, i16_top_row_index, screens_menus.i16_num_options);
	
	// refresh the content of the buffer to screen
	GRAPHIX_text_buffer_refresh();
	
}//SCREENS_menus_show



void SCREENS_menus_ev_manager(int16_t * pi16_encoders_var_value, uint8_t * pui8_pushbutton_values){
	int16_t i16_aux;
	

	// ON LOAD event, a new menu screen has been loaded
	if ((screens_control.ui8_on_load_event==TRUE)){

	    // reset the OnLoad event flag
	    screens_control.ui8_on_load_event = FALSE;
	
		// as the new menu has been loaded, then clear the text buffer with the specified SCREENS_MENU_BACKGROUND_CHAR
		GRAPHIX_text_buffer_fill(SCREENS_MENU_BACKGROUND_CHAR,ATTR_NO_ATTRIBS,GRAPHIX_TEXT_COL_IDX_DARK_GREY,GRAPHIX_TEXT_COL_IDX_BLACK,GRAPHIX_TEXT_COL_IDX_BLACK);
	
		// print in screen the title of the new loaded menu
		SCREENS_print_title(screens_menus.pui8_title,0);
		
		// update the help content lines in screen according to new loaded menu
		SCREENS_print_help(screens_menus.option[screens_menus.stack[screens_menus.ui8_depth].i16_selected_index].pui8_help[0],0);
		SCREENS_print_help(screens_menus.option[screens_menus.stack[screens_menus.ui8_depth].i16_selected_index].pui8_help[1],1);
		
		// show the new menu
		SCREENS_menus_show();	

	}//if
		
	// RIGHT push button ( or ENTER ) has been pressed
	if ( (pui8_pushbutton_values[USER_IFACE_PUSHBT_4]==USER_IFACE_PRESSED) || (pui8_pushbutton_values[USER_IFACE_PUSHBT_ENC]==USER_IFACE_PRESSED) ) {
	
		i16_aux=screens_menus.stack[screens_menus.ui8_depth].i16_selected_index;
			
		int8_t i8_aux;
		i8_aux = screens_menus.option[i16_aux].i8_screen_type;
			
		if (i8_aux==SCREENS_MENU_SCREEN){
				
			// load the structure of the menu pointed by menu_to_jump
			SCREENS_menus_load(screens_menus.option[i16_aux].i16_screen_item_id);

		}else if (i8_aux == SCREENS_PARAMS_EDITION_SCREEN){
															
			// load the information of the parameters edition screen linked to that menu option
			SCREENS_params_edition_load(screens_menus.option[i16_aux].i16_screen_item_id);

		}else if (i8_aux == SCREENS_DIALOG_SCREEN){
																							
			// load the information of the dialog screen linked to that menu option
			SCREENS_dialog_load(screens_menus.option[i16_aux].i16_screen_item_id,SCREEN_DIALOG_INITIAL_STATE);
			
		}else if (i8_aux == SCREENS_F_EXPLORER_SCREEN){
		
			// load the information of the file explorer linked to that menu option
			SCREENS_f_explorer_load(screens_menus.option[i16_aux].i16_screen_item_id,SCREEN_F_EXPLORER_INITIAL_STATE);

		}else if (i8_aux == SCREENS_ENTER_TXT_SCREEN){
		
			// load the information of the enter text screen linked to that menu option
			SCREENS_enter_txt_load(screens_menus.option[i16_aux].i16_screen_item_id,SCREEN_ENTER_TXT_INITIAL_STATE);
			
		}//if (screens_menus.option[i16_aux].i16_menu_to_jump!=NULL){
		
	}//if (ui8_pushbutton_values[USER_IFACE_PUSHBT_4]==USER_IFACE_PRESSED)
	
	// LEFT push button ( or BACK ) has been pressed
	if (pui8_pushbutton_values[USER_IFACE_PUSHBT_1]==USER_IFACE_PRESSED){
		
		// before going back to the previous menu check that we are not in the first menu
		if (screens_menus.ui8_depth>0){
			
			// load the structure of the previous menu
			SCREENS_menus_load(SCREEN_MENU_PREVIOUS);
						
		}//if
		
	}//if

	// UP push button ( or UP direction in joystick ) has been pressed, or the encoder has increased its value
	if ( (pui8_pushbutton_values[USER_IFACE_PUSHBT_2]==USER_IFACE_PRESSED) || (pi16_encoders_var_value[USER_IFACE_ENC_1]>0) ){
		
		// before moving the cursor check that there are still positions to move it up
		if (screens_menus.stack[screens_menus.ui8_depth].i16_selected_index>0){
						
			// move the menu cursor up one position
			screens_menus.stack[screens_menus.ui8_depth].i16_selected_index--;
			
			// if necessary update the indexes that control what part of the menu is shown in the LCD
			if (screens_menus.stack[screens_menus.ui8_depth].i16_selected_index<screens_menus.stack[screens_menus.ui8_depth].i16_bottom_index){
				
				screens_menus.stack[screens_menus.ui8_depth].i16_bottom_index--;
				screens_menus.stack[screens_menus.ui8_depth].i16_top_index--;

				// as the navigation index has moved out of the screen then clear all the characters to write them all
				GRAPHIX_text_buffer_fill(SCREENS_MENU_BACKGROUND_CHAR,ATTR_NO_ATTRIBS,GRAPHIX_TEXT_COL_IDX_DARK_GREY,GRAPHIX_TEXT_COL_IDX_BLACK,GRAPHIX_TEXT_COL_IDX_BLACK);

			}//if
			
			// if selected index has changed then also update the help text
			SCREENS_print_help(screens_menus.option[screens_menus.stack[screens_menus.ui8_depth].i16_selected_index].pui8_help[0],0);
			SCREENS_print_help(screens_menus.option[screens_menus.stack[screens_menus.ui8_depth].i16_selected_index].pui8_help[1],1);

		}//if
		
		// show the new menu
		SCREENS_menus_show();

	}//if
	
	// DOWN push button ( or DOWN direction in joystick ) has been pressed, or the encoder has decreased its value
	if ( (pui8_pushbutton_values[USER_IFACE_PUSHBT_3]==USER_IFACE_PRESSED) || (pi16_encoders_var_value[USER_IFACE_ENC_1]<0) ){
		
		// before moving the cursor check that there are still positions to move it down
		if (screens_menus.stack[screens_menus.ui8_depth].i16_selected_index<(screens_menus.i16_num_options-1)){
			
			// move the menu cursor down one position
			screens_menus.stack[screens_menus.ui8_depth].i16_selected_index++;
			
			// if necessary update the indexes that control what part of the menu is shown in the LCD
			if (screens_menus.stack[screens_menus.ui8_depth].i16_selected_index>screens_menus.stack[screens_menus.ui8_depth].i16_top_index){
				
				screens_menus.stack[screens_menus.ui8_depth].i16_top_index++;
				screens_menus.stack[screens_menus.ui8_depth].i16_bottom_index++;

				// as the navigation index has moved out of the screen then clear all the characters to write them all
				GRAPHIX_text_buffer_fill(SCREENS_MENU_BACKGROUND_CHAR,ATTR_NO_ATTRIBS,GRAPHIX_TEXT_COL_IDX_DARK_GREY,GRAPHIX_TEXT_COL_IDX_BLACK,GRAPHIX_TEXT_COL_IDX_BLACK);
		
			}//if

			// if selected index has changed then also update the help text
			SCREENS_print_help(screens_menus.option[screens_menus.stack[screens_menus.ui8_depth].i16_selected_index].pui8_help[0],0);
			SCREENS_print_help(screens_menus.option[screens_menus.stack[screens_menus.ui8_depth].i16_selected_index].pui8_help[1],1);
			
		}//if

		// show the new menu
		SCREENS_menus_show();

	}//if

	// If any special function key has been pressed:
	// 1 - load the POINTER functions of the corresponding edition_screen ( screens_edition.current_param_edit_screen=x,
	// screens_edition.key_process_function=x, screens_edition.load_function=x ,screens_edition.show_function=x )
	// 2 - sets the "screens_control.state" to SCREENS_SHOWING_EDITION_SCREEN
	// 3 - call the POINTER function screens_edition.load_function which initializes the corresponding edition screen
	// parameters and also calls to screens_edition.show_function to update the content in the screen.
	// 4 - clears the pushbuttons and encoders values
	// SCREENS_special_ev_manager(encoders_var_value,pushbutton_values);		
	
}//SCREENS_menus_ev_manager
