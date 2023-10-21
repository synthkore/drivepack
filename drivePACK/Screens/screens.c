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

#include "screens.h"


struct ref_screens_control screens_control;



void SCREENS_Init(){


	SCREENS_menus_init();
	SCREENS_dialog_init();
	SCREENS_params_edition_init();
	SCREENS_f_explorer_init();

	// initialize parameters edition screen and set the pointers to the functions that manage the selected screeen
	SCREENS_dialog_clear();
			
	// load the information of the dialog screen linked to that menu option
	SCREENS_dialog_load(SCREEN_DIALOG_ID_INTRO,SCREEN_DIALOG_INITIAL_STATE);
	
}//SCREENS_Init	



void SCREENS_print_up_down_symbols(int16_t i16_bottom_index, int16_t i16_top_index, int16_t i16_max_options ){
    int16_t i16_aux = 0;

	
	// print the UP ARROW if necessary
	if (i16_bottom_index>0){
		// SCREEN_UP_CHAR is the UP ARROW char code
		GRAPHIX_text_buffer_set_char(21,0,SCREEN_UP_CHAR, ATTR_NO_ATTRIBS, GRAPHIX_TEXT_COL_IDX_YELLOW, GRAPHIX_TEXT_COL_IDX_BLACK, GRAPHIX_TEXT_COL_IDX_BLACK);
	}else{
		GRAPHIX_text_buffer_set_char(21,0,SCREEN_END_CHAR, ATTR_NO_ATTRIBS, GRAPHIX_TEXT_COL_IDX_DARK_YELLOW, GRAPHIX_TEXT_COL_IDX_BLACK, GRAPHIX_TEXT_COL_IDX_BLACK);
	}//if
	
	// print the DOWN ARROW if necessary
	if (i16_top_index<(i16_max_options-1)){
		// SCREEN_DOWN_CHAR is the DOWN ARROW char code
		GRAPHIX_text_buffer_set_char(21,11,SCREEN_DOWN_CHAR, ATTR_NO_ATTRIBS, GRAPHIX_TEXT_COL_IDX_YELLOW, GRAPHIX_TEXT_COL_IDX_BLACK, GRAPHIX_TEXT_COL_IDX_BLACK);
	}else{
		GRAPHIX_text_buffer_set_char(21,11,SCREEN_END_CHAR, ATTR_NO_ATTRIBS, GRAPHIX_TEXT_COL_IDX_DARK_YELLOW, GRAPHIX_TEXT_COL_IDX_BLACK, GRAPHIX_TEXT_COL_IDX_BLACK);
	}//if
 
    // print the lateral bar
	for (i16_aux=1;i16_aux<11;i16_aux++){
		GRAPHIX_text_buffer_set_char(21,i16_aux,SCREEN_BAR_CHAR, ATTR_NO_ATTRIBS, GRAPHIX_TEXT_COL_IDX_DARK_YELLOW, GRAPHIX_TEXT_COL_IDX_BLACK, GRAPHIX_TEXT_COL_IDX_BLACK);
	}//for

}//SCREENS_print_up_down_symbols



void SCREENS_print_title(uint8_t * pui8_title, uint8_t ui8_line_idx ){
    uint8_t ui8_line_content[AUX_FUNCS_F_P_MAX_STR_SIZE_32];

    
    memset(ui8_line_content,'\0',sizeof(ui8_line_content));

	switch (ui8_line_idx){
		case 0:
		    // clear the title area before writing the string characters
		    GRAPHIX_rectangle(0, 0, 159, 8, LCD_COLOR_BLACK, LCD_COLOR_BLACK, FALSE, TRUE);	
			AUX_FUNCS_lstrcpy(ui8_line_content, pui8_title, AUX_FUNCS_F_P_MAX_STR_SIZE_32);
			AUX_FUNCS_pad_end_with_char(ui8_line_content, ' ' , AUX_FUNCS_F_P_MAX_STR_SIZE_32-1,  AUX_FUNCS_F_P_MAX_STR_SIZE_32);
				
			// write the received title line
			GRAPHIX_print_string(1, 1, 1, 1, LCD_COLOR_YELLOW, LCD_COLOR_BLACK, ui8_line_content);		
			break;
		default:
			break;
	}//switch
	
}//SCREENS_print_title



