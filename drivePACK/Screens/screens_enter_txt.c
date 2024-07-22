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

#include "screens_enter_txt.h"


// reference to external structures
extern struct ref_screens_control screens_control;

struct ref_screens_enter_txt screens_enter_txt;



void SCREENS_enter_txt_clear(){


	screens_enter_txt.ev_process_cur_function = NULL;
	screens_enter_txt.load_cur_function = NULL;
	screens_enter_txt.show_cur_function = NULL;
	
	screens_enter_txt.i8_state = 0;
    screens_enter_txt.i8_cursor_blink_flag = 0;	
	screens_enter_txt.ui8_max_allowed_len = 0;
	memset(screens_enter_txt.ui8_text,'\0',sizeof(screens_enter_txt.ui8_text));
		
}//SCREENS_enter_txt_clear



void SCREENS_enter_txt_init(){

	SCREENS_enter_txt_clear();

}//SCREENS_enter_txt_init



int8_t SCREENS_enter_txt_load(int16_t i16_enter_text_id,int16_t i16_enter_text_state){
	int i8_ret_val = 1;

	switch (i16_enter_text_id){

		case SCREEN_ENTER_TXT_STANDARD:
		case SCREEN_ENTER_TXT_FILE_TO_SAVE:
			SCREENS_enter_txt_clear();
			screens_enter_txt.load_cur_function = SCREENS_enter_txt_load_SAVE_RAM;
			screens_enter_txt.show_cur_function =  SCREENS_enter_txt_show_SAVE_RAM;
			screens_enter_txt.ev_process_cur_function = SCREENS_enter_txt_ev_manager_SAVE_RAM;
			screens_enter_txt.ui8_max_allowed_len = 8;
			screens_enter_txt.ui8_text[0]='a';// initialize first character in the screen			
			break;

		default:
			i8_ret_val = -1;
			break;
		
	}//switch
	
	if (i8_ret_val>=0){
		
		// update screens_enter_txt.i8_state before calling current enter txt specific screen load function
		screens_enter_txt.i8_state = i16_enter_text_state;
		if (screens_enter_txt.load_cur_function!=NULL){
			i8_ret_val = screens_enter_txt.load_cur_function();
		}//if		
		
		// initialize variables used to manage edition cursor blinking
		screens_enter_txt.i8_cursor_blink_flag = 0;
		screens_enter_txt.timer_blink = TIMING_SetTicksDelay(SCREEN_ENTER_TXT_BLINK_TIMEOUT);
								
		// set the SCREENS control in ENTER TXT SCREEN mode
		screens_control.i8_current_type=SCREENS_ENTER_TXT_SCREEN;
		screens_control.i16_current_ID=i16_enter_text_id;
		// set the OnLoad event to show the screen content
		screens_control.ui8_on_load_event = TRUE;
		
	}//if
		
	return i8_ret_val;
	
}//SCREENS_enter_txt_load


void SCREENS_enter_txt_show(){

	
	// call to the function that shows current dialog screen specific data
	if (screens_enter_txt.show_cur_function!=NULL){
		screens_enter_txt.show_cur_function(SCREEN_EMPTY_MESSAGE);
	}//if

}//SCREENS_enter_txt_show



