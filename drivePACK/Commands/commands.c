// *****************************************************************
// ****                       drivePACK                         ****
// ****                  www.tolaemon.com/dpack                 ****
// ****                       Source code                       ****
// ****                       10/07/2020                        ****
// ****                     Jordi Bartolome                     ****
// ****                                                         ****
// **** IMPORTANT:                                              ****
// **** Using this code or any part of it means accepting all   ****
// **** conditions exposed in: http://www.tolaemon.com/dpack    ****
// *****************************************************************

#include "commands.h"



int16_t COMMANDS_clear_command(t_command * command){
	uint16_t ui16_aux1;
	uint16_t ui16_aux2;


    if ( command != NULL){
		
		// clear and initialize the content of all the tokens
		for (ui16_aux1=0;ui16_aux1<MAX_TOKENS_X_COMMAND;ui16_aux1++){			
			for (ui16_aux2=0;ui16_aux2<(MAX_TOKEN_STR_SIZE-1);ui16_aux2++){				
					command->token[ui16_aux1][ui16_aux2]='\0';
			}
			command->token_type[ui16_aux1] = TOKEN_IS_EMPTY;
		}
		
		// clear other command fields
		command->total = 0;
		command->ready_to_execute = 0;
	
		return 1;
	
	}else{
	
		return 0;
	
	}//if ( command != NULL)

}//COMMANDS_insert_token_in_command


int16_t COMMANDS_insert_token_in_command(t_command * command,uint8_t * token_string){
	uint16_t i;


	// if there is space for another token in the command structure we insert the token
	if (command->total<(MAX_TOKENS_X_COMMAND-1)){
		
		// remove all the spaces at the beginning and the end of the token
		AUX_FUNCS_trim_begining_end(token_string);
		
		// update the field that tells if the token is an ALIAS, a VALUE or a CONSTANT
		if (token_string[0]==STRING_TOKEN_LIMIT_SYMBOL){
			command->token_type[command->total]=TOKEN_IS_ALIAS;			
			AUX_FUNCS_trim_string_char(token_string,STRING_TOKEN_LIMIT_SYMBOL);// remove the " from the beggining and the end			
		}else if (AUX_FUNCS_check_if_valid_number(token_string, MAX_COMMAND_LEN )==1){
			command->token_type[command->total]=TOKEN_IS_VALUE;
		}else{
			command->token_type[command->total]=TOKEN_IS_CONST;		
		}//if		
		
		// token string is inserted into the tokens structure
		i=0;
		while (token_string[i]!='\0'){
			command->token[command->total][i]=token_string[i];
			i++;
		};//while
		command->token[command->total][i]='\0';

		command->total++;
		return 1;
		
	}else{
		
		// cannot be inserted another token into the command
		return 0;
		
	}//if

}//COMMANDS_insert_token_in_command


void COMMANDS_print_tokens(t_command command){
	uint16_t ui16_com_list=0;
	uint8_t aux_string[MAX_TOKEN_STR_SIZE];

	for (ui16_com_list=0;ui16_com_list<command.total;ui16_com_list++){
					
		USART_send_string("\r\nToken:",32);
		AUX_FUNCS_lstrcpy(aux_string,command.token[ui16_com_list],MAX_TOKEN_STR_LEN);
		USART_send_string(aux_string,32);
					
		switch (command.token_type[ui16_com_list]){
			case TOKEN_IS_ALIAS:
				USART_send_string(":ALIAS",32);
				break;
			case TOKEN_IS_VALUE:
				USART_send_string(":VALUE",32);
				break;
			case TOKEN_IS_CONST:
				USART_send_string(":CONST",32);
				break;
			default:
				USART_send_string(":UNKNWON",32);
				break;
		}//switch
					
	}//for

}//COMMANDS_insert_token_in_command


