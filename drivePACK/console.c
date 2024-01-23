// *****************************************************************
// ****                       drivePACK                         ****
// ****                  www.tolaemon.com/dpack                 ****
// ****                       Source code                       ****
// ****                       21/08/2020                        ****
// ****                     Jordi Bartolome                     ****
// ****                                                         ****
// **** IMPORTANT:                                              ****
// **** Using this code or any part of it means accepting all   ****
// **** conditions exposed in: http://www.tolaemon.com/dpack    ****
// **** This code is part of the bareSAM platform. All info at: ****
// **** http://www.tolaemon.com/baresam                         ****
// *****************************************************************

#include "console.h"

#include <stdio.h>

// buffer used to store the characters of the currently entered command
struct {
	uint8_t ui8_buffer[MAX_LINE_CONSOLE_BUFFER];// the buffer with the received characters
	int16_t i16_last_index;// points to the first free position in the received characters buffer
	int16_t i16_cursor_index;// points to the current cursor position of the received characters buffer
} rx_chars;

// buffer used to store the characters of the currently received escape sequence
struct {
	uint8_t ui8_receiving;// ==0 means that the system is waiting for normal characters, >0 is the number of received scape sequence characters
	uint8_t ui8_buffer[MAX_LINE_CONSOLE_BUFFER];// stores the received scape sequence characters in case the system is receiving scape sequence characters
	TIMING_TICKS tt_tout;// used to avoid stay waiting for scape sequence characters indefinitely
} esc_seq;

// struct with the list of last entered commands, it is used to store last entered commands and work faster
struct {
	uint8_t queue[MAX_COMMANDS_QUEUE][MAX_LINE_CONSOLE_BUFFER];// queue with the last MAX_COMMANDS_QUEUE commands
	int16_t i16_last_index;// with the last occupied position in the queue
	int16_t i16_index;// with the currently selected command in the queue
	uint8_t ui8_enabled;// to enable or disable console module
} commands;


void CONSOLE_Init(){
	int16_t i=0;
	int16_t j=0;

	rx_chars.i16_cursor_index=0;
	rx_chars.i16_last_index=0;
	for (i=0;i<MAX_LINE_CONSOLE_BUFFER;i++){
		rx_chars.ui8_buffer[i]='\0';
	}
		
	esc_seq.ui8_receiving=0;
	for (i=0;i<MAX_LINE_CONSOLE_BUFFER;i++){
		esc_seq.ui8_buffer[i]='\0';
	}
		
	for (i=0;i<MAX_COMMANDS_QUEUE;i++){
		for (j=0;j<MAX_LINE_CONSOLE_BUFFER;j++){
			commands.queue[i][j]='\0';
		}//for
	}///for
	commands.i16_last_index=-1;// queue is empty
	commands.i16_index=0;
    commands.ui8_enabled=1;
	
    // show console welcome message
	COMMANDS_IMPL_execute_info(NULL);

	CONSOLE_reset();

}//CONSOLE_init


void CONSOLE_enable(){
	
    commands.ui8_enabled=1;	
	
}//CONSOLE_enable


void CONSOLE_disable(){
	
	commands.ui8_enabled=0;
	
}//CONSOLE_enable


void CONSOLE_reset(){


	rx_chars.i16_cursor_index=0;
	rx_chars.i16_last_index=0;
	
	rx_chars.ui8_buffer[0]='\0';
	esc_seq.ui8_receiving = 0;
	esc_seq.ui8_buffer[0]='\0';
	esc_seq.tt_tout = 0;

	commands.i16_index=-1;

	USART_send_string(CONSOLE_PROMPT_STRING,20);

}//CONSOLE_reset


