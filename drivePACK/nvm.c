// *****************************************************************
// ****                       drivePACK                         ****
// ****                  www.tolaemon.com/dpack                 ****
// ****                       Source code                       ****
// ****                       21/04/2023                        ****
// ****                     Jordi Bartolome                     ****
// ****                                                         ****
// **** IMPORTANT:                                              ****
// **** Using this code or any part of it means accepting all   ****
// **** conditions exposed in: http://www.tolaemon.com/dpack    ****
// *****************************************************************

#include "nvm.h"

// structure with the general information of the internal flash organization
struct{
	uint32_t ui32_num_pages;
	uint32_t ui32_flash_size; // NVM main address space flash size
	uint32_t ui32_region_size; // NVM region size in bytes
	uint32_t ui32_block_size; // NVM block size in bytes
	uint32_t ui32_page_size; // NVM page size in bytes
}NVM_info;

// structure with the information of current hex file
struct{
	uint16_t  ui16_addr;// most recent 16 bit address value in the '@@@@' of the last read data record header ':LL@@@@TT' 
	uint32_t  ui32_ext_seg_addr;// most recent extended segment address value
	uint32_t  ui32_ext_lin_addr;// most recent extended linear address value
	uint8_t   ui8_last_rd_byte;// keeps the last successfully read byte
	uint32_t  ui32_last_rd_byte_abs_addr;// keeps the absolute address of last successfully read byte value. Absolute address corresponds to the combination of 16 bit addr, the extended segment address and the extended linear address, and the offset in the DATA record
	uint32_t  ui32_curr_abs_addr;// keeps the address the file reader cursor is pointing to, but has not been read yet. The address indicated in this field is the address that will be read on ReadNextByte() call. Absolute address corresponds to the combination of the 16 bit addr, the extended segment address, the extended linear address, and the offset in the DATA record.
	uint8_t   ui8_curr_rec_type;// type of the currently processed hex file record
	uint8_t   ui8_curr_rec_byte_count;// number of bytes of the current processed hex file record
	uint16_t  ui16_curr_rec_byte_idx;// keeps the index of the current pair of payload bytes ( each byte corresponds to a pair of hex digits ). Payload includes the data bytes but also the checksum and the end of line. (\r\n)
	uint8_t   ui8_curr_rec_checksum;// to accumulate the current record checksum value
	uint32_t  ui32_parsed_records;// the number of hex file parsed records
}NVM_hex_data;



// JBR 2023-05-01 Borra puesto para depurar
uint8_t USART_send_char(uint8_t ui8_to_send){
	uint8_t ret_val = TRUE;
	
	
	while (SERCOM3->USART.INTFLAG.bit.DRE != 1 ){}

	// SERCOM3->USART.INTENSET.bit.TXC == 1;
	SERCOM3->USART.DATA.bit.DATA = ui8_to_send;
	ui8_to_send = 0;
	
	return ret_val;
	
}//USART_send_char



uint8_t USART_send_string(uint8_t * ui8_to_string, uint16_t ush_MAX_LEN ){
	uint16_t ui16_ch_counter;
	uint8_t ret_val = TRUE;

	
	ui16_ch_counter = 0;
	while ( (ui16_ch_counter<ush_MAX_LEN)&&(ui8_to_string[ui16_ch_counter]!='\0') ){
		
		if (SERCOM3->USART.INTFLAG.bit.DRE == 1) {
			SERCOM3->USART.DATA.bit.DATA = ui8_to_string[ui16_ch_counter];
			ui16_ch_counter++;
		}//if
		
	}//while
	
	if (ui16_ch_counter>=ush_MAX_LEN) ret_val = FALSE;
	
	return ret_val;
	
}//USART_send_string
// FIN JBR 2023-05-01 Borra puesto para depurar




 void NVM_Init(){
    uint8_t ui8_aux = 0;	 

       // clear the NVM_info structure
       memset(&NVM_info,0,sizeof(NVM_info));

       // clear the hex file state structure
       memset(&NVM_hex_data,0,sizeof(NVM_hex_data));
	     
       NVMCTRL->CTRLA.bit.AUTOWS = 1;// automatic wait state configuration
       
	   // get the number of pages in the NVM main address space
       NVM_info.ui32_num_pages = (uint32_t)NVMCTRL->PARAM.bit.NVMP;
       
	   // Page size:
	   // PSZ bits   Size ( * Not all device families will provide all the page sizes indicated in the table. )
	   // 0x00       8 bytes
	   // 0x01      16 bytes
	   // 0x02      32 bytes
	   // 0x03      64 bytes
	   // 0x04     128 bytes
	   // 0x05     256 bytes
	   // 0x06     512 bytes
	   // 0x07    1024 bytes
	   ui8_aux = (uint8_t)NVMCTRL->PARAM.bit.PSZ;
	   switch (ui8_aux){
		   case 0x0:
		        NVM_info.ui32_page_size = 8;
				break;
		   case 0x1:
		        NVM_info.ui32_page_size = 16;
				break;				
		   case 0x2:
		        NVM_info.ui32_page_size = 32;
                break;
		   case 0x3:
		        NVM_info.ui32_page_size = 64;
				break;
		   case 0x4:
		        NVM_info.ui32_page_size = 128;
				break;				
		   case 0x5:
		        NVM_info.ui32_page_size = 256;
				break;				
		   case 0x6: 
		        NVM_info.ui32_page_size = 512;
				break;								
		   case 0x7:
		        NVM_info.ui32_page_size = 1024;
				break;
	   }//switch

       // cacluate the flash size
       NVM_info.ui32_flash_size = NVM_info.ui32_num_pages*NVM_info.ui32_page_size ; // NVM main address space flash size
	  
	   // calculate region size. According to data sheet:
	   // Models with 1024KB Flash   32Kb per region
	   // Models with  512KB Flash   16Kb per region
	   // Models with  256KB Flash    8Kb per region
      if ( NVM_info.ui32_flash_size>=1048576){
		 NVM_info.ui32_region_size = 32768;// bytes
	  }else if ( NVM_info.ui32_flash_size>=524288){
		 NVM_info.ui32_region_size = 16384; // bytes
	  }else if ( NVM_info.ui32_flash_size>=262144){
		 NVM_info.ui32_region_size = 8192;
	  }else{
		  NVM_info.ui32_region_size = 8192;
	  }//if

      NVM_info.ui32_block_size = 8192;// TODO pendiente de confirmar, escribir en AVR FREAKS

 }// NVM_Init

  

 uint32_t NVM_GetFlashSize(){
 
     return NVM_info.ui32_flash_size;
 
 }//NVM_GetFlashSize



 void NVM_ReadFromFlash(uint32_t *data, uint32_t ui32_n_dw_to_read, const uint32_t address ){
	 
    uint32_t *paddress = (uint32_t*)address;
    (void)memcpy(data, paddress, ui32_n_dw_to_read);

 }// NVM_ReadFromFlash
 
 
 
 int8_t NVM_WriteInPageBuffer( uint32_t *data, uint32_t ui32_n_dw_to_write, const uint32_t ui32_address){
	 int8_t i8_ret_val = 0;
	 uint32_t i = 0U;
	 uint32_t * paddress = (uint32_t *)ui32_address;
     uint32_t ui32_n_dw_page = NVM_info.ui32_page_size / 4; // convert the number of bytes to words

     // in case the number of ui32_n_dw_to_write is greater than the page size then set the number
	 // of values to write to the page size, if not write only the number of specified number of dw
     if (ui32_n_dw_to_write>ui32_n_dw_page) ui32_n_dw_to_write = ui32_n_dw_page;

	 // writing 32-bit data into the given address
	 for (i = 0U; i < ui32_n_dw_to_write; i++){
		 *paddress = *(data + i);
		 paddress++;
	 }//for

	return i8_ret_val;

}//NVM_WriteInPageBuffer



