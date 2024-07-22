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
// *****************************************************************


#include "data_io.h"


// declare extern structures
extern uint8_t ui8_nibbles_buffer[MAX_ROM_NIBBLES_BUFFER];
extern uint8_t ui8_nibbles_buffer_initialized;
extern int32_t i32_dumper_rom_size;
extern uint8_t ui8_rom_title[MAX_ROM_TITLE_BUFFER];
extern uint8_t ui8_themes_titles_arr[MAX_THEME_TITLES_ARRAY][MAX_THEME_TITLE_BUFFER];
extern uint8_t ui8_file_name[MAX_ROM_FILE_NAME];
extern uint8_t ui8_file_path[MAX_ROM_FILE_PATH];



int16_t DATA_IO_file_rom_load(uint8_t * pui8_file_name ){
	int16_t i16_ret_val=0;
	uint32_t ui32_bytes_to_read;
	uint32_t ui32_read_bytes;
	uint32_t ui32_data_block_read_bytes;
	uint16_t ui16_tag_read_bytes;
	uint16_t ui16_rom_title_read_bytes;
	uint16_t ui16_curr_theme_title_read_bytes;
	uint16_t ui16_curr_theme_title_idx;
	uint32_t ui32_data_block_size = 0;
	uint8_t  ui8_version_bytes[ROMPACK_FILE_TAG_N_VERSION_SIZE];
	uint8_t  ui8_read_data[ROMPACK_FILE_RW_DEFAULT_N_BYTES];
	uint8_t  ui8_read_tag_name[ROMPACK_FILE_MAX_XML_TAG_SIZE];
	uint16_t ui16_aux = 0;
	uint8_t * pui8_aux = 0;
	uint8_t ui8_read_fsm = 0;

    // update current rom file path and file general information
    i16_ret_val = FILE_SYS_get_cur_dir(ui8_file_path,MAX_ROM_FILE_PATH);
    if (i16_ret_val>=0){
	    AUX_FUNCS_lstrcpy(ui8_file_name,pui8_file_name,MAX_ROM_FILE_NAME);
    }//if

	if (i16_ret_val>=0){
				
		i16_ret_val = FILE_SYS_file_open((TCHAR*)pui8_file_name, FA_READ);
		if (i16_ret_val<0) {
			
			i16_ret_val = -1;//File open error.
			
		}//if
		
	}//if (ret_val>=0)
	
	if (i16_ret_val>=0){
		
		// read and check ROMPACK format and version 
		i16_ret_val=FILE_SYS_file_read(ui8_version_bytes, ROMPACK_FILE_TAG_N_VERSION_SIZE,  &ui32_read_bytes);
		if (AUX_FUNCS_lstrcmp((uint8_t*)ROMPACK_FILE_TAG_N_VERSION, ui8_version_bytes, ROMPACK_FILE_TAG_N_VERSION_SIZE)!=0){
			
			FILE_SYS_file_close();
			i16_ret_val = -2;//Error in ROM PACK file format / version
			
		}//if
		
	}//if (ret_val>=0)
		
	while ( (i16_ret_val>=0) && (FILE_SYS_file_eof()>=0) ){
		
		// process the file content
		
		// get DATA_BLOCK_TYPE byte and process the content of the DATA_BLOCK it
		i16_ret_val = FILE_SYS_file_read( ui8_version_bytes, 1,  &ui32_read_bytes);
		switch (ui8_version_bytes[0]){

			case FILE_DATA_BLOCK_SONGS_INFO:
				// read the size of the SONGS_INFO DATA_BLOCK
				i16_ret_val = FILE_SYS_file_read(&ui32_data_block_size, sizeof(uint32_t), &ui32_read_bytes);
				
                // clear the content of the array where the different theme titles are stored
				memset(ui8_themes_titles_arr,'\0',sizeof(ui8_themes_titles_arr));
                memset(ui8_rom_title,'\0',sizeof(ui8_rom_title));

				// keep reading the ROM TITLE and the THEME TITLES embedded into the SONGS_INFO DATA_BLOCK of  
				// the DRP file until all the content of the block has been read. This information is embedded
				// between the corresponding <xxxxx> </xxxx> XML style tags inside the SONGS_INFO DATA_BLOCK.
				ui32_data_block_read_bytes =0 ;
				ui8_read_fsm = FSM_LOAD_DRP_XML_UNDEFINED;
				ui16_tag_read_bytes = 0;
				ui16_rom_title_read_bytes = 0;
				ui16_curr_theme_title_read_bytes = 0;
				ui16_curr_theme_title_idx = 0;				
				while ( (i16_ret_val>=0) && (ui32_data_block_read_bytes<ui32_data_block_size) ){
					
					// calculate the number of bytes of the next data chunk to read ( data is read from file in blocks  
					// of maximum ROMPACK_FILE_RW_DEFAULT_N_BYTES but it may not be multiple of the data block size )				
					if ( (ui32_data_block_read_bytes + ROMPACK_FILE_RW_DEFAULT_N_BYTES)>=ui32_data_block_size) {
						ui32_bytes_to_read = ui32_data_block_size - ui32_data_block_read_bytes;
					}else{
						ui32_bytes_to_read = ROMPACK_FILE_RW_DEFAULT_N_BYTES;
					}
					
                    // clear the chunk buffer before reading the next data chunk of SONG INFO bytes from file
					memset(ui8_read_data,0,sizeof(ui8_read_data));
					ui32_read_bytes = 0;
					i16_ret_val = FILE_SYS_file_read(ui8_read_data, ui32_bytes_to_read,  &ui32_read_bytes);
					
					// process the bytes read from the file, identifying the different "XML" data TAGs, updating the 
					// corresponding structs with the thread data taking care of not writing beyond the buffer limits
					ui16_aux=0;
					while ( (i16_ret_val>=0) && (ui16_aux<ui32_read_bytes) ){
									
						switch (ui8_read_fsm){ // 0 UNDEFINED, 1 TAG MARK, 2 START TAG, 3 END TAG, 4 ROM TITLE, 5 THEME TITLE
							
							case FSM_LOAD_DRP_XML_UNDEFINED:
							   // keep processing the content of the SONGS_INFO DATA_BLOCK without any special 
							   // action until a '<' character is found ( or the end of the SONGS_INFO block si reached )
							   if (ui8_read_data[ui16_aux]=='<'){
								   ui8_read_fsm =  FSM_LOAD_DRP_XML_TAG_MARK;
							   }
							   ui16_tag_read_bytes = 0;
							   break;
							   
							case FSM_LOAD_DRP_XML_TAG_MARK: 							
							   // the initial character of a TAG '<' was previously found, so now must decide 
							   // if it is a START TAG  '<xxxx>' or an END TAG '</xxxx>'
							   if (ui8_read_data[ui16_aux]=='/'){
								   ui8_read_fsm =  FSM_LOAD_DRP_XML_END_TAG;
							   }else{
								   // previous read tag was '<' and current TAG is not a '/' so it is
								   // a START TAG. Take and store the current TAG char and jump to the
								   // following FSM_LOAD_DRP_XML_START_TAG to continue reading the TAG			
								   ui8_read_fsm =  FSM_LOAD_DRP_XML_START_TAG;
								   ui8_read_tag_name[0] = ui8_read_data[ui16_aux];
								   ui16_tag_read_bytes++;
							   }							
							   break;
							   
							case FSM_LOAD_DRP_XML_START_TAG:
							   //  a START TAG  '<xxxx>' is being read so keep reading the START TAG chars
							   // until the '>' symbol is found and then decide which TAG is beign read.
							   if (ui8_read_data[ui16_aux]=='>'){
								  
								  // then  START TAG has been completely read so decide which TAG is it							   
							      ui8_read_tag_name[ui16_tag_read_bytes] = '\0';
								  if (AUX_FUNCS_lstrcmp( ui8_read_tag_name, "rom_info", ROMPACK_FILE_MAX_XML_TAG_SIZE )==0){
									  ui8_read_fsm = FSM_LOAD_DRP_XML_ROM_INFO;
								  }else if (AUX_FUNCS_lstrcmp( ui8_read_tag_name, "ro_ti", ROMPACK_FILE_MAX_XML_TAG_SIZE )==0){
                                      ui8_read_fsm = FSM_LOAD_DRP_XML_ROM_TITLE;
									  ui16_rom_title_read_bytes = 0;
								  }else if (AUX_FUNCS_lstrcmp( ui8_read_tag_name, "li_ti", ROMPACK_FILE_MAX_XML_TAG_SIZE )==0){
								      ui8_read_fsm = FSM_LOAD_DRP_XML_LI_THEMES_TITLES;
									  ui16_curr_theme_title_read_bytes = 0;
				                      ui16_curr_theme_title_idx = 0;						  
								  }else if (AUX_FUNCS_lstrcmp( ui8_read_tag_name, "ti", ROMPACK_FILE_MAX_XML_TAG_SIZE )==0){									  
									  ui8_read_fsm = FSM_LOAD_DRP_XML_THEME_TITLE;
									  ui16_curr_theme_title_read_bytes = 0;								  
								  }else if (AUX_FUNCS_lstrcmp( ui8_read_tag_name, "info", ROMPACK_FILE_MAX_XML_TAG_SIZE )==0){
								      ui8_read_fsm = FSM_LOAD_DRP_XML_INFO;									  
								  }else{
									  ui8_read_fsm = FSM_LOAD_DRP_XML_UNDEFINED;
								  }//if
								  
							   }else{
								  								  
								  if (ui16_tag_read_bytes<(ROMPACK_FILE_MAX_XML_TAG_SIZE-1)){
  									 // store and continue reading the characters of the processed START TAG
									 ui8_read_tag_name[ui16_tag_read_bytes] = ui8_read_data[ui16_aux];
									 ui16_tag_read_bytes++;
								  }else if (ui16_tag_read_bytes>=(ROMPACK_FILE_MAX_XML_TAG_SIZE-1)){
								     // it is not possible to continue reading the processed START TAG because there is no space 
									 // in the buffer used to read the TAG ( position ROMPACK_FILE_MAX_XML_TAG_SIZE-1 is for the '\0' )
									 i16_ret_val=-2;// error, abort!								  
								  }
								   
							   }//if
							   break;
							   
							case FSM_LOAD_DRP_XML_END_TAG:
								//  an END TAG  '</xxxx>' is being read so keep reading the END TAG chars
								// until the '>' symbol is found and then jump to UNDEFINED.					
								if (ui8_read_data[ui16_aux]=='>'){
									ui8_read_fsm =  FSM_LOAD_DRP_XML_UNDEFINED;
								}
								ui16_tag_read_bytes = 0;							
								break;
							   
							case FSM_LOAD_DRP_XML_ROM_TITLE:
							   // reading the content of between th ROM_TITLE '<ro_ti>...</ro_ti>' tags
							   // Check if a '<' TAG MARK has been read, that means that the end of the current
							   // ROM_TITLE XML field content has been reached and a new START TAG '<xxxx>'
							   // or END TAG '</xxx>' has been found						
								if (ui8_read_data[ui16_aux]=='<'){
									ui8_rom_title[ui16_rom_title_read_bytes] = '\0';
									ui8_read_fsm =  FSM_LOAD_DRP_XML_TAG_MARK;
								}else{

									// check that the "ui16_rom_title_read_bytes" index is still in array bounds
									// and generate an error to abort if not.
									if (ui16_rom_title_read_bytes<(MAX_ROM_TITLE_BUFFER-1)){
										 ui8_rom_title[ui16_rom_title_read_bytes]=ui8_read_data[ui16_aux];
										 ui16_rom_title_read_bytes++;
									}else{
										// do not continue storing more ROM TITLE bytes ( position MAX_ROM_TITLE_BUFFER-1
										// is only allowed for the '\0' ) 
									}
									
								}																						
								ui16_tag_read_bytes = 0;
							   break;

							case FSM_LOAD_DRP_XML_LI_THEMES_TITLES:
							   // reading the content of between the LIST_TITLE '<li_ti>...</li_ti>' tags
							   // check if a '<' TAG MARK has been read, that means that the end of the current 
							   // LI_THEMES_TITLES XML field content has been reached and a new START TAG '<xxxx>'
							   // or END TAG '</xxx>' has been found										
							   if (ui8_read_data[ui16_aux]=='<'){
								   ui8_read_fsm =  FSM_LOAD_DRP_XML_TAG_MARK;
							   }
							   ui16_tag_read_bytes = 0;  							   
							   break;
	
							case FSM_LOAD_DRP_XML_THEME_TITLE:
							   // reading the content of between the THEME_TITLE '<ti>...</ti>' tags
							   // check if a '<' TAG MARK has been read, that means that the end of the current 
							   // THEME_TITLE XML field content has been reached and a new START TAG '<xxxx>'
							   // or END TAG '</xxx>' has been found								   							
							   if (ui8_read_data[ui16_aux]=='<'){
								   ui8_read_fsm =  FSM_LOAD_DRP_XML_TAG_MARK;
								   ui8_themes_titles_arr[ui16_curr_theme_title_idx][ui16_curr_theme_title_read_bytes] ='\0';
								   ui16_curr_theme_title_idx++;
							   }else{
								   // take only the first (MAX_THEME_TITLE_BUFFER-1) characters of each theme title 
								   // because there is no space in the array for the characters beyond that limit 
								   if (ui16_curr_theme_title_read_bytes<(MAX_THEME_TITLE_BUFFER-1)){							   
								      ui8_themes_titles_arr[ui16_curr_theme_title_idx][ui16_curr_theme_title_read_bytes]=ui8_read_data[ui16_aux];
									  ui16_curr_theme_title_read_bytes++;							   
								   }//if

							   }//if					   
							   ui16_tag_read_bytes = 0;								
							   break;
							
							case FSM_LOAD_DRP_XML_ROM_INFO:
							   // reading the content of between the ROM_INFO '<rom_info>...</rom_info>' tags
							   // check if a '<' TAG MARK has been read, that means that the end of the current 
							   // ROM_INFO XML field content has been reached and a new START TAG '<xxxx>'
							   // or END TAG '</xxx>' has been found								   
							   if (ui8_read_data[ui16_aux]=='<'){
								   ui8_read_fsm =  FSM_LOAD_DRP_XML_TAG_MARK;
							   }
							   ui16_tag_read_bytes = 0;							
							  break;
							  
							case FSM_LOAD_DRP_XML_INFO:
							   // check if a '<' TAG MARK has been read, that means that the end of the current 
							   // INFO XML XML field content has been reached and a new START TAG '<xxxx>'
							   // or END TAG '</xxx>' has been found									
								if (ui8_read_data[ui16_aux]=='<'){
									ui8_read_fsm =  FSM_LOAD_DRP_XML_TAG_MARK;
								}
								ui16_tag_read_bytes = 0;
								break;
														   
						}//switch

						ui16_aux++;
					
					}//while
					
					// update the amount of block data read from disk
					ui32_data_block_read_bytes = ui32_data_block_read_bytes + ui32_read_bytes;

				}//while				
				break;
					
			case FILE_DATA_BLOCK_SONGS_ROM:
				// read the size of the SONGS_ROM DATA_BLOCK
				i16_ret_val = FILE_SYS_file_read(&ui32_data_block_size, sizeof(uint32_t), &ui32_read_bytes);

				if (ui32_data_block_size>MAX_ROM_NIBBLES_BUFFER) ui32_data_block_size = MAX_ROM_NIBBLES_BUFFER;

				// keep reading the ROM themes data nibbles embedded in the SONGS_ROM DATA BLOCK, until having
				// read all them or until have filled the ROM memory buffer
				ui32_data_block_read_bytes =0;
				memset(ui8_nibbles_buffer,0,sizeof(ui8_nibbles_buffer));				
				while ( (i16_ret_val>=0) && (ui32_data_block_read_bytes<ui32_data_block_size) ){
					
					// calculate the number of bytes of next data chunk to read to avoid writing beyond the buffer limit
					if ( (ui32_data_block_read_bytes + ROMPACK_FILE_RW_DEFAULT_N_BYTES)>=ui32_data_block_size) {
						ui32_bytes_to_read = ui32_data_block_size - ui32_data_block_read_bytes;
					}else{
						ui32_bytes_to_read = ROMPACK_FILE_RW_DEFAULT_N_BYTES;
					}
					// read next block of ROM bytes from file
					memset(ui8_read_data,0,sizeof(ui8_read_data));
					ui32_read_bytes = 0;
					i16_ret_val = FILE_SYS_file_read(ui8_read_data, ui32_bytes_to_read,  &ui32_read_bytes);
					// store data read from file in the ROM bytes buffer taking care of not writing beyond the buffer limits
					ui16_aux=0;
					while ( ((ui32_data_block_read_bytes+ui16_aux)<MAX_ROM_NIBBLES_BUFFER) && (ui16_aux<ui32_read_bytes) ){	
										
						pui8_aux = &(ui8_nibbles_buffer[ui32_data_block_read_bytes + ui16_aux]);
						(*pui8_aux)=ui8_read_data[ui16_aux];
						ui16_aux++;
								
					}
					// update the amount of block data read from disk
					ui32_data_block_read_bytes = ui32_data_block_read_bytes + ui32_read_bytes;

				}//while				
				break;
					
			default:
				break;
					
		}//switch
	
		if (i16_ret_val<0) {
			
			// if any error occurred while reading the ROM file invalidate and close the read file content it
			FILE_SYS_file_close();
			// clear the flag that indicates if the ui8_dpack_dumper_buffer contains valid ROM data
			ui8_nibbles_buffer_initialized = FALSE; 
			i32_dumper_rom_size = 0;
            // clear the content of the array where the rom title and the different theme titles are stored
            memset(ui8_themes_titles_arr,'\0',sizeof(ui8_themes_titles_arr));
            memset(ui8_rom_title,'\0',sizeof(ui8_rom_title));

			i16_ret_val = -3;// Error while reading from file

		}//if
		
	}//while
			
	if (i16_ret_val>=0){
		
		// Close file
		FILE_SYS_file_close();
			
		ui8_nibbles_buffer_initialized = TRUE;// set the flag that indicates that the ui8_dpack_dumper_buffer contains valid ROM data		
		i32_dumper_rom_size = DPACK_CTRL_get_size_rom_in_buffer();
		
		i16_ret_val = 0;

	}//(ret_val>=0)

	return i16_ret_val;
	
}//DATA_IO_file_rom_load



