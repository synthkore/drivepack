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

#include "screens_f_explorer.h"

// references to external structures
extern struct ref_screens_control screens_control;

struct ref_screens_f_explorer screens_f_explorer;



void SCREENS_f_explorer_clear(){
	int16_t i16_aux = 0;


    screens_f_explorer.load_cur_function = NULL;
    screens_f_explorer.show_cur_function = NULL;
	screens_f_explorer.ev_process_cur_function = NULL;
	
    screens_f_explorer.i8_state = 0;
	screens_f_explorer.timer = 0;
	screens_f_explorer.ui8_depth = 0;
	screens_f_explorer.ui8_consider_folders_files = FILE_SYS_FLAG_FILE|FILE_SYS_FLAG_FOLDER;
	
	for (i16_aux=0;i16_aux<SCREENS_F_EXPLORER_MAX_FOLDERS_DEPTH;i16_aux++){
			
		screens_f_explorer.stack[i16_aux].i16_selected_index =0;
		screens_f_explorer.stack[i16_aux].i16_bottom_index = 0;
		screens_f_explorer.stack[i16_aux].i16_top_index = 0;
		screens_f_explorer.stack[i16_aux].i16_elements_number = 0;
		
	}//for
	
}//SCREENS_f_explorer_clear



void SCREENS_f_explorer_init(){

	SCREENS_f_explorer_clear();

}//SCREENS_f_explorer_init


int8_t SCREENS_f_explorer_init_indexes(uint8_t ui8_consider_folders_files){
	int8_t  i8_ret_value= 1;	
	uint8_t ui8_depth = 0;

	// initialize the menu navigation general parameters
    screens_f_explorer.ui8_consider_folders_files = ui8_consider_folders_files;
	ui8_depth = screens_f_explorer.ui8_depth;
	screens_f_explorer.stack[ui8_depth].i16_elements_number = FILE_SYS_dir_file_count(SCREENS_F_EXPLORER_FILE_EXT_ROM_FILE,screens_f_explorer.ui8_consider_folders_files);
		
	if (screens_f_explorer.stack[ui8_depth].i16_elements_number<0){
		
		// ERROR: unit access error
		i8_ret_value = -1;
		
	}else if (screens_f_explorer.stack[ui8_depth].i16_elements_number==0){
			
		// no elements in the directory
		// OK: initialize the indexes used to show the patches in the LCD
		screens_f_explorer.stack[ui8_depth].i16_selected_index=0;
		screens_f_explorer.stack[ui8_depth].i16_bottom_index=0;
		screens_f_explorer.stack[ui8_depth].i16_top_index=0;

	}else{

		// OK: initialize the indexes used to show the patches in the LCD
		screens_f_explorer.stack[ui8_depth].i16_selected_index=0;
		screens_f_explorer.stack[ui8_depth].i16_bottom_index=0;
		if (screens_f_explorer.stack[ui8_depth].i16_elements_number<SCREENS_F_EXPLORER_VISIBLE_OPTIONS){
			screens_f_explorer.stack[ui8_depth].i16_top_index=screens_f_explorer.stack[ui8_depth].i16_elements_number-1;
		}else{
			screens_f_explorer.stack[ui8_depth].i16_top_index=SCREENS_F_EXPLORER_VISIBLE_OPTIONS-1;
		}//if
			
	}//if


}//SCREENS_f_explorer_init_indexes



int8_t SCREENS_f_explorer_load(int16_t i16_f_explorer_id,int16_t i16_f_explorer_state){
	int i8_ret_val = 1;


	switch (i16_f_explorer_id){

		case SCREEN_F_EXPLORER_STANDARD:		
		case SCREEN_F_EXPLORER_LOAD_ROM_FILE:
			screens_f_explorer.load_cur_function = SCREENS_f_explorer_load_LOAD_ROM_FILE;
			screens_f_explorer.show_cur_function =  SCREENS_f_explorer_show_LOAD_ROM_FILE;
			screens_f_explorer.ev_process_cur_function = SCREENS_f_explorer_ev_manager_LOAD_ROM_FILE;
			break;
		
		case SCREEN_F_EXPLORER_LOAD_RUN_ROM_FILE:
			screens_f_explorer.load_cur_function = SCREENS_f_explorer_load_LOAD_RUN_ROM_FILE;
			screens_f_explorer.show_cur_function = SCREENS_f_explorer_show_LOAD_RUN_ROM_FILE;
			screens_f_explorer.ev_process_cur_function = SCREENS_f_explorer_ev_manager_LOAD_RUN_ROM_FILE;
			break;
			
		case SCREEN_F_EXPLORER_SAVE_RAM:
			screens_f_explorer.load_cur_function = SCREENS_f_explorer_load_SAVE_RAM;
			screens_f_explorer.show_cur_function = SCREENS_f_explorer_show_SAVE_RAM;
			screens_f_explorer.ev_process_cur_function = SCREENS_f_explorer_ev_manager_SAVE_RAM;
			break;
							
		default:
			i8_ret_val = -1;
			break;
	
	}//switch

	if (i8_ret_val>=0){

		// update screens_f_explorer.i8_state before calling current file explorer screen specific load function
		screens_f_explorer.i8_state = i16_f_explorer_state;
		if (screens_f_explorer.load_cur_function!=NULL){
			i8_ret_val = screens_f_explorer.load_cur_function();
		}//if

		// set the SCREENS control FILE EXPLORER SCREEN mode
		screens_control.i8_current_type=SCREENS_F_EXPLORER_SCREEN;
		screens_control.i16_current_ID=i16_f_explorer_id;
		// set the OnLoad event to show the screen content
		screens_control.ui8_on_load_event = TRUE;
		
	}//if

	return i8_ret_val;

}//SCREENS_f_explorer_load