int16_t CONSOLE_decode_key_from_char(uint8_t ui8_usart_char, uint8_t * pui8_received_key_type, uint8_t * pui8_received_key){
	int16_t i16_aux;
	uint8_t ui8_received_key;
	uint8_t ui8_received_key_type;
	int16_t i16_ret_vale = 0;


	ui8_received_key_type=CONSOLE_KEY_TYPE_UNKNOWN;
    ui8_received_key=0;

	if (esc_seq.ui8_receiving==0){
				
		// receiving normal characters ( system is not waiting for scape sequence characters  )
				
		if ((ui8_usart_char>=32)&&(ui8_usart_char<=126)){
		    // from ' ' to '~'
			ui8_received_key_type=CONSOLE_KEY_TYPE_TEXT;
			ui8_received_key  = ui8_usart_char;// key code corresponds to a normal ASCII key 
			i16_ret_vale = 1;
			
		}else if ((ui8_usart_char==CONSOLE_KEY_CODE_RETURN)){// received RETURN ASCII key char code
					
			ui8_received_key_type=CONSOLE_KEY_TYPE_CTRL;
			ui8_received_key=CONSOLE_KEY_CTRL_RETURN;
			i16_ret_vale = 1;
					
		}else if ((ui8_usart_char==CONSOLE_KEY_CODE_BACK)){// received BACKSPACE key char code
					
			ui8_received_key_type=CONSOLE_KEY_TYPE_CTRL;
			ui8_received_key = CONSOLE_KEY_CTRL_BACK;
			i16_ret_vale = 1;

		}else if ((ui8_usart_char==CONSOLE_KEY_CODE_DELETE)){// received DEL-SUPR key char code
		
			ui8_received_key_type=CONSOLE_KEY_TYPE_CTRL;
			ui8_received_key = CONSOLE_KEY_CODE_DELETE;
			i16_ret_vale = 1;
					
		}else if (ui8_usart_char==CONSOLE_KEY_CODE_ESC_SEQUENCE_START){// 0x1B corresponds to the first character of a escape sequence
					
			ui8_received_key_type=CONSOLE_KEY_TYPE_ESCAPE_SEQUENCE;
			i16_ret_vale = 1;
			
			// initialize the escape sequence reception buffer and variables
			for (i16_aux=0;i16_aux<MAX_ESCAPE_SEQUENCE_CHARS;i16_aux++){
				esc_seq.ui8_buffer[i16_aux]='\0';
			}//for
			// set ui8_esc_seq_receiving to >0 to indicate the number of received scape characters
			// ( == 0 means that system is not receiving a scape sequence )
			esc_seq.ui8_buffer[0]=ui8_usart_char;
			esc_seq.ui8_receiving = 1;
			// initialize and start the scape sequence timeout. If that timeout elapses without receiving
			// a complete scape sequence then clear the flag
			esc_seq.tt_tout = TIMING_SetTicksDelay(CONSOLE_SCAPE_SEQUENCE_TOUT);
					
		}//if ((ui8_usart_char...
				
	}else{
				
		// receiving escape sequence character, so store it into the escape sequence buffer until the complete escape
		// sequence has been received and it can be processed or an escape sequence reception timeout occurs
		if (esc_seq.ui8_receiving<MAX_ESCAPE_SEQUENCE_CHARS-1){
			esc_seq.ui8_buffer[esc_seq.ui8_receiving]=ui8_usart_char;
			esc_seq.ui8_receiving++;
		}//if
				
		// try to decode the received scape sequence if complete
		if ( (esc_seq.ui8_buffer[1]==0x5B) && (esc_seq.ui8_buffer[2]==0x44) ){ // left row escape sequence
					
			ui8_received_key_type=CONSOLE_KEY_TYPE_ESCAPE_SEQUENCE;
			ui8_received_key=CONSOLE_KEY_ESC_SEQ_LEFT;
			i16_ret_vale = 1;
								
		}else if ( (esc_seq.ui8_buffer[1]==0x5B) && (esc_seq.ui8_buffer[2]==0x43) ){ // right row escape sequence
					
			ui8_received_key_type=CONSOLE_KEY_TYPE_ESCAPE_SEQUENCE;
			ui8_received_key=CONSOLE_KEY_ESC_SEQ_RIGHT;
			i16_ret_vale = 1;
					
		}else if ( (esc_seq.ui8_buffer[1]==0x5B) && (esc_seq.ui8_buffer[2]==0x41) ){ // up row escape sequence
					
			ui8_received_key_type=CONSOLE_KEY_TYPE_ESCAPE_SEQUENCE;
			ui8_received_key=CONSOLE_KEY_ESC_SEQ_UP;
			i16_ret_vale = 1;
					
		}else if ( (esc_seq.ui8_buffer[1]==0x5B) && (esc_seq.ui8_buffer[2]==0x42) ){ // down row escape sequence
					
			ui8_received_key_type=CONSOLE_KEY_TYPE_ESCAPE_SEQUENCE;
			ui8_received_key=CONSOLE_KEY_ESC_SEQ_DOWN;
			i16_ret_vale = 1;

		}else if ( (esc_seq.ui8_buffer[1]==0x5B) && (esc_seq.ui8_buffer[2]==0x31) && (esc_seq.ui8_buffer[3]==0x31) && (esc_seq.ui8_buffer[4]==0x7e) ){ // F1 key escape code

			ui8_received_key_type=CONSOLE_KEY_TYPE_ESCAPE_SEQUENCE;
			ui8_received_key=CONSOLE_KEY_ESC_SEQ_F1;
			i16_ret_vale = 1;


		}else if ( (esc_seq.ui8_buffer[1]==0x5B) && (esc_seq.ui8_buffer[2]==0x31) && (esc_seq.ui8_buffer[3]==0x7e) ){ // F1 key escape code

			ui8_received_key_type=CONSOLE_KEY_TYPE_ESCAPE_SEQUENCE;
			ui8_received_key=CONSOLE_KEY_ESC_SEQ_HOME;
			i16_ret_vale = 1;
								
		}else{
					
			// haven't received any known escape sequence yet
			i16_ret_vale = 1;
								
		}//if ( (ui8_esc_seq
				
		// if a valid escape sequence has been identified then reset the escape sequence buffer
		if (ui8_received_key_type!=CONSOLE_KEY_TYPE_UNKNOWN){
			esc_seq.ui8_receiving = 0;
			esc_seq.ui8_buffer[0]='\0';
			esc_seq.tt_tout = 0;
		}//if
								
	}//if (esc_seq.ui8_receiving_scape_sequence==0)

	(*pui8_received_key) = ui8_received_key;
	(*pui8_received_key_type) = ui8_received_key_type;

	return i16_ret_vale;

}//CONSOLE_decode_key_from_char