int16_t DATA_IO_file_rom_write(uint8_t * pui8_file_name ){
    int16_t i16_ret_val=0;
	uint32_t ui32_bytes_to_write;
	uint32_t ui32_written_bytes;
	uint32_t ui32_total_written_bytes;
	uint8_t ui8_file_bytes[ROMPACK_FILE_RW_DEFAULT_N_BYTES];
	uint8_t * pui8_aux = 0;
	uint64_t ui64_rompack_cnt_start_offset_field = 0;
	uint64_t ui64_rompack_cnt_start_offset = 0;


    // update current rom file path and file general information
    i16_ret_val = FILE_SYS_get_cur_dir(ui8_file_path,MAX_ROM_FILE_PATH);
	if (i16_ret_val>=0){	
		AUX_FUNCS_lstrcpy(ui8_file_name,pui8_file_name,MAX_ROM_FILE_NAME);
    }//if

	if (i16_ret_val>=0){
	
		i16_ret_val = FILE_SYS_file_open((TCHAR*)pui8_file_name, FA_WRITE | FA_CREATE_ALWAYS);
		if (i16_ret_val<0) {
		
			i16_ret_val = -1;//File open error.
		
		}//if
	
	}//if (ret_val>=0)
	
	if (i16_ret_val>=0){
				
		// write ROMPACK format and version identification		
		AUX_FUNCS_lstrcpy(ui8_file_bytes,ROMPACK_FILE_TAG_N_VERSION,ROMPACK_FILE_RW_DEFAULT_N_BYTES);
		ui32_bytes_to_write = AUX_FUNCS_lstrlen(ui8_file_bytes,ROMPACK_FILE_RW_DEFAULT_N_BYTES);
		ui32_bytes_to_write++;// +1 for the '\0'		
		i16_ret_val = FILE_SYS_file_write(ui8_file_bytes, ui32_bytes_to_write, &ui32_written_bytes);// Write it to the destination file
		
        // write the default empty SONGS INFO METADATA block
        ui8_file_bytes[0]=FILE_DATA_BLOCK_SONGS_INFO;
        i16_ret_val = FILE_SYS_file_write(ui8_file_bytes, sizeof(uint8_t), &ui32_written_bytes );// Write the type of the metadata block
        ui32_bytes_to_write = DRP_DEFAULT_INFO_BLOCK_LEN;
		i16_ret_val = FILE_SYS_file_write(&ui32_bytes_to_write, sizeof(uint32_t), &ui32_written_bytes);// Write the size of the metadata block
        i16_ret_val = FILE_SYS_file_write(DRP_DEFAULT_INFO_BLOCK, DRP_DEFAULT_INFO_BLOCK_LEN, &ui32_written_bytes);// Write it to the destination file	

		// write a default SONGS ROM METADATA block
        ui8_file_bytes[0]=FILE_DATA_BLOCK_SONGS_ROM;
        i16_ret_val = FILE_SYS_file_write(ui8_file_bytes, sizeof(uint8_t), &ui32_written_bytes );// Write the type of the metadata block
		ui32_bytes_to_write = i32_dumper_rom_size;
		if ( ui32_bytes_to_write>=MAX_ROM_NIBBLES_BUFFER) ui32_bytes_to_write = MAX_ROM_NIBBLES_BUFFER;
        i16_ret_val = FILE_SYS_file_write(&ui32_bytes_to_write, sizeof(uint32_t), &ui32_written_bytes);// Write the size of the metadata block

		// write ROM data in the ROM METADATA block. Write the content of the ROM memory buffer in RAM ( ui8_nibbles_buffer ) to file
		ui32_total_written_bytes = 0;
		do{

			ui32_written_bytes = 0;
			pui8_aux = &(ui8_nibbles_buffer[ui32_total_written_bytes]);		
			i16_ret_val = FILE_SYS_file_write( pui8_aux, 128, &ui32_written_bytes);
				
			ui32_total_written_bytes = ui32_total_written_bytes + ui32_written_bytes;
			
		}while ( (i16_ret_val>=0) && (ui32_total_written_bytes<ui32_bytes_to_write) );

		// when no error occurs, FILE_SYS_file_write returns the number of written bytes(>0) instead of the error code(<0), so if 
		// i16_ret_val contains the number of written bytes then reset the variable to '0'
		if (i16_ret_val>=0){ i16_ret_val=0;}

		// close file
		FILE_SYS_file_close();

	}//if (ret_val>=0)

	return i16_ret_val;
	
}//DATA_IO_file_rom_write



