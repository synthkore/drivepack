// *****************************************************************
// ****                       drivePACK                         ****
// ****                  www.tolaemon.com/dpack                 ****
// ****                       Source code                       ****
// ****                       8/05/2022                        ****
// ****                     Jordi Bartolome                     ****
// ****                                                         ****
// **** IMPORTANT:                                              ****
// **** Using this code or any part of it means accepting all   ****
// **** conditions exposed in: http://www.tolaemon.com/dpack    ****
// **** This code is part of the bareSAM platform. All info at: ****
// **** http://www.tolaemon.com/baresam                         ****
// *****************************************************************

#include "commands_impl.h"


// declare extern assembly public functions
extern void dpack_int_main(void);
extern void dpack_dumper_main(void);
extern void config_ctrl_lines_as_input(void);
extern void config_data_lines_as_input(void);

// reference to external structures
extern uint8_t ui8_dpack_dumper_nibbles_buffer[MAX_ROM_NIBBLES_BUFFER];
extern uint8_t ui8_dpack_dumper_buffer_initialized;
extern uint8_t ui8_dpack_title_buffer[MAX_ROM_TITLE_BUFFER];
extern uint8_t ui8_dpack_songs_info_buffer[MAX_ROM_SONGS_INFO_BUFFER];
extern uint8_t ui8_dpack_file_name[MAX_ROM_FILE_NAME];
extern uint8_t ui8_dpack_file_path[MAX_ROM_FILE_PATH];