int8_t NVM_WritePageBufferToFlash(const uint32_t ui32_address){
    int8_t i8_ret_val = 0;

    // set address
    NVMCTRL->ADDR.reg = ui32_address;
	// set command code and execution code.  The key value must be written at the same time as CMD.
	NVMCTRL->CTRLB.reg = NVM_CMD_WP|NVMCTR_CMD_EXEC;	

    // STATUS.READY is cleared when a command is issued and set when it has completed, so wait until NVM 
	// controller has finished saving the Page Buffer content to the corresponding flash NVM Page	
	while ( (NVMCTRL->STATUS.bit.READY==0) && (NVMCTRL->INTFLAG.bit.ADDRE!=1) && (NVMCTRL->INTFLAG.bit.PROGE!=1)) {};
	
	if ((NVMCTRL->INTFLAG.bit.ADDRE==1)||(NVMCTRL->INTFLAG.bit.PROGE==1)){
		i8_ret_val = -1;
	}
	
	return i8_ret_val;	
	
}//NVM_WritePageBufferToFlash



int8_t NVM_RegionUnlock(const uint32_t ui32_address){
	int8_t i8_ret_val = 0;
    uint8_t ui8_unlock_bit_n = 0;
	uint32_t ui32_unlock_reg_mask = 0;
	uint32_t ui32_unlock_reg_val = 0;


    // get the mask of the bit that corresponds to the region where the received address belongs to		
	ui8_unlock_bit_n = (uint8_t) ( ui32_address / NVM_info.ui32_region_size );
    ui32_unlock_reg_mask = 0x1<<ui8_unlock_bit_n;
	
	// Get the bits of RUNLOCK register and compare them with the obtained mask to check
	// if the region where the received address belongs to, is aleady unlocked or not:
	//  0: The corresponding region is locked.
	//  1: The corresponding region is not locked.
	ui32_unlock_reg_val = NVMCTRL->RUNLOCK.reg;
	if ((ui32_unlock_reg_mask&&ui32_unlock_reg_val)==0){
		
		// corresponding region bit is 0 so the region where the received address belongs to is locked. Execute the actions to unlock it
		
    	NVMCTRL->INTFLAG.bit.PROGE = 0;
 
		// set address
		NVMCTRL->ADDR.reg = ui32_address;
		// set command code and execution code.  The CMD_EX value must be written at the same time as CMD.
		NVMCTRL->CTRLB.reg = NVM_CMD_UR|NVMCTR_CMD_EXEC;

		// wait until NVM controller has finished executing the received command
		while ( (NVMCTRL->STATUS.bit.READY==0) && (NVMCTRL->INTFLAG.bit.ADDRE!=1) && (NVMCTRL->INTFLAG.bit.PROGE!=1)) {};
		
	    if ((NVMCTRL->INTFLAG.bit.ADDRE==1)||(NVMCTRL->INTFLAG.bit.PROGE==1)){
			i8_ret_val = -1;
		}
	
	}//if

	return i8_ret_val;
		
}//NVM_RegionUnlock



int8_t NVM_EraseFlashBlock(const uint32_t ui32_address){
	int8_t i8_ret_val = 0;

    NVMCTRL->INTFLAG.bit.PROGE = 0;

	// set address
	NVMCTRL->ADDR.reg = ui32_address;
	// set command code and execution code.  The CMD_EX value must be written at the same time as CMD.
	NVMCTRL->CTRLB.reg = NVM_CMD_EB|NVMCTR_CMD_EXEC;

	// wait until NVM controller has finished executing the received command ( The key value must be written at the same time as CMD. )
	while ( (NVMCTRL->STATUS.bit.READY==0) && (NVMCTRL->INTFLAG.bit.ADDRE!=1) && (NVMCTRL->INTFLAG.bit.PROGE!=1)) {};
		
	if ((NVMCTRL->INTFLAG.bit.ADDRE==1)||(NVMCTRL->INTFLAG.bit.PROGE==1)){
		i8_ret_val = -1;
	}
	
	return i8_ret_val;
	
}//NVM_EraseFlashBlock