void SCREENS_enter_txt_ev_manager(int16_t * pi16_encoders_var_value, uint8_t * pui8_pushbutton_values){
	uint8_t ui8_aux1=0;
	uint8_t ui8_aux2=0;
	
	
	// processes the common events in the current enter text screen

	// in the on load event show screen for first time
	if (screens_control.ui8_on_load_event==TRUE){
				
		// reset the OnLoad event flag
		screens_control.ui8_on_load_event = FALSE;
				
		// show current dialog screen
		SCREENS_enter_txt_show();
					
	}//if		
		
	// check if the time to update the blinking cursor
	if (TIMING_CheckTicksDelayReached(screens_enter_txt.timer_blink)){
				
		// get the index of the last entered character
		ui8_aux1 = 0;
		while ((screens_enter_txt.ui8_text[ui8_aux1]!='\0')&&(ui8_aux1<(MAX_ENTER_TEXT_LEN-1))){
			ui8_aux1++;
		}//while
				
		if (ui8_aux1>0){
					
			if (screens_enter_txt.i8_cursor_blink_flag==1){
						
				// the blinking cursor character SCREEN_ENTER_TXT_CURSOR_CHAR has to be shown	
				
				// temporally store the currently edited character and replace it by the SCREEN_EDITION_CURSOR_CHAR
				ui8_aux2 = screens_enter_txt.ui8_text[ui8_aux1-1];
				screens_enter_txt.ui8_text[ui8_aux1-1] = SCREEN_CURSOR_CHAR;

				// program the timer for the next cursor blink check
				screens_enter_txt.timer_blink = TIMING_SetTicksDelay(SCREEN_ENTER_TXT_BLINK_TIMEOUT);
						
				// update the current enter text screen with the SCREEN_EDITION_CURSOR_CHAR character
				SCREENS_enter_txt_show();
						
				// restore the character
				screens_enter_txt.ui8_text[ui8_aux1-1] = ui8_aux2;
						
				// invert the cursor blinking flag for the next cycle						
				screens_enter_txt.i8_cursor_blink_flag = 0;
						
			}else{
						
				// the current entered character has to be shown, NOT the cursor
					
				// program the timer for the next cursor blink check
				screens_enter_txt.timer_blink = TIMING_SetTicksDelay(SCREEN_ENTER_TXT_BLINK_TIMEOUT);
						
				// update the current enter text screen with the current entered character
				SCREENS_enter_txt_show();
						
				// invert the cursor blinking flag for the next cycle
				screens_enter_txt.i8_cursor_blink_flag = 1;
					
			}//if (screens_enter_txt.
				
		}//if (ui8_aux1>0)
				
	}//if
				
	// UP push button has been pressed or the ENCODER wheel has been turned 
	if ( (pui8_pushbutton_values[USER_IFACE_PUSHBT_2]==USER_IFACE_PRESSED) || (pi16_encoders_var_value[USER_IFACE_ENC_1]>0) ){
			
		// get the index of the last entered character
		ui8_aux1 = 0;
		while ((screens_enter_txt.ui8_text[ui8_aux1]!='\0')&&(ui8_aux1<(MAX_ENTER_TEXT_LEN-1))){
			ui8_aux1++;
		}//while
		
		if (ui8_aux1>0){
			
			// set the next character
			if ((screens_enter_txt.ui8_text[ui8_aux1]=='\0')&&(ui8_aux1<MAX_ENTER_TEXT_LEN)){			
			
				ui8_aux1--;
				// set the next character in UP direction: 'a' ... 'z' -> 'A' ... 'Z' -> '0' ... '9' -> '_' -> CANCEL -> ACCEPT ->
				if (screens_enter_txt.ui8_text[ui8_aux1]=='z'){
					screens_enter_txt.ui8_text[ui8_aux1]='A';
				}else if (screens_enter_txt.ui8_text[ui8_aux1]=='Z'){
					screens_enter_txt.ui8_text[ui8_aux1]='0';
				}else if (screens_enter_txt.ui8_text[ui8_aux1]=='9'){
					screens_enter_txt.ui8_text[ui8_aux1]=SCREEN_ACCEPT_CHAR; 
				}else if (screens_enter_txt.ui8_text[ui8_aux1]==SCREEN_ACCEPT_CHAR){
					screens_enter_txt.ui8_text[ui8_aux1]=SCREEN_CANCEL_CHAR;
				}else if (screens_enter_txt.ui8_text[ui8_aux1]==SCREEN_CANCEL_CHAR){
					screens_enter_txt.ui8_text[ui8_aux1]='_';
				}else if (screens_enter_txt.ui8_text[ui8_aux1]=='_'){											
					screens_enter_txt.ui8_text[ui8_aux1]='a';
				}else{
					screens_enter_txt.ui8_text[ui8_aux1]++;
				}//if
				screens_enter_txt.ui8_text[ui8_aux1+1]='\0';
				
			}//if ((screens_enter_txt.ui8_text[ui8_aux1]=='\0')
			
		}//if
		
		// reset the encoder var value
		pi16_encoders_var_value[USER_IFACE_ENC_1]=0;		
		
		// update the current enter text screen
		SCREENS_enter_txt_show();
		
	}//if	

	// DOWN push button has been pressed or the ENCODER wheel has been turned
	if ( (pui8_pushbutton_values[USER_IFACE_PUSHBT_3]==USER_IFACE_PRESSED ) || (pi16_encoders_var_value[USER_IFACE_ENC_1]<0) ){
			
		// get the index of the last entered character
		ui8_aux1 = 0;
		while ((screens_enter_txt.ui8_text[ui8_aux1]!='\0')&&(ui8_aux1<(MAX_ENTER_TEXT_LEN-1))){
			ui8_aux1++;
		}//while
		
		if (ui8_aux1>0){
			
			// set the next character
			if ((screens_enter_txt.ui8_text[ui8_aux1]=='\0')&&(ui8_aux1<MAX_ENTER_TEXT_LEN)){			
			
				ui8_aux1--;
				// set the next character in DOWN direction : 'a' ... 'z' <- 'A' ... 'Z' <- '0' ... '9' <- '_' <- CANCEL <- ACCEPT <-
				if (screens_enter_txt.ui8_text[ui8_aux1]=='a'){
					screens_enter_txt.ui8_text[ui8_aux1]='_';
				}else if (screens_enter_txt.ui8_text[ui8_aux1]=='_'){
					screens_enter_txt.ui8_text[ui8_aux1]=SCREEN_CANCEL_CHAR;
				}else if (screens_enter_txt.ui8_text[ui8_aux1]==SCREEN_CANCEL_CHAR){					
					screens_enter_txt.ui8_text[ui8_aux1]=SCREEN_ACCEPT_CHAR;
				}else if (screens_enter_txt.ui8_text[ui8_aux1]==SCREEN_ACCEPT_CHAR){					
					screens_enter_txt.ui8_text[ui8_aux1]='9';
				}else if (screens_enter_txt.ui8_text[ui8_aux1]=='0'){
					screens_enter_txt.ui8_text[ui8_aux1]='Z';
				}else if (screens_enter_txt.ui8_text[ui8_aux1]=='A'){
					screens_enter_txt.ui8_text[ui8_aux1]='z';
				}else{
					screens_enter_txt.ui8_text[ui8_aux1]--;
				}//if
				screens_enter_txt.ui8_text[ui8_aux1+1]='\0';			
							
			}//if ((screens_enter_txt.ui8_text[ui8_aux1]=='\0')
			
		}//if
		
		// reset the encoder var value
		pi16_encoders_var_value[USER_IFACE_ENC_1]=0;		
		
		// update the current enter text screen
		SCREENS_enter_txt_show();
			
	}//if

	// call to the specific function that processes the KEYS specific for that enter text screen
	if (screens_enter_txt.ev_process_cur_function!=NULL){
		
		screens_enter_txt.ev_process_cur_function(pi16_encoders_var_value, pui8_pushbutton_values);
		
	}//if

}//SCREENS_enter_txt_ev_manager