int16_t COMMANDS_IMPL_execute_help(t_command * p_command){
	int16_t ret_val=0;
	

	if (p_command->token_type[1] == TOKEN_IS_ALIAS){
	
        if (AUX_FUNCS_lstrcmp(p_command->token[1],COMMAND_MOUNT,MAX_TOKEN_STR_LEN)==0){

			_USART_SEND_STRING("\r\nMounts disk file system.");
			_USART_SEND_STRING("\r\n");
			_USART_SEND_STRING("\r\nUsage: mnt");
			_USART_SEND_STRING("\r\n");
			_USART_SEND_STRING("\r\nExample: mnt");
			_USART_SEND_STRING("\r\n");
									
		}else if (AUX_FUNCS_lstrcmp(p_command->token[1],COMMAND_DIR,MAX_TOKEN_STR_LEN)==0){

			_USART_SEND_STRING("\r\nShows current directory content: files and");
			_USART_SEND_STRING("\r\nsubdirectories. Mount unit before using this command.");
			_USART_SEND_STRING("\r\n");
			_USART_SEND_STRING("\r\nUsage: dir");
			_USART_SEND_STRING("\r\n");
			_USART_SEND_STRING("\r\nExample: dir");
			_USART_SEND_STRING("\r\n");
		
		}else if (AUX_FUNCS_lstrcmp(p_command->token[1],COMMAND_TYPE,MAX_TOKEN_STR_LEN)==0){

			_USART_SEND_STRING("\r\nShows the content of specified file. Mount unit");
			_USART_SEND_STRING("\r\nbefore using this command.");
			_USART_SEND_STRING("\r\nAvailable parameters are:");
			_USART_SEND_STRING("\r\nTXT\t to show the content of the file in text format.");
			_USART_SEND_STRING("\r\nHEX\t to show the content of the file as hexa bytes.");
			_USART_SEND_STRING("\r\n");
			_USART_SEND_STRING("\r\nUsage: type txt \"file\"");
			_USART_SEND_STRING("\r\nUsage: type hex \"file\"");
			_USART_SEND_STRING("\r\n");		
			_USART_SEND_STRING("\r\nExample: type txt \"lyrics.txt\"");
			_USART_SEND_STRING("\r\nExample: type hex \"mifile.bin\"");
			_USART_SEND_STRING("\r\n");	

		}else if (AUX_FUNCS_lstrcmp(p_command->token[1],COMMAND_WRITE,MAX_TOKEN_STR_LEN)==0){

		    _USART_SEND_STRING("\r\nCreates a file with the specified name and with the");
			_USART_SEND_STRING("\r\nspecified short text content. Mount unit before using");
			_USART_SEND_STRING("\r\nthis command.");			
			_USART_SEND_STRING("\r\n");
			_USART_SEND_STRING("\r\nUsage: write \"file\" \"text_to_write\"");
			_USART_SEND_STRING("\r\n");
			_USART_SEND_STRING("\r\nExample: write \"sdtest.txt\" \"Hello world\"");
			_USART_SEND_STRING("\r\nExample: write \"mi_file\" \"Line one\" \"Line two\"");
			_USART_SEND_STRING("\r\n");

		}else if (AUX_FUNCS_lstrcmp(p_command->token[1],COMMAND_RM,MAX_TOKEN_STR_LEN)==0){

			_USART_SEND_STRING("\r\nRemoves from current directory the file or subdirectory ");
			_USART_SEND_STRING("\r\nwith the specified name. Mount unit before using this");
			_USART_SEND_STRING("\r\ncommand.");
			_USART_SEND_STRING("\r\n");
			_USART_SEND_STRING("\r\nUsage: rm \"file_name\" ");
			_USART_SEND_STRING("\r\nUsage: rm \"directory_name\" ");
			_USART_SEND_STRING("\r\n");
			_USART_SEND_STRING("\r\nExample: rm \"mi_file.txt\"");
			_USART_SEND_STRING("\r\nExample: rm \"mi_dir\"");
			_USART_SEND_STRING("\r\n");
								
		}else if (AUX_FUNCS_lstrcmp(p_command->token[1],COMMAND_CHANGE_DIR,MAX_TOKEN_STR_LEN)==0){

			_USART_SEND_STRING("\r\nChanges current directory to specified directory. Mount");
			_USART_SEND_STRING("\r\nunit before using this command.");
			_USART_SEND_STRING("\r\n");
			_USART_SEND_STRING("\r\nUsage: cd \"directory\"");
			_USART_SEND_STRING("\r\n");
			_USART_SEND_STRING("\r\nExample: cd \"mi_folder\"");
			_USART_SEND_STRING("\r\nExample: cd \"..\"");
			_USART_SEND_STRING("\r\n");

		}else if (AUX_FUNCS_lstrcmp(p_command->token[1],COMMAND_MAKEDIR,MAX_TOKEN_STR_LEN)==0){

			_USART_SEND_STRING("\r\nCreates a directory with the specified name. Mount");
			_USART_SEND_STRING("\r\nunit before using this command.");
			_USART_SEND_STRING("\r\n");
			_USART_SEND_STRING("\r\nUsage: mkdir \"directory\"");
			_USART_SEND_STRING("\r\n");
			_USART_SEND_STRING("\r\nExample: mkdir \"mi_new_dir\"");
			_USART_SEND_STRING("\r\n");


		}else if (AUX_FUNCS_lstrcmp(p_command->token[1],COMMAND_GPIO,MAX_TOKEN_STR_LEN)==0){

			_USART_SEND_STRING("\r\nTo check the state of the specified input and output GPIO");
			_USART_SEND_STRING("\r\nlines. The value of the lines must be in decimal. Available");
			_USART_SEND_STRING("\r\nparameters are:");
            _USART_SEND_STRING("\r\nIN\t to read the value from the specified I/O lines.");
            _USART_SEND_STRING("\r\nOUT\t to write a value to the specified I/O lines.");
			_USART_SEND_STRING("\r\nDATA\t to test data I/O lines (DATA3-DATA0): ");
			_USART_SEND_STRING("\r\n\t 8:D3, 4:D2, 2:D1, 1:D0");
			_USART_SEND_STRING("\r\nCTRL\t to test control I/O lines:");
			_USART_SEND_STRING("\r\n\t 128:IO3, 64:IO2, 32:IO1, 16:?");
			_USART_SEND_STRING("\r\n\t 8:CLCK2, 4:CLCK1, 2:RnW, 1:nCS");
			_USART_SEND_STRING("\r\n");
			_USART_SEND_STRING("\r\nUsage: gpio in data");
			_USART_SEND_STRING("\r\nUsage: gpio out data value");
			_USART_SEND_STRING("\r\n");
			_USART_SEND_STRING("\r\nExample: gpio in data");
			_USART_SEND_STRING("\r\nExample: gpio in ctrl");
			_USART_SEND_STRING("\r\nExample: gpio out data 5");
			_USART_SEND_STRING("\r\nExample: gpio out ctrl 16");
			_USART_SEND_STRING("\r\n");
			
		}else if (AUX_FUNCS_lstrcmp(p_command->token[1],COMMAND_ROM,MAX_TOKEN_STR_LEN)==0){

			_USART_SEND_STRING("\r\nAllows to execute different operations over a ROM PACK.");		
			_USART_SEND_STRING("\r\nAvailable parameters are:");
			_USART_SEND_STRING("\r\nDUMP\t to read and store into the memory buffer the");
			_USART_SEND_STRING("\r\ncontent of the connected ROM PACK.");
			_USART_SEND_STRING("\r\nWRITE\t to write to the specified disk file the content");
			_USART_SEND_STRING("\r\nof the memory buffer.");
			_USART_SEND_STRING("\r\nLOAD\t to load into the memory buffer the content of the");
			_USART_SEND_STRING("\r\nspecified disk file.");			
			_USART_SEND_STRING("\r\nTYPE\t shows the content of the ROM PACK memory buffer");
			_USART_SEND_STRING("\r\nINFO\t shows the title and the song information of the");
			_USART_SEND_STRING("\r\nsongs in the memory buffer.");				
 			_USART_SEND_STRING("\r\nRUN\t to set the system in ROM PACK emulation mode.");
			_USART_SEND_STRING("\r\nCHECK\t to verify the ROM PACK header in memory buffer.");
			_USART_SEND_STRING("\r\nRX1K\t to receive and to load to memory buffer a ROM ");
			_USART_SEND_STRING("\r\nfile received through the 1KXmodem connection.");
			_USART_SEND_STRING("\r\nTX1K\t to send the ROM in memory throught the 1KXmodem ");
			_USART_SEND_STRING("\r\nconnection.");			
			_USART_SEND_STRING("\r\n");			
			_USART_SEND_STRING("\r\n");
			_USART_SEND_STRING("\r\nUsage: rom dump");
			_USART_SEND_STRING("\r\nUsage: rom write \"file\"");
			_USART_SEND_STRING("\r\nUsage: rom load \"file\"");
			_USART_SEND_STRING("\r\nUsage: rom type");
			_USART_SEND_STRING("\r\nUsage: rom info");
			_USART_SEND_STRING("\r\nUsage: rom run");
			_USART_SEND_STRING("\r\nUsage: rom check");
			_USART_SEND_STRING("\r\nUsage: rom rx1k");
			_USART_SEND_STRING("\r\nUsage: rom tx1k");
			_USART_SEND_STRING("\r\n");
			_USART_SEND_STRING("\r\nExample: rom dump");
			_USART_SEND_STRING("\r\nExample: rom write \"ro_274.rdp\"");
			_USART_SEND_STRING("\r\nExample: rom load \"ro_274.rdp\"");
			_USART_SEND_STRING("\r\nExample: rom type");
			_USART_SEND_STRING("\r\nExample: rom info");
			_USART_SEND_STRING("\r\nExample: rom run");
			_USART_SEND_STRING("\r\nExample: rom check");
			_USART_SEND_STRING("\r\nExample: rom rx1k");
			_USART_SEND_STRING("\r\nExample: rom tx1k");
			_USART_SEND_STRING("\r\n");
		
		}else if (AUX_FUNCS_lstrcmp(p_command->token[1],COMMAND_INFO,MAX_TOKEN_STR_LEN)==0){

			_USART_SEND_STRING("\r\nShows system general information.");
			_USART_SEND_STRING("\r\n");
			_USART_SEND_STRING("\r\nUsage: info");
			_USART_SEND_STRING("\r\n");
		
		}else{
			
			_USART_SEND_STRING("\r\nCan't provide help for that command.");
		
		}//if
	
	}else{

		_USART_SEND_STRING("\r\nTo get specific help of a command, write help");
		_USART_SEND_STRING("\r\nfollowed by the name of the command between");
		_USART_SEND_STRING("\r\nquotations.");
		_USART_SEND_STRING("\r\n");
		_USART_SEND_STRING("\r\nUsage: help");
		_USART_SEND_STRING("\r\nUsage: help \"command\"");
		_USART_SEND_STRING("\r\n");
		_USART_SEND_STRING("\r\nExample: help \"dir\"");
		_USART_SEND_STRING("\r\n");		
		_USART_SEND_STRING("\r\nAvailable commands are:");
		_USART_SEND_STRING("\r\nMNT \t Mounts the disk");
		_USART_SEND_STRING("\r\nDIR \t Shows current directory content.");
		_USART_SEND_STRING("\r\nCD \t Change to specified directory.");
		_USART_SEND_STRING("\r\nMKDIR \t Creates a directory with the specified name.");
		_USART_SEND_STRING("\r\nRM \t Removes the specified file or directory.");
		_USART_SEND_STRING("\r\nTYPE \t Shows specified file content.");
		_USART_SEND_STRING("\r\nWRITE \t Creates a file with the specified text content.");
		_USART_SEND_STRING("\r\nROM \t Dumps, loads, reads and writes a ROMPACK.");
		_USART_SEND_STRING("\r\nINFO \t Shows system general information.");
		_USART_SEND_STRING("\r\nGPIO \t Checks specified system I/O lines.");
		_USART_SEND_STRING("\r\n");
	
	}//if
	
	return ret_val;
	
}//COMMANDS_IMPL_execute_help