int8_t NVM_UnlockFlashFromAddressToEnd(const uint32_t ui32_address){
	int8_t i8_ret_val = 0;
	uint32_t ui32_region_base_address = 0;
	uint16_t ui16_aux = 0;
	
	// get the region base address of the received address
	ui32_region_base_address = (uint32_t)(ui32_address/NVM_info.ui32_region_size);
	
	// unlock region by region until the end, starting from the region of received address
	while (ui32_region_base_address<NVM_info.ui32_flash_size){
		
		NVM_RegionUnlock(ui32_region_base_address);
		// move to next block
		ui32_region_base_address = ui32_region_base_address + NVM_info.ui32_region_size;
	}
	
	return i8_ret_val;
	
}//NVM_UnlockFlashFromAddressToEnd



int8_t NVM_EraseFlashFromAddressToEnd(const uint32_t ui32_address){
	int8_t i8_ret_val = 0;
    uint32_t ui32_block_base_address = 0;
	uint16_t ui16_aux = 0;
	
	// get the block base address of the received address
	ui32_block_base_address = (uint32_t)(ui32_address/NVM_info.ui32_block_size)*NVM_info.ui32_block_size;
	
	// erase flash block by block until the end, starting from the block of received address
	while (ui32_block_base_address<NVM_info.ui32_flash_size){
	
	   NVM_EraseFlashBlock(ui32_block_base_address);
	   // move to next block
	   ui32_block_base_address = ui32_block_base_address + NVM_info.ui32_block_size;
	}
		
	return i8_ret_val;
	
}//NVM_EraseFlashFromAddressToEnd



int16_t NVM_HEXFileOpen(uint8_t * pui8_file_name ){
	int16_t i16_ret_val=0;

    // initialize the different hex file internal variables
	NVM_hex_data.ui16_addr = 0;
	NVM_hex_data.ui32_ext_seg_addr = 0;	
	NVM_hex_data.ui32_ext_lin_addr = 0;
	NVM_hex_data.ui8_last_rd_byte = 0;
	NVM_hex_data.ui32_last_rd_byte_abs_addr = NVMCTRL_INVALID_32BIT_ADDR;
	NVM_hex_data.ui8_curr_rec_byte_count = 0;
	NVM_hex_data.ui8_curr_rec_type = NVM_HEX_REC_TY_NONE;
	NVM_hex_data.ui16_curr_rec_byte_idx = 0;
	NVM_hex_data.ui8_curr_rec_checksum = 0;
	NVM_hex_data.ui32_parsed_records = 0;
		
	i16_ret_val = FILE_SYS_file_open((TCHAR*)pui8_file_name, FA_READ);
	if (i16_ret_val<0) {
		
		i16_ret_val = -1;//File open error.
		
	}//if

	return i16_ret_val;
	
}//NVM_HEXFileOpen



int16_t NVM_HEXFileClose(){
	int16_t i16_ret_val=0;
	
	i16_ret_val = FILE_SYS_file_close();
	
	
	return i16_ret_val;
	
}//NVM_HEXFileClose



// >=0 the last address of the hex file has been found and returned by reference
// <0 the last address of the hex file could not be reached
int16_t NVM_HEXGetHexLastAddress(uint32_t * pui32_lat_abs_addr){
	int16_t i16_ret_val=0;
	uint32_t ui32_address_to_seek = 0;


    (*pui32_lat_abs_addr)=0;

    // set the largest possible address value to force parsing the complete hex file until the end of file is found
    ui32_address_to_seek = NVMCTRL_INVALID_32BIT_ADDR;
    
	i16_ret_val = NVM_HEXSeekToByteAddr(ui32_address_to_seek);
	if (i16_ret_val==-2){
		// END OF FILE has been found so return the last read address
		(*pui32_lat_abs_addr)=NVM_hex_data.ui32_last_rd_byte_abs_addr;
		i16_ret_val = 0;
	}else{
		// the end of file has not been reached so something has failed
		i16_ret_val==-1;
	}//if
	
	return i16_ret_val;

}//NVM_HEXGetHexLastAddress