void SCREENS_f_explorer_show(){
	int16_t i16_row;
	uint8_t ui8_aux_string1[AUX_FUNCS_F_P_MAX_STR_SIZE_64];
	uint8_t ui8_aux_string2[AUX_FUNCS_F_P_MAX_STR_SIZE_64];
	uint8_t ui8_aux1;
	int16_t i16_file_index;
	int16_t i16_aux1;
	uint64_t ui64_size;
	uint8_t ui8_flags;
	uint8_t ui8_depth;

	
	// show current folder elements according to the settings specified on initialization 
	i16_row=0;
	ui8_depth = screens_f_explorer.ui8_depth;
	i16_file_index=  screens_f_explorer.stack[ui8_depth].i16_bottom_index;
	// place the file navigation index on the corresponding item
	i16_aux1=FILE_SYS_dir_file_goto_index( i16_file_index, SCREENS_F_EXPLORER_FILE_EXT_ROM_FILE, screens_f_explorer.ui8_consider_folders_files);
	while ( (i16_aux1>=0) &&(i16_file_index<=screens_f_explorer.stack[ui8_depth].i16_top_index) ){

		// IMPORTANT: PATCHES_FILE_NAME_N_EXT_LEN: Take care and always be sure
		// that ui8_aux_string is longer than PATCHES_FILE_NAME_N_EXT_LEN elements
		// ( +1 for the '0' )
		
		// get the type,name and size of the element ( file or folder )
		i16_aux1 = FILE_SYS_get_current_info((uint8_t*)ui8_aux_string1, AUX_FUNCS_F_P_MAX_STR_SIZE_64, &ui64_size, &ui8_flags);
		if (i16_aux1>=0){
			
			// get the icon code to show in front of the element name depending if it is a FOLDER or a FILE
			if (ui8_flags&FILE_SYS_FLAG_FILE){
				// currently selected item is a FILE, so set the FILE icon
				ui8_aux1=GRAPHIX_CHAR_FILE;
			}else if (ui8_flags&FILE_SYS_FLAG_FOLDER){
				// currently selected item is a FOLDER, so set the FODLER icon
				ui8_aux1=GRAPHIX_CHAR_FOLDER;
			}else{
				ui8_aux1='?';
			}//switch

			ui8_aux_string2[0]='\0';
			if (i16_file_index==screens_f_explorer.stack[ui8_depth].i16_selected_index){
				// add the brackets to mark the selected file
				AUX_FUNCS_strcat(ui8_aux_string2,"[ ",AUX_FUNCS_F_P_MAX_STR_SIZE_64);
				AUX_FUNCS_strcat_char(ui8_aux_string2,ui8_aux1,AUX_FUNCS_F_P_MAX_STR_SIZE_64);// add the FILE or FOLDER icon
				AUX_FUNCS_strcat(ui8_aux_string2," ",AUX_FUNCS_F_P_MAX_STR_SIZE_64);
				AUX_FUNCS_strcat(ui8_aux_string2,ui8_aux_string1, AUX_FUNCS_F_P_MAX_STR_SIZE_64);
				AUX_FUNCS_strcat(ui8_aux_string2,"]",AUX_FUNCS_F_P_MAX_STR_SIZE_64);
				// print the currently selected file/folder in the screen
				GRAPHIX_text_buffer_set_string(0,(uint16_t)i16_row,(uint8_t*)ui8_aux_string2,ATTR_NO_ATTRIBS, GRAPHIX_TEXT_COL_IDX_BLACK, GRAPHIX_TEXT_COL_NEUTRAL, GRAPHIX_TEXT_COL_IDX_BLACK);
			}else{
				AUX_FUNCS_strcat(ui8_aux_string2,"- ",AUX_FUNCS_F_P_MAX_STR_SIZE_64);
				AUX_FUNCS_strcat_char(ui8_aux_string2,ui8_aux1,AUX_FUNCS_F_P_MAX_STR_SIZE_64);// add the FILE or FOLDER icon
				AUX_FUNCS_strcat(ui8_aux_string2," ",AUX_FUNCS_F_P_MAX_STR_SIZE_64);
				AUX_FUNCS_strcat(ui8_aux_string2,ui8_aux_string1, AUX_FUNCS_F_P_MAX_STR_SIZE_64);
				AUX_FUNCS_strcat(ui8_aux_string2," ",AUX_FUNCS_F_P_MAX_STR_SIZE_64);// add " " blank space to clear the bracket "]"
				// print the currently selected file/folder in the screen
				GRAPHIX_text_buffer_set_string(0,(uint16_t)i16_row,(uint8_t*)ui8_aux_string2,ATTR_SPACE_BACKSYMBOL, GRAPHIX_TEXT_COL_NEUTRAL, GRAPHIX_TEXT_COL_IDX_BLACK, GRAPHIX_TEXT_COL_IDX_BLACK);
			}//
			
		}//if (i16_aux1<0)
		
		i16_file_index++;
		i16_row++;

		// get the next directory item
		i16_aux1=FILE_SYS_dir_file_goto_next(SCREENS_F_EXPLORER_FILE_EXT_ROM_FILE, screens_f_explorer.ui8_consider_folders_files);
		
	}//while

	// call to the function that shows current dialog screen specific data
	if (screens_f_explorer.show_cur_function!=NULL){
		screens_f_explorer.show_cur_function(SCREEN_EMPTY_MESSAGE);
	}//if
	
	// prints the 'up', 'down' or 'score' indicating the user if there are more elements available
	SCREENS_print_up_down_symbols(screens_f_explorer.stack[ui8_depth].i16_bottom_index, screens_f_explorer.stack[ui8_depth].i16_top_index, screens_f_explorer.stack[ui8_depth].i16_elements_number );

	// refresh the content of the buffer to screen
	GRAPHIX_text_buffer_refresh();
	
}//SCREENS_f_explorer_show



void SCREENS_f_explorer_ev_manager(int16_t * pi16_encoders_var_value, uint8_t * pui8_pushbutton_values){

	// call to the function that processes the KEYS pressed by the user in the current dialog screen
	if (screens_f_explorer.ev_process_cur_function!=NULL){
		screens_f_explorer.ev_process_cur_function(pi16_encoders_var_value, pui8_pushbutton_values);
	}//if

}//SCREENS_f_explorer_ev_manager



// LOAD_RUN_ROM_FILE file selection screen
int8_t SCREENS_f_explorer_load_LOAD_ROM_FILE(){
	int8_t i8_ret_val = 1;

	return i8_ret_val;
	
}//SCREENS_f_explorer_load_LOAD_ROM_FILE



int8_t SCREENS_f_explorer_show_LOAD_ROM_FILE(uint8_t * ui8_message){
	int8_t i8_ret_val = 1;
	
	return i8_ret_val;
	
}//SCREENS_f_explorer_show_LOAD_ROM_FILE