int16_t COMMANDS_parse_command_string(uint8_t * command_string,  uint16_t ush_MAX_LEN, t_command * command){
	int16_t parsing_mode; // used to distinguish if we are parsing a normal token, a string token ...
	int16_t i,j,k,l;
	int16_t ret_val = 1;
	uint8_t aux_string[128];
	int16_t ui16_end_index = 0;


	// received command structure can not be modified because it contains a command
	// that is waiting to be executed or that it is going to be executed
	if ( command->ready_to_execute == 1){
		ret_val = -3;
	}
	
	if (ret_val>=0){
		
		// useless symbols are also removed from the buffer
		ui16_end_index = AUX_FUNCS_lstrlen(command_string,MAX_COMMAND_LEN);
		i=0;
		while (i<ui16_end_index){
			// remove characters that are not alphanumeric chars, accept only from ' '(ASCII:32) to 'Z'(ASCII:122) 
			if ((command_string[i]<32)||(command_string[i]>122)){
				for (j=i;j<ui16_end_index;j++){
					command_string[j]=command_string[j+1];
				};//for
				ui16_end_index--;
			}//if
			i++;

		}//while

		// if the string is empty we return -1
		if (AUX_FUNCS_lstrlen(command_string, ush_MAX_LEN)==0){
			 ret_val=-1;
		}
    
	}//if (ret_val>=0)
			
	if (ret_val>=0){		

		// if there is no error and string is NOT empty then we process it

		// loop that takes each token in the keyboard buffer and inserts it into the command structure
		i=0;
		j=0;
		k=0;
		l=0;
		parsing_mode=0;// 0 normal mode , 1 string mode
		command->total=0;// the command structure is initialized
		ret_val=1;
		while   ((ret_val!=-1)&&(i<=ui16_end_index)) {

			// depending on the parsing mode we take the characters in one way or another
			switch (parsing_mode){

				case 0:// NORMAL PARSING MODE

					// buffer is being parsed, detecting tokens and inserting them into the token structure
					if ( (command_string[i]==TOKEN_END_SYMBOL)||(command_string[i]==COMMAND_END_SYMBOL) ) {

						// the TOKEN_END_SYMBOL will only be taken into consideration
						// if some chars have been stored into the current token
						if (k!=0){
							
							aux_string[k]='\0';
							// the token is inserted into the command structure
							if (COMMANDS_insert_token_in_command(command,aux_string)){
								// the token has been inserted into the command structure
								l++;// number of tokens is increased
							}else{
								// the token could not be inserted into the command structure
								ret_val=-2;
								// printf ("\r\nERROR: tokens cannot be inserted into the command structure");
							}//if
							k=0;

						}//if

					// if string parse symbol is detected we start string parsing mode
					}else if (command_string[i]==STRING_TOKEN_LIMIT_SYMBOL){

						aux_string[k]=STRING_TOKEN_LIMIT_SYMBOL;
						k++;
						parsing_mode=1;

					}else{

						//if current symbol is different than TOKEN_END_SYMBOL the symbol is stored into the command buffer
						aux_string[k]=AUX_FUNCS_character_to_low_case(command_string[i]);
						k++;

					}//if
					i++;
					break;
					
				case 1:// STRING PARSING MODE

					// if we detect the string parse symbol we start into string parsing mode
					if (command_string[i]==STRING_TOKEN_LIMIT_SYMBOL){
						aux_string[k]=STRING_TOKEN_LIMIT_SYMBOL;
						k++;
						parsing_mode=0;
					}else{
						aux_string[k]=AUX_FUNCS_character_to_low_case(command_string[i]);
						k++;
					}//if
					i++;
					break;

				default:
					break;

			}//switch
				
			// we check if token lenght is greater than it should, if it is greater we stop parsing and give a message error
			if (k>MAX_TOKEN_STR_LEN){
				ret_val=-2;
				// printf ("\r\nERROR: too long token");
			}//if

			// if number of tokens is greater than it shoud, a error message is returned
			if (l>MAX_TOKENS_X_COMMAND){
				ret_val=-2;
				// printf ("\r\nERROR: too much tokens");
			}//if

		}//while

	}//	if (ret_val>=0)

	// parsed string command is set to ready for execution
	if (ret_val>= 0){

		command->ready_to_execute=1;

	}
	
	return ret_val;

}//COMMANDS_parse_command_string


