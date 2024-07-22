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

#include "commands_fs_impl.h"


void COMMANDS_FS_IMPL_print_file_error(int16_t i16_file_result){

    i16_file_result = -1*i16_file_result;

	switch (i16_file_result){
		case FR_OK:	// FR_OK: (0) Succeeded
			_USART_SEND_STRING("\r\nFR_OK:(0) Succeeded.");
			break;
		case FR_DISK_ERR:// FR_DISK_ERR:(1) A hard error occurred in the low level disk I/O layer
			_USART_SEND_STRING("\r\nFR_DISK_ERR:(1) A hard error occurred in the low level disk I/O layer.");
			break;
		case FR_INT_ERR:// FR_INT_ERR:(2) Assertion failed
			_USART_SEND_STRING("\r\nFR_INT_ERR:(2) Assertion failed.");
			break;
		case FR_NOT_READY: // FR_NOT_READY:(3) The physical drive cannot work
			_USART_SEND_STRING("\r\nFR_NOT_READY:(3) The physical drive cannot work.");
			break;
		case FR_NO_FILE: // FR_NO_FILE:(4) Could not find the file
			_USART_SEND_STRING("\r\nFR_NO_FILE:(4) Could not find the file.");
			break;
		case FR_NO_PATH:// FR_NO_PATH:(5) Could not find the path
			_USART_SEND_STRING("\r\nFR_NO_PATH:(5) Could not find the path.");
			break;
		case FR_INVALID_NAME:// FR_INVALID_NAME:(6) The path name format is invalid
			_USART_SEND_STRING("\r\nFR_INVALID_NAME:(6) The path name format is invalid.");
			break;
		case FR_DENIED:// FR_DENIED:(7) Access denied due to prohibited access or directory full
			_USART_SEND_STRING("\r\nFR_DENIED:(7) Access denied due to prohibited access or directory full.");
			break;
		case FR_EXIST:// FR_EXIST:(8) Access denied due to prohibited access
			_USART_SEND_STRING("\r\n FR_EXIST:(8) Access denied due to prohibited access.");
			break;
		case FR_INVALID_OBJECT:// FR_INVALID_OBJECT:(9) The file/directory object is invalid
			_USART_SEND_STRING("\r\n FR_INVALID_OBJECT:(9) The file/directory object is invalid.");
			break;
		case FR_WRITE_PROTECTED:// FR_WRITE_PROTECTED:(10) The physical drive is write protected
			_USART_SEND_STRING("\r\n FR_WRITE_PROTECTED:(10) The physical drive is write protected.");
			break;
		case FR_INVALID_DRIVE:// FR_INVALID_DRIVE:(11) The logical drive number is invalid
			_USART_SEND_STRING("\r\n FR_INVALID_DRIVE:(11) The logical drive number is invalid.");
			break;
		case FR_NOT_ENABLED:// FR_NOT_ENABLED:(12) The volume has no work area
			_USART_SEND_STRING("\r\n FR_NOT_ENABLED:(12) The volume has no work area.");
			break;
		case FR_NO_FILESYSTEM:// FR_NO_FILESYSTEM:(13) There is no valid FAT volume
			_USART_SEND_STRING("\r\n FR_NO_FILESYSTEM:(13) There is no valid FAT volume.");
			break;
		case FR_MKFS_ABORTED:// FR_MKFS_ABORTED:(14) The f_mkfs() aborted due to any problem
			_USART_SEND_STRING("\r\n FR_MKFS_ABORTED:(14) The f_mkfs() aborted due to any problem.");
			break;
		case FR_TIMEOUT:// FR_TIMEOUT:(15) Could not get a grant to access the volume within defined period
			_USART_SEND_STRING("\r\n FR_TIMEOUT:(15) Could not get a grant to access the volume within defined period.");
			break;
		case FR_LOCKED:// FR_LOCKED:(16) The operation is rejected according to the file sharing policy
			_USART_SEND_STRING("\r\n FR_LOCKED:(16) The operation is rejected according to the file sharing policy.");
			break;
		case FR_NOT_ENOUGH_CORE:// FR_NOT_ENOUGH_CORE:(17) LFN working buffer could not be allocated
			_USART_SEND_STRING("\r\n FR_NOT_ENOUGH_CORE:(17) LFN working buffer could not be allocated.");
			break;
		case FR_TOO_MANY_OPEN_FILES:// FR_TOO_MANY_OPEN_FILES:(18) Number of open files > FF_FS_LOCK
			_USART_SEND_STRING("\r\n FR_TOO_MANY_OPEN_FILES:(18) Number of open files > FF_FS_LOCK.");
			break;
		case FR_INVALID_PARAMETER:// FR_INVALID_PARAMETER:(19) Given parameter is invalid
			_USART_SEND_STRING("\r\n FR_INVALID_PARAMETER:(19) Given parameter is invalid.");
			break;
		default:
			_USART_SEND_STRING("\r\n Unknown FRESULT error code.");
			break;
	}//switch
	
}//COMMANDS_FS_IMPL_print_file_error