int8_t SCREENS_f_explorer_ev_manager_LOAD_ROM_FILE(int16_t * pi16_encoders_var_value, uint8_t * pui8_pushbutton_values){
	int8_t i8_ret_val = 1;
	uint8_t ui8_depth=0;
    uint64_t ui64_size = 0;
	uint8_t ui8_file_folder=0;
    int16_t i16_error_code = 0;
	uint8_t ui8_load_file = FALSE;
	uint8_t ui8_go_back_dir = FALSE;
	uint8_t ui8_enter_dir = FALSE;
	uint8_t ui8_is_parent_dir = FALSE;
	uint8_t ui8_is_curr_dir = FALSE;	
	uint8_t ui8_aux_string[AUX_FUNCS_F_P_MAX_STR_SIZE_64];


	// ON LOAD event, a new file explorer screen has been loaded
	if (screens_control.ui8_on_load_event==TRUE){

		// lines commented because the title and help are the same than the
		// previous screen and there is no need to rewrite them ( it causes
		// an undesired flicker )
		// print in screen the title of the new loaded menu
		// SCREENS_print_title("Load ROM file to RAM" ,0);
		// update the help content lines in screen according to new loaded menu
		SCREENS_print_help("Select ROM file to",0);
		SCREENS_print_help("load to RAM.",1);

		// reset the OnLoad event flag
		screens_control.ui8_on_load_event = FALSE;

		// clear the text buffer with the specified SCREENS_F_EXPLORER_BACKGROUND_CHAR before showing the information of the last open folder
		GRAPHIX_text_buffer_fill(SCREENS_F_EXPLORER_BACKGROUND_CHAR,ATTR_NO_ATTRIBS,GRAPHIX_TEXT_COL_IDX_DARK_GREY,GRAPHIX_TEXT_COL_IDX_BLACK,GRAPHIX_TEXT_COL_IDX_BLACK);
		// show the file explorer screen with the information of the last open folder
		SCREENS_f_explorer_show();

	}//if (screens_f_explorer.ui8_on_load_event==TRUE)

	// UP push button ( or UP direction in joystick ) has been pressed, or the encoder has increased its value
	if ( (pui8_pushbutton_values[USER_IFACE_PUSHBT_2]==USER_IFACE_PRESSED) || (pi16_encoders_var_value[USER_IFACE_ENC_1]>0) ){

        // check if the unit/directory is still available
        if (FILE_SYS_check_current_state()<0){
			
			// an error occurred so set the flag/code to load the corresponding error dialog when leaving
            i16_error_code = -1;
	
		}else{
		
			ui8_depth = screens_f_explorer.ui8_depth;
		
			// before moving the cursor check that there are still positions to move it up
			if (screens_f_explorer.stack[ui8_depth].i16_selected_index>0){
			
				// move the menu cursor up one position
				screens_f_explorer.stack[ui8_depth].i16_selected_index--;
			
				// if necessary update the indexes that control what part of the menu is shown in the LCD
				if (screens_f_explorer.stack[ui8_depth].i16_selected_index<screens_f_explorer.stack[ui8_depth].i16_bottom_index){
				
					screens_f_explorer.stack[ui8_depth].i16_bottom_index--;
					screens_f_explorer.stack[ui8_depth].i16_top_index--;
				
					// as the navigation index has moved out of the screen then clear all the characters to write them all
					GRAPHIX_text_buffer_fill(SCREENS_F_EXPLORER_BACKGROUND_CHAR,ATTR_NO_ATTRIBS,GRAPHIX_TEXT_COL_IDX_DARK_GREY,GRAPHIX_TEXT_COL_IDX_BLACK,GRAPHIX_TEXT_COL_IDX_BLACK);				
				
				}//if
			
			}//if

			// show the new file explorer screen
			SCREENS_f_explorer_show();
		
		}//if (FILE_SYS_check_current_state()
		
	}//if ( (pui8_pushbutton_values[USER_IFACE_PUSHBT_2]==USER_IFACE_PRESSED) 
	
	// DOWN push button ( or DOWN direction in joystick ) has been pressed, or the encoder has decreased its value
	if ( (pui8_pushbutton_values[USER_IFACE_PUSHBT_3]==USER_IFACE_PRESSED) || (pi16_encoders_var_value[USER_IFACE_ENC_1]<0) ){
		
        // check if the unit/directory is still available
        if (FILE_SYS_check_current_state()<0){
	        
			// an error occurred so set the flag/code to load the corresponding error dialog when leaving
			i16_error_code = -1;
	        
	     }else{		
		
			ui8_depth = screens_f_explorer.ui8_depth;
		
			// before moving the cursor check that there are still positions to move it down
			if (screens_f_explorer.stack[ui8_depth].i16_selected_index<(screens_f_explorer.stack[ui8_depth].i16_elements_number-1)){
			
				// move the menu cursor down one position
				screens_f_explorer.stack[ui8_depth].i16_selected_index++;
			
				// if necessary update the indexes that control what part of the menu is shown in the LCD
				if (screens_f_explorer.stack[ui8_depth].i16_selected_index>screens_f_explorer.stack[ui8_depth].i16_top_index){
				
					screens_f_explorer.stack[ui8_depth].i16_top_index++;
					screens_f_explorer.stack[ui8_depth].i16_bottom_index++;
				
					// as the navigation index has moved out of the screen then clear all the characters before showing all the folder items
					GRAPHIX_text_buffer_fill(SCREENS_F_EXPLORER_BACKGROUND_CHAR,ATTR_NO_ATTRIBS,GRAPHIX_TEXT_COL_IDX_DARK_GREY,GRAPHIX_TEXT_COL_IDX_BLACK,GRAPHIX_TEXT_COL_IDX_BLACK);
								
				}//if
			
			}//if

            // show the new file explorer screen
			SCREENS_f_explorer_show();
		
		}//if (FILE_SYS_check_current_state()
		
	}//if ( (pui8_pushbutton_values[USER_IFACE_PUSHBT_3]==USER_IFACE_PRESSED) 

    // BACK push button has been pressed
	if ( pui8_pushbutton_values[USER_IFACE_PUSHBT_1]==USER_IFACE_PRESSED ){

		if (screens_f_explorer.ui8_depth>0){
		
		    // set the flag to go back to previous directory
			ui8_go_back_dir = TRUE;
						
		}else{
			
			// the user is in the root directory so leave the directories navigator and go back to the previous screen
			SCREENS_dialog_load(SCREEN_DIALOG_ID_LOAD_ROM_FILE,SCREEN_DIALOG_LOAD_ROM_FILE_CANCELLED);
							
		}//if
				
	}//if ( pui8_pushbutton_values[USER_IFACE_PUSHBT_1]==USER_IFACE_PRESSED )	

    // ENTER push button has been pressed
	if ((pui8_pushbutton_values[USER_IFACE_PUSHBT_4]==USER_IFACE_PRESSED) || (pui8_pushbutton_values[USER_IFACE_PUSHBT_ENC]==USER_IFACE_PRESSED) ){
		
        // check if the unit/directory is still available
        if (FILE_SYS_check_current_state()<0){
			
			// an error occurred so set the flag/code to load the corresponding error dialog when leaving
			i16_error_code = -1;
			
	    }else{		
		
			// place the file navigation pointer pointing to the selected item index in the file explorer
			if (FILE_SYS_dir_file_goto_index(screens_f_explorer.stack[screens_f_explorer.ui8_depth].i16_selected_index, SCREENS_F_EXPLORER_FILE_EXT_ROM_FILE, screens_f_explorer.ui8_consider_folders_files)<0){
			
				// an error occurred so set the flag/code to load the corresponding error dialog when leaving
				i16_error_code = -1;
				
			}else{
			
				// get the information of selected item in the file explorer			
				if (FILE_SYS_get_current_info(ui8_aux_string,AUX_FUNCS_F_P_MAX_STR_SIZE_64,&ui64_size,&ui8_file_folder)<0){
			
					// an error occurred so set the flag/code to load the corresponding error dialog when leaving
					i16_error_code = -1;
			
				}else{
					 
					 // check if the current item is the '..' ( parent dir ) or '.' ( curr dir ) entries					
					 if ( (ui8_aux_string[0]=='.') && (ui8_aux_string[1]=='.') && (ui8_aux_string[2]=='\0') ){ ui8_is_parent_dir = TRUE;}
					 else if ( (ui8_aux_string[0]=='.') && (ui8_aux_string[1]=='\0') ){ ui8_is_curr_dir = TRUE;}
					 
					 if ( ((ui8_file_folder&FILE_SYS_FLAG_FOLDER)!=0) && ((screens_f_explorer.ui8_depth<(SCREENS_F_EXPLORER_MAX_FOLDERS_DEPTH-1)) || ui8_is_parent_dir ) ){
                        // selected item in the file explorer is a FOLDER and the current navigation depth is lower than 
						// SCREENS_F_EXPLORER_MAX_FOLDERS_DEPTH or the selected item is '..' to go bakc to previous dir

						 // selected item in the file explorer is a FOLDER
						 if (ui8_is_parent_dir){
							 // selected folder item is the '..' parent directory symbol
							 ui8_go_back_dir = TRUE;
						}else if (ui8_is_curr_dir){
							 // selected folder item is the '.' current directory symbol
							 // ... do nothing to keep in that directory
						}else{
							 // selected folder is a regular directory
							 ui8_enter_dir=TRUE;
						 }//if
						 
					 }else if ((ui8_file_folder&FILE_SYS_FLAG_FILE)!=0){
						 
						 ui8_load_file = TRUE;
						 
					 }//if ( ((ui8_file_folder&FILE_SYS_FLAG_FOLDER)!=0)
				
				}//if (FILE_SYS_get_current_info
					
			}//if (FILE_SYS_dir_file_goto_index 
			
		}//if (FILE_SYS_check_current_state()<0){

	}//if (pui8_pushbutton_values[USER_IFACE_PUSHBT_ENC]==USER_IFACE_PRESSED)

    // if the flag is set then go back to previous directory
	if (ui8_go_back_dir==TRUE){
		
		// move back to the previous directory and open it
		if ( (FILE_SYS_dir_ch("..")>=0) && (FILE_SYS_dir_open("")<0) ){
			
			// an error occurred so set the flag/code to load the corresponding error dialog when leaving
			i16_error_code = -1;
			
		}else{
						
			screens_f_explorer.ui8_depth--;
			// reload the file explorer on the new selected folder
			SCREENS_f_explorer_load(screens_control.i16_current_ID, SCREEN_F_EXPLORER_INITIAL_STATE);
								
		}//if
		
	}//if (ui8_go_back_directory
	
	// if the flag is set then enter to specified directory 
	if (ui8_enter_dir==TRUE){
		
		// try to change to new directory and open it
		if ( (FILE_SYS_dir_ch(ui8_aux_string)>=0) && (FILE_SYS_dir_open("")<0) ){

			// an error occurred so set the flag/code to load the corresponding error dialog when leaving
			i16_error_code = -1;
			
		}else{

			screens_f_explorer.ui8_depth++;
			// once the new selected folder has been open, update the file explorer navigation indexes
			SCREENS_f_explorer_init_indexes(FILE_SYS_FLAG_FILE|FILE_SYS_FLAG_FOLDER);
			// reload the file explorer on the new selected folder
			SCREENS_f_explorer_load(screens_control.i16_current_ID, SCREEN_F_EXPLORER_INITIAL_STATE);

		}//if
				
	}//if (ui8_enter_dir==TRUE)
	
	// if the flag is set then load the specified file directory 
	if (ui8_load_file==TRUE){
		
		// selected item in the file explorer is a FILE
		i16_error_code = DATA_IO_file_rom_load(ui8_aux_string);
		if (i16_error_code>=0){
			
			// show LOADED dialog screen
			SCREENS_dialog_load(SCREEN_DIALOG_ID_LOAD_ROM_FILE,SCREEN_DIALOG_LOAD_ROM_FILE_LOADED);
			
		}//if
		
	}//if (ui8_load_file==TRUE)

	// if the error flag/code is set to ERROR then load the corresponding dialog error
	if (i16_error_code <0){

		// show the corresponding error dialog screen
		if (i16_error_code==-1){
			SCREENS_dialog_load(SCREEN_DIALOG_ID_LOAD_ROM_FILE,SCREEN_DIALOG_LOAD_ROM_FILE_ACCESS_ERROR);
		}else if (i16_error_code==-2){
			SCREENS_dialog_load(SCREEN_DIALOG_ID_LOAD_ROM_FILE,SCREEN_DIALOG_LOAD_ROM_FILE_FORMAT_VER_ERROR);
		}else{
			SCREENS_dialog_load(SCREEN_DIALOG_ID_LOAD_ROM_FILE,SCREEN_DIALOG_LOAD_ROM_FILE_ACCESS_ERROR);
		}//if
			
	}//if
	
	return i8_ret_val;
	
}//SCREENS_f_explorer_ev_manager_LOAD_ROM_FILE