int16_t DATA_IO_1KXmodem_rom_receive(){
	int16_t  i16_ret_val=0;
	uint32_t ui32_bytes_to_read;
	uint32_t ui32_read_bytes;
	uint32_t ui32_data_block_read_bytes;
	uint32_t ui32_data_block_size = 0;
	uint8_t  ui8_version_bytes[ROMPACK_FILE_TAG_N_VERSION_SIZE];
	uint8_t  ui8_read_data[ROMPACK_FILE_RW_DEFAULT_N_BYTES];
	uint8_t  ui8_read_tag_name[ROMPACK_FILE_MAX_XML_TAG_SIZE];
	uint16_t ui16_curr_theme_title_idx = 0;
	uint16_t ui16_curr_theme_title_read_bytes = 0;
	uint16_t ui16_rom_title_read_bytes = 0;
	uint16_t ui16_tag_read_bytes = 0;
	uint8_t  ui8_read_fsm = 0;
	uint16_t ui16_aux = 0;
	uint8_t * pui8_aux = 0;

	// read and check ROMPACK format and version 
	i16_ret_val=USART_1KXmodem_read(ui8_version_bytes, ROMPACK_FILE_TAG_N_VERSION_SIZE,  &ui32_read_bytes);
	if (AUX_FUNCS_lstrcmp((uint8_t*)ROMPACK_FILE_TAG_N_VERSION, ui8_version_bytes, ROMPACK_FILE_TAG_N_VERSION_SIZE)!=0){
			
		// FILE_SYS_file_close();
		i16_ret_val = USART_1KX_HEADER_ERR;//Error in ROM PACK file format / version
			
	}//if
				
	while ((i16_ret_val>=0)&&(i16_ret_val!=USART_1KX_EOT)){
		
		// process the file content
		
		// get DATA_BLOCK_TYPE byte and process it
		i16_ret_val = USART_1KXmodem_read(ui8_version_bytes, 1,  &ui32_read_bytes);
		switch (ui8_version_bytes[0]){
					
			case FILE_DATA_BLOCK_SONGS_INFO:	

				// get the size of the SONGS_INFO DATA_BLOCK
				i16_ret_val = USART_1KXmodem_read(&ui32_data_block_size, sizeof(uint32_t), &ui32_read_bytes);

				// clear the content of the array where the different theme titles are stored
				memset(ui8_themes_titles_arr,'\0',sizeof(ui8_themes_titles_arr));
				memset(ui8_rom_title,'\0',sizeof(ui8_rom_title));
				
				// keep reading the ROM TITLE and the THEME TITLES embedded into the SONGS_INFO DATA_BLOCK of the
				// received DRP file until all the content of the block has been read. This information is embedded
				// between the corresponding <xxxxx> </xxxx> XML style tags inside the SONGS_INFO DATA_BLOCK.
				ui32_data_block_read_bytes =0 ;
				ui8_read_fsm = FSM_LOAD_DRP_XML_UNDEFINED;
				ui16_tag_read_bytes = 0;
				ui16_rom_title_read_bytes = 0;
				ui16_curr_theme_title_read_bytes = 0;
				ui16_curr_theme_title_idx = 0;
				while ( (i16_ret_val>=0) && (ui32_data_block_read_bytes<ui32_data_block_size) ){
				
					// calculate the number of bytes of the next data chunk to read ( data is read from 1KxModem module 
					// in blocks of maximum ROMPACK_FILE_RW_DEFAULT_N_BYTES but it may not be multiple of the data block size )
					if ( (ui32_data_block_read_bytes + ROMPACK_FILE_RW_DEFAULT_N_BYTES)>=ui32_data_block_size) {
						ui32_bytes_to_read = ui32_data_block_size - ui32_data_block_read_bytes;
					}else{
						ui32_bytes_to_read = ROMPACK_FILE_RW_DEFAULT_N_BYTES;
					}
				
					// clear the chunk buffer before reading the next data chunk of SONG INFO bytes from received file
					memset(ui8_read_data,0,sizeof(ui8_read_data));
					ui32_read_bytes = 0;
				    i16_ret_val = USART_1KXmodem_read(ui8_read_data, ui32_bytes_to_read, &ui32_read_bytes);
				
					// process the bytes read from the received file, identifying the different "XML" data TAGs, updating 
					// the corresponding structs with the thread data taking care of not writing beyond the buffer limits
					ui16_aux=0;
					while ( (i16_ret_val>=0) && (ui16_aux<ui32_read_bytes) ){
					
						switch (ui8_read_fsm){ // 0 UNDEFINED, 1 TAG MARK, 2 START TAG, 3 END TAG, 4 ROM TITLE, 5 THEME TITLE
						
							case FSM_LOAD_DRP_XML_UNDEFINED:
								// keep processing the content of the SONGS_INFO DATA_BLOCK without any special
								// action until a '<' character is found ( or the end of the SONGS_INFO block si reached )
								if (ui8_read_data[ui16_aux]=='<'){
									ui8_read_fsm =  FSM_LOAD_DRP_XML_TAG_MARK;
								}
								ui16_tag_read_bytes = 0;
								break;
						
							case FSM_LOAD_DRP_XML_TAG_MARK:
								// the initial character of a TAG '<' was previously found, so now must decide
								// if it is a START TAG  '<xxxx>' or an END TAG '</xxxx>'
								if (ui8_read_data[ui16_aux]=='/'){
									ui8_read_fsm =  FSM_LOAD_DRP_XML_END_TAG;
									}else{
									// previous read tag was '<' and current TAG is not a '/' so it is
									// a START TAG. Take and store the current TAG char and jump to the
									// following FSM_LOAD_DRP_XML_START_TAG to continue reading the TAG
									ui8_read_fsm =  FSM_LOAD_DRP_XML_START_TAG;
									ui8_read_tag_name[0] = ui8_read_data[ui16_aux];
									ui16_tag_read_bytes++;
								}
								break;
						
							case FSM_LOAD_DRP_XML_START_TAG:
								//  a START TAG  '<xxxx>' is being read so keep reading the START TAG chars
								// until the '>' symbol is found and then decide which TAG is beign read.
								if (ui8_read_data[ui16_aux]=='>'){
							
									// then  START TAG has been completely read so decide which TAG is it
									ui8_read_tag_name[ui16_tag_read_bytes] = '\0';
									if (AUX_FUNCS_lstrcmp( ui8_read_tag_name, "rom_info", ROMPACK_FILE_MAX_XML_TAG_SIZE )==0){
										ui8_read_fsm = FSM_LOAD_DRP_XML_ROM_INFO;
									}else if (AUX_FUNCS_lstrcmp( ui8_read_tag_name, "ro_ti", ROMPACK_FILE_MAX_XML_TAG_SIZE )==0){
										ui8_read_fsm = FSM_LOAD_DRP_XML_ROM_TITLE;
										ui16_rom_title_read_bytes = 0;
									}else if (AUX_FUNCS_lstrcmp( ui8_read_tag_name, "li_ti", ROMPACK_FILE_MAX_XML_TAG_SIZE )==0){
										ui8_read_fsm = FSM_LOAD_DRP_XML_LI_THEMES_TITLES;
										ui16_curr_theme_title_read_bytes = 0;
										ui16_curr_theme_title_idx = 0;
									}else if (AUX_FUNCS_lstrcmp( ui8_read_tag_name, "ti", ROMPACK_FILE_MAX_XML_TAG_SIZE )==0){
										ui8_read_fsm = FSM_LOAD_DRP_XML_THEME_TITLE;
										ui16_curr_theme_title_read_bytes = 0;
									}else if (AUX_FUNCS_lstrcmp( ui8_read_tag_name, "info", ROMPACK_FILE_MAX_XML_TAG_SIZE )==0){
										ui8_read_fsm = FSM_LOAD_DRP_XML_INFO;
									}else{
										ui8_read_fsm = FSM_LOAD_DRP_XML_UNDEFINED;
									}//if
							
								}else{
							
									if (ui16_tag_read_bytes<(ROMPACK_FILE_MAX_XML_TAG_SIZE-1)){
										// store and continue reading the characters of the processed START TAG
										ui8_read_tag_name[ui16_tag_read_bytes] = ui8_read_data[ui16_aux];
										ui16_tag_read_bytes++;
									}else if (ui16_tag_read_bytes>=(ROMPACK_FILE_MAX_XML_TAG_SIZE-1)){
										// it is not possible to continue reading the processed START TAG because there is no space
										// in the buffer used to read the TAG ( position ROMPACK_FILE_MAX_XML_TAG_SIZE-1 is for the '\0' )
										i16_ret_val=-2;// error, abort!
									}
							
								}//if
								break;
						
							case FSM_LOAD_DRP_XML_END_TAG:
								//  an END TAG  '</xxxx>' is being read so keep reading the END TAG chars
								// until the '>' symbol is found and then jump to UNDEFINED.
								if (ui8_read_data[ui16_aux]=='>'){
									ui8_read_fsm =  FSM_LOAD_DRP_XML_UNDEFINED;
								}
								ui16_tag_read_bytes = 0;
								break;
						
							case FSM_LOAD_DRP_XML_ROM_TITLE:
								// reading the content of between th ROM_TITLE '<ro_ti>...</ro_ti>' tags
								// Check if a '<' TAG MARK has been read, that means that the end of the current
								// ROM_TITLE XML field content has been reached and a new START TAG '<xxxx>'
								// or END TAG '</xxx>' has been found
								if (ui8_read_data[ui16_aux]=='<'){
									ui8_rom_title[ui16_rom_title_read_bytes] = '\0';
									ui8_read_fsm =  FSM_LOAD_DRP_XML_TAG_MARK;
								}else{

									// check that the "ui16_rom_title_read_bytes" index is still in array bounds
									// and generate an error to abort if not.
									if (ui16_rom_title_read_bytes<(MAX_ROM_TITLE_BUFFER-1)){
										ui8_rom_title[ui16_rom_title_read_bytes]=ui8_read_data[ui16_aux];
										ui16_rom_title_read_bytes++;
										}else{
										// do not continue storing more ROM TITLE bytes ( position MAX_ROM_TITLE_BUFFER-1
										// is only allowed for the '\0' )
									}
							
								}
								ui16_tag_read_bytes = 0;
								break;

							case FSM_LOAD_DRP_XML_LI_THEMES_TITLES:
								// reading the content of between the LIST_TITLE '<li_ti>...</li_ti>' tags
								// check if a '<' TAG MARK has been read, that means that the end of the current
								// LI_THEMES_TITLES XML field content has been reached and a new START TAG '<xxxx>'
								// or END TAG '</xxx>' has been found
								if (ui8_read_data[ui16_aux]=='<'){
									ui8_read_fsm =  FSM_LOAD_DRP_XML_TAG_MARK;
								}
								ui16_tag_read_bytes = 0;
								break;

							case FSM_LOAD_DRP_XML_THEME_TITLE:
								// reading the content of between the THEME_TITLE '<ti>...</ti>' tags
								// check if a '<' TAG MARK has been read, that means that the end of the current
								// THEME_TITLE XML field content has been reached and a new START TAG '<xxxx>'
								// or END TAG '</xxx>' has been found
								if (ui8_read_data[ui16_aux]=='<'){
									ui8_read_fsm =  FSM_LOAD_DRP_XML_TAG_MARK;
									ui8_themes_titles_arr[ui16_curr_theme_title_idx][ui16_curr_theme_title_read_bytes] ='\0';
									ui16_curr_theme_title_idx++;
								}else{
									// take only the first (MAX_THEME_TITLE_BUFFER-1) characters of each theme title
									// because there is no space in the array for the characters beyond that limit
									if (ui16_curr_theme_title_read_bytes<(MAX_THEME_TITLE_BUFFER-1)){
										ui8_themes_titles_arr[ui16_curr_theme_title_idx][ui16_curr_theme_title_read_bytes]=ui8_read_data[ui16_aux];
										ui16_curr_theme_title_read_bytes++;
									}//if

								}//if
								ui16_tag_read_bytes = 0;
								break;
						
							case FSM_LOAD_DRP_XML_ROM_INFO:
								// reading the content of between the ROM_INFO '<rom_info>...</rom_info>' tags
								// check if a '<' TAG MARK has been read, that means that the end of the current
								// ROM_INFO XML field content has been reached and a new START TAG '<xxxx>'
								// or END TAG '</xxx>' has been found
								if (ui8_read_data[ui16_aux]=='<'){
									ui8_read_fsm =  FSM_LOAD_DRP_XML_TAG_MARK;
								}
								ui16_tag_read_bytes = 0;
								break;
						
							case FSM_LOAD_DRP_XML_INFO:
								// check if a '<' TAG MARK has been read, that means that the end of the current
								// INFO XML XML field content has been reached and a new START TAG '<xxxx>'
								// or END TAG '</xxx>' has been found
								if (ui8_read_data[ui16_aux]=='<'){
									ui8_read_fsm =  FSM_LOAD_DRP_XML_TAG_MARK;
								}
								ui16_tag_read_bytes = 0;
								break;
						
						}//switch

						ui16_aux++;
					
					}//while
				
					// update the amount of block data read from disk
					ui32_data_block_read_bytes = ui32_data_block_read_bytes + ui32_read_bytes;

				}//while			
				
				break;
					
			case FILE_DATA_BLOCK_SONGS_ROM:
				// get the size of the SONGS_ROM DATA_BLOCK
				i16_ret_val = USART_1KXmodem_read(&ui32_data_block_size, sizeof(uint32_t), &ui32_read_bytes);

				if (ui32_data_block_size>MAX_ROM_NIBBLES_BUFFER) ui32_data_block_size = MAX_ROM_NIBBLES_BUFFER;

				// read ROM data nibbles until having read all them or until have filled the ROM memory buffer
				memset(ui8_nibbles_buffer,0,sizeof(ui8_nibbles_buffer));
				ui32_data_block_read_bytes =0 ;
				while ( (i16_ret_val>=0) && (ui32_data_block_read_bytes<ui32_data_block_size ) ){
					
					// calculate the number of bytes of next data chunk to read to avoid writing beyond the buffer limit
					if ( (ui32_data_block_read_bytes + ROMPACK_FILE_RW_DEFAULT_N_BYTES)>=ui32_data_block_size) {
						ui32_bytes_to_read = ui32_data_block_size - ui32_data_block_read_bytes;
					}else{
						ui32_bytes_to_read = ROMPACK_FILE_RW_DEFAULT_N_BYTES;
					}

					// read next block of ROM bytes from 1KXmodem connection
					memset(ui8_read_data,0,sizeof(ui8_read_data));
					ui32_read_bytes = 0;
					i16_ret_val = USART_1KXmodem_read(ui8_read_data, ui32_bytes_to_read,  &ui32_read_bytes);
					// store data read from 1KXmodem connection in the ROM bytes buffer taking care of not writing beyond the buffer limits
					ui16_aux=0;
					while ( ((ui32_data_block_read_bytes+ui16_aux)<MAX_ROM_NIBBLES_BUFFER) && (ui16_aux<ui32_read_bytes) ){
						pui8_aux = &(ui8_nibbles_buffer[ui32_data_block_read_bytes + ui16_aux]);
						(*pui8_aux)=ui8_read_data[ui16_aux];
						ui16_aux++;
					}
					// update the amount of block data read from 1KXmodem connection
					ui32_data_block_read_bytes = ui32_data_block_read_bytes + ui32_read_bytes;

				}//while
				break;
					
			default:
				break;
					
		}//switch
		
	}//while
	
	// close 1KXmodem data reception 
	USART_1KXmodem_rx_close();
	
	if (i16_ret_val<0){		
		ui8_nibbles_buffer_initialized = FALSE; // clear the flag that indicates if the ui8_dpack_dumper_buffer contains valid ROM data
	}else{	
		ui8_nibbles_buffer_initialized = TRUE;// set the flag that indicates that the ui8_dpack_dumper_buffer contains valid ROM data
		i32_dumper_rom_size = DPACK_CTRL_get_size_rom_in_buffer();
	}//(ret_val<=0)

	return i16_ret_val;
	
}//DATA_IO_rom_1KXmodem_receive