int16_t COMMANDS_FS_IMPL_execute_mnt(t_command * p_command){
	int16_t i16_ret_val=0;

	
    _USART_SEND_STRING("\r\nMounting disk...");
	
	i16_ret_val = FILE_SYS_mount("", 1);
	if (i16_ret_val<0){
		_USART_SEND_STRING("\r\nDisk mount error!");
	}else{
		_USART_SEND_STRING("\r\nDisk mounted.");		
	}
	
	return i16_ret_val;
	
}//COMMANDS_FS_IMPL_execute_mnt


int16_t COMMANDS_FS_IMPL_execute_dir(t_command * p_command){
	int16_t i16_ret_val=0;
	uint8_t * path;
	DWORD AccSize;				/* Work register for fs command */
	WORD AccFiles, AccDirs;
	uint8_t ui8_aux_string1_64[AUX_FUNCS_F_P_MAX_STR_SIZE_64];
	uint8_t ui8_flags;
	uint64_t ui64_size;
	uint16_t i=0;
	

    i16_ret_val = FILE_SYS_dir_open("");
	if (i16_ret_val<0) {

		_USART_SEND_STRING("\r\nNo disk mounted");
		i16_ret_val = -1;
		
	}else{
		
		_USART_SEND_STRING("\r\nCurrent directory content: ");
	    
		// print information of each element in the directory
		FILE_SYS_dir_rewind();// rewind directory cursor to first directory element
		while ( (i16_ret_val=FILE_SYS_dir_file_goto_next(NULL, FILE_SYS_FLAG_FOLDER|FILE_SYS_FLAG_FILE))>=0 ){
			
			// get current listed element information			
			if (FILE_SYS_get_current_info(ui8_aux_string1_64, AUX_FUNCS_F_P_MAX_STR_SIZE_64, &ui64_size, &ui8_flags)>=0){
						
				if (ui8_flags & FILE_SYS_FLAG_FOLDER) {
				
					// current entry corresponds to a directory
				
					// print directory information:
					AccDirs++;
					_USART_SEND_STRING("\r\n");
					USART_send_string(ui8_aux_string1_64,AUX_FUNCS_F_P_MAX_STR_SIZE_64);
					_USART_SEND_STRING("\t <DIR>");

				}else if (ui8_flags & FILE_SYS_FLAG_FILE) {
				
					// current entry corresponds to a file
				
					// print file information
					// file name:
					// AUX_FUNCS_strcpy(ui8_aux_string,(uint8_t*)ui8_aux_string,AUX_FUNCS_F_P_MAX_STR_SIZE);
					_USART_SEND_STRING("\r\n");
					USART_send_string(ui8_aux_string1_64,63);
				
					// file size:
					AUX_FUNCS_itoa((int32_t)ui64_size, ui8_aux_string1_64, 10,AUX_FUNCS_F_P_MAX_STR_SIZE_64);// base 10
					_USART_SEND_STRING("\t");
					USART_send_string(ui8_aux_string1_64,AUX_FUNCS_F_P_MAX_STR_SIZE_64);
					_USART_SEND_STRING(" bytes");
			
				}//if
			
			}//if (FILE_SYS_dir_get_current_info(
			
		}//while
		
	}//if (funcResult != FR_OK)
	
	return i16_ret_val;
	
}//COMMANDS_FS_IMPL_execute_dir