void SCREENS_print_help(uint8_t * pui8_help, uint8_t ui8_line_idx){
    uint8_t ui8_line_content[AUX_FUNCS_F_P_MAX_STR_SIZE_32];

    
	memset(ui8_line_content,'\0',sizeof(ui8_line_content));

	switch (ui8_line_idx){
		case 0:
		    // clear the help first line area before writing the string characters
		    // GRAPHIX_rectangle(0, 109, 159, 117, LCD_COLOR_BLACK, LCD_COLOR_BLACK, FALSE, TRUE);
			AUX_FUNCS_lstrcpy(ui8_line_content,pui8_help, AUX_FUNCS_F_P_MAX_STR_SIZE_32);
			AUX_FUNCS_pad_end_with_char(ui8_line_content, ' ' , AUX_FUNCS_F_P_MAX_STR_SIZE_32-1,  AUX_FUNCS_F_P_MAX_STR_SIZE_32);			
			// write the received help line
			GRAPHIX_print_string(1, 109, 1, 1, LCD_COLOR_YELLOW, LCD_COLOR_BLACK, ui8_line_content);
			break;
		case 1:
		    // clear the help second line area before writing the string characters
			// GRAPHIX_rectangle(0, 118, 159, 126, LCD_COLOR_BLACK, LCD_COLOR_BLACK, FALSE, TRUE);
			AUX_FUNCS_lstrcpy(ui8_line_content,pui8_help, AUX_FUNCS_F_P_MAX_STR_SIZE_32);
			AUX_FUNCS_pad_end_with_char(ui8_line_content, ' ' , AUX_FUNCS_F_P_MAX_STR_SIZE_32-1,  AUX_FUNCS_F_P_MAX_STR_SIZE_32);		
			// write the received help line
			GRAPHIX_print_string(1, 118, 1, 1, LCD_COLOR_YELLOW, LCD_COLOR_BLACK, ui8_line_content);
			break;
		default:
			break;
	}//switch
	
}//SCREENS_print_help



void SCREENS_ev_manager(int16_t * pi16_encoders_var_value, uint8_t * pui8_buttons_state){


	switch (screens_control.i8_current_type){
	
		case SCREENS_MENU_SCREEN:
			// if screens_control.state==SCREENS_MENU_SCREEN check keys:
			// 1 - if the user presses right button and selected menu option points to another menu
			//    1.1 - load and show the corresponding menu
			// 2 - if the user presses right button and selected menu option points to an edition screen:
			//    2.1 - load the POINTER functions of the corresponding edition_screen ( screens_edition.current_param_edit_screen=x,
			//    screens_edition.key_process_function=x, screens_edition.load_function=x ,screens_edition.show_function=x )
			//    2.2 - sets the "screens_control.state" to SCREENS_SHOWING_EDITION_SCREEN
			//    2.3 - call the POINTER function screens_edition.load_function which initializes the corresponding edition screen
			//    parameters and also calls to screens_edition.show_function to update the content in the screen.
			//    2.4 - clears the pushbuttons and encoders values
			// 3 - if the user presses left button:
			//    3.1 load and show the previous menu
			//    3.2 clears the pushbuttons and encoders values
			// 4 - if the user presses up button
			//    4.1 decrease one position the selection index and show the current menu
			//    4.2 clears the pushbuttons and encoders values
			// 5 - if the user presses down button
			//    5.1 decrease one position the selection index and show the current menu
			//    5.2 clears the pushbuttons and encoders values
			// ...
		    SCREENS_menus_ev_manager(pi16_encoders_var_value, pui8_buttons_state);
			break;
	
		case SCREENS_PARAMS_EDITION_SCREEN:
			SCREENS_params_edition_ev_manager(pi16_encoders_var_value, pui8_buttons_state);
			break;
	
		case SCREENS_DIALOG_SCREEN:
			SCREENS_dialog_ev_manager(pi16_encoders_var_value, pui8_buttons_state);
			break;

		case SCREENS_F_EXPLORER_SCREEN:
			SCREENS_f_explorer_ev_manager( pi16_encoders_var_value, pui8_buttons_state);
			break;

		case SCREENS_ENTER_TXT_SCREEN:
			SCREENS_enter_txt_ev_manager( pi16_encoders_var_value, pui8_buttons_state);
			break;

	}//switch

}//SCREENS_ev_manager