int16_t COMMANDS_execute(t_command * p_command){
	int16_t ret_val = 0;
	int16_t i16_command_ret_val = 1;
	int16_t int_aux1,int_aux2,int_aux3;


	// if the command is ready to be executed, we execute it
	if (p_command->ready_to_execute==1){

       // case: <CONSTANT>*
	   if (p_command->token_type[0] == TOKEN_IS_CONST){

			// COMMAND_HELP ===============================================================
			// case: <CONSTANT==COMMAND_HELP>*
			if (AUX_FUNCS_lstrcmp(p_command->token[0],COMMAND_HELP,MAX_TOKEN_STR_LEN)==0){
			
				i16_command_ret_val = COMMANDS_IMPL_execute_help(p_command);
				ret_val = 1;
				
			// COMMAND_MOUNT ========================================================================
			// case: <CONSTANT==COMMAND_MOUNT>*
			}else if (AUX_FUNCS_lstrcmp(p_command->token[0],COMMAND_MOUNT,MAX_TOKEN_STR_LEN)==0){
										
				i16_command_ret_val = COMMANDS_FS_IMPL_execute_mnt(p_command);
				ret_val = 1;

			// COMMAND_DIR ========================================================================
			// case: <CONSTANT==COMMAND_DIR>*
			}else if (AUX_FUNCS_lstrcmp(p_command->token[0],COMMAND_DIR,MAX_TOKEN_STR_LEN)==0){

				i16_command_ret_val = COMMANDS_FS_IMPL_execute_dir(p_command);
				ret_val = 1;
				
			// COMMAND_CD ========================================================================
			// case: <CONSTANT==COMMAND_CD>*
			}else if (AUX_FUNCS_lstrcmp(p_command->token[0],COMMAND_CHANGE_DIR,MAX_TOKEN_STR_LEN)==0){

				i16_command_ret_val = COMMANDS_FS_IMPL_execute_change_dir(p_command);
				ret_val = 1;
				
			// COMMAND_MKDIR ========================================================================
			// case: <CONSTANT==COMMAND_MKDIR>*
			}else if (AUX_FUNCS_lstrcmp(p_command->token[0],COMMAND_MAKEDIR,MAX_TOKEN_STR_LEN)==0){

				i16_command_ret_val = COMMANDS_FS_IMPL_execute_make_dir(p_command);
				ret_val = 1;
				
			// COMMAND_RM ========================================================================
			// case: <CONSTANT==COMMAND_RM>*
			}else if (AUX_FUNCS_lstrcmp(p_command->token[0],COMMAND_RM,MAX_TOKEN_STR_LEN)==0){

				i16_command_ret_val = COMMANDS_FS_IMPL_execute_remove(p_command);
				ret_val = 1;

			// COMMAND_GPIO ========================================================================
			// case: <CONSTANT==COMMAND_GPIO>*
			}else if (AUX_FUNCS_lstrcmp(p_command->token[0],COMMAND_GPIO,MAX_TOKEN_STR_LEN)==0){

				i16_command_ret_val = COMMANDS_IMPL_execute_gpio(p_command);
				ret_val = 1;
				
			// COMMAND_TYPE ========================================================================
			// case: <CONSTANT==COMMAND_TYPE>*
			}else if (AUX_FUNCS_lstrcmp(p_command->token[0],COMMAND_TYPE,MAX_TOKEN_STR_LEN)==0){
								
				i16_command_ret_val = COMMANDS_FS_IMPL_execute_command_type(p_command);				
				ret_val = 1;

			// COMMAND_WRITE ========================================================================
			// case: <CONSTANT==COMMAND_WRITE>*
			}else if (AUX_FUNCS_lstrcmp(p_command->token[0],COMMAND_WRITE,MAX_TOKEN_STR_LEN)==0){
			
				i16_command_ret_val = COMMANDS_FS_IMPL_execute_command_write(p_command);
				ret_val = 1;
				
			// COMMAND_ROM ========================================================================
			// case: <CONSTANT==COMMAND_ROM>*
			}else if (AUX_FUNCS_lstrcmp(p_command->token[0],COMMAND_ROM,MAX_TOKEN_STR_LEN)==0){

				i16_command_ret_val = COMMANDS_IMPL_execute_rom(p_command);
				ret_val = 1;

			// COMMAND_INFO ========================================================================
			// case: <CONSTANT==COMMAND_INFO>
			}else if (AUX_FUNCS_lstrcmp(p_command->token[0],COMMAND_INFO,MAX_TOKEN_STR_LEN)==0){

				i16_command_ret_val = COMMANDS_IMPL_execute_info(p_command);
				ret_val = 1;				
										
			}else{
				
				// -2 the command has not been identified
				ret_val=-2;
			
			}//end if
			
		}else{
			
			if (AUX_FUNCS_lstrcmp(p_command->token[0],(uint8_t*)"",MAX_TOKEN_STR_LEN)!=0){
				
				// it has not been possible to identify command
				USART_send_string("\r\nInvalid command format.",32);
				ret_val=-2;
				
			}else{
				
				// it is an empty command/string
				ret_val=-1;
				
			}//if

		}//if (command->token_type[0] == TOKEN_IS_CONST)
		
		// if the command was wrong we show the error message
		if (ret_val==-1){
			
			// received an empty string
			p_command->ready_to_execute = 0;
			
		}else if (ret_val<-1){
			
			// received an invalid command or with errors
			p_command->ready_to_execute = 0;
			USART_send_string("\r\nCommand error.",32);
			
		}//if
		
	}//if	
	
	return ret_val;
	
}//COMMANDS_execute