int16_t COMMANDS_IMPL_execute_gpio(t_command * p_command){
	uint8_t ui8_aux_string[AUX_FUNCS_F_P_MAX_STR_SIZE_64];
	int32_t i32_aux = 0;
	uint8_t ui8_aux = 0;
	int16_t ret_val=0;


	_USART_SEND_STRING("\r\n");

    // check if received command parameters are valid 
	if (p_command->token_type[1]!=TOKEN_IS_CONST){
        
		// first command parameter is empty or not valid		
		_USART_SEND_STRING("\r\nInvalid or missing parameter 1: ");
		USART_send_string(p_command->token[1],MAX_TOKEN_STR_SIZE);
		ret_val=-1;
		
	}else if (p_command->token_type[2]!=TOKEN_IS_CONST){
		
		// second command parameter is empty or not valid
		_USART_SEND_STRING("\r\nInvalid or missing parameter 2: ");
		USART_send_string(p_command->token[2],MAX_TOKEN_STR_SIZE);
		ret_val=-1;		

	}else{
		
		// check which is the first received subcommand:
		// -PARAM_GPIO_IN
		// -PARAM_GPIO_OUT
		if (AUX_FUNCS_lstrcmp(p_command->token[1],PARAM_GPIO_IN,MAX_TOKEN_STR_LEN)==0){
			
			// a read (IN) command has been received then check which is the second received subcommand:
			// -PARAM_GPIO_DATA
			// -PARAM_GPIO_CTRL			
			if (AUX_FUNCS_lstrcmp(p_command->token[2],PARAM_GPIO_DATA,MAX_TOKEN_STR_LEN)==0){
				
				// execute read the state of gpio DATA lines
				ui8_aux=GPIO_GetDataLinesInput();
				// show the state of read bits
				_USART_SEND_STRING("\r\nGPIO DATA lines read: ");
				AUX_FUNCS_itoa(ui8_aux,ui8_aux_string,10,AUX_FUNCS_F_P_MAX_STR_SIZE_64);
				USART_send_string(ui8_aux_string,AUX_FUNCS_F_P_MAX_STR_SIZE_64);
				_USART_SEND_STRING("\r\nD3:");if ((ui8_aux&0x08)!=0) USART_send_string("1",2); else USART_send_string("0",2);
				_USART_SEND_STRING(" D2:");   if ((ui8_aux&0x04)!=0) USART_send_string("1",2); else USART_send_string("0",2);
				_USART_SEND_STRING(" D1:");   if ((ui8_aux&0x02)!=0) USART_send_string("1",2); else USART_send_string("0",2);
				_USART_SEND_STRING(" D0:");   if ((ui8_aux&0x01)!=0) USART_send_string("1",2); else USART_send_string("0",2);
						
			}else if (AUX_FUNCS_lstrcmp(p_command->token[2],PARAM_GPIO_CTRL,MAX_TOKEN_STR_LEN)==0){
				
				// execute read the state of gpio CTRL lines
				ui8_aux=GPIO_GetCtrlLinesInput();
				// show the state of the read bits
				_USART_SEND_STRING("\r\nGPIO CTRL lines read:");
				AUX_FUNCS_itoa(ui8_aux,ui8_aux_string,10,AUX_FUNCS_F_P_MAX_STR_SIZE_64);
				USART_send_string(ui8_aux_string,AUX_FUNCS_F_P_MAX_STR_SIZE_64);
				_USART_SEND_STRING(" \r\nIO3:"); if ((ui8_aux&VAR_CTRL_GPIO03)!=0)        _USART_SEND_STRING("1"); else _USART_SEND_STRING("0");
				_USART_SEND_STRING(" IO2:");     if ((ui8_aux&VAR_CTRL_GPIO02)!=0)        _USART_SEND_STRING("1"); else _USART_SEND_STRING("0");
				_USART_SEND_STRING(" IO1:");     if ((ui8_aux&VAR_CTRL_GPIO01)!=0)        _USART_SEND_STRING("1"); else _USART_SEND_STRING("0");
				_USART_SEND_STRING(" ?:");       if ((ui8_aux&VAR_CTRL_nUNKNOWN_MASK)!=0) _USART_SEND_STRING("1"); else _USART_SEND_STRING("0");					
				_USART_SEND_STRING(" CLCK2:");   if ((ui8_aux&VAR_CTRL_nCLCK2_MASK)!=0)   _USART_SEND_STRING("1"); else _USART_SEND_STRING("0");
				_USART_SEND_STRING(" CLCK1:");   if ((ui8_aux&VAR_CTRL_nCLCK1_MASK)!=0)   _USART_SEND_STRING("1"); else _USART_SEND_STRING("0");
				_USART_SEND_STRING(" RnW:");     if ((ui8_aux&VAR_CTRL_RnW_MASK)!=0)      _USART_SEND_STRING("1"); else _USART_SEND_STRING("0");
				_USART_SEND_STRING(" nCS:");     if ((ui8_aux&VAR_CTRL_nCS_MASK)!=0)      _USART_SEND_STRING("1"); else _USART_SEND_STRING("0");
			
			}else{

				_USART_SEND_STRING("\r\nInvalid gpio name:");
				USART_send_string(p_command->token[2],MAX_TOKEN_STR_LEN);
				
			}//if
		
		}else if (AUX_FUNCS_lstrcmp(p_command->token[1],PARAM_GPIO_OUT,MAX_TOKEN_STR_LEN)==0){
				
			// a write (OUT) command has been received then check which is the second received subcommand:
			// -PARAM_GPIO_DATA
			// -PARAM_GPIO_CTRL								
			if (AUX_FUNCS_lstrcmp((uint8_t*)p_command->token[2],PARAM_GPIO_DATA,MAX_TOKEN_STR_LEN)==0){
						
				// execute gpio OUT on DATA	lines
				if (p_command->token_type[3]!=TOKEN_IS_VALUE){

					_USART_SEND_STRING("\r\nInvalid out value:");
					USART_send_string(p_command->token[3],MAX_TOKEN_STR_LEN);

				}else{
							
					if (AUX_FUNCS_atoi((uint8_t*)p_command->token[3], 10, MAX_TOKEN_STR_LEN, &i32_aux)>=0){

						ui8_aux = (uint8_t)i32_aux;
						GPIO_SetDataLinesOutput(ui8_aux);
						
						// show the state of the written bits
						_USART_SEND_STRING("\r\nGPIO DATA lines write:");
						AUX_FUNCS_itoa(ui8_aux,ui8_aux_string,10,AUX_FUNCS_F_P_MAX_STR_SIZE_64);
						USART_send_string(ui8_aux_string,AUX_FUNCS_F_P_MAX_STR_SIZE_64);
						_USART_SEND_STRING("\r\nD3:");if ((ui8_aux&0x08)!=0) USART_send_string("1",2); else USART_send_string("0",2);
						_USART_SEND_STRING(" D2:");   if ((ui8_aux&0x04)!=0) USART_send_string("1",2); else USART_send_string("0",2);
						_USART_SEND_STRING(" D1:");   if ((ui8_aux&0x02)!=0) USART_send_string("1",2); else USART_send_string("0",2);
						_USART_SEND_STRING(" D0:");   if ((ui8_aux&0x01)!=0) USART_send_string("1",2); else USART_send_string("0",2);
					
					}else{
						_USART_SEND_STRING("\r\nInvalid port value:");
						USART_send_string(p_command->token[3],MAX_TOKEN_STR_LEN);
						
					}
													
				}//if (p_command->token_type[3]!=TOKEN_IS_VALUE)
						
			}else if (AUX_FUNCS_lstrcmp((uint8_t*)p_command->token[2],PARAM_GPIO_CTRL,MAX_TOKEN_STR_LEN)==0){
						
				// execute gpio OUT on CTRL	lines 
				if (p_command->token_type[3]!=TOKEN_IS_VALUE){

					_USART_SEND_STRING("\r\nInvalid out value:");
					USART_send_string(p_command->token[3],MAX_TOKEN_STR_SIZE);

				}else{

					if (AUX_FUNCS_atoi((uint8_t*)p_command->token[3], 10, MAX_TOKEN_STR_LEN, &i32_aux)>=0){

						ui8_aux = (uint8_t)i32_aux;
						GPIO_SetCtrlLinesOutput(ui8_aux);
						
						// show the state of the written bits
						_USART_SEND_STRING("\r\nGPIO CTRL lines write:");
						AUX_FUNCS_itoa(ui8_aux,ui8_aux_string,10,AUX_FUNCS_F_P_MAX_STR_SIZE_64);
						_USART_SEND_STRING(ui8_aux_string);
						_USART_SEND_STRING(" \r\nIO3:");  if ((ui8_aux&VAR_CTRL_GPIO03)!=0)        USART_send_string("1",2); else USART_send_string("0",2);
						_USART_SEND_STRING(" IO2:");      if ((ui8_aux&VAR_CTRL_GPIO02)!=0)        USART_send_string("1",2); else USART_send_string("0",2);
						_USART_SEND_STRING(" IO1:");      if ((ui8_aux&VAR_CTRL_GPIO01)!=0)        USART_send_string("1",2); else USART_send_string("0",2);
						_USART_SEND_STRING(" ?:");        if ((ui8_aux&VAR_CTRL_nUNKNOWN_MASK)!=0) USART_send_string("1",2); else USART_send_string("0",2);
						_USART_SEND_STRING(" CLCK2:");    if ((ui8_aux&VAR_CTRL_nCLCK2_MASK)!=0)   USART_send_string("1",2); else USART_send_string("0",2);
						_USART_SEND_STRING(" CLCK1:");    if ((ui8_aux&VAR_CTRL_nCLCK1_MASK)!=0)   USART_send_string("1",2); else USART_send_string("0",2);
						_USART_SEND_STRING(" RnW:");      if ((ui8_aux&VAR_CTRL_RnW_MASK)!=0)      USART_send_string("1",2); else USART_send_string("0",2);
						_USART_SEND_STRING(" nCS:");      if ((ui8_aux&VAR_CTRL_nCS_MASK)!=0)      USART_send_string("1",2); else USART_send_string("0",2);
						
					}else{
						_USART_SEND_STRING("\r\nInvalid port value:");
						USART_send_string(p_command->token[3],MAX_TOKEN_STR_SIZE);
					}				
						
				}//if (p_command->token_type[3]!=TOKEN_IS_VALUE)
						
			}else{
					
				_USART_SEND_STRING("\r\nInvalid gpio name:");
				USART_send_string(p_command->token[2],MAX_TOKEN_STR_SIZE);			
					
			}//if
				
		}else{

			_USART_SEND_STRING("\r\nInvalid I/O direction: ");
			USART_send_string(p_command->token[1],MAX_TOKEN_STR_SIZE);
		
		}//if

	}//if
	
	return ret_val;	
	
}//COMMANDS_IMPL_execute_gpio