// LOAD_RUN_ROM_FILE file selection screen
int8_t SCREENS_f_explorer_load_LOAD_RUN_ROM_FILE(){
	int8_t i8_ret_val = 1;
	
	return i8_ret_val;
	
}//SCREENS_f_explorer_load_LOAD_RUN_ROM_FILE



int8_t SCREENS_f_explorer_show_LOAD_RUN_ROM_FILE(uint8_t * ui8_message){
	int8_t i8_ret_val = 1;
	
	return i8_ret_val;
	
}//SCREENS_f_explorer_show_LOAD_RUN_ROM_FILE



int8_t SCREENS_f_explorer_ev_manager_LOAD_RUN_ROM_FILE(int16_t * pi16_encoders_var_value, uint8_t * pui8_pushbutton_values){
	int8_t i8_ret_val = 1;
	uint8_t ui8_depth=0;
    uint64_t ui64_size = 0;
	uint8_t ui8_file_folder=0;
    int16_t i16_error_code = 0;
	uint8_t ui8_load_run_file = FALSE;
	uint8_t ui8_go_back_dir = FALSE;
	uint8_t ui8_enter_dir = FALSE;
	uint8_t ui8_is_parent_dir = FALSE;
	uint8_t ui8_is_curr_dir = FALSE;
	uint8_t ui8_aux_string[AUX_FUNCS_F_P_MAX_STR_SIZE_64];


	// ON LOAD event, a new file explorer screen has been loaded
	if (screens_control.ui8_on_load_event==TRUE){

		// lines commented because the title and help are the same than the
		// previous screen and there is no need to rewrite them ( it causes
		// an undesired flicker )
		// print in screen the title of the new loaded menu
		// SCREENS_print_title("Load and run ROM file" ,0);
		// update the help content lines in screen according to new loaded menu
		SCREENS_print_help("Select ROM file to",0);
		SCREENS_print_help("load to RAM and run.",1);

		// reset the OnLoad event flag
		screens_control.ui8_on_load_event = FALSE;
	
		// as the new folder has been loaded then clear the text buffer with the specified SCREENS_F_EXPLORER_BACKGROUND_CHAR
		GRAPHIX_text_buffer_fill(SCREENS_F_EXPLORER_BACKGROUND_CHAR,ATTR_NO_ATTRIBS,GRAPHIX_TEXT_COL_IDX_DARK_GREY,GRAPHIX_TEXT_COL_IDX_BLACK,GRAPHIX_TEXT_COL_IDX_BLACK);

		// show the file explorer screen
		SCREENS_f_explorer_show();
	
	}//if (screens_f_explorer.ui8_on_load_event==TRUE)

	// UP push button ( or UP direction in joystick ) has been pressed, or the encoder has increased its value
	if ( (pui8_pushbutton_values[USER_IFACE_PUSHBT_2]==USER_IFACE_PRESSED) || (pi16_encoders_var_value[USER_IFACE_ENC_1]>0) ){

        // check if the unit/directory is still available
        if (FILE_SYS_check_current_state()<0){
			
			// an error occurred so set the flag/code to load the corresponding error dialog when leaving
            i16_error_code = -1;
	
		}else{
		
			ui8_depth = screens_f_explorer.ui8_depth;
		
			// before moving the cursor check that there are still positions to move it up
			if (screens_f_explorer.stack[ui8_depth].i16_selected_index>0){
			
				// move the menu cursor up one position
				screens_f_explorer.stack[ui8_depth].i16_selected_index--;
			
				// if necessary update the indexes that control what part of the menu is shown in the LCD
				if (screens_f_explorer.stack[ui8_depth].i16_selected_index<screens_f_explorer.stack[ui8_depth].i16_bottom_index){
				
					screens_f_explorer.stack[ui8_depth].i16_bottom_index--;
					screens_f_explorer.stack[ui8_depth].i16_top_index--;
				
					// as the navigation index has moved out of the screen then clear all the characters to write them all
					GRAPHIX_text_buffer_fill(SCREENS_F_EXPLORER_BACKGROUND_CHAR,ATTR_NO_ATTRIBS,GRAPHIX_TEXT_COL_IDX_DARK_GREY,GRAPHIX_TEXT_COL_IDX_BLACK,GRAPHIX_TEXT_COL_IDX_BLACK);				
				
				}//if
			
			}//if

			// show the new file explorer screen
			SCREENS_f_explorer_show();
		
		}//if (FILE_SYS_check_current_state()
		
	}//if ( (pui8_pushbutton_values[USER_IFACE_PUSHBT_2]==USER_IFACE_PRESSED) 
	
	// DOWN push button ( or DOWN direction in joystick ) has been pressed, or the encoder has decreased its value
	if ( (pui8_pushbutton_values[USER_IFACE_PUSHBT_3]==USER_IFACE_PRESSED) || (pi16_encoders_var_value[USER_IFACE_ENC_1]<0) ){
		
        // check if the unit/directory is still available
        if (FILE_SYS_check_current_state()<0){
	        
			// an error occurred so set the flag/code to load the corresponding error dialog when leaving
			i16_error_code = -1;
	        
	     }else{		
		
			ui8_depth = screens_f_explorer.ui8_depth;
		
			// before moving the cursor check that there are still positions to move it down
			if (screens_f_explorer.stack[ui8_depth].i16_selected_index<(screens_f_explorer.stack[ui8_depth].i16_elements_number-1)){
			
				// move the menu cursor down one position
				screens_f_explorer.stack[ui8_depth].i16_selected_index++;
			
				// if necessary update the indexes that control what part of the menu is shown in the LCD
				if (screens_f_explorer.stack[ui8_depth].i16_selected_index>screens_f_explorer.stack[ui8_depth].i16_top_index){
				
					screens_f_explorer.stack[ui8_depth].i16_top_index++;
					screens_f_explorer.stack[ui8_depth].i16_bottom_index++;
				
					// as the navigation index has moved out of the screen then clear all the characters before showing all the folder items
					GRAPHIX_text_buffer_fill(SCREENS_F_EXPLORER_BACKGROUND_CHAR,ATTR_NO_ATTRIBS,GRAPHIX_TEXT_COL_IDX_DARK_GREY,GRAPHIX_TEXT_COL_IDX_BLACK,GRAPHIX_TEXT_COL_IDX_BLACK);
								
				}//if
			
			}//if

            // show the new file explorer screen
			SCREENS_f_explorer_show();
		
		}//if (FILE_SYS_check_current_state()
		
	}//if ( (pui8_pushbutton_values[USER_IFACE_PUSHBT_3]==USER_IFACE_PRESSED) 

    // BACK push button has been pressed
	if ( pui8_pushbutton_values[USER_IFACE_PUSHBT_1]==USER_IFACE_PRESSED ){

		if (screens_f_explorer.ui8_depth>0){
		
		    // set the flag to go back to previous directory
		    ui8_go_back_dir = TRUE;
				
		}else{
			
			// the user is in the root directory so leave the directories navigator and go back to the previous menu					
			// show corresponding dialog screen
			SCREENS_dialog_load(SCREEN_DIALOG_ID_LOAD_RUN_ROM_FILE,SCREEN_DIALOG_LOAD_RUN_ROM_FILE_CANCELLED);
					
		}//if
				
	}//if ( pui8_pushbutton_values[USER_IFACE_PUSHBT_1]==USER_IFACE_PRESSED )	
 
    // ENTER push button has been pressed
	if ( (pui8_pushbutton_values[USER_IFACE_PUSHBT_4]==USER_IFACE_PRESSED) || (pui8_pushbutton_values[USER_IFACE_PUSHBT_ENC]==USER_IFACE_PRESSED) ){
		
        // check if the unit/directory is still available
        if (FILE_SYS_check_current_state()<0){
			
			// an error occurred so set the flag/code to load the corresponding error dialog when leaving
			i16_error_code = -1;
			
	    }else{		
		
			// place the file navigation pointer pointing to the selected item index in the file explorer
			if (FILE_SYS_dir_file_goto_index(screens_f_explorer.stack[screens_f_explorer.ui8_depth].i16_selected_index, SCREENS_F_EXPLORER_FILE_EXT_ROM_FILE, screens_f_explorer.ui8_consider_folders_files)<0){
			
				// an error occurred so set the flag/code to load the corresponding error dialog when leaving
				i16_error_code = -1;
				
			}else{
						
				// get the information of selected item in the file explorer			
				if (FILE_SYS_get_current_info(ui8_aux_string,AUX_FUNCS_F_P_MAX_STR_SIZE_64,&ui64_size,&ui8_file_folder)<0){
			
					// an error occurred so set the flag/code to load the corresponding error dialog when leaving
					i16_error_code = -1;
			
				}else{
					
					 // check if the current item is the '..' ( parent dir ) or '.' ( curr dir ) entries
					 if ( (ui8_aux_string[0]=='.') && (ui8_aux_string[1]=='.') && (ui8_aux_string[2]=='\0') ){ ui8_is_parent_dir = TRUE;}
					 else if ( (ui8_aux_string[0]=='.') && (ui8_aux_string[1]=='\0') ){ ui8_is_curr_dir = TRUE;}
										
					 if ( ((ui8_file_folder&FILE_SYS_FLAG_FOLDER)!=0) && ((screens_f_explorer.ui8_depth<(SCREENS_F_EXPLORER_MAX_FOLDERS_DEPTH-1)) || ui8_is_parent_dir ) ){
                        // selected item in the file explorer is a FOLDER and the current navigation depth is lower than 
						// SCREENS_F_EXPLORER_MAX_FOLDERS_DEPTH or the selected item is '..' to go back to previous dir

						// selected item in the file explorer is a FOLDER
						if (ui8_is_parent_dir){
							// selected folder item is the '..' parent directory symbol
							ui8_go_back_dir = TRUE;
						}else if (ui8_is_curr_dir){
							// selected folder item is the '.' current directory symbol
							// ... do nothing to keep in that directory
						}else{
							// selected folder is a regular directory
							ui8_enter_dir=TRUE;
						}//if
				                
			          }else if ((ui8_file_folder&FILE_SYS_FLAG_FILE)!=0){		 
			                
							ui8_load_run_file = TRUE;
						
					}//if ( ((ui8_file_folder&FILE_SYS_FLAG_FOLDER)!=0)
				
				}//if (FILE_SYS_get_current_info
					
			}//if (FILE_SYS_dir_file_goto_index 
			
		}//if (FILE_SYS_check_current_state()<0){

	}//if (pui8_pushbutton_values[USER_IFACE_PUSHBT_ENC]==USER_IFACE_PRESSED)

	// if the flag is set then go back to previous directory
	if (ui8_go_back_dir==TRUE){
		
		// move back to the previous directory and open it
		if ( (FILE_SYS_dir_ch("..")>=0) && (FILE_SYS_dir_open("")<0) ){
			
			// an error occurred so set the flag/code to load the corresponding error dialog when leaving
			i16_error_code = -1;
			
		}else{
			
			screens_f_explorer.ui8_depth--;
			// reload the file explorer on the previous selected folder
			SCREENS_f_explorer_load(screens_control.i16_current_ID, SCREEN_F_EXPLORER_INITIAL_STATE);
			
		}//if
		
	}//if (ui8_go_back_directory
	
	// if the flag is set then enter to specified directory 
	if (ui8_enter_dir==TRUE){
		
		// try to change to new directory and open it
		if ( (FILE_SYS_dir_ch(ui8_aux_string)>=0) && (FILE_SYS_dir_open("")<0) ){

			// an error occurred so set the flag/code to load the corresponding error dialog when leaving
			i16_error_code = -1;
			
		}else{

			screens_f_explorer.ui8_depth++;
			// once the new selected folder has been open, update the file explorer navigation indexes
			SCREENS_f_explorer_init_indexes(FILE_SYS_FLAG_FILE|FILE_SYS_FLAG_FOLDER);
			// reload the file explorer on the new selected folder
			SCREENS_f_explorer_load(screens_control.i16_current_ID, SCREEN_F_EXPLORER_INITIAL_STATE);
			
		}//if
				
	}//if (ui8_enter_dir==TRUE){

	// if the flag is set then load the specified file directory
	if (ui8_load_run_file==TRUE){
		
		// selected item in the file explorer is a FILE
		i16_error_code = DATA_IO_file_rom_load(ui8_aux_string);
		if (i16_error_code>=0){
							
			// show LOADED and RUNNING dialog screen
			SCREENS_dialog_load(SCREEN_DIALOG_ID_LOAD_RUN_ROM_FILE,SCREEN_DIALOG_LOAD_RUN_ROM_FILE_LOADED);
							
		}//if (i16_error_code
		
	}//if (ui8_load_file==TRUE)

	// if the error flag/code is set to ERROR then load the corresponding dialog error
	if (i16_error_code <0){
		
		// show the corresponding error dialog screen
		if (i16_error_code==-1){
			SCREENS_dialog_load(SCREEN_DIALOG_ID_LOAD_RUN_ROM_FILE,SCREEN_DIALOG_LOAD_RUN_ROM_FILE_ACCESS_ERROR);
		}else if (i16_error_code==-2){
			SCREENS_dialog_load(SCREEN_DIALOG_ID_LOAD_RUN_ROM_FILE,SCREEN_DIALOG_LOAD_RUN_ROM_FILE_FORMAT_VER_ERROR);
		}else{
			SCREENS_dialog_load(SCREEN_DIALOG_ID_LOAD_RUN_ROM_FILE,SCREEN_DIALOG_LOAD_RUN_ROM_FILE_ACCESS_ERROR);
		}//if
			
	}//if
	
	return i8_ret_val;
	
}//SCREENS_f_explorer_ev_manager_LOAD_RUN_ROM_FILE