int16_t COMMANDS_FS_IMPL_execute_make_dir(t_command * p_command){
	int16_t i16_ret_val=0;


	if (p_command->token_type[1] != TOKEN_IS_ALIAS){
		
		_USART_SEND_STRING("\r\nWrong directory name. Use: mkdir \"dir_name\" ");
		i16_ret_val = -1;
		
	}else{
		
		i16_ret_val = FILE_SYS_dir_mk(p_command->token[1]);
		if (i16_ret_val<0) {
			
			_USART_SEND_STRING("\r\nCould not create \"");
			USART_send_string(p_command->token[1],MAX_TOKEN_STR_LEN);
			_USART_SEND_STRING("\" directory.");
			i16_ret_val = -1;
			
		}else{
			
			_USART_SEND_STRING("\r\nCreated \"");
			USART_send_string(p_command->token[1],MAX_TOKEN_STR_LEN);
			_USART_SEND_STRING("\" directory.");
			
		}//if
		
	}//if
	
	return i16_ret_val;
	
}//COMMANDS_FS_IMPL_execute_make_dir


int16_t COMMANDS_FS_IMPL_execute_change_dir(t_command * p_command){
	int16_t i16_ret_val=0;


	if (p_command->token_type[1] != TOKEN_IS_ALIAS){
	
		_USART_SEND_STRING("\r\nWrong directory name. Use: cd \"dir_name\" ");
		i16_ret_val = -1;
		
	}else{
				
		i16_ret_val = FILE_SYS_dir_ch(p_command->token[1]);
		if (i16_ret_val<0) {
		
			_USART_SEND_STRING("\r\nCould not open \"");			
			USART_send_string(p_command->token[1],MAX_TOKEN_STR_LEN);			
			_USART_SEND_STRING("\" folder.");
			i16_ret_val = -1;
			
		}else{
		
			_USART_SEND_STRING("\r\n Current dir is ");
			USART_send_string(p_command->token[1],MAX_TOKEN_STR_LEN);
			
		}//if
	
	}//if
	
	return i16_ret_val;

}//COMMANDS_FS_IMPL_execute_change_dir


int16_t COMMANDS_FS_IMPL_execute_remove(t_command * p_command){
	int16_t i16_ret_val=0;


	if (p_command->token_type[1] != TOKEN_IS_ALIAS){
		
		_USART_SEND_STRING("\r\nWrong directory or file name. Use: rm \"dir_file_name\" ");
		i16_ret_val = -1;
		
	}else{
		
		i16_ret_val = FILE_SYS_dir_rm(p_command->token[1]);
		if (i16_ret_val<0) {
			
			_USART_SEND_STRING("\r\nCould not remove \"");
			USART_send_string(p_command->token[1],MAX_TOKEN_STR_LEN);
			i16_ret_val = -1;
			
		}else{
			
			_USART_SEND_STRING("\r\nRemoved \"");
			USART_send_string(p_command->token[1],MAX_TOKEN_STR_LEN);
			_USART_SEND_STRING("\".");
			
		}//if
		
	}//if
	
	return i16_ret_val;
	
}//COMMANDS_FS_IMPL_execute_remove