void CONSOLE_rewrite_from_cursor_to_end(){
	int16_t i16_aux;


	// Erase End of Line <ESC>[K: Erases from the current cursor position to the end of the current line.
	USART_send_char(0x1B);//<ESC>
	USART_send_char('[');//[
	USART_send_char('K');//K

	// print again all the buffer chars that have been cleared
	for (i16_aux=rx_chars.i16_cursor_index;i16_aux<=rx_chars.i16_last_index;i16_aux++){
		USART_send_char(rx_chars.ui8_buffer[i16_aux]);
	}//for
	
	// after printing all shifted chars, then place the cursor again on its position
	for (i16_aux=rx_chars.i16_last_index;i16_aux>rx_chars.i16_cursor_index;i16_aux--){
		// send Cursor Backward command: Moves the cursor backward by COUNT columns; the default count is 1.
		USART_send_char(0x1B);//<ESC>
		USART_send_char('[');//[
		USART_send_char('1');//{count}
		USART_send_char('D');//D
	}//for

}//CONSOLE_rewrite_from_cursor_to_end


int16_t CONSOLE_enqueue_command_string(uint8_t * command_to_store){
	int16_t i16_ret_val=-1;
	int16_t i;


	// if the content is>0, store the received command into the comands queue
	if (AUX_FUNCS_lstrlen(command_to_store,MAX_LINE_CONSOLE_BUFFER)>0){

		// move the index if is still place in the buffer
		if (commands.i16_last_index<(MAX_COMMANDS_QUEUE-1)){
			commands.i16_last_index++;
		}//if

		// move one position all the stored commands 
		for (i=commands.i16_last_index;i>0;i--){
			AUX_FUNCS_lstrcpy((uint8_t*)commands.queue[i],(uint8_t*)commands.queue[i-1],MAX_LINE_CONSOLE_BUFFER);
		}//for
		// store the received new command
		AUX_FUNCS_lstrcpy((uint8_t*)commands.queue[0],command_to_store,MAX_LINE_CONSOLE_BUFFER);
	
		i16_ret_val = 1;
	
	}//if
	
	return i16_ret_val;

}//CONSOLE_enqueue_command_string