// SAVE_RAM folder selection screen
int8_t SCREENS_f_explorer_load_SAVE_RAM(){
	int8_t i8_ret_val = 1;
	
	return i8_ret_val;
	
}//SCREENS_f_explorer_load_SAVE_RAM



int8_t SCREENS_f_explorer_show_SAVE_RAM(uint8_t * ui8_message){
	int8_t i8_ret_val = 1;
	
	return i8_ret_val;
	
}//SCREENS_f_explorer_show_SAVE_RAM



int8_t SCREENS_f_explorer_ev_manager_SAVE_RAM(int16_t * pi16_encoders_var_value, uint8_t * pui8_pushbutton_values){
	int8_t i8_ret_val = 1;
	uint8_t ui8_depth=0;
    uint64_t ui64_size = 0;
	uint8_t ui8_file_folder=0;
    int16_t i16_error_code = 0;
	uint8_t ui8_save_file = FALSE;
	uint8_t ui8_go_back_dir = FALSE;
	uint8_t ui8_enter_dir = FALSE;
	uint8_t ui8_is_parent_dir = FALSE;
	uint8_t ui8_is_curr_dir = FALSE;
	uint8_t ui8_aux_string[AUX_FUNCS_F_P_MAX_STR_SIZE_64];


	// ON LOAD event, a new file explorer screen has been loaded
	if (screens_control.ui8_on_load_event==TRUE){

		// lines commented because the title and help are the same than the
		// previous screen and there is no need to rewrite them ( it causes
		// an undesired flicker )
		// print in screen the title of the new loaded menu
		// SCREENS_print_title("Load and run ROM file" ,0);
		// update the help content lines in screen according to new loaded menu
		SCREENS_print_help("Select folder to save ",0);
		SCREENS_print_help("RAM content in.",1);

		// reset the OnLoad event flag
		screens_control.ui8_on_load_event = FALSE;

		// clear the text buffer with the specified SCREENS_F_EXPLORER_BACKGROUND_CHAR before showing the information of the last open folder
		GRAPHIX_text_buffer_fill(SCREENS_F_EXPLORER_BACKGROUND_CHAR,ATTR_NO_ATTRIBS,GRAPHIX_TEXT_COL_IDX_DARK_GREY,GRAPHIX_TEXT_COL_IDX_BLACK,GRAPHIX_TEXT_COL_IDX_BLACK);
		// show the file explorer screen with the information of the last open folder
		SCREENS_f_explorer_show();
		
	}//if (screens_f_explorer.ui8_on_load_event==TRUE)

	// UP push button ( or UP direction in joystick ) has been pressed, or the encoder has increased its value
	if ( (pui8_pushbutton_values[USER_IFACE_PUSHBT_2]==USER_IFACE_PRESSED) || (pi16_encoders_var_value[USER_IFACE_ENC_1]>0) ){

        // check if the unit/directory is still available
        if (FILE_SYS_check_current_state()<0){
			
			// an error occurred so set the flag/code to load the corresponding error dialog when leaving
            i16_error_code = -1;
	
		}else{
		
			ui8_depth = screens_f_explorer.ui8_depth;
		
			// before moving the cursor check that there are still positions to move it up
			if (screens_f_explorer.stack[ui8_depth].i16_selected_index>0){
			
				// move the menu cursor up one position
				screens_f_explorer.stack[ui8_depth].i16_selected_index--;
			
				// if necessary update the indexes that control what part of the menu is shown in the LCD
				if (screens_f_explorer.stack[ui8_depth].i16_selected_index<screens_f_explorer.stack[ui8_depth].i16_bottom_index){
				
					screens_f_explorer.stack[ui8_depth].i16_bottom_index--;
					screens_f_explorer.stack[ui8_depth].i16_top_index--;
				
					// as the navigation index has moved out of the screen then clear all the characters to write them all
					GRAPHIX_text_buffer_fill(SCREENS_F_EXPLORER_BACKGROUND_CHAR,ATTR_NO_ATTRIBS,GRAPHIX_TEXT_COL_IDX_DARK_GREY,GRAPHIX_TEXT_COL_IDX_BLACK,GRAPHIX_TEXT_COL_IDX_BLACK);				
				
				}//if
			
			}//if

			// show the new file explorer screen
			SCREENS_f_explorer_show();
		
		}//if (FILE_SYS_check_current_state()
		
	}//if ( (pui8_pushbutton_values[USER_IFACE_PUSHBT_2]==USER_IFACE_PRESSED) 
	
	// DOWN push button ( or DOWN direction in joystick ) has been pressed, or the encoder has decreased its value
	if ( (pui8_pushbutton_values[USER_IFACE_PUSHBT_3]==USER_IFACE_PRESSED) || (pi16_encoders_var_value[USER_IFACE_ENC_1]<0) ){
		
        // check if the unit/directory is still available
        if (FILE_SYS_check_current_state()<0){
	        
			// an error occurred so set the flag/code to load the corresponding error dialog when leaving
			i16_error_code = -1;
	        
	     }else{		
		
			ui8_depth = screens_f_explorer.ui8_depth;
		
			// before moving the cursor check that there are still positions to move it down
			if (screens_f_explorer.stack[ui8_depth].i16_selected_index<(screens_f_explorer.stack[ui8_depth].i16_elements_number-1)){
			
				// move the menu cursor down one position
				screens_f_explorer.stack[ui8_depth].i16_selected_index++;
			
				// if necessary update the indexes that control what part of the menu is shown in the LCD
				if (screens_f_explorer.stack[ui8_depth].i16_selected_index>screens_f_explorer.stack[ui8_depth].i16_top_index){
				
					screens_f_explorer.stack[ui8_depth].i16_top_index++;
					screens_f_explorer.stack[ui8_depth].i16_bottom_index++;
				
					// as the navigation index has moved out of the screen then clear all the characters before showing all the folder items
					GRAPHIX_text_buffer_fill(SCREENS_F_EXPLORER_BACKGROUND_CHAR,ATTR_NO_ATTRIBS,GRAPHIX_TEXT_COL_IDX_DARK_GREY,GRAPHIX_TEXT_COL_IDX_BLACK,GRAPHIX_TEXT_COL_IDX_BLACK);
								
				}//if
			
			}//if

            // show the new file explorer screen
			SCREENS_f_explorer_show();
		
		}//if (FILE_SYS_check_current_state()
		
	}//if ( (pui8_pushbutton_values[USER_IFACE_PUSHBT_3]==USER_IFACE_PRESSED) 

    // BACK push button has been pressed
	if ( pui8_pushbutton_values[USER_IFACE_PUSHBT_1]==USER_IFACE_PRESSED ){

		if (screens_f_explorer.ui8_depth>0){
		
		    // set the flag to go back to previous directory
		    ui8_go_back_dir = TRUE;
				
		}else{
			
			// the user is in the root directory so leave the directories navigator and go back to the previous menu					
			// show corresponding dialog screen
			SCREENS_dialog_load(SCREEN_DIALOG_ID_LOAD_RUN_ROM_FILE,SCREEN_DIALOG_LOAD_RUN_ROM_FILE_CANCELLED);
					
		}//if
				
	}//if ( pui8_pushbutton_values[USER_IFACE_PUSHBT_1]==USER_IFACE_PRESSED )	
 
    // ENTER push button has been pressed
	if ( (pui8_pushbutton_values[USER_IFACE_PUSHBT_4]==USER_IFACE_PRESSED) || (pui8_pushbutton_values[USER_IFACE_PUSHBT_ENC]==USER_IFACE_PRESSED) ){
		
        // check if the unit/directory is still available
        if (FILE_SYS_check_current_state()<0){
			
			// an error occurred so set the flag/code to load the corresponding error dialog when leaving
			i16_error_code = -1;
			
	    }else{		
		
			// place the file navigation pointer pointing to the selected item index in the file explorer
			if (FILE_SYS_dir_file_goto_index(screens_f_explorer.stack[screens_f_explorer.ui8_depth].i16_selected_index, SCREENS_F_EXPLORER_FILE_EXT_ROM_FILE, screens_f_explorer.ui8_consider_folders_files)<0){
			
				// an error occurred so set the flag/code to load the corresponding error dialog when leaving
				i16_error_code = -1;
				
			}else{
						
				// get the information of selected item in the file explorer			
				if (FILE_SYS_get_current_info(ui8_aux_string,AUX_FUNCS_F_P_MAX_STR_SIZE_64,&ui64_size,&ui8_file_folder)<0){
			
					// an error occurred so set the flag/code to load the corresponding error dialog when leaving
					i16_error_code = -1;
			
				}else{
					
					// check if the current item is the '..' ( parent dir ) or '.' ( curr dir ) entries
					if ( (ui8_aux_string[0]=='.') && (ui8_aux_string[1]=='.') && (ui8_aux_string[2]=='\0') ){ ui8_is_parent_dir = TRUE;}
					else if ( (ui8_aux_string[0]=='.') && (ui8_aux_string[1]=='\0') ){ ui8_is_curr_dir = TRUE;}
					
					if ( ((ui8_file_folder&FILE_SYS_FLAG_FOLDER)!=0) && ((screens_f_explorer.ui8_depth<(SCREENS_F_EXPLORER_MAX_FOLDERS_DEPTH-1)) || ui8_is_parent_dir ) ){
						// selected item in the file explorer is a FOLDER and the current navigation depth is lower than 
						// SCREENS_F_EXPLORER_MAX_FOLDERS_DEPTH or the selected item is '..' to go bakc to previous dir

						// selected item in the file explorer is a FOLDER
						if (ui8_is_parent_dir){
							// selected folder item is the '..' parent directory symbol
							ui8_go_back_dir = TRUE;
						}else if (ui8_is_curr_dir){
							// selected folder item is the '.' current directory symbol, so it means the user
							// has pressed over the current directory symbol to save file in
				            ui8_save_file=TRUE;
						}else{
							// selected folder is a regular directory
							ui8_enter_dir=TRUE;
						}//if
						
					}//if ( ((ui8_file_folder&FILE_SYS_FLAG_FOLDER)!=0)
				
				}//if (FILE_SYS_get_current_info
					
			}//if (FILE_SYS_dir_file_goto_index 
			
		}//if (FILE_SYS_check_current_state()<0){

	}//if (pui8_pushbutton_values[USER_IFACE_PUSHBT_4]==USER_IFACE_PRESSED)

    // ENTER push button has been pressed
	if  (pui8_pushbutton_values[USER_IFACE_PUSHBT_ENC]==USER_IFACE_PRESSED) {
		
        // check if the unit/directory is still available
        if (FILE_SYS_check_current_state()<0){
			
			// an error occurred so set the flag/code to load the corresponding error dialog when leaving
			i16_error_code = -1;
			
	    }else{		
		
			// place the file navigation pointer pointing to the selected item index in the file explorer
			if (FILE_SYS_dir_file_goto_index(screens_f_explorer.stack[screens_f_explorer.ui8_depth].i16_selected_index, SCREENS_F_EXPLORER_FILE_EXT_ROM_FILE, screens_f_explorer.ui8_consider_folders_files)<0){
			
				// an error occurred so set the flag/code to load the corresponding error dialog when leaving
				i16_error_code = -1;
				
			}else{
                
				// if the user presses that key then directly save file in current folder
				ui8_save_file=TRUE;

/*						
				// get the information of selected item in the file explorer			
				if (FILE_SYS_get_current_info(ui8_aux_string,AUX_FUNCS_F_P_MAX_STR_SIZE_64,&ui64_size,&ui8_file_folder)<0){
			
					// an error occurred so set the flag/code to load the corresponding error dialog when leaving
					i16_error_code = -1;
			
				}else{
					
					// check if the current item is the '..' ( parent dir ) or '.' ( curr dir ) entries
					if ( (ui8_aux_string[0]=='.') && (ui8_aux_string[1]=='.') && (ui8_aux_string[2]=='\0') ){ ui8_is_parent_dir = TRUE;}
					else if ( (ui8_aux_string[0]=='.') && (ui8_aux_string[1]=='\0') ){ ui8_is_curr_dir = TRUE;}
					
					if ( ((ui8_file_folder&FILE_SYS_FLAG_FOLDER)!=0) && ((screens_f_explorer.ui8_depth<(SCREENS_F_EXPLORER_MAX_FOLDERS_DEPTH-1)) || ui8_is_parent_dir ) ){
						
						// selected item in the file explorer is a FOLDER and the current navigation depth is lower than 
						// SCREENS_F_EXPLORER_MAX_FOLDERS_DEPTH or the selected item is '..' to go bakc to previous dir

						// selected item in the file explorer is a FOLDER
						if (ui8_is_parent_dir){
							// selected folder item is the '..' parent directory symbol
							ui8_go_back_dir = TRUE;
						}else if (ui8_is_curr_dir){
							// selected folder item is the '.' current directory symbol
							// ... do nothing to keep in that directory
						}else{
							// selected folder is a regular directory
							ui8_enter_dir=TRUE;
						}//if
				                
			        }else if ((ui8_file_folder&FILE_SYS_FLAG_FILE)!=0){		 
			                
						ui8_save_file = TRUE;
						
					}//if ( ((ui8_file_folder&FILE_SYS_FLAG_FOLDER)!=0)
					
				}//if (FILE_SYS_get_current_info
*/				
			}//if (FILE_SYS_dir_file_goto_index 
			
		}//if (FILE_SYS_check_current_state()<0){

	}//if (pui8_pushbutton_values[USER_IFACE_PUSHBT_ENC]==USER_IFACE_PRESSED)

	// if the flag is set then go back to previous directory
	if (ui8_go_back_dir==TRUE){
		
		// move back to the previous directory and open it
		if ( (FILE_SYS_dir_ch("..")>=0) && (FILE_SYS_dir_open("")<0) ){
			
			// an error occurred so set the flag/code to load the corresponding error dialog when leaving
			i16_error_code = -1;
			
		}else{
			
			screens_f_explorer.ui8_depth--;
			// reload the file explorer on the previous selected folder
			SCREENS_f_explorer_load(screens_control.i16_current_ID, SCREEN_F_EXPLORER_INITIAL_STATE);
			
		}//if
		
	}//if (ui8_go_back_directory
	
	// if the flag is set then enter to specified directory 
	if (ui8_enter_dir==TRUE){
		
		// try to change to new directory and open it
		if ( (FILE_SYS_dir_ch(ui8_aux_string)>=0) && (FILE_SYS_dir_open("")<0) ){

			// an error occurred so set the flag/code to load the corresponding error dialog when leaving
			i16_error_code = -1;
			
		}else{

			screens_f_explorer.ui8_depth++;
			// once the new selected folder has been open, update the file explorer navigation indexes
			SCREENS_f_explorer_init_indexes(FILE_SYS_FLAG_FOLDER);
			// reload the file explorer on the new selected folder
			SCREENS_f_explorer_load(screens_control.i16_current_ID, SCREEN_F_EXPLORER_INITIAL_STATE);			
			
		}//if
				
	}//if (ui8_enter_dir==TRUE){

	// if the flag is set then load the specified file directory
	if (ui8_save_file==TRUE){
		
		// get the information of selected item in the file explorer
		if (FILE_SYS_get_current_info(ui8_aux_string,AUX_FUNCS_F_P_MAX_STR_SIZE_64,&ui64_size,&ui8_file_folder)<0){
					
			// an error occurred so set the flag/code to load the corresponding error dialog when leaving
			i16_error_code = -1;
					
		}else{
					
			// load and show ENTER_FILE_NAME
			SCREENS_enter_txt_load(SCREEN_ENTER_TXT_FILE_TO_SAVE, SCREEN_F_EXPLORER_INITIAL_STATE);
					
		}//if (FILE_SYS_get_current_info
		
	}//if (ui8_save_file==TRUE)

	if (i16_error_code <0){
			
		// show the corresponding error dialog screen
		if (i16_error_code==-1){
			SCREENS_dialog_load(SCREEN_DIALOG_ID_SAVE_RAM_AS_FILE,SCREEN_DIALOG_SAVE_RAM_AS_FILE_ACCESS_ERROR);
		}else{
			SCREENS_dialog_load(SCREEN_DIALOG_ID_SAVE_RAM_AS_FILE,SCREEN_DIALOG_SAVE_RAM_AS_FILE_ACCESS_ERROR);
		}//if
			
	}//if
	
	return i8_ret_val;
	
}//SCREENS_f_explorer_ev_manager_SAVE_RAM