// -1 despite requested address is in the hex file address range there is no value definded for that address
// -2 reached end of file, requested address is out of hex range
int16_t NVM_HEXSeekToByteAddr(uint32_t ui32_abs_addr_to_seek){
	int16_t i16_ret_val=0;
	uint8_t  ui8arr_read_bytes[10];
	uint32_t ui32_n_read_bytes = 0;
    uint32_t ui32_addr_field=0;
    uint8_t  ui8arr_byte_string[9];
    uint32_t ui32_aux=0;


	// if the received address is before the NVM_hex_data.ui32_curr_abs_addr then rewind the file
	// reader to set the NVM_hex_data.ui32_curr_addr to 0x0000 to start looking for the received 
	// address from the begining of the hex file. 
	if (ui32_abs_addr_to_seek<NVM_hex_data.ui32_curr_abs_addr){
		 
		// rewind the file reader cursor to the begining of the file
		i16_ret_val = FILE_SYS_file_lseek(0);
		 
		// the file reader has been reset so also reset the current hex record data info
		NVM_hex_data.ui16_addr = 0;
		NVM_hex_data.ui32_ext_seg_addr = 0;
		NVM_hex_data.ui32_ext_lin_addr = 0;
		NVM_hex_data.ui8_last_rd_byte = 0;
		NVM_hex_data.ui32_last_rd_byte_abs_addr = NVMCTRL_INVALID_32BIT_ADDR;
		NVM_hex_data.ui8_curr_rec_byte_count = 0;
		NVM_hex_data.ui8_curr_rec_type = NVM_HEX_REC_TY_NONE;
		NVM_hex_data.ui16_curr_rec_byte_idx = 0;
		NVM_hex_data.ui8_curr_rec_checksum = 0;	 
        NVM_hex_data.ui32_parsed_records = 0;

	}//if

    // keep parsing the records of the hex file until the received ui32_addr_to_seek address has been reached
    i16_ret_val=0;
	while ( (i16_ret_val>=0) && (NVM_hex_data.ui32_curr_abs_addr!=ui32_abs_addr_to_seek) ){

        if ( (i16_ret_val>=0) && (FILE_SYS_file_eof()<0) ){
			// the end of file has been reached
			i16_ret_val = -2;			
		}

		ui32_n_read_bytes = 0;
		if ( (i16_ret_val>=0) && (FILE_SYS_file_read(ui8arr_read_bytes, 1,  &ui32_n_read_bytes)>0) ){
					
			if (ui8arr_read_bytes[0]==NVM_HEX_REC_START_MARK){
				    
				// PROCESS RECORD HEADER bytes : process the 8 bytes of the hex record header
					
				// HEX file record fields:
				//:LL@@@@TTDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDCC
				// ':':record start
				// 'LL':byte count
				// '@@@@':byte count
				// 'TT':record type
				// 'DDD..D': data type
				// 'CC': checksum
					
				// new record starts, so initialize the record variables
				NVM_hex_data.ui8_curr_rec_byte_count = 0;
				NVM_hex_data.ui8_curr_rec_type = NVM_HEX_REC_TY_NONE;
				NVM_hex_data.ui16_curr_rec_byte_idx = 0;
				NVM_hex_data.ui8_curr_rec_checksum = 0;
				NVM_hex_data.ui32_parsed_records++;
				
				// get the record header LL@@@@TT bytes:
				i16_ret_val=FILE_SYS_file_read(ui8arr_read_bytes, 8,  &ui32_n_read_bytes);				   
				if (i16_ret_val>0){
					// if requested record start bytes have been successfully read then initialize
					// the current hex record information. The '-0x30 is to convert each byte to 
				
					// ui8_curr_rec_byte_count: get value from the string with the hex representation
					ui8arr_byte_string[0] = ui8arr_read_bytes[0];
					ui8arr_byte_string[1] = ui8arr_read_bytes[1];
					ui8arr_byte_string[2] ='\0';
					AUX_FUNCS_atoi(ui8arr_byte_string, 16, 9,&ui32_aux);
					NVM_hex_data.ui8_curr_rec_byte_count = (uint8_t)ui32_aux;
						
					// ui32_curr_addr: get value from the string with the hex representation
					ui8arr_byte_string[0] = ui8arr_read_bytes[2];
					ui8arr_byte_string[1] = ui8arr_read_bytes[3];
					ui8arr_byte_string[2] = ui8arr_read_bytes[4];
					ui8arr_byte_string[3] = ui8arr_read_bytes[5];
					ui8arr_byte_string[4] = '\0';						
					AUX_FUNCS_atoi(ui8arr_byte_string, 16, 5,&ui32_addr_field);

					// ui8_curr_rec_type: get value from the string with the hex representation
					ui8arr_byte_string[0] = ui8arr_read_bytes[6];
					ui8arr_byte_string[1] = ui8arr_read_bytes[7];
					ui8arr_byte_string[2] ='\0';
					AUX_FUNCS_atoi(ui8arr_byte_string, 16, 9,&ui32_aux);
					NVM_hex_data.ui8_curr_rec_type = (uint8_t)ui32_aux;
					
                    // the 4 address data bytes ( @@@@ of ':LL@@@@TT') may be used differently depending on the record type
					switch (NVM_hex_data.ui8_curr_rec_type){				     
                         case NVM_HEX_REC_TY_DATA:
						    // in the DATA records the 4 address bytes correspond to the 16bit data address bytes. To calculate
							// the absolute address need to combine the extended segment address and the extended linear address
							// with the read data address.						    
							NVM_hex_data.ui16_addr = ui32_addr_field;
							
							// update in the abs addr the bits that correspond to the 16bit address record
							NVM_hex_data.ui32_curr_abs_addr = (NVM_hex_data.ui32_curr_abs_addr & 0xFFFF0000) | NVM_hex_data.ui16_addr;
                          
						    // check if current processed address is beyond the requested address to seek
                            if (NVM_hex_data.ui32_curr_abs_addr>ui32_abs_addr_to_seek){
							   i16_ret_val = -1;
						    }
						    break;							
                         case NVM_HEX_REC_TY_EOF:
                            // the end of the hex file has been reached
                            i16_ret_val = -2;						 
                            break;
                         case NVM_HEX_REC_TY_EXT_SEG_ADDR:
						    // in the EXTENDED SEGMENT ADDRESS records the 4 address bytes of the 16bit address are always 0
							NVM_hex_data.ui16_addr = ui32_addr_field;
                            break;
                         case NVM_HEX_REC_TY_START_SEG_ADDR:
						    // in the START SEGMENT ADDRESS records the 4 address bytes are not used
                            break;
                         case NVM_HEX_REC_TY_EXT_LIN_ADDR:
						    // in the EXTENDED LIENAR ADDRESS records the 4 address bytes of the 16bit address are always 0
							NVM_hex_data.ui16_addr = ui32_addr_field;
                            break;
                         case NVM_HEX_REC_TY_START_LIN_ADDR:
						    // in the START LINEAR ADDRESS records the 4 address bytes are not used
                            break;
                         default:
                            break;							
					}//switch
                        
					// reset the record data index
					NVM_hex_data.ui16_curr_rec_byte_idx = 0;
						
				}//if (i16_ret_val>0){
				
			}else{

				// PROCESS RECORD PAYLOAD bytes: process the record payload bytes ( data, checksum end of line...). E

				// check if that PAYLOAD read byte corresponds to DATA, to CHECKSUM or to end lines symbols
				if (NVM_hex_data.ui16_curr_rec_byte_idx<NVM_hex_data.ui8_curr_rec_byte_count){
	
					// process record data bytes in different way depending on the record type
					if (NVM_hex_data.ui8_curr_rec_type==NVM_HEX_REC_TY_DATA){
				
						// read the following file byte that completes the hex representation of the current DATA 
						// byte and then convert that hex representation to the corresponding numerical value
						ui8arr_byte_string[0] = ui8arr_read_bytes[0];
						i16_ret_val=FILE_SYS_file_read(ui8arr_read_bytes, 1,  &ui32_n_read_bytes);
						ui8arr_byte_string[1] = ui8arr_read_bytes[0];// the [0] is because FILE_SYS_file_read starts filling from idx 0!
						ui8arr_byte_string[2] ='\0';
						AUX_FUNCS_atoi(ui8arr_byte_string, 16, 9,&ui32_aux);				
																	    
                        // keep the value and address of the read byte and increase the absolute address counter 
				        NVM_hex_data.ui8_last_rd_byte = (uint8_t)ui32_aux;
				        NVM_hex_data.ui32_last_rd_byte_abs_addr = NVM_hex_data.ui32_curr_abs_addr;
						NVM_hex_data.ui32_curr_abs_addr++;
						
						// increase record payload byte index ( DATA, CHECKSUM or end lines )
						NVM_hex_data.ui16_curr_rec_byte_idx++;						
						
					}else if (NVM_hex_data.ui8_curr_rec_type==NVM_HEX_REC_TY_EXT_SEG_ADDR){
						
						// read the following 3 bytes that complete the hex representation of the 2 data
						// bytes of the extended segment address record
						ui8arr_byte_string[0] = ui8arr_read_bytes[0];
						i16_ret_val=FILE_SYS_file_read(ui8arr_read_bytes, 3,  &ui32_n_read_bytes);
						ui8arr_byte_string[1] = ui8arr_read_bytes[0];
						ui8arr_byte_string[2] = ui8arr_read_bytes[1];
						ui8arr_byte_string[3] = ui8arr_read_bytes[2];
						ui8arr_byte_string[4] = '\0';
						AUX_FUNCS_atoi(ui8arr_byte_string, 16, 9,&ui32_aux);

						NVM_hex_data.ui32_ext_seg_addr = ui32_aux;
						
						// add the extended segment address to the current absolute address. "...The extended segment address data field
						// contains a 16-bit segment base address. This is multiplied by 16 ( 4bits shift ) and added to each
						// subsequent data  record address to form the address for the data. This allows addressing up to one
						// megabyte (1048576 bytes) of address space..."
                        NVM_hex_data.ui32_curr_abs_addr = (NVM_hex_data.ui32_ext_seg_addr<<4)+NVM_hex_data.ui16_addr;
						
						// increase record payload byte index ( DATA, CHECKSUM or end lines )
						NVM_hex_data.ui16_curr_rec_byte_idx=NVM_hex_data.ui16_curr_rec_byte_idx+2;
											
					}else if (NVM_hex_data.ui8_curr_rec_type==NVM_HEX_REC_TY_EXT_LIN_ADDR){
							
						// read the following 3 bytes that complete the hex representation of the 2 data
						// bytes of the extended linear address record
						ui8arr_byte_string[0] = ui8arr_read_bytes[0];
						i16_ret_val=FILE_SYS_file_read(ui8arr_read_bytes, 3,  &ui32_n_read_bytes);
						ui8arr_byte_string[1] = ui8arr_read_bytes[0];
						ui8arr_byte_string[2] = ui8arr_read_bytes[1];
						ui8arr_byte_string[3] = ui8arr_read_bytes[2];
						ui8arr_byte_string[4] = '\0';
						AUX_FUNCS_atoi(ui8arr_byte_string, 16, 9,&ui32_aux);
												
						NVM_hex_data.ui32_ext_lin_addr = ui32_aux;
						
						//  add the extended linear address to the current absolute address. "...The extended linear address allows for
						// 32 bit addressing (up to 4 GiB). The byte count is always 02 and the address field is ignored (typically 0000).
						// The two data bytes (big endian) specify the upper 16 bits of the 32 bit absolute address for all subsequent
						// type 00 records; these upper address bits apply until the next 04 record. The absolute address for a type 00
						// record is formed by combining the upper 16 address bits of the most recent 04 record with the low 16 address
						// bits of the 00 record. If a type 00 record is not preceded by any type 04 records then its upper 16 address
						// bits default to 0000..."
                        NVM_hex_data.ui32_curr_abs_addr = (NVM_hex_data.ui32_ext_lin_addr<<16)|NVM_hex_data.ui16_addr;
						
						// increase record payload byte index ( DATA, CHECKSUM or end lines )
						NVM_hex_data.ui16_curr_rec_byte_idx=NVM_hex_data.ui16_curr_rec_byte_idx+2;
			
					//}else{
					// other record types are not supported and data bytes are ignored					
					}//if
															
				}else{
					
					// CHECKSUM on end line bytes : process the bytes of the checksum					
					// TODO: calculate checksum ....

				}//if
				
			}//if (ui8_read_bytes[0]==NVM_HEX_REC_START_MARK)
				
		}//if ( (i16_ret_val>=0) && (FILE_SYS_file_read(ui8arr_read_bytes, 1,  &ui32_n_read_bytes)>0) )
		
	}//while
	
	return i16_ret_val;

}//NVM_HEXSeekToByteAddr