int16_t CONSOLE_get_next_queue_command_string(uint8_t* next_command){
	int16_t return_value=-1;


	// check if there are commands in the queue
	if ((commands.i16_index>=-1)&&(commands.i16_index<commands.i16_last_index)){

		// move the index to the next command
		commands.i16_index++;

		// copy the next command in the queue into the string
		AUX_FUNCS_lstrcpy((uint8_t*)next_command,(uint8_t*)commands.queue[commands.i16_index],MAX_LINE_CONSOLE_BUFFER);
		
		// limit next position
		if (commands.i16_index>commands.i16_last_index){
			commands.i16_index=commands.i16_last_index;
		}//if

		return_value=1;

	}else{

		next_command[0]='\0';
		return_value=-1;

	}//if

	return return_value;
	
}//CONSOLE_get_next_queue_command_string


int16_t CONSOLE_get_prev_queue_command_string(uint8_t* prev_command){
	int16_t return_value=-1;


	// check if there are commands in the queue
	if ((commands.i16_index>0)&&(commands.i16_index<=commands.i16_last_index)){

		// move the index to the prev command
		commands.i16_index--;

		// copy the next command in the queue into the string
		AUX_FUNCS_lstrcpy((uint8_t*)prev_command,(uint8_t*)commands.queue[commands.i16_index],MAX_LINE_CONSOLE_BUFFER);
		
		// limit next position
		if (commands.i16_index==0){
			commands.i16_index=1;
		}//if

		return_value=1;

	}else{

		prev_command[0]='\0';
		return_value=-1;

	}//if

	return return_value;
	
}//CONSOLE_get_prev_queue_command_string