int16_t COMMANDS_FS_IMPL_execute_command_type_txt(uint8_t * pui8_file_name){
	int16_t i16_ret_val=0;
	uint32_t ui32_read_bytes; // read bytes on each f_read iteration
	int32_t i32_total_read_bytes; // Total read bytes
	uint8_t ui8_buffer[32]; // file read buffer
	uint8_t ui8_aux_string1_64[AUX_FUNCS_F_P_MAX_STR_SIZE_64];
	uint16_t ui16_aux;

		
	i16_ret_val = FILE_SYS_file_open((TCHAR*)pui8_file_name, FA_READ);
	if (i16_ret_val <0) {
			
		_USART_SEND_STRING("\r\n Could not open \"");
		USART_send_string(pui8_file_name,MAX_TOKEN_STR_LEN);
		_USART_SEND_STRING("\" file.");
		COMMANDS_FS_IMPL_print_file_error(i16_ret_val);
		i16_ret_val = -1;
			
	}else{
			
		// while no error and there are still bytes available keep reading from file
		i32_total_read_bytes=0;
		do{
				
			ui32_read_bytes=0;
									
			// read and print a block of bytes 		
		    i16_ret_val = FILE_SYS_file_read(ui8_buffer, sizeof ui8_buffer, &ui32_read_bytes);// Read a chunk of data from the source file

			//  print the file position/address at the beginning of the line
			if (ui32_read_bytes!=0){
				_USART_SEND_STRING("\r\n0x");
				AUX_FUNCS_itoa(i32_total_read_bytes, ui8_aux_string1_64, 16, AUX_FUNCS_F_P_MAX_STR_SIZE_64); // print each char in hex (16 ) format
				AUX_FUNCS_pad_begin_with_char(ui8_aux_string1_64,'0',4,AUX_FUNCS_F_P_MAX_STR_SIZE_64);// pad the number with '0's at the beginning
				USART_send_string(ui8_aux_string1_64,AUX_FUNCS_F_P_MAX_STR_SIZE_64);
				_USART_SEND_STRING(": ");
			}//if			
			
			// print the block of bytes			
			for (ui16_aux=0;ui16_aux<ui32_read_bytes;ui16_aux++){
				
				if ( (ui8_buffer[ui16_aux]>31) &&  (ui8_buffer[ui16_aux]<127) ){
					// if current char is a printable char then it is sent
					USART_send_char(ui8_buffer[ui16_aux]);
				}else{
					// if current char is not a printable char then send an '?'
					USART_send_char('?');					
				}//if
			
			}//for
				
			i32_total_read_bytes = i32_total_read_bytes + ui32_read_bytes;
				
		}while ((ui32_read_bytes!=0)&&(i16_ret_val>=0));
			
		// if any error has occurred show the corresponding message
		if (i16_ret_val<0){
			_USART_SEND_STRING("\r\nFile error.");
			COMMANDS_FS_IMPL_print_file_error(i16_ret_val);
			i16_ret_val = -1;
		}

		// close open file
        FILE_SYS_file_close();

		// print number of read bytes
		_USART_SEND_STRING("\r\n");
		AUX_FUNCS_itoa(i32_total_read_bytes, ui8_aux_string1_64, 10,AUX_FUNCS_F_P_MAX_STR_SIZE_64);
		USART_send_string(ui8_aux_string1_64,AUX_FUNCS_F_P_MAX_STR_SIZE_64);
		_USART_SEND_STRING(" bytes read.");
			
	}//if
	
	return i16_ret_val;

}//COMMANDS_FS_IMPL_execute_command_type_txt