int16_t NVM_HEXReadNextByte(uint8_t * pui8_read_bye,uint32_t * pui32_read_abs_address, uint32_t * pui32_curr_abs_address){
	int16_t i16_ret_val=0;
	uint8_t  ui8arr_read_bytes[10];
	uint32_t ui32_n_read_bytes = 0;
    uint32_t ui32_addr_field=0;
    uint8_t  ui8arr_byte_string[9];
    uint32_t ui32_aux=0;
    uint32_t ui32_abs_addr_to_read = 0;


    // the read address will be the one pointed by the current absolute address file cursor
    ui32_abs_addr_to_read = NVM_hex_data.ui32_curr_abs_addr;

    // keep parsing the records of the hex file until the next byte has been read: next byte to
	// read may be in the current record, but also in the following one, or maybe it is not present.
    i16_ret_val=0;
	while ( (i16_ret_val>=0) && (NVM_hex_data.ui32_last_rd_byte_abs_addr!=ui32_abs_addr_to_read) ){

        if ( (i16_ret_val>=0) && (FILE_SYS_file_eof()<0) ){
			// the end of file has been reached
			i16_ret_val = -2;			
		}

		ui32_n_read_bytes = 0;
		if ( (i16_ret_val>=0) && (FILE_SYS_file_read(ui8arr_read_bytes, 1,  &ui32_n_read_bytes)>0) ){
					
			if (ui8arr_read_bytes[0]==NVM_HEX_REC_START_MARK){
				    
				// PROCESS RECORD HEADER bytes : process the 8 bytes of the hex record header
					
				// HEX file record fields:
				//:LL@@@@TTDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDCC
				// ':':record start
				// 'LL':byte count
				// '@@@@':byte count
				// 'TT':record type
				// 'DDD..D': data type
				// 'CC': checksum
					
				// new record starts, so initialize the record variables
				NVM_hex_data.ui8_curr_rec_byte_count = 0;
				NVM_hex_data.ui8_curr_rec_type = NVM_HEX_REC_TY_NONE;
				NVM_hex_data.ui16_curr_rec_byte_idx = 0;
				NVM_hex_data.ui8_curr_rec_checksum = 0;
				NVM_hex_data.ui32_parsed_records++;
				
				// get the record header LL@@@@TT bytes:
				i16_ret_val=FILE_SYS_file_read(ui8arr_read_bytes, 8,  &ui32_n_read_bytes);				   
				if (i16_ret_val>0){
					// if requested record start bytes have been successfully read then initialize
					// the current hex record information. The '-0x30 is to convert each byte to 
				
					// ui8_curr_rec_byte_count: get value from the string with the hex representation
					ui8arr_byte_string[0] = ui8arr_read_bytes[0];
					ui8arr_byte_string[1] = ui8arr_read_bytes[1];
					ui8arr_byte_string[2] ='\0';
					AUX_FUNCS_atoi(ui8arr_byte_string, 16, 9,&ui32_aux);
					NVM_hex_data.ui8_curr_rec_byte_count = (uint8_t)ui32_aux;
						
					// ui32_curr_addr: get value from the string with the hex representation
					ui8arr_byte_string[0] = ui8arr_read_bytes[2];
					ui8arr_byte_string[1] = ui8arr_read_bytes[3];
					ui8arr_byte_string[2] = ui8arr_read_bytes[4];
					ui8arr_byte_string[3] = ui8arr_read_bytes[5];
					ui8arr_byte_string[4] = '\0';						
					AUX_FUNCS_atoi(ui8arr_byte_string, 16, 5,&ui32_addr_field);

					// ui8_curr_rec_type: get value from the string with the hex representation
					ui8arr_byte_string[0] = ui8arr_read_bytes[6];
					ui8arr_byte_string[1] = ui8arr_read_bytes[7];
					ui8arr_byte_string[2] ='\0';
					AUX_FUNCS_atoi(ui8arr_byte_string, 16, 9,&ui32_aux);
					NVM_hex_data.ui8_curr_rec_type = (uint8_t)ui32_aux;
					
                    // the 4 address data bytes ( @@@@ of ':LL@@@@TT') may be used differently depending on the record type
					switch (NVM_hex_data.ui8_curr_rec_type){				     
                         case NVM_HEX_REC_TY_DATA:
						    // in the DATA records the 4 address bytes correspond to the 16bit data address bytes. To calculate
							// the absolute address need to combine the extended segment address and the extended linear address
							// with the read data address.						    
							NVM_hex_data.ui16_addr = ui32_addr_field;
							
							// update in the abs addr the bits that correspond to the 16bit address record
							NVM_hex_data.ui32_curr_abs_addr = (NVM_hex_data.ui32_curr_abs_addr & 0xFFFF0000) | NVM_hex_data.ui16_addr;
                          
						    // check if current processed address is beyond the requested address to seek
                            if (NVM_hex_data.ui32_curr_abs_addr>ui32_abs_addr_to_read){
							   i16_ret_val = -1;
						    }
						    break;							
                         case NVM_HEX_REC_TY_EOF:
                            // the end of the hex file has been reached
                            i16_ret_val = -2;						 
                            break;
                         case NVM_HEX_REC_TY_EXT_SEG_ADDR:
						    // in the EXTENDED SEGMENT ADDRESS records the 4 address bytes of the 16bit address are always 0
							NVM_hex_data.ui16_addr = ui32_addr_field;
                            break;
                         case NVM_HEX_REC_TY_START_SEG_ADDR:
						    // in the START SEGMENT ADDRESS records the 4 address bytes are not used
                            break;
                         case NVM_HEX_REC_TY_EXT_LIN_ADDR:
						    // in the EXTENDED LIENAR ADDRESS records the 4 address bytes of the 16bit address are always 0
							NVM_hex_data.ui16_addr = ui32_addr_field;
                            break;
                         case NVM_HEX_REC_TY_START_LIN_ADDR:
						    // in the START LINEAR ADDRESS records the 4 address bytes are not used
                            break;
                         default:
                            break;							
					}//switch
                        
					// reset the record data index
					NVM_hex_data.ui16_curr_rec_byte_idx = 0;
						
				}//if (i16_ret_val>0){
				
			}else{

				// PROCESS RECORD PAYLOAD bytes: process the record payload bytes ( data, checksum end of line...). E
									
				// check if that PAYLOAD read byte corresponds to DATA, to CHECKSUM or to end lines symbols
                if (NVM_hex_data.ui16_curr_rec_byte_idx<NVM_hex_data.ui8_curr_rec_byte_count){
					
					// process record data bytes in different way deppending on the record type
                    if (NVM_hex_data.ui8_curr_rec_type==NVM_HEX_REC_TY_DATA){
				
						// read the following file byte that completes the hex representation of the current DATA 
						// byte and then convert that hex representation to the corresponding numerical value
						ui8arr_byte_string[0] = ui8arr_read_bytes[0];
						i16_ret_val=FILE_SYS_file_read(ui8arr_read_bytes, 1,  &ui32_n_read_bytes);
						ui8arr_byte_string[1] = ui8arr_read_bytes[0];// the [0] is because FILE_SYS_file_read starts filling from idx 0!
						ui8arr_byte_string[2] ='\0';
						AUX_FUNCS_atoi(ui8arr_byte_string, 16, 9,&ui32_aux);				
																	    
                        // keep the value and address of the read byte and increase the absolute address counter 
				        NVM_hex_data.ui8_last_rd_byte = (uint8_t)ui32_aux;
				        NVM_hex_data.ui32_last_rd_byte_abs_addr = NVM_hex_data.ui32_curr_abs_addr;
						
						NVM_hex_data.ui32_curr_abs_addr++;
						
						(*pui8_read_bye) =  NVM_hex_data.ui8_last_rd_byte;
						
						// increase record payload byte index ( DATA, CHECKSUM or end lines )
						NVM_hex_data.ui16_curr_rec_byte_idx++;						
						
					}else if (NVM_hex_data.ui8_curr_rec_type==NVM_HEX_REC_TY_EXT_SEG_ADDR){
						
						// read the following 3 bytes that complete the hex representation of the 2 data
						// bytes of the extended segment address record
						ui8arr_byte_string[0] = ui8arr_read_bytes[0];
						i16_ret_val=FILE_SYS_file_read(ui8arr_read_bytes, 3,  &ui32_n_read_bytes);
						ui8arr_byte_string[1] = ui8arr_read_bytes[0];
						ui8arr_byte_string[2] = ui8arr_read_bytes[1];
						ui8arr_byte_string[3] = ui8arr_read_bytes[2];
						ui8arr_byte_string[4] = '\0';
						AUX_FUNCS_atoi(ui8arr_byte_string, 16, 9,&ui32_aux);

						NVM_hex_data.ui32_ext_seg_addr = ui32_aux;
						
						// add the extended segment address to the current absolute address. "...The extended segment address data field
						// contains a 16-bit segment base address. This is multiplied by 16 ( 4bits shift ) and added to each
						// subsequent data  record address to form the address for the data. This allows addressing up to one
						// megabyte (1048576 bytes) of address space..."
                        NVM_hex_data.ui32_curr_abs_addr = (NVM_hex_data.ui32_ext_seg_addr<<4)+NVM_hex_data.ui16_addr;
						
						// increase record payload byte index ( DATA, CHECKSUM or end lines )
						NVM_hex_data.ui16_curr_rec_byte_idx=NVM_hex_data.ui16_curr_rec_byte_idx+2;
											
					}else if (NVM_hex_data.ui8_curr_rec_type==NVM_HEX_REC_TY_EXT_LIN_ADDR){
							
						// read the following 3 bytes that complete the hex representation of the 2 data
						// bytes of the extended linear address record
						ui8arr_byte_string[0] = ui8arr_read_bytes[0];
						i16_ret_val=FILE_SYS_file_read(ui8arr_read_bytes, 3,  &ui32_n_read_bytes);
						ui8arr_byte_string[1] = ui8arr_read_bytes[0];
						ui8arr_byte_string[2] = ui8arr_read_bytes[1];
						ui8arr_byte_string[3] = ui8arr_read_bytes[2];
						ui8arr_byte_string[4] = '\0';
						AUX_FUNCS_atoi(ui8arr_byte_string, 16, 9,&ui32_aux);
												
						NVM_hex_data.ui32_ext_lin_addr = ui32_aux;
						
						//  add the extended linear address to the current absolute address. "...The extended linear address allows for
						// 32 bit addressing (up to 4 GiB). The byte count is always 02 and the address field is ignored (typically 0000).
						// The two data bytes (big endian) specify the upper 16 bits of the 32 bit absolute address for all subsequent
						// type 00 records; these upper address bits apply until the next 04 record. The absolute address for a type 00
						// record is formed by combining the upper 16 address bits of the most recent 04 record with the low 16 address
						// bits of the 00 record. If a type 00 record is not preceded by any type 04 records then its upper 16 address
						// bits default to 0000..."
                        NVM_hex_data.ui32_curr_abs_addr = (NVM_hex_data.ui32_ext_lin_addr<<16)|NVM_hex_data.ui16_addr;
						
						// increase record payload byte index ( DATA, CHECKSUM or end lines )
						NVM_hex_data.ui16_curr_rec_byte_idx=NVM_hex_data.ui16_curr_rec_byte_idx+2;
			
					//}else{
					// other record types are not supported and data bytes are ignored					
					}//if
															
				}else{
					
					// CHECKSUM on end line bytes : process the bytes of the checksum					
					// TODO: calculate checksum ....

				}//if
				
			}//if (ui8_read_bytes[0]==NVM_HEX_REC_START_MARK)
				
		}//if ( (i16_ret_val>=0) && (FILE_SYS_file_read(ui8arr_read_bytes, 1,  &ui32_n_read_bytes)>0) )
		
	}//while

    // update some return parametrs	
	(*pui32_read_abs_address) = NVM_hex_data.ui32_last_rd_byte_abs_addr ;
	(*pui32_curr_abs_address) = NVM_hex_data.ui32_curr_abs_addr ;
	
	return i16_ret_val;

}//NVM_HEXReadNextByte