int16_t CONSOLE_process_received_keys(uint8_t ui8_received_key_type, uint8_t ui8_received_key, uint8_t * command_string){
	int16_t i16_ret_vale = 0;
	int16_t i16_aux;


	if (ui8_received_key_type==CONSOLE_KEY_TYPE_TEXT){

		// if the character is a normal character and there is space into the buffer, then we store in it
		if ((rx_chars.i16_cursor_index>=0)&&(rx_chars.i16_cursor_index<MAX_LINE_CONSOLE_BUFFER-2)){
				
			// if the edition cursor is not in the last position of the entered string ( user is editing 
			// a char that it is not the last one ), then move all the chars one position to get space
			// for the new received character 
			for (i16_aux=rx_chars.i16_last_index;i16_aux>rx_chars.i16_cursor_index;i16_aux--){
				rx_chars.ui8_buffer[i16_aux]=rx_chars.ui8_buffer[i16_aux-1];
			}//for
			rx_chars.ui8_buffer[rx_chars.i16_cursor_index]=ui8_received_key;
	        USART_send_char(ui8_received_key);
					
			// update the buffer control vars.
			rx_chars.i16_cursor_index++;
			rx_chars.i16_last_index++;
			rx_chars.ui8_buffer[rx_chars.i16_last_index]='\0';// thought it is is not necessary, we place a '\0' for security
			
			// rewrite the command line from current cursor postion to end
			CONSOLE_rewrite_from_cursor_to_end();
			
		}//if ((char_buffer_index>=0)&&(char_buffer_index<MAX_LINE_CONSOLE_BUFFER-2))
				
	}else if (ui8_received_key_type==CONSOLE_KEY_TYPE_CTRL){
								
		if ( ui8_received_key == CONSOLE_KEY_CTRL_RETURN ){
			
			// the KEY_CTRL_RETURN has been received and there are chars in the buffer so a command has been received
								
			// if pressed key is command termination key ( ex. RETURN ) buffer is parsed. When the user
			// hits the COMMAND_END_KEY tokens in the buffer are parsed and inserted into the command structure
			rx_chars.i16_cursor_index++;
			rx_chars.i16_last_index++;
			rx_chars.ui8_buffer[rx_chars.i16_last_index]='\0';
			AUX_FUNCS_lstrcpy((uint8_t*)command_string,(uint8_t*)rx_chars.ui8_buffer,MAX_COMMAND_LEN);
			AUX_FUNCS_trim_begining_end( command_string);
							
			//command chars buffer is "emptied"
			rx_chars.i16_cursor_index=0;
			rx_chars.i16_last_index=0;
			for (i16_aux=0;i16_aux<MAX_LINE_CONSOLE_BUFFER;i16_aux++){
				rx_chars.ui8_buffer[i16_aux]='\0';
			}			
			
			i16_ret_vale=1;
	
		}else if ( (ui8_received_key==CONSOLE_KEY_CTRL_BACK) || (ui8_received_key==CONSOLE_KEY_CTRL_DEL) ){
					
			// the BACK or DEL key has been received so delete last entered character
			
			if (rx_chars.i16_cursor_index>0){
			
				// decrease cursor index in 1
				rx_chars.i16_cursor_index--;			
				rx_chars.i16_last_index--;								

				for (i16_aux=rx_chars.i16_cursor_index;i16_aux<=rx_chars.i16_last_index;i16_aux++){
					rx_chars.ui8_buffer[i16_aux] = rx_chars.ui8_buffer[i16_aux+1];
				}//for
				
				// send Cursor Backward command: Moves the cursor backward by COUNT columns; the default count is 1.
				USART_send_char(0x1B);//<ESC>
				USART_send_char('[');//[
				USART_send_char('1');//{count}
				USART_send_char('D');//D
				// erase previous character by writing a white space
				USART_send_char(' ');
				// send Cursor Backward command: Moves the cursor backward by COUNT columns; the default count is 1.
				USART_send_char(0x1B);//<ESC>
				USART_send_char('[');//[
				USART_send_char('1');//{count}
				USART_send_char('D');//D
			
			    // rewrite the command line from current cursor postion to end 
				CONSOLE_rewrite_from_cursor_to_end();
			
			}//if (rx_chars.i16_cursor_index>0)
		}
				
	}else if (ui8_received_key_type==CONSOLE_KEY_TYPE_ESCAPE_SEQUENCE){			
								
		if ( ui8_received_key==CONSOLE_KEY_ESC_SEQ_UP){
			USART_send_string((uint8_t*)"ARR!",20);  
		}else if ( ui8_received_key==CONSOLE_KEY_ESC_SEQ_DOWN){
			USART_send_string((uint8_t*)"ABA!",20);
		}else if ( ui8_received_key==CONSOLE_KEY_ESC_SEQ_RIGHT){
			
			if (rx_chars.i16_cursor_index<rx_chars.i16_last_index){
				// increase cursor index in 1
				rx_chars.i16_cursor_index++;

				// send Cursor Forward command: Moves the cursor forward by COUNT columns; the default count is 1.
				USART_send_char(0x1B);//<ESC>
				USART_send_char('[');//[
				USART_send_char('1');//{count}
				USART_send_char('C');//C
			}
			
		}else if ( ui8_received_key==CONSOLE_KEY_ESC_SEQ_LEFT){

			if (rx_chars.i16_cursor_index>0){
				// decrease cursor index in 1
				rx_chars.i16_cursor_index--;

				// send Cursor Backward command: Moves the cursor backward by COUNT columns; the default count is 1.
				USART_send_char(0x1B);//<ESC>
				USART_send_char('[');//[
				USART_send_char('1');//{count}
				USART_send_char('D');//D
			}
				
		}else if ( ui8_received_key==CONSOLE_KEY_ESC_SEQ_F1){
			USART_send_string((uint8_t*)"F1!",20);
		}else if ( ui8_received_key==CONSOLE_KEY_ESC_SEQ_HOME){
			USART_send_string((uint8_t*)"HOME!",20);
		}//if
		
	}else if (ui8_received_key_type==CONSOLE_KEY_CTRL_BACK){	

							
	}//if
			
/*			
		if (key_extended==EXTENDED_HOME_KEY){

			// moves cursor to the beginning when the key is pressed
			printf("\r%s",PROMPT_CHAR);
			keyboard_buffer_index=0;

		}else if (key_extended==EXTENDED_END_KEY){

			// moves cursor to the end when the key is pressed
			keyboard_buffer_index=strlen((uint8_t*)pressed_keys_buffer);
			printf("\r%s",PROMPT_CHAR);
			printf("%s  %c%c",pressed_keys_buffer,BACK_KEY,BACK_KEY);

		}else if (key_extended==BACK_KEY){
				
			// check index status and command length before deleting
			if (keyboard_buffer_index>0){

				keyboard_buffer_index--;

				printf ("%c %c",BACK_KEY,BACK_KEY);
				// currently selected char is removed from the keyboard buffer if back key is pressed 
				for (i=keyboard_buffer_index;i<keyboard_last_index;i++){
					pressed_keys_buffer[i]=pressed_keys_buffer[i+1];
					printf ("%c",pressed_keys_buffer[i]);
				}//for
				pressed_keys_buffer[i]='\0';

				keyboard_last_index--;
				
				// move the cursos to the new position
				while ((i>keyboard_buffer_index)&&(i>0)) {
					printf ("%c",BACK_KEY);
					i--;
				}//while
				
			}//if (keyboard_buffer_index>0)

		}else if (key_extended==EXTENDED_SUPR_KEY){
				
			// check index status and command length before erasing
			if ((keyboard_last_index>0)&&(keyboard_buffer_index!=keyboard_last_index)){

				// currently selected char is removed from the keyboard buffer if back key is pressed 
				for (i=keyboard_buffer_index;i<=keyboard_last_index;i++){
					pressed_keys_buffer[i]=pressed_keys_buffer[i+1];
					printf ("%c",pressed_keys_buffer[i]);
				}//for
				pressed_keys_buffer[i]='\0';

				keyboard_last_index--;
				
				// move the cursos to the new position
				while ((i>keyboard_buffer_index)&&(i>0)) {
					printf ("%c",BACK_KEY);
					i--;
				}//while

			}//if ((keyboard_last_index>0)&&

		}else if (key_extended==COMMAND_END_KEY){
				
			// if the hit key is command termination key ( ex. RETURN ) buffer is parsed. When the user 
			// hits the COMMAND_END_KEY tokens in the buffer are parsed and inserted into the command structure
			keyboard_buffer_index++;
			keyboard_last_index++;
			pressed_keys_buffer[keyboard_last_index]='\0';
			strcpy ((uint8_t*)command_string,(uint8_t*)pressed_keys_buffer);
								
			//command chars buffer is "emptied"
			keyboard_buffer_index=0;
			keyboard_last_index=0;
			return_value=1;

		}else if (key_extended==EXTENDED_UP_KEY){

			// check if there are commands in the next commands queue position
			if (CONSOLE_get_next_queue_command(keyb_buffer_aux)>0){										

				// copy the stored next command into the current command array
				strcpy((uint8_t*)pressed_keys_buffer,(uint8_t*)keyb_buffer_aux);					
				keyboard_buffer_index=strlen((uint8_t*)pressed_keys_buffer);
				keyboard_last_index=strlen((uint8_t*)pressed_keys_buffer);
				// clear prompt and print the current new command
				printf("\r%s",PROMPT_uint8_t);
				printf("%s  %c%c",pressed_keys_buffer,BACK_KEY,BACK_KEY);

			}//if

		}else if (key_extended==EXTENDED_DOWN_KEY){

			// check if there are commands in the previous commands queue position
			if (CONSOLE_get_prev_queue_command(keyb_buffer_aux)>0){	

				// copy the stored next command into the current command array
				strcpy((uint8_t*)pressed_keys_buffer,(uint8_t*)keyb_buffer_aux);					
				keyboard_buffer_index=strlen((uint8_t*)pressed_keys_buffer);
				keyboard_last_index=strlen((uint8_t*)pressed_keys_buffer);
				// clear prompt and print the current new command
				printf("\r%s",PROMPT_CHAR);
				printf("%s  %c%c",pressed_keys_buffer,BACK_KEY,BACK_KEY);

			}//if

		else if (key_extended==EXTENDED_LEFT_KEY){
				
			//} move the cursor to the left if possible
			if (keyboard_buffer_index>0){
				keyboard_buffer_index--;
				printf ("%c",8);
			}//if

		}else if (key_extended==EXTENDED_RIGHT_KEY){

			// move the cursor to the right if possible
			if (keyboard_buffer_index<keyboard_last_index){
				printf ("%c",pressed_keys_buffer[keyboard_buffer_index]);
				keyboard_buffer_index++;
			}//if

		}else if ((key_extended>30)&&(key_extended<200)){

			// if the key is a normal key and there is space into the buffer we store in it 
			if ((keyboard_buffer_index>=0)&&(keyboard_buffer_index<MAX_LINE_KEYBOARD_BUFFER-2)){

				// move all the chars one position to get place for the new received key
				for (i=keyboard_last_index;i>keyboard_buffer_index;i--){
					pressed_keys_buffer[i]=pressed_keys_buffer[i-1];
				}//for
				pressed_keys_buffer[keyboard_buffer_index]=key;

				// print all the chars of the buffer
				for (i=keyboard_buffer_index;i<=keyboard_last_index;i++){
					printf ("%c",pressed_keys_buffer[i]);
				}//for
					
				// update the buffer control vars.
				keyboard_buffer_index++;
				keyboard_last_index++;

				// move thee cursor to the orginal position
				i=keyboard_last_index;
				while ((i>0)&&(i!=keyboard_buffer_index)){
					printf ("%c",BACK_KEY);
					i--;
				}//while

				pressed_keys_buffer[keyboard_last_index]='\0';// thought it is is not necessary, we place a '\0' for security

			}else{

				printf ("ERROR: buffer is full");
				
			};//if
								
		}//if		
*/

	return i16_ret_vale;

}//CONSOLE_process_received_keys