int16_t DATA_IO_1KXmodem_rom_send(){
  int16_t i16_ret_val=0;
  uint32_t ui32_bytes_to_write;
  uint32_t ui32_written_bytes;
  uint32_t ui32_total_written_bytes;
  uint8_t ui8_file_bytes[ROMPACK_FILE_RW_DEFAULT_N_BYTES];
  uint8_t * pui8_aux = 0;
  uint64_t ui64_rompack_cnt_start_offset_field = 0;
  uint64_t ui64_rompack_cnt_start_offset = 0;


  if (ui8_nibbles_buffer_initialized == FALSE){
	  
	  i16_ret_val = USART_1KX_NOT_INITIALIZED;
	  
  }//if

  if (i16_ret_val>=0){
	  
	  // write ROMPACK format and version identification
	  AUX_FUNCS_lstrcpy(ui8_file_bytes,ROMPACK_FILE_TAG_N_VERSION,ROMPACK_FILE_RW_DEFAULT_N_BYTES);
	  ui32_bytes_to_write = AUX_FUNCS_lstrlen(ui8_file_bytes,ROMPACK_FILE_RW_DEFAULT_N_BYTES);
	  ui32_bytes_to_write++;// +1 for the '\0'
	  i16_ret_val =  USART_1KXmodem_write(ui8_file_bytes, ui32_bytes_to_write, &ui32_written_bytes);// Write it to the destination file
	  
	  // write the default empty SONGS INFO METADATA block
	  ui8_file_bytes[0]=FILE_DATA_BLOCK_SONGS_INFO;
	  i16_ret_val =  USART_1KXmodem_write(ui8_file_bytes, sizeof(uint8_t), &ui32_written_bytes);//  Write the type of the metadata block	
	  ui32_bytes_to_write = DRP_DEFAULT_INFO_BLOCK_LEN;
      i16_ret_val = USART_1KXmodem_write(&ui32_bytes_to_write, sizeof(uint32_t), &ui32_written_bytes);// Write the size of the metadata block
      i16_ret_val = USART_1KXmodem_write(DRP_DEFAULT_INFO_BLOCK, DRP_DEFAULT_INFO_BLOCK_LEN, &ui32_written_bytes);// Write it to remote computer
	  
	  // write a default SONGS ROM METADATA block
	  ui8_file_bytes[0]=FILE_DATA_BLOCK_SONGS_ROM;
	  i16_ret_val =  USART_1KXmodem_write(ui8_file_bytes, sizeof(uint8_t), &ui32_written_bytes);//  Write the type of the metadata block	
	  ui32_bytes_to_write = i32_dumper_rom_size;
	  if ( ui32_bytes_to_write>=MAX_ROM_NIBBLES_BUFFER) ui32_bytes_to_write = MAX_ROM_NIBBLES_BUFFER;	  
	  i16_ret_val = USART_1KXmodem_write(&ui32_bytes_to_write, sizeof(uint32_t), &ui32_written_bytes);// Write the size of the metadata block

	  // write ROM data in the ROM METADATA block. Write the content of the ROM memory buffer in RAM ( ui8_nibbles_buffer ) to remote computer
	  ui32_total_written_bytes = 0;
	  do{

		  ui32_written_bytes = 0;
		  pui8_aux = &(ui8_nibbles_buffer[ui32_total_written_bytes]);
		  i16_ret_val = USART_1KXmodem_write( pui8_aux, 128, &ui32_written_bytes);
  
		  ui32_total_written_bytes = ui32_total_written_bytes + ui32_written_bytes;
		  
	  }while ( (i16_ret_val>=0) && (ui32_total_written_bytes<ui32_bytes_to_write) );

	  // when no error occurs, USART_1KXmodem_write returns the number of successfully sent bytes(>0) instead of the error code(<0), 
	  // so if i16_ret_val contains the number of written bytes then reset the variable to '0'
	  if (i16_ret_val>=0){ i16_ret_val=0;}

   }//if (ret_val>=0)

	// close 1KXmodem data transmission
	USART_1KXmodem_tx_close();

	return i16_ret_val;
	
}//DATA_IO_1KXmodem_rom_send