int16_t NVM_HEXFileBurnToFlash(uint8_t * pui8_file_name, uint32_t ui32_start_abs_addr, uint32_t ui32_end_abs_addr ){
	int16_t i16_ret_val=0;
    uint8_t ui8_read_byte = 0;	
	uint8_t ui8_read_idx=0;
	uint8_t ui8arr_read_bytes[4];
	uint32_t ui32_start_page_abs_addr = 0;// the absolute base address of the first page to write to flash
	uint32_t ui32_curr_page_abs_addr = 0;// the absolute base address of the current processed page to write to flash
	uint32_t ui32_last_page_abs_addr = 0;// the absolute base address of the last page to write to flash
	uint32_t ui32_read_abs_addr = 0;// the absolute address of the last successfully read byte
	uint32_t ui32_curr_abs_addr = 0;// the current position of the file read cursor
    uint16_t ui32_in_page_address = 0;
	uint16_t ui16_aux=0;
    uint32_t ui32_aux=0;
	uint32_t ui32_aux2=0;
    uint8_t ui8_aux = 0;
    uint8_t ui8arr_string[16];
    uint32_t ui32_dw_page[NVMCTRL_MAX_PAGE_SIZE_DW]; // array used to pass the page content to the page write function
    uint8_t ui8_end_of_hex = FALSE;
	uint8_t ui8_end_of_flash = FALSE;

int16_t i16_borrame = 0;

    // unlock all the flash pages from the last bootlader address to the end of the flash memory
    i16_ret_val = (int16_t)NVM_UnlockFlashFromAddressToEnd(ui32_start_abs_addr);

	if (i16_ret_val>=0){
		
       // erase all the blocks of the flash reserved for the application program binary
       i16_ret_val = (int16_t)NVM_EraseFlashFromAddressToEnd(ui32_start_abs_addr);
	
	}//if

	if (i16_ret_val>=0){
 
        // try to open the specified hex file
 	    i16_ret_val = NVM_HEXFileOpen(pui8_file_name);

	}//if

	if (i16_ret_val>=0){
		
		ui32_start_page_abs_addr = ((uint32_t)(ui32_start_abs_addr / NVM_info.ui32_page_size)) * NVM_info.ui32_page_size;
		ui32_last_page_abs_addr = ((uint32_t)(ui32_end_abs_addr / NVM_info.ui32_page_size)) * NVM_info.ui32_page_size;
		ui32_curr_page_abs_addr = ui32_start_page_abs_addr;
		ui32_curr_abs_addr = ui32_start_page_abs_addr;
		ui32_read_abs_addr = NVMCTRL_INVALID_32BIT_ADDR;
		
		// place the hex file read cursor pointing to the first address of the first page of the range to start reading from it
		i16_ret_val=NVM_HEXSeekToByteAddr(ui32_start_page_abs_addr);
	
	}//if
		
	if (i16_ret_val>=0){	

		// read the hex file, updating the flash page by page with the content read from the file until the end
		// of the hex file or the end of the flash is reached, or any error occurs
		ui8_end_of_hex = FALSE;
		ui8_end_of_flash = FALSE;
		while ( (i16_ret_val>=0) && (!ui8_end_of_hex) && (!ui8_end_of_flash)){
		
			// READ CURRENT PAGE:
			// read from the hex file all the data of the current processed page. Data is read byte by byte 
			// from the hex file and then stored in the temporary DWORDs ( 4 bytes ) array.
			ui32_in_page_address = 0;// reset the index used to process all the bytes in the current processed page
			memset(ui8arr_read_bytes,0,sizeof(ui8arr_read_bytes)); // clear the temporary array used to store read bytes from the file
			memset(ui32_dw_page,0xFF,sizeof(ui32_dw_page)); // clear the buffer used to temporary store the DWORds of the page read from the file
			ui8_read_idx=0;// clear the index used to fill the ui8arr_read_bytes array
			while ( (i16_ret_val>=0) && (ui32_in_page_address<NVM_info.ui32_page_size) ){
				
				i16_ret_val=NVM_HEXReadNextByte(&ui8_read_byte, &ui32_read_abs_addr, &ui32_curr_abs_addr);
                if (i16_ret_val>=0){
					
					ui8arr_read_bytes[ui8_read_idx]=ui8_read_byte;		
					ui8_read_idx++;
				
					// once all the 4 bytes of a DWORD have been read then they are converted to a DWORD value
					// and then it is stored in the DWORD array
					if (ui8_read_idx==4){
						ui32_aux = (ui8arr_read_bytes[3]<<24)|(ui8arr_read_bytes[2]<<16)|(ui8arr_read_bytes[1]<<8)|ui8arr_read_bytes[0];
						ui32_dw_page[(uint32_t)(ui32_in_page_address/4)]=ui32_aux;
						// reset the variables used to read the words in the hex file
						memset(ui8arr_read_bytes,0,sizeof(ui8arr_read_bytes)); // clear the temporary array used to store read bytes from the file
						ui8_read_idx=0;// index used to fill the ui8arr_read_bytes array
					}//if
				    
					// increase the index used to process each byte of the current page
					ui32_in_page_address++;
					
				}else if (i16_ret_val==-2){

			       // the end of the hex file has been reached so there is no more content in the file to burn to flash
					ui8_end_of_hex = TRUE;
					
				}//if (i16_ret_val>=0)			
			
			}//while

i16_borrame++;
if (i16_borrame==270){
	i16_borrame++;
}

            // ERASE AND WRITE TO FLASH THE CURRENT READ PAGE:
			// if the page has been read without errors then write the read page to flash
		    if ( (ui8_end_of_hex)||(i16_ret_val>=0) ){
			
			       // write the data into the page buffer
			       i16_ret_val = (int16_t)NVM_WriteInPageBuffer(ui32_dw_page,128,ui32_curr_page_abs_addr);

			}//if
					
		    if ( (ui8_end_of_hex)||(i16_ret_val>=0) ){
				
			    // write the content of the page buffer to flash
			    i16_ret_val = (int16_t)NVM_WritePageBufferToFlash(ui32_curr_page_abs_addr);
			    
			}//if

			// move the address pointer to process the next page ( read and then write ) 
			ui32_curr_page_abs_addr = ui32_curr_page_abs_addr + NVM_info.ui32_page_size;	
	        if (ui32_curr_abs_addr>=ui32_last_page_abs_addr) {			
			     // the ui32_curr_page_abs_addr address has reached the las address available in the flash so it 
				 // and it is not possible to continue writing beyond that address
				 ui8_end_of_flash = TRUE;			
			}//if
			
		}//while

	}//if
	
	NVM_HEXFileClose();
	
	return i16_ret_val;
	
}//NVM_HEXFileBurnToFlash