// SAVE_RAM text enter screen
int8_t SCREENS_enter_txt_load_SAVE_RAM(){
	int8_t i8_ret_val = 1;
	
	return i8_ret_val;
	
}//SCREENS_enter_txt_load_SAVE_RAM



int8_t SCREENS_enter_txt_show_SAVE_RAM(uint8_t * ui8_message){
	int8_t i8_ret_val = 1;
	uint8_t ui8_aux_string64_1[AUX_FUNCS_F_P_MAX_STR_SIZE_64];


	switch (screens_enter_txt.i8_state){
		
		case SCREEN_ENTER_TXT_INITIAL_STATE:
			// clear the screen buffer content by filling it with the SCREENS_ENTER_TXT_BACKGROUND_CHAR char
			GRAPHIX_text_buffer_fill(SCREENS_ENTER_TXT_BACKGROUND_CHAR,ATTR_NO_ATTRIBS,GRAPHIX_TEXT_COL_IDX_DARK_GREY,GRAPHIX_TEXT_COL_IDX_BLACK,GRAPHIX_TEXT_COL_IDX_BLACK);
			// set the message in the screen buffer
			GRAPHIX_text_buffer_set_string(0,4,(uint8_t*)"   Enter file name:   ",ATTR_SPACE_BACKSYMBOL,GRAPHIX_TEXT_COL_NEUTRAL,GRAPHIX_TEXT_COL_IDX_BLACK,GRAPHIX_TEXT_COL_IDX_BLACK);
			// centered entered text before placing it in the screen buffer
			AUX_FUNCS_lstrfill(ui8_aux_string64_1,' ',GRAPHIX_TEXT_BUFFER_MAX_COLUMNS-1); // important GRAPHIX_TEXT_BUFFER_MAX_COLUMNS-1 << AUX_FUNCS_F_P_MAX_STR_SIZE
			AUX_FUNCS_center_into_string(ui8_aux_string64_1,screens_enter_txt.ui8_text,0,GRAPHIX_TEXT_BUFFER_MAX_COLUMNS-1); // important GRAPHIX_TEXT_BUFFER_MAX_COLUMNS-1 << AUX_FUNCS_F_P_MAX_STR_SIZE
			// place entered text in screen buffer
			GRAPHIX_text_buffer_set_string(0, 5,ui8_aux_string64_1,  ATTR_SPACE_BACKSYMBOL,GRAPHIX_TEXT_COL_NEUTRAL,GRAPHIX_TEXT_COL_IDX_BLACK,GRAPHIX_TEXT_COL_IDX_BLACK);
			// refresh the content of the buffer to screen
			GRAPHIX_text_buffer_refresh();
			break;
		
		default:
			i8_ret_val = -1;
			break;
		
	}//switch

	return i8_ret_val;
	
}//SCREENS_enter_txt_show_SAVE_RAM