int16_t COMMANDS_IMPL_execute_info(t_command * p_command){
	uint8_t ui8_aux_string[AUX_FUNCS_F_P_MAX_STR_SIZE_64];
	int16_t i16_ret_val=0;


	_USART_SEND_STRING("\r\n");
	
	AUX_FUNCS_lstrcpy(ui8_aux_string,(uint8_t*)SW_NAME,AUX_FUNCS_F_P_MAX_STR_SIZE_64);
	USART_send_string(ui8_aux_string,AUX_FUNCS_F_P_MAX_STR_SIZE_64);
	
	_USART_SEND_STRING("-");
	
	AUX_FUNCS_lstrcpy(ui8_aux_string,(uint8_t*)SW_VERSION,AUX_FUNCS_F_P_MAX_STR_SIZE_64);
	USART_send_string(ui8_aux_string,AUX_FUNCS_F_P_MAX_STR_SIZE_64);
    
	_USART_SEND_STRING("\r\nVT-100 - ANSI terminal");

	return i16_ret_val;
	
}//COMMANDS_IMPL_execute_info


int16_t COMMANDS_IMPL_execute_command_rom_write(t_command * p_command){
    int16_t i16_ret_val=0;
	uint32_t ui32_bytes_to_write;
	uint32_t ui32_written_bytes;
	uint32_t ui32_total_written_bytes;
	uint8_t ui8_file_bytes[ROMPACK_FILE_RW_DEFAULT_N_BYTES];
	uint8_t * pui8_aux = 0;
	uint64_t ui64_rompack_cnt_start_offset_field = 0;
	uint64_t ui64_rompack_cnt_start_offset = 0;


    if (ui8_dpack_dumper_buffer_initialized==FALSE){
		
		_USART_SEND_STRING("\r\nThere is no valid data in memory buffer to write to disk.");
	    _USART_SEND_STRING("\r\nLoad or dump to memory buffer the content of a ROM first.");
				
	}else{

		if (p_command->token_type[2]!=TOKEN_IS_ALIAS){
	
			_USART_SEND_STRING("\r\nInvalid command parameter: ");
			USART_send_string(p_command->token[2],MAX_TOKEN_STR_SIZE);
			i16_ret_val=-1;	
	
		}//if

		if (i16_ret_val>=0){				
		
			// create specified destination file 
			_USART_SEND_STRING("\r\nCreating ROM PACK file... ");
			USART_send_string(p_command->token[2],MAX_TOKEN_STR_SIZE);
			_USART_SEND_STRING(".");

			i16_ret_val = DATA_IO_file_rom_write(p_command->token[2]);
			switch (i16_ret_val){
				
				case 0:
					_USART_SEND_STRING("\r\nROM PACK memory buffer content successfully");
					_USART_SEND_STRING("\r\nwritten to file.");
					break;
				
				case -1:
					_USART_SEND_STRING("\r\nUnit or file access error.");
					COMMANDS_FS_IMPL_print_file_error(i16_ret_val);
					break;
				
				case -2:
					_USART_SEND_STRING("\r\nError in ROM PACK file format or version.");
					break;
				
				case -3:
					_USART_SEND_STRING("\r\nError while writting to file.");
					COMMANDS_FS_IMPL_print_file_error(i16_ret_val);
					break;
				
				default:
					_USART_SEND_STRING("\r\nUnknown default error.");
					break;
				
			}//switch

		}//if (ret_val>=0)
	
	}//if (ui8_dpack_dumper_buffer_initialized==FALSE)
	
	return i16_ret_val;

}//COMMANDS_IMPL_execute_command_rom_write