int16_t COMMANDS_FS_IMPL_execute_command_type_hex(uint8_t * pui8_file_name){
	int16_t i16_ret_val=0;
	uint32_t ui32_read_bytes; // read bytes on each f_read iteration
	uint32_t ui32_total_read_bytes; // Total read bytes
	uint8_t ui8_buffer[8]; // file read buffer
	uint8_t ui8_aux_string1_64[AUX_FUNCS_F_P_MAX_STR_SIZE_64];
	uint16_t ui16_aux = 0;
	int32_t i32_aux = 0;


	i16_ret_val = FILE_SYS_file_open((TCHAR*)pui8_file_name, FA_READ);
	if (i16_ret_val<0) {
			
		_USART_SEND_STRING("\r\n Could not open \"");
		USART_send_string(pui8_file_name,MAX_TOKEN_STR_LEN);
		_USART_SEND_STRING("\" file.");
		COMMANDS_FS_IMPL_print_file_error(i16_ret_val);
		i16_ret_val = -1;
			
	}else{
			
		// while no error and there are still bytes available keep reading from file
		ui32_total_read_bytes=0;
		do{
				
			ui32_read_bytes=0;
												
			// read block of bytes			
			i16_ret_val = FILE_SYS_file_read( ui8_buffer, sizeof ui8_buffer, &ui32_read_bytes);// Read a chunk of data from the source file
			
			//  print the file position/address at the beginning of the line
			if (ui32_read_bytes!=0){
				_USART_SEND_STRING("\r\n0x");
				AUX_FUNCS_itoa(ui32_total_read_bytes, ui8_aux_string1_64, 16, AUX_FUNCS_F_P_MAX_STR_SIZE_64); // print each char in hex (16 ) format
				AUX_FUNCS_pad_begin_with_char(ui8_aux_string1_64,'0',4,AUX_FUNCS_F_P_MAX_STR_SIZE_64);// pad the number with '0's at the beginning
				USART_send_string(ui8_aux_string1_64,AUX_FUNCS_F_P_MAX_STR_SIZE_64);
				_USART_SEND_STRING(": ");
			}//if
			
			// print the block of bytes
			for (ui16_aux=0;ui16_aux<ui32_read_bytes;ui16_aux++){
				_USART_SEND_STRING(" 0x");
				i32_aux = (int32_t)ui8_buffer[ui16_aux];
				AUX_FUNCS_itoa(i32_aux, ui8_aux_string1_64, 16, AUX_FUNCS_F_P_MAX_STR_SIZE_64); // print each char in hex (16 ) format
				AUX_FUNCS_pad_begin_with_char(ui8_aux_string1_64,'0',2,AUX_FUNCS_F_P_MAX_STR_SIZE_64);// pad the number with '0's at the beginning
				USART_send_string(ui8_aux_string1_64,AUX_FUNCS_F_P_MAX_STR_SIZE_64);
			}//for
				
			ui32_total_read_bytes = ui32_total_read_bytes + ui32_read_bytes;
				
		}while ((ui32_read_bytes!=0)&&(i16_ret_val>=0));
			
		// if any error has occurred show the corresponding message
		if (i16_ret_val<0){
			_USART_SEND_STRING("\r\nFile error.");
			COMMANDS_FS_IMPL_print_file_error(i16_ret_val);
			i16_ret_val = -1;
		}

		// close open file
        FILE_SYS_file_close();

		// print number of read bytes
		_USART_SEND_STRING("\r\n");
		AUX_FUNCS_itoa(ui32_total_read_bytes, ui8_aux_string1_64, 10,AUX_FUNCS_F_P_MAX_STR_SIZE_64);
		USART_send_string(ui8_aux_string1_64,AUX_FUNCS_F_P_MAX_STR_SIZE_64);
		_USART_SEND_STRING(" bytes read.");
			
	}//if

	return i16_ret_val;

}//COMMANDS_FS_IMPL_execute_command_type_hex