int8_t SCREENS_enter_txt_ev_manager_SAVE_RAM(int16_t * pi16_encoders_var_value, uint8_t * pui8_pushbutton_values){
	int8_t i8_ret_val = 1;
	uint8_t ui8_aux1=0;


	// RIGHT push button ( or ENTER ) has been pressed
	if (pui8_pushbutton_values[USER_IFACE_PUSHBT_4]==USER_IFACE_PRESSED){

		// get the index of the last entered character
		ui8_aux1 = 0;
		while ((screens_enter_txt.ui8_text[ui8_aux1]!='\0')&&(ui8_aux1<(MAX_ENTER_TEXT_LEN-1))){
			ui8_aux1++;
		}//while
		
		if (ui8_aux1>0){
				
			// check if the user has pressed the right key when he is over the abort  symbol		
			if (screens_enter_txt.ui8_text[ui8_aux1-1]==SCREEN_CANCEL_CHAR){
				
				// load the corresponding dialog screen
				SCREENS_dialog_load(SCREEN_DIALOG_ID_SAVE_RAM_AS_FILE,SCREEN_DIALOG_SAVE_RAM_AS_FILE_CANCELLED);				
			
			// check if the user has pressed the right key when he is over the accept symbol, also check
			// that he has entered at less 1 valid character before the accept symbol
			}else if ( (screens_enter_txt.ui8_text[ui8_aux1-1]==SCREEN_ACCEPT_CHAR) && (ui8_aux1>1) ){

                // before saving the file remove the ACCEPT char from the string 
				screens_enter_txt.ui8_text[ui8_aux1-1]='\0';
				ui8_aux1--;
				// add the file extension to the entered name before calling the save file dialog screen
				AUX_FUNCS_strcat(screens_enter_txt.ui8_text,SCREENS_F_EXPLORER_FILE_EXT_ROM_FILE,MAX_ENTER_TEXT_LEN);			
				// load the corresponding dialog screen
				SCREENS_dialog_load(SCREEN_DIALOG_ID_SAVE_RAM_AS_FILE,SCREEN_DIALOG_SAVE_RAM_AS_FILE_SAVING_FILE);

			}else{

				// the user has pressed the right key but is not over any of special symbols so another
				// character is added to the patch name if possible.
				if ( (ui8_aux1<(MAX_ENTER_TEXT_LEN-1)) && (screens_enter_txt.ui8_text[ui8_aux1]=='\0') && (ui8_aux1<screens_enter_txt.ui8_max_allowed_len) ){
					screens_enter_txt.ui8_text[ui8_aux1]='a';
					screens_enter_txt.ui8_text[ui8_aux1+1]='\0';
				}//if

				// update the current enter text screen
				SCREENS_enter_txt_show();

			}//if		
		
		}//if (ui8_aux1>0)

	}//if	
	
	// LEFT push button ( or BACK ) has been pressed
	if (pui8_pushbutton_values[USER_IFACE_PUSHBT_1]==USER_IFACE_PRESSED){
							
		// get the index of the last entered character
		ui8_aux1 = 0;
		while ((screens_enter_txt.ui8_text[ui8_aux1]!='\0')&&(ui8_aux1<(MAX_ENTER_TEXT_LEN-1))){
			ui8_aux1++;
		}//while

		// check if the user has pressed the back or return when he is over the first character 										
		if (ui8_aux1==1){
			
			// load the corresponding dialog screen
			SCREENS_dialog_load(SCREEN_DIALOG_ID_SAVE_RAM_AS_FILE,SCREEN_DIALOG_SAVE_RAM_AS_FILE_CANCELLED);				
							
		}else{
					
			// it the user IS NOT over the first character, the edition cursor is moved back one position
			if ((screens_enter_txt.ui8_text[ui8_aux1]=='\0')&&(ui8_aux1>1)){
				ui8_aux1--;
				screens_enter_txt.ui8_text[ui8_aux1]='\0';
			}//if
					
			// update the current enter text screen
			SCREENS_enter_txt_show();
					
		}//if		
			
	}//if	
		
	// ENC push button has been pressed 
	if (pui8_pushbutton_values[USER_IFACE_PUSHBT_ENC]==USER_IFACE_PRESSED){
	
		// get the index of the last entered character
		ui8_aux1 = 0;
		while ((screens_enter_txt.ui8_text[ui8_aux1]!='\0')&&(ui8_aux1<(MAX_ENTER_TEXT_LEN-1))){
			ui8_aux1++;
		}//while
		
		if (ui8_aux1>0){
				
			// check if the user has pressed the encoder pushbutton when he is over a special symbol ( the  
			// abort or the cancel symbol) and in that case remove that special symbol from the name
			if ( (screens_enter_txt.ui8_text[ui8_aux1-1]==SCREEN_CANCEL_CHAR) || (screens_enter_txt.ui8_text[ui8_aux1-1]==SCREEN_ACCEPT_CHAR) ) {
			    screens_enter_txt.ui8_text[ui8_aux1-1] = '\0';
				ui8_aux1--;
			}//if
			
			if (ui8_aux1>0){
				
				// add the file extension to the entered name before calling the save file dialog screen
				AUX_FUNCS_strcat(screens_enter_txt.ui8_text,SCREENS_F_EXPLORER_FILE_EXT_ROM_FILE,MAX_ENTER_TEXT_LEN);			
				// load the corresponding dialog screen
				SCREENS_dialog_load(SCREEN_DIALOG_ID_SAVE_RAM_AS_FILE,SCREEN_DIALOG_SAVE_RAM_AS_FILE_SAVING_FILE);
				
			}else{
				// load the corresponding dialog screen
				SCREENS_dialog_load(SCREEN_DIALOG_ID_SAVE_RAM_AS_FILE,SCREEN_DIALOG_SAVE_RAM_AS_FILE_CANCELLED);
			}//if
			
		}//if (ui8_aux1>0)
		
	}//if		
	
	return i8_ret_val;
	
}//SCREENS_enter_txt_ev_manager_SAVE_RAM