int16_t COMMANDS_IMPL_execute_command_rom_dump(t_command * p_command){
	int16_t i16_ret_val=0;
	
	_USART_SEND_STRING("\r\nReading ROM PACK content...");

	// switch on the YELLOW LED to indicate that the system is busy emulating the cart
	GPIO_SetCtrlLinesOutput(VAR_CTRL_GPIO03);

	// initialize all the buffers and variables in RAM used to store the information of the ROM PACK
	DPACK_CTRL_Init();
			
	// to grant proper assembly execution and signals timings while
	// reading the ROM Pack content, dissable interrupts
	__disable_irq();

	// call assembly routine that reads ROM PACK content
	dpack_dumper_main();

	// once the content of the ROM Pack has been read, then restore interrupts
	__enable_irq();

	// switch off the YELLOW LED and others used to indicate that the system has stopped emulating the cart
	GPIO_SetCtrlLinesOutput(0x00);
			
	_USART_SEND_STRING("\r\nROM PACK content successfully read and dumped to ROM PACK");
    _USART_SEND_STRING("\r\nmemory buffer.");
		
	return i16_ret_val;	

}//COMMANDS_IMPL_execute_command_rom_dump


int16_t COMMANDS_IMPL_execute_command_rom_load(t_command * p_command){
	int16_t i16_ret_val=0;
	uint8_t * pui8_aux = 0;


	if (p_command->token_type[2]!=TOKEN_IS_ALIAS){
		
		_USART_SEND_STRING("\r\nInvalid command parameter: ");
		USART_send_string(p_command->token[2],MAX_TOKEN_STR_SIZE);
		i16_ret_val=-1;
		
	}//if

	if (i16_ret_val>=0){
			
		// open the the specified file in read mode
		_USART_SEND_STRING("\r\nOpening and loading ROM PACK file \"");
		USART_send_string(p_command->token[2],MAX_TOKEN_STR_SIZE);
		_USART_SEND_STRING("\"...");

		i16_ret_val = DATA_IO_file_rom_load(p_command->token[2]);
		switch (i16_ret_val){
			
			case 0:
				_USART_SEND_STRING("\r\nROM PACK file content successfully loaded to ROM PACK");
				_USART_SEND_STRING("\r\nmmemory buffer.");
				break;
				
			case -1:
				_USART_SEND_STRING("\r\nUnit or file access error.");
				COMMANDS_FS_IMPL_print_file_error(i16_ret_val);						
				break;
				
			case -2:
				_USART_SEND_STRING("\r\nError in ROM PACK file format or version.");			
				break;
				
			case -3:
			    _USART_SEND_STRING("\r\nError while reading from file.");
			    COMMANDS_FS_IMPL_print_file_error(i16_ret_val);			
				break;
							
			default:
				_USART_SEND_STRING("\r\nUnknown default error.");
				break;
				
		}//switch
				
	}//if (ret_val>=0)
	
	return i16_ret_val;

}//COMMANDS_IMPL_execute_command_rom_load


