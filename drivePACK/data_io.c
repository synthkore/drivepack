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
extern uint8_t ui8_dpack_dumper_nibbles_buffer[MAX_ROM_NIBBLES_BUFFER];
extern uint8_t ui8_dpack_dumper_buffer_initialized;
extern int32_t i32_dpack_dumper_rom_size;
extern uint8_t ui8_dpack_title_buffer[MAX_ROM_TITLE_BUFFER];
extern uint8_t ui8_dpack_songs_info_buffer[MAX_ROM_SONGS_INFO_BUFFER];
extern uint8_t ui8_dpack_file_name[MAX_ROM_FILE_NAME];
extern uint8_t ui8_dpack_file_path[MAX_ROM_FILE_PATH];



int16_t DATA_IO_file_rom_load(uint8_t * pui8_file_name ){
	int16_t i16_ret_val=0;
	uint32_t ui32_bytes_to_read;
	uint32_t ui32_read_bytes;
	uint32_t ui32_data_block_read_bytes;
	uint32_t ui32_data_block_size = 0;
	uint8_t  ui8_version_bytes[ROMPACK_FILE_TAG_N_VERSION_SIZE];
	uint8_t  ui8_read_data[ROMPACK_FILE_RW_DEFAULT_N_BYTES];
	uint8_t  crc_byte_0 = 0;
	uint8_t  crc_byte_1 = 0;
	uint16_t ui16_aux = 0;
	uint8_t * pui8_aux = 0;

    // update current rom file path and file general information
    i16_ret_val = FILE_SYS_get_cur_dir(ui8_dpack_file_path,MAX_ROM_FILE_PATH);
    if (i16_ret_val>=0){
	    AUX_FUNCS_lstrcpy(ui8_dpack_file_name,pui8_file_name,MAX_ROM_FILE_NAME);
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
		
		// get DATA_BLOCK_TYPE byte and process it
		i16_ret_val = FILE_SYS_file_read( ui8_version_bytes, 1,  &ui32_read_bytes);
		switch (ui8_version_bytes[0]){
				
			case FILE_DATA_BLOCK_TITLE:				
				// get the size of the TITLE DATA_BLOCK
				i16_ret_val = FILE_SYS_file_read(&ui32_data_block_size, sizeof(uint32_t), &ui32_read_bytes);
				
				// get the TITLE bytes
				memset(ui8_dpack_title_buffer,0,sizeof(ui8_dpack_title_buffer));
				ui32_data_block_read_bytes =0 ;
				while ( (i16_ret_val>=0) && (ui32_data_block_read_bytes<ui32_data_block_size) ){
				    
					// calculate the number of bytes of next data chunk to read ( data is read from file in blocks of
					// maximum ROMPACK_FILE_RW_DEFAULT_N_BYTES but it may not be multiple of the data block size)
					if ( (ui32_data_block_read_bytes + ROMPACK_FILE_RW_DEFAULT_N_BYTES)>=ui32_data_block_size) {
						ui32_bytes_to_read = ui32_data_block_size - ui32_data_block_read_bytes;
					}else{
						ui32_bytes_to_read = ROMPACK_FILE_RW_DEFAULT_N_BYTES;
					}
					// read next block of TITLE bytes from file
					memset(ui8_read_data,0,sizeof(ui8_read_data));
					ui32_read_bytes = 0;
					i16_ret_val = FILE_SYS_file_read(ui8_read_data, ui32_bytes_to_read,  &ui32_read_bytes);
					// store data read from file in the TITLE buffer taking care of not writing beyond the buffer limits
					ui16_aux=0;					
					while ( ((ui32_data_block_read_bytes+ui16_aux)<MAX_ROM_TITLE_BUFFER) && (ui16_aux<ui32_read_bytes) ){
						pui8_aux = &(ui8_dpack_title_buffer[ui32_data_block_read_bytes + ui16_aux]);
						(*pui8_aux)=ui8_read_data[ui16_aux];
						ui16_aux++;
					}
					// update the amount of block data read from disk
					ui32_data_block_read_bytes = ui32_data_block_read_bytes + ui32_read_bytes;

				}//while				
				break;
					
			case FILE_DATA_BLOCK_SONGS_INFO:	
				// get the size of the SONGS_INFO DATA_BLOCK
				i16_ret_val = FILE_SYS_file_read(&ui32_data_block_size, sizeof(uint32_t), &ui32_read_bytes);
				
				// get the SONGS_INFO bytes
				memset(ui8_dpack_songs_info_buffer,0,sizeof(ui8_dpack_songs_info_buffer));				
				ui32_data_block_read_bytes =0 ;
				while ( (i16_ret_val>=0) && (ui32_data_block_read_bytes<ui32_data_block_size) ){
					
					// calculate the number of bytes of next data chunk to read ( data is read from file in blocks of 
					// maximum ROMPACK_FILE_RW_DEFAULT_N_BYTES but it may not be multiple of the data block size )				
					if ( (ui32_data_block_read_bytes + ROMPACK_FILE_RW_DEFAULT_N_BYTES)>=ui32_data_block_size) {
						ui32_bytes_to_read = ui32_data_block_size - ui32_data_block_read_bytes;
					}else{
						ui32_bytes_to_read = ROMPACK_FILE_RW_DEFAULT_N_BYTES;
					}
                    // read next block of SONG INFO bytes from file
					memset(ui8_read_data,0,sizeof(ui8_read_data));
					ui32_read_bytes = 0;
					i16_ret_val = FILE_SYS_file_read(ui8_read_data, ui32_bytes_to_read,  &ui32_read_bytes);
					// store data read from file in the SONG INFO buffer taking care of not writing beyond the buffer limits
					ui16_aux=0;					
					while ( ((ui32_data_block_read_bytes+ui16_aux)<MAX_ROM_SONGS_INFO_BUFFER) && (ui16_aux<ui32_read_bytes) ){
					    pui8_aux = &(ui8_dpack_songs_info_buffer[ui32_data_block_read_bytes+ui16_aux]);
						(*pui8_aux)=ui8_read_data[ui16_aux];
						ui16_aux++;
					}
					// update the amount of block data read from disk
					ui32_data_block_read_bytes = ui32_data_block_read_bytes + ui32_read_bytes;

				}//while				
				break;
					
			case FILE_DATA_BLOCK_SONGS_ROM:
				// get the size of the SONGS_ROM DATA_BLOCK
				i16_ret_val = FILE_SYS_file_read(&ui32_data_block_size, sizeof(uint32_t), &ui32_read_bytes);

				if (ui32_data_block_size>MAX_ROM_NIBBLES_BUFFER) ui32_data_block_size = MAX_ROM_NIBBLES_BUFFER;

				// read ROM data nibbles until having read all them or until have filled the ROM memory buffer
				ui32_data_block_read_bytes =0;
				memset(ui8_dpack_dumper_nibbles_buffer,0,sizeof(ui8_dpack_dumper_nibbles_buffer));				
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
						pui8_aux = &(ui8_dpack_dumper_nibbles_buffer[ui32_data_block_read_bytes + ui16_aux]);
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
			ui8_dpack_dumper_buffer_initialized = FALSE; // clear the flag that indicates if the ui8_dpack_dumper_buffer contains valid ROM data
			i16_ret_val = -3;// Error while reading from file

		}//if
		
	}//while
			
	if (i16_ret_val>=0){
		
		// Close file
		FILE_SYS_file_close();
			
		ui8_dpack_dumper_buffer_initialized = TRUE;// set the flag that indicates that the ui8_dpack_dumper_buffer contains valid ROM data		
		i32_dpack_dumper_rom_size = DPACK_CTRL_get_size_rom_in_buffer();
		
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
    i16_ret_val = FILE_SYS_get_cur_dir(ui8_dpack_file_path,MAX_ROM_FILE_PATH);
	if (i16_ret_val>=0){	
		AUX_FUNCS_lstrcpy(ui8_dpack_file_name,pui8_file_name,MAX_ROM_FILE_NAME);
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
				
        // write a default TITLE METADATA block		
		ui8_file_bytes[0]=FILE_DATA_BLOCK_TITLE;
		i16_ret_val = FILE_SYS_file_write(ui8_file_bytes, sizeof(uint8_t), &ui32_written_bytes );// Write the type of the metadata block			
		ui32_bytes_to_write = AUX_FUNCS_lstrlen(ui8_dpack_title_buffer,MAX_ROM_TITLE_BUFFER);
		ui32_bytes_to_write++;// +1 for the '\0'		
		i16_ret_val = FILE_SYS_file_write(&ui32_bytes_to_write, sizeof(uint32_t), &ui32_written_bytes);// Write the size of the metadata block
		i16_ret_val = FILE_SYS_file_write(ui8_dpack_title_buffer, ui32_bytes_to_write, &ui32_written_bytes);// Write it to the destination file
	
        // write a default  SONGS INFO METADATA block
        ui8_file_bytes[0]=FILE_DATA_BLOCK_SONGS_INFO;
        i16_ret_val = FILE_SYS_file_write(ui8_file_bytes, sizeof(uint8_t), &ui32_written_bytes );// Write the type of the metadata block
        ui32_bytes_to_write = AUX_FUNCS_lstrlen(ui8_dpack_songs_info_buffer,MAX_ROM_SONGS_INFO_BUFFER);
		ui32_bytes_to_write++;// +1 for the '\0'		
		i16_ret_val = FILE_SYS_file_write(&ui32_bytes_to_write, sizeof(uint32_t), &ui32_written_bytes);// Write the size of the metadata block
        i16_ret_val = FILE_SYS_file_write(ui8_dpack_songs_info_buffer, ui32_bytes_to_write, &ui32_written_bytes);// Write it to the destination file	
		
		// write a default SONGS ROM METADATA block
        ui8_file_bytes[0]=FILE_DATA_BLOCK_SONGS_ROM;
        i16_ret_val = FILE_SYS_file_write(ui8_file_bytes, sizeof(uint8_t), &ui32_written_bytes );// Write the type of the metadata block
		ui32_bytes_to_write = i32_dpack_dumper_rom_size;
		if ( ui32_bytes_to_write>=MAX_ROM_NIBBLES_BUFFER) ui32_bytes_to_write = MAX_ROM_NIBBLES_BUFFER;
        i16_ret_val = FILE_SYS_file_write(&ui32_bytes_to_write, sizeof(uint32_t), &ui32_written_bytes);// Write the size of the metadata block

		// write ROM data in the ROM METADATA block. Write the content of the ROM memory buffer in RAM ( ui8_dpack_dumper_nibbles_buffer ) to file
		ui32_total_written_bytes = 0;
		do{

			ui32_written_bytes = 0;
			pui8_aux = &(ui8_dpack_dumper_nibbles_buffer[ui32_total_written_bytes]);		
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
	int16_t i16_ret_val=0;
	uint32_t ui32_bytes_to_read;
	uint32_t ui32_read_bytes;
	uint32_t ui32_data_block_read_bytes;
	uint32_t ui32_data_block_size = 0;
	uint8_t  ui8_version_bytes[ROMPACK_FILE_TAG_N_VERSION_SIZE];
	uint8_t  ui8_read_data[ROMPACK_FILE_RW_DEFAULT_N_BYTES];	
	uint8_t  crc_byte_0 = 0;
	uint8_t  crc_byte_1 = 0;
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
				
			case FILE_DATA_BLOCK_TITLE:				
				// get the size of the TITLE DATA_BLOCK
				i16_ret_val = USART_1KXmodem_read(&ui32_data_block_size, sizeof(uint32_t), &ui32_read_bytes);
								
				// get the TITLE bytes
				memset(ui8_dpack_title_buffer,0,sizeof(ui8_dpack_title_buffer));
				ui32_data_block_read_bytes =0 ;				
				while ( (i16_ret_val>=0) && (ui32_data_block_read_bytes<ui32_data_block_size) ){
				    
					// calculate the number of bytes of next data chunk to read to avoid reading/writting beyond the buffers limits
					if ( (ui32_data_block_read_bytes + ROMPACK_FILE_RW_DEFAULT_N_BYTES)>=ui32_data_block_size) {
						ui32_bytes_to_read = ui32_data_block_size - ui32_data_block_read_bytes;
					}else{
						ui32_bytes_to_read = ROMPACK_FILE_RW_DEFAULT_N_BYTES;
					}

					// read next block of TITLE bytes from 1KXmodem connection
					memset(ui8_read_data,0,sizeof(ui8_read_data));
					ui32_read_bytes = 0;
					i16_ret_val = USART_1KXmodem_read(ui8_read_data, ui32_bytes_to_read,  &ui32_read_bytes);
					// store data read from 1KXmodem connection in the TITLE buffer taking care of not writing beyond the buffer limits
					ui16_aux=0;
					while ( ((ui32_data_block_read_bytes+ui16_aux)<MAX_ROM_TITLE_BUFFER) && (ui16_aux<ui32_read_bytes) ){
						pui8_aux = &(ui8_dpack_title_buffer[ui32_data_block_read_bytes + ui16_aux]);
						(*pui8_aux)=ui8_read_data[ui16_aux];
						ui16_aux++;
					}
					// update the amount of block data read from disk
					ui32_data_block_read_bytes = ui32_data_block_read_bytes + ui32_read_bytes;
				}//while
				break;
					
			case FILE_DATA_BLOCK_SONGS_INFO:	
				// get the size of the SONGS_INFO DATA_BLOCK
				i16_ret_val = USART_1KXmodem_read(&ui32_data_block_size, sizeof(uint32_t), &ui32_read_bytes);

				if (ui32_data_block_size>MAX_ROM_SONGS_INFO_BUFFER) ui32_data_block_size = MAX_ROM_SONGS_INFO_BUFFER;
				
				// get the SONGS_INFO bytes
				memset(ui8_dpack_songs_info_buffer,0,sizeof(ui8_dpack_songs_info_buffer));				
				ui32_data_block_read_bytes =0 ;
				while ( (i16_ret_val>=0) && (ui32_data_block_read_bytes<ui32_data_block_size) ){
					
					// calculate the number of bytes of next data chunk to read to avoid writing beyond the buffer limit					
					if ( (ui32_data_block_read_bytes + ROMPACK_FILE_RW_DEFAULT_N_BYTES)>=ui32_data_block_size) {
						ui32_bytes_to_read = ui32_data_block_size - ui32_data_block_read_bytes;
					}else{
						ui32_bytes_to_read = ROMPACK_FILE_RW_DEFAULT_N_BYTES;
					}

					// read next block of SONG INFO bytes bytes from 1KXmodem connection
					memset(ui8_read_data,0,sizeof(ui8_read_data));
					ui32_read_bytes = 0;
					i16_ret_val = USART_1KXmodem_read(ui8_read_data, ui32_bytes_to_read,  &ui32_read_bytes);
					// store data read from 1KXmodem connection in the SONG INF buffer taking care of not writing beyond the buffer limits
					ui16_aux=0;
					while ( ((ui32_data_block_read_bytes+ui16_aux)<MAX_ROM_SONGS_INFO_BUFFER) && (ui16_aux<ui32_read_bytes) ){
						pui8_aux = &(ui8_dpack_songs_info_buffer[ui32_data_block_read_bytes + ui16_aux]);
						(*pui8_aux)=ui8_read_data[ui16_aux];
						ui16_aux++;
					}
					// update the amount of block data read from 1KXmodem connection
					ui32_data_block_read_bytes = ui32_data_block_read_bytes + ui32_read_bytes;

				}//while				
				break;
					
			case FILE_DATA_BLOCK_SONGS_ROM:
				// get the size of the SONGS_ROM DATA_BLOCK
				i16_ret_val = USART_1KXmodem_read(&ui32_data_block_size, sizeof(uint32_t), &ui32_read_bytes);

				if (ui32_data_block_size>MAX_ROM_NIBBLES_BUFFER) ui32_data_block_size = MAX_ROM_NIBBLES_BUFFER;

				// read ROM data nibbles until having read all them or until have filled the ROM memory buffer
				memset(ui8_dpack_dumper_nibbles_buffer,0,sizeof(ui8_dpack_dumper_nibbles_buffer));
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
						pui8_aux = &(ui8_dpack_dumper_nibbles_buffer[ui32_data_block_read_bytes + ui16_aux]);
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
		ui8_dpack_dumper_buffer_initialized = FALSE; // clear the flag that indicates if the ui8_dpack_dumper_buffer contains valid ROM data
	}else{	
		ui8_dpack_dumper_buffer_initialized = TRUE;// set the flag that indicates that the ui8_dpack_dumper_buffer contains valid ROM data
		i32_dpack_dumper_rom_size = DPACK_CTRL_get_size_rom_in_buffer();
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


  if (ui8_dpack_dumper_buffer_initialized == FALSE){
	  
	  i16_ret_val = USART_1KX_NOT_INITIALIZED;
	  
  }//if

  if (i16_ret_val>=0){
	  
	  // write ROMPACK format and version identification
	  AUX_FUNCS_lstrcpy(ui8_file_bytes,ROMPACK_FILE_TAG_N_VERSION,ROMPACK_FILE_RW_DEFAULT_N_BYTES);
	  ui32_bytes_to_write = AUX_FUNCS_lstrlen(ui8_file_bytes,ROMPACK_FILE_RW_DEFAULT_N_BYTES);
	  ui32_bytes_to_write++;// +1 for the '\0'
	  i16_ret_val =  USART_1KXmodem_write(ui8_file_bytes, ui32_bytes_to_write, &ui32_written_bytes);// Write it to the destination file
	  
	  // write a default TITLE METADATA block
	  ui8_file_bytes[0]=FILE_DATA_BLOCK_TITLE;
	  i16_ret_val =  USART_1KXmodem_write(ui8_file_bytes, sizeof(uint8_t), &ui32_written_bytes);//  Write the type of the metadata block	  
	  ui32_bytes_to_write = AUX_FUNCS_lstrlen(ui8_dpack_title_buffer,MAX_ROM_TITLE_BUFFER);
	  ui32_bytes_to_write++;// +1 for the '\0'
	  i16_ret_val = USART_1KXmodem_write(&ui32_bytes_to_write, sizeof(uint32_t), &ui32_written_bytes);// Write the size of the metadata block
	  i16_ret_val = USART_1KXmodem_write(ui8_dpack_title_buffer, ui32_bytes_to_write, &ui32_written_bytes);// Write it to remote computer
	  
	  // write a default  SONGS INFO METADATA block
	  ui8_file_bytes[0]=FILE_DATA_BLOCK_SONGS_INFO;
	  i16_ret_val =  USART_1KXmodem_write(ui8_file_bytes, sizeof(uint8_t), &ui32_written_bytes);//  Write the type of the metadata block	
	  ui32_bytes_to_write = AUX_FUNCS_lstrlen(ui8_dpack_songs_info_buffer,MAX_ROM_SONGS_INFO_BUFFER);
	  ui32_bytes_to_write++;// +1 for the '\0'
	  i16_ret_val = USART_1KXmodem_write(&ui32_bytes_to_write, sizeof(uint32_t), &ui32_written_bytes);// Write the size of the metadata block
	  i16_ret_val = USART_1KXmodem_write(ui8_dpack_songs_info_buffer, ui32_bytes_to_write, &ui32_written_bytes);// Write it to remote computer
  
	  // write a default SONGS ROM METADATA block
	  ui8_file_bytes[0]=FILE_DATA_BLOCK_SONGS_ROM;
	  i16_ret_val =  USART_1KXmodem_write(ui8_file_bytes, sizeof(uint8_t), &ui32_written_bytes);//  Write the type of the metadata block	
	  ui32_bytes_to_write = i32_dpack_dumper_rom_size;
	  if ( ui32_bytes_to_write>=MAX_ROM_NIBBLES_BUFFER) ui32_bytes_to_write = MAX_ROM_NIBBLES_BUFFER;	  
	  
	  i16_ret_val = USART_1KXmodem_write(&ui32_bytes_to_write, sizeof(uint32_t), &ui32_written_bytes);// Write the size of the metadata block

	  // write ROM data in the ROM METADATA block. Write the content of the ROM memory buffer in RAM ( ui8_dpack_dumper_nibbles_buffer ) to remote computer
	  ui32_total_written_bytes = 0;
	  do{

		  ui32_written_bytes = 0;
		  pui8_aux = &(ui8_dpack_dumper_nibbles_buffer[ui32_total_written_bytes]);
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