int16_t COMMANDS_FS_IMPL_execute_command_type(t_command * p_command){
	int16_t ret_val=0;


	if (p_command->token_type[1] == TOKEN_IS_CONST){

		// check which is the received subcommand:
		// -PARAM_TYPE_TXT
		// -PARAM_TYPE_HEX
		if (AUX_FUNCS_lstrcmp(p_command->token[1],PARAM_TYPE_TXT,MAX_TOKEN_STR_LEN)==0){
			
			// case <CONSTANT==COMMAND_TYPE><CONSTANT==PARAM_TYPE_TXT>*
			ret_val = COMMANDS_FS_IMPL_execute_command_type_txt((uint8_t*)p_command->token[2]);
			
		}else if (AUX_FUNCS_lstrcmp(p_command->token[1],PARAM_TYPE_HEX,MAX_TOKEN_STR_LEN)==0){
			
			// case <CONSTANT==COMMAND_TYPE><CONSTANT==PARAM_TYPE_HEX>*
			ret_val = COMMANDS_FS_IMPL_execute_command_type_hex((uint8_t*)p_command->token[2]);
			
		}else{

			_USART_SEND_STRING("\r\nInvalid command parameter: ");
			USART_send_string(p_command->token[1],MAX_TOKEN_STR_SIZE);
			ret_val=0;
			
		}//if
		
	}else if (p_command->token_type[1] == TOKEN_IS_ALIAS){
		
		// if no TXT or HEX parameter is specified then directly call the print in hex
		// case <CONSTANT==COMMAND_TYPE>*
		ret_val = COMMANDS_FS_IMPL_execute_command_type_txt((uint8_t*)p_command->token[1]);
		
	}else{
		
		_USART_SEND_STRING("\r\nMissing parameters.");
		
		ret_val = -1;
		
	}//if
	
	return ret_val;

}//COMMANDS_FS_IMPL_execute_command_type


int16_t COMMANDS_FS_IMPL_execute_command_write(t_command * p_command){
   int16_t ret_val=0;
   int16_t i16_aux = 0;
   uint32_t ui32_bytes_to_write;
   uint32_t ui32_written_bytes;
		
		
	if (p_command->token_type[1]!=TOKEN_IS_ALIAS){
	
		_USART_SEND_STRING("\r\nInvalid command parameter: ");
		USART_send_string(p_command->token[1],MAX_TOKEN_STR_SIZE);
		ret_val=-1;	
	
	}

	if (ret_val>=0){
		
		// create specified destination file 
		ret_val = FILE_SYS_file_open( p_command->token[1], FA_WRITE | FA_CREATE_ALWAYS);
		if (ret_val<0) {		
				
			_USART_SEND_STRING("\r\nFile error.");
			COMMANDS_FS_IMPL_print_file_error(ret_val);
			ret_val = -1;	
					
		}//if
		
	}//if (ret_val>=0)
	
	if (ret_val>=0){
							
		// write into the file each of the tokens received after the file name
		i16_aux = 2;// first content token
		ret_val = 0;
		while ( (ret_val>=0)&&(i16_aux<p_command->total) ){
			
			ui32_bytes_to_write = AUX_FUNCS_lstrlen(p_command->token[i16_aux],MAX_TOKEN_STR_LEN);
			FILE_SYS_file_write( p_command->token[i16_aux], ui32_bytes_to_write, &ui32_written_bytes);// Write it to the destination file
			ret_val=FILE_SYS_file_write( "\r\n", 2, &ui32_written_bytes);// Write it to the destination file
			i16_aux++;
			
		}//while

		if (ret_val<0) {
				
			_USART_SEND_STRING("\r\nFile error.");
			COMMANDS_FS_IMPL_print_file_error(ret_val);
			ret_val = -1;
				
		}//if
		
		// Close open files
		ret_val=FILE_SYS_file_close();
		if (ret_val<0) {
			
			_USART_SEND_STRING("\r\nError closing file.");
			COMMANDS_FS_IMPL_print_file_error(ret_val);
			ret_val = -1;
			
		}//if
	
	}//if (ret_val>=0)
	
	if (ret_val>=0){	

		_USART_SEND_STRING("\r\nCreated file \"");
		USART_send_string(p_command->token[1],MAX_TOKEN_STR_LEN);
		_USART_SEND_STRING("\".");
		
		ret_val = 1; 
			
	}//if (return>=0)
	
	return ret_val;

}//COMMANDS_FS_IMPL_execute_command_write