int16_t COMMANDS_IMPL_execute_command_rom_type(t_command * p_command){
	int16_t i16_ret_val=0;
    int32_t i32_read_bytes;
	uint8_t ui8_aux_string[AUX_FUNCS_F_P_MAX_STR_SIZE_64];	
    // uint16_t ui16_aux = 0;
	int32_t  i32_aux = 0;


    if (ui8_dpack_dumper_buffer_initialized==FALSE){
		
		_USART_SEND_STRING("\r\nThere is no valid data to type in memory buffer. Load");
	    _USART_SEND_STRING("\r\nor dump to memory buffer the content of a ROM first.");
				
	}else{

		_USART_SEND_STRING("\r\n");
			
		// while no error and while there are still bytes available to print 
		// keep reading from memory
		i32_read_bytes=0;	
		do{

			if ((i32_read_bytes%8)==0){
			
				//  print the file position at the beginning of the line
				_USART_SEND_STRING("\r\n0x");
				AUX_FUNCS_itoa(i32_read_bytes, ui8_aux_string, 16,AUX_FUNCS_F_P_MAX_STR_SIZE_64); // print each char in hex (16 ) format
				AUX_FUNCS_pad_begin_with_char(ui8_aux_string,'0',4,AUX_FUNCS_F_P_MAX_STR_SIZE_64);// pad the number with '0's at the beginning
				USART_send_string(ui8_aux_string,AUX_FUNCS_F_P_MAX_STR_SIZE_64);
				_USART_SEND_STRING(": ");
			
			}//if
		
			// print each ROM PACK buffer byte
			_USART_SEND_STRING(" 0x");
			i32_aux = (int32_t)ui8_dpack_dumper_nibbles_buffer[i32_read_bytes];
			AUX_FUNCS_itoa(i32_aux, ui8_aux_string, 16,AUX_FUNCS_F_P_MAX_STR_SIZE_64); // print each char in hex (16) format
			AUX_FUNCS_pad_begin_with_char(ui8_aux_string,'0',2,AUX_FUNCS_F_P_MAX_STR_SIZE_64);// pad the number with '0's at the beginning
			USART_send_string(ui8_aux_string,AUX_FUNCS_F_P_MAX_STR_SIZE_64);
				
			i32_read_bytes++;
		
		}while (i32_read_bytes<MAX_ROM_NIBBLES_BUFFER);
	
	}//if (ui8_dpack_dumper_buffer_initialized==FALSE)
	
	return i16_ret_val;

}//COMMANDS_IMPL_execute_command_rom_type


int16_t COMMANDS_IMPL_execute_command_rom_run(t_command * p_command){
	int16_t i16_ret_val=0;
	uint8_t ui8_usart_char = 0;


    if (ui8_dpack_dumper_buffer_initialized==FALSE){
		
		_USART_SEND_STRING("\r\nThere is no valid data to run in memory buffer. Load");
	    _USART_SEND_STRING("\r\nor dump to memory buffer the content of a ROM first.");
				
	}else{

		_USART_SEND_STRING("\r\nSet keyboard in play mode and press the song key.");
		_USART_SEND_STRING("\r\nEmulating ROM PACK ...");
	
		// switch on the YELLOW LED to indicate that the system is busy emulating the cart
		GPIO_SetCtrlLinesOutput(VAR_CTRL_GPIO03);
	
		// to grant proper assembly execution and signals timings while reading the ROM Pack content, disable all  
		// interrupts and keep only the interrupts used to detect changes on the ROM pack CLCK2 line
        GPIO_KeepOnlyDrivePackInterrupts();

		// bus initialization: start with control lines. When emulating the ROM PACK ctrl lines stay always in input mode
		config_ctrl_lines_as_input();     // call function to set CTRL lines to INPUT
		config_data_lines_as_input();	  // call function to set DATA lines to INPUT

		// ################ begin: call assembly routine that emulates ROM PACK until any char is received through USART
		while (USART_receive_char(&ui8_usart_char)==FALSE){
			dpack_int_main();
		}//while
        // ################ end: call assembly routine that emulates ROM PACK

		// once the ROM Pack has been emulated then restore interrupts, and disable the interrupts used to 
		// sample ROM Pack CLKC1 and CLCK2 signals
        GPIO_RestoreAllSystemInterrupts();
			
		// switch off the YELLOW LED and others used to indicate that the system has stopped emulating the cart
		GPIO_SetCtrlLinesOutput(0x00);
	
		_USART_SEND_STRING("\r\nROM PACK emulation stopped.");
		
	}//if (ui8_dpack_dumper_buffer_initialized==FALSE)
	
	return i16_ret_val;

}//COMMANDS_IMPL_execute_command_rom_run


int16_t COMMANDS_IMPL_execute_command_check(t_command * p_command){
	int16_t i16_ret_val=0;
	int32_t i32_aux = 0;
	int32_t i32_aux2 = 0;
	uint8_t ui8_aux_string[AUX_FUNCS_F_P_MAX_STR_SIZE_64];	
	uint8_t ui8_expected_header[] = ROMPACK_HEADER_NIBBLES;
	

    if (ui8_dpack_dumper_buffer_initialized==FALSE){
	    
		_USART_SEND_STRING("\r\nThere is no valid data to check in memory buffer. Load");
		_USART_SEND_STRING("\r\nor dump to memory buffer the content of a ROM first.");	
			
    }else{
	
		_USART_SEND_STRING("\r\nChecking ROM PACK header in memory buffer.");	
	
		// print expected header bytes
		_USART_SEND_STRING("\r\nExpected ROM PACK header is:");	
		for (i32_aux=0;i32_aux<ROMPACK_HEADER_NUM_BYTES;i32_aux++){
	
			// print each ROM PACK header byte
			_USART_SEND_STRING(" 0x");
			i32_aux2 = (int32_t)ui8_expected_header[i32_aux];
			AUX_FUNCS_itoa(i32_aux2, ui8_aux_string, 16,AUX_FUNCS_F_P_MAX_STR_SIZE_64); // print each char in hex (16) format
			AUX_FUNCS_pad_begin_with_char(ui8_aux_string,'0',2,AUX_FUNCS_F_P_MAX_STR_SIZE_64);// pad the number with '0's at the beginning
			USART_send_string(ui8_aux_string,AUX_FUNCS_F_P_MAX_STR_SIZE_64);
	
		}//for
	
		// print current header bytes 
		_USART_SEND_STRING("\r\nMemory ROM PACK header is  :");
		for (i32_aux=0;i32_aux<ROMPACK_HEADER_NUM_BYTES;i32_aux++){
		
			// print each ROM PACK buffer byte
			_USART_SEND_STRING(" 0x");
			i32_aux2 = (int32_t)ui8_dpack_dumper_nibbles_buffer[i32_aux];
			AUX_FUNCS_itoa(i32_aux2, ui8_aux_string, 16,AUX_FUNCS_F_P_MAX_STR_SIZE_64); // print each char in hex (16) format
			AUX_FUNCS_pad_begin_with_char(ui8_aux_string,'0',2,AUX_FUNCS_F_P_MAX_STR_SIZE_64);// pad the number with '0's at the beginning
			USART_send_string(ui8_aux_string,AUX_FUNCS_F_P_MAX_STR_SIZE_64);
				
		}//for	
	
		// check the header in the nibbles buffer header print the result
		if ( DPACK_CTRL_check_buffer()>=0){	
			_USART_SEND_STRING("\r\nROM PACK header in memory buffer seems OK!");	
		}else{
			_USART_SEND_STRING("\r\nERROR: ROM PACK header in memory buffer seems WRONG!");
		}//if
	
	 }//if (ui8_dpack_dumper_buffer_initialized==FALSE)
	
	 return i16_ret_val;

}//COMMANDS_IMPL_execute_command_check