int16_t CONSOLE_check_input(uint8_t * command_string){
		uint8_t ui8_usart_char=0;
		uint8_t ui8_received_char=0;
		uint8_t ui8_received_char_type=CONSOLE_KEY_TYPE_UNKNOWN;
	 	int16_t i16_ret_vale = -1 ;		


        if (commands.ui8_enabled==TRUE){
			
			// check if a new char/key code has been received through the USART, and process it if affirmative
			if (USART_receive_char(&ui8_usart_char)==TRUE){
			
				// check and process received char code and decide if it corresponds to a "normal" key char or to a "escape sequence" key char.
				CONSOLE_decode_key_from_char(ui8_usart_char, &ui8_received_char_type, &ui8_received_char);
		
				// process the received character key type and key code and check if a complete command string has been received
				i16_ret_vale = CONSOLE_process_received_keys(ui8_received_char_type, ui8_received_char,command_string);
				if (i16_ret_vale==1){

					// a command has been received so store it into the commands queue to make it available for future uses
					CONSOLE_enqueue_command_string(command_string);
				
					i16_ret_vale = 1;
				}
				
			}//if (USART_receive_char(&ui8_received)==TRUE){

			// to avoid staying indefinitely waiting for an escape sequence character, when the maximum allowed time to
			// receive an escape sequence characters has elapsed, then clear the flag used to identify the "receiving 
			// escape sequence" state and go back to receiving "normal" characters
			if ( (esc_seq.ui8_receiving>0) && (TIMING_CheckTicksDelayReached(esc_seq.tt_tout)==TRUE) ){			
				esc_seq.ui8_receiving = 0;	
			}//if
			
		}//if (commands.ui8_enabled==TRUE)
		
		return i16_ret_vale;

}//CONSOLE_check_input