int16_t COMMANDS_IMPL_execute_command_rom_info(t_command * p_command){
	int16_t i16_ret_val=0;
	int32_t i32_read_bytes;
	uint8_t ui8_aux_string[AUX_FUNCS_F_P_MAX_STR_SIZE_64];
    uint8_t ui8_aux=0;
	int32_t i32_aux = 0;


    if (ui8_dpack_dumper_buffer_initialized==FALSE){
	    
	    _USART_SEND_STRING("\r\nThere is no valid data or information in memory buffer.");
	    _USART_SEND_STRING("\r\nLoad or dump to memory buffer the content of a ROM first.");
				
	}else{

		_USART_SEND_STRING("\r\n");
		
		_USART_SEND_STRING("File name:\r\n");
		if (AUX_FUNCS_lstrlen(ui8_dpack_file_name,MAX_ROM_FILE_NAME)>0){
			_USART_SEND_STRING(ui8_dpack_file_name);
		}else{
		    _USART_SEND_STRING("(no file name set)");
		}//if	    	
		_USART_SEND_STRING("\r\n");

		_USART_SEND_STRING("File path:\r\n");
		if (AUX_FUNCS_lstrlen(ui8_dpack_file_path,MAX_ROM_FILE_NAME)>0){
			_USART_SEND_STRING(ui8_dpack_file_path);
		}else{
			_USART_SEND_STRING("(no file path set)");
		}//if		
		_USART_SEND_STRING("\r\n");

		// keep reading from title buffer if there are still bytes available to print
		_USART_SEND_STRING("Title:\r\n");
		i32_read_bytes=0;
		do{
            ui8_aux = ui8_dpack_title_buffer[i32_read_bytes];
            if ( ( (ui8_aux<=31) ||  (ui8_aux>=127) ) && (ui8_aux!=10) && (ui8_aux!=13)){
				// if current char is not a printable char then send an '?'
				ui8_aux = '?';
			}//if
		    USART_send_char(ui8_aux);
		
			i32_read_bytes++;
		
		}while ( (ui8_dpack_title_buffer[i32_read_bytes]!='\0') && (i32_read_bytes<MAX_ROM_TITLE_BUFFER) );
 
 		_USART_SEND_STRING("\r\n");
	
		// keep reading from song info buffer if there are still bytes available to print
		_USART_SEND_STRING("Songs Info:\r\n");
		i32_read_bytes=0;
		do{
	
            ui8_aux = ui8_dpack_songs_info_buffer[i32_read_bytes];
            if ( ( (ui8_aux<=31) ||  (ui8_aux>=127) ) && (ui8_aux!=10) && (ui8_aux!=13)){
	            // if current char is not a printable char then send an '?'
	            ui8_aux = '?';
            }//if
            USART_send_char(ui8_aux);
			
			i32_read_bytes++;
		
		}while ( (ui8_dpack_songs_info_buffer[i32_read_bytes]!='\0') && (i32_read_bytes<MAX_ROM_SONGS_INFO_BUFFER) );
   
	}//if (ui8_dpack_dumper_buffer_initialized==FALSE)
	
	return i16_ret_val;

}//COMMANDS_IMPL_execute_command_rom_info


int16_t COMMANDS_IMPL_execute_rom(t_command * p_command){
	int16_t i16_ret_val=0;


	if (p_command->token_type[1] == TOKEN_IS_CONST){

		// check which is the received subcommand:
		// -PARAM_ROM_DUMP
		// -PARAM_ROM_WRITE		
		// -PARAM_ROM_LOAD
		// -PARAM_ROM_TYPE
		// -PARAM_ROM_INFO
		// -PARAM_ROM_RUN
		// -PARAM_ROM_CHECK
		// -PARAM_ROM_CHECK
		if (AUX_FUNCS_lstrcmp(p_command->token[1],PARAM_ROM_DUMP,MAX_TOKEN_STR_LEN)==0){
			
			// case <CONSTANT==COMMAND_ROM><CONSTANT==PARAM_ROM_DUMP>
			i16_ret_val = COMMANDS_IMPL_execute_command_rom_dump(p_command);
			
		}else if (AUX_FUNCS_lstrcmp(p_command->token[1],PARAM_ROM_WRITE,MAX_TOKEN_STR_LEN)==0){
			
			// case <CONSTANT==COMMAND_ROM><CONSTANT==PARAM_ROM_WRITE>*
			i16_ret_val = COMMANDS_IMPL_execute_command_rom_write(p_command);
			
		}else if (AUX_FUNCS_lstrcmp(p_command->token[1],PARAM_ROM_LOAD,MAX_TOKEN_STR_LEN)==0){
		
			// case <CONSTANT==COMMAND_ROM><CONSTANT==PARAM_ROM_LOAD>*
			i16_ret_val = COMMANDS_IMPL_execute_command_rom_load(p_command);
			
		}else if (AUX_FUNCS_lstrcmp(p_command->token[1],PARAM_ROM_TYPE,MAX_TOKEN_STR_LEN)==0){
		
			// case <CONSTANT==COMMAND_ROM><CONSTANT==PARAM_ROM_TYPE>
			i16_ret_val = COMMANDS_IMPL_execute_command_rom_type(p_command);
			
		}else if (AUX_FUNCS_lstrcmp(p_command->token[1],PARAM_ROM_INFO,MAX_TOKEN_STR_LEN)==0){
		
			// case <CONSTANT==COMMAND_ROM><CONSTANT==PARAM_ROM_INFO>
			i16_ret_val = COMMANDS_IMPL_execute_command_rom_info(p_command);
					
		}else if (AUX_FUNCS_lstrcmp(p_command->token[1],PARAM_ROM_RUN,MAX_TOKEN_STR_LEN)==0){
		
			// case <CONSTANT==COMMAND_ROM><CONSTANT==PARAM_ROM_RUN>
			i16_ret_val = COMMANDS_IMPL_execute_command_rom_run(p_command);			

		}else if (AUX_FUNCS_lstrcmp(p_command->token[1],PARAM_ROM_CHECK,MAX_TOKEN_STR_LEN)==0){
		
			// case <CONSTANT==COMMAND_ROM><CONSTANT==PARAM_ROM_CHECK>
			i16_ret_val = COMMANDS_IMPL_execute_command_check(p_command);

		}else if (AUX_FUNCS_lstrcmp(p_command->token[1],PARAM_ROM_RX1K,MAX_TOKEN_STR_LEN)==0){
		
			// case <CONSTANT==COMMAND_ROM><CONSTANT==PARAM_ROM_RX1K>
			i16_ret_val = COMMANDS_IMPL_execute_command_rom_rx1k(p_command);

		}else if (AUX_FUNCS_lstrcmp(p_command->token[1],PARAM_ROM_TX1K,MAX_TOKEN_STR_LEN)==0){
		
			// case <CONSTANT==COMMAND_ROM><CONSTANT==PARAM_ROM_TX1K>
			i16_ret_val = COMMANDS_IMPL_execute_command_rom_tx1k(p_command);
			
		}else{

			_USART_SEND_STRING("\r\nInvalid command parameter: ");
			USART_send_string(p_command->token[1],MAX_TOKEN_STR_SIZE);
			i16_ret_val=0;
			
		}//if
		
	}else{
		
		_USART_SEND_STRING("\r\nMissing parameters.");
		
		i16_ret_val = -1;
		
	}//if
	
	return i16_ret_val;

}//COMMANDS_IMPL_execute_rom


int16_t COMMANDS_IMPL_execute_command_rom_rx1k(t_command * p_command){
	int32_t i32_aux = 0;
	uint8_t ui8_aux = 0;
	int16_t i16_ret_val=0;
	TIMING_TICKS timer_wait;


    // check if received command parameters are valid 
	if (p_command->token_type[1]!=TOKEN_IS_CONST){
        
		// first command parameter is empty or not valid		
		_USART_SEND_STRING("\r\nInvalid or missing parameter 1: ");
		USART_send_string(p_command->token[1],MAX_TOKEN_STR_SIZE);
		i16_ret_val=-1;
		
	}else{
		
		// check which is the received subcommand:
		// -PARAM_COMS_RX1K
		if (AUX_FUNCS_lstrcmp(p_command->token[1],PARAM_ROM_RX1K,MAX_TOKEN_STR_LEN)==0){
					
			_USART_SEND_STRING("\r\nWaiting to start file transfer\r\n");	

			// try to establish communications with sender and to receive the first 1KXmodem packet
			timer_wait = TIMING_SetTicksDelay(0);
			i16_ret_val = USART_1KX_TOUT_ERR;
			ui8_aux = USART_1KXMODEM_RX_RETRIES;
			while ( (ui8_aux>0) && (i16_ret_val==USART_1KX_TOUT_ERR) ){

				if (TIMING_CheckTicksDelayReached(timer_wait)){
					i16_ret_val = USART_1KXmodem_rx_open();
					timer_wait = TIMING_SetTicksDelay(TIMMING_TICKS_1_S);
					ui8_aux--;
				}//if
		
			}//while			
			
			// check if the first 1KXmodem packet that confirms that 1KXmodem connection has been established has been received
			if (i16_ret_val == USART_1KX_PACKET_OK){
				
				// call the function that reads, processes and stores in the RAM buffer the ROM file received through 1kXmodem connection
				i16_ret_val = DATA_IO_1KXmodem_rom_receive();
				if (i16_ret_val >= 0){
					_USART_SEND_STRING("\r\nFile succesfully received and loaded to RAM!");	
				}else{
					_USART_SEND_STRING("\r\nError processing file!");	
				}//if
				
			}else{
				_USART_SEND_STRING("\r\nFile transfer start timeout!");					
			}//if
	
		}else{

			_USART_SEND_STRING("\r\nInvalid command parameter: ");
			USART_send_string(p_command->token[1],MAX_TOKEN_STR_SIZE);
			i16_ret_val=0;
		
		}//if

	}//if
	
	return i16_ret_val;	

}//COMMANDS_IMPL_execute_command_rom_rx1k


int16_t COMMANDS_IMPL_execute_command_rom_tx1k(t_command * p_command){
	int32_t i32_aux = 0;
	uint8_t ui8_aux = 0;
	int16_t i16_ret_val=0;
	TIMING_TICKS timer_wait;


	// check if received command parameters are valid
	if (p_command->token_type[1]!=TOKEN_IS_CONST){
		
		// first command parameter is empty or not valid
		_USART_SEND_STRING("\r\nInvalid or missing parameter 1: ");
		USART_send_string(p_command->token[1],MAX_TOKEN_STR_SIZE);
		i16_ret_val=-1;
		
	}else{
		
		// check which is the received subcommand:
		// -PARAM_COMS_TX1K
		if (AUX_FUNCS_lstrcmp(p_command->token[1],PARAM_ROM_TX1K,MAX_TOKEN_STR_LEN)==0){
			
			_USART_SEND_STRING("\r\nWaiting to start file transfer\r\n");

			// wait for the start communications request from sender USART_1KXMODEM_CHAR_C 
			timer_wait = TIMING_SetTicksDelay(0);
			i16_ret_val = USART_1KX_TOUT_ERR;
			ui8_aux = USART_1KXMODEM_RX_RETRIES;
			while ( (ui8_aux>0) && (i16_ret_val==USART_1KX_TOUT_ERR) ){

				if (TIMING_CheckTicksDelayReached(timer_wait)){
					i16_ret_val = USART_1KXmodem_tx_open();
					timer_wait = TIMING_SetTicksDelay(TIMMING_TICKS_1_S);
					ui8_aux--;
				}//if
				
			}//while
			
			// check if the receiver answered with USART_1KXMODEM_CHAR_C code indicating that he is ready to start communications
			if (i16_ret_val == USART_1KX_CHAR_C_OK){
				
				// call the function that reads, processes and stores in the RAM buffer the ROM file received through 1kXmodem connection
				i16_ret_val = DATA_IO_1KXmodem_rom_send();
				if (i16_ret_val >= 0){
					_USART_SEND_STRING("\r\nFile successfully sent to remote computer!");
				}else{
					_USART_SEND_STRING("\r\nError processing file!");
				}//if
				
				}else{
				_USART_SEND_STRING("\r\nFile transfer start timeout!");
			}//if
			
		}else{

			_USART_SEND_STRING("\r\nInvalid command parameter: ");
			USART_send_string(p_command->token[1],MAX_TOKEN_STR_SIZE);
			i16_ret_val=0;
			
		}//if

	}//if
	
	return i16_ret_val;

}//COMMANDS_IMPL_execute_command_rom_tx1k