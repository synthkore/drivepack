// *****************************************************************
// ****                       drivePACK                         ****
// ****                  www.tolaemon.com/dpack                 ****
// ****                       Source code                       ****
// ****                       01/04/2020                        ****
// ****                     Jordi Bartolome                     ****
// ****                                                         ****
// **** IMPORTANT:                                              ****
// **** Using this code or any part of it means accepting all   ****
// **** conditions exposed in: http://www.tolaemon.com/dpack    ****
// *****************************************************************
#include "dpack_ctrl.h"

// ASSEMBLY PUBLIC FUNCTIONS
uint8_t ui8_ctrl_bits        = 0x00;// variable ctrl bits: 7:x 6:x 5:x 4:'?' 3:CLCK2 2:CLCK1 1:R/!W 0:!CS

// bus states:
// 0 not valid
// 1 high impedance                     - ctrl bits:INPUT  data bits:INPUT
// 2 read address requested by keyboard - ctrl bits:INPUT  data bits:INPUT  - ROM EMULATION MODE
// 3 write data requested by keyboard   - ctrl bits:INPUT  data bits:OUTPUT - ROM EMULATION MODE
// 4 write address requested to ROMPACK - ctrl bits:OUTPUT data bits:OUTPUT - KEYBOARD EMULATION MODE
// 5 read data requested to ROMPACK     - ctrl bits:OUTPUT data bits:INPUT  - KEYBOARD EMULATION MODE
uint8_t  ui8_bus_state                             = 0;

uint8_t  ui8_dpack_dumper_state                    = 0;
uint8_t  ui8_dpack_dumper_idle_clck_wait_cycles    = 0;
uint8_t  ui8_dpack_dumper_addres_nibbles_written   = 0;
uint8_t  ui8_dpack_dumper_data_nibbles_read        = 0;
uint32_t ui32_dpack_dumper_total_data_nibbles_read = 0;
uint32_t ui32_dpack_dumper_current_address         = 0x0000000;  // 0x0004321;
uint32_t ui32_dpack_dumper_address_to_write        = 0x0000000;
uint8_t  ui8_dpack_dumper_last_data_nibble_read    = 0;          // stores the value of last read nibble

// read data nibbles Tx buffer control variables
uint8_t ui8_nibbles_buffer[MAX_ROM_NIBBLES_BUFFER];
uint8_t ui8_nibbles_buffer_initialized = FALSE; // FALSE if the ui8_nibbles_buffer does NOT contain valid ROM data, TRUE if ui8_nibbles_buffer contains valid ROM data
int32_t i32_dumper_rom_size;// size in bytes of the ROM PACK loaded into the nibbles buffer. ROM PACK cartridges may have different sizes, and the size of a ROM PACK cartridge is determined by searching the end sequence: 0x23 0x83 0x93 0x06 0x47 0x83 0xAB 0x02 0x63 0x27 0x4B 0x27 0x47 0x93 0x2B 0x83 0xFF 0xFF 0xFF 0xFF 0x00 0x00 0x00 0x00
uint8_t ui8_rom_title[MAX_ROM_TITLE_BUFFER];
uint8_t ui8_themes_titles_arr[MAX_THEME_TITLES_ARRAY][MAX_THEME_TITLE_BUFFER];
uint8_t ui8_file_name[MAX_ROM_FILE_NAME];
uint8_t ui8_file_path[MAX_ROM_FILE_PATH];
uint8_t * pui8_nibbles_buffer = NULL;// pointer to the elements of the ui8_nibbles_buffer

//           CLCK2  |1   1     0   1  1   1 0   1 |1   1
//           CLCK1  |0   1     1   1  0   1 1   1 |0   1
// ui8_clck12_state  0   1     2   3  4   5 6   7  0   1
uint8_t  ui8_clck12_state                            = 0;
uint8_t  ui8_clck12_state_transition                 = 0;
uint8_t  ui8_clck12_state_wait_cycles                = 1;

uint8_t  ui8_read_add_writ_by_keyb_state             = 0;
uint8_t  ui8_write_data_req_by_keyb_state            = 0;
uint8_t  ui8_clck_cycle                              = 0;

uint8_t  ui8_read_add_nibble_numb                    = 0; // when an address nibble is read from the bus then it must be shifted to its corresponding position in the final address, this variable is used to control the number of nibbles ( x4 bits ) to shift on each nibble
uint32_t ui32_read_nibble_address                    = 0; // last read address
uint8_t  ui8_read_device_code                        = 0; // the device code of the last read address
uint8_t  ui8_read_nibble_val                         = 0; // address nibble value read on last read cycle
uint8_t  ui8_write_nibble_val                        = 0; // the nibble to write into the bus into the next nibble write cycle

void DPACK_CTRL_Init(){
	
	// clear the ui8_nibbles_buffer content to all 0xFF
	memset (ui8_nibbles_buffer,0xFF,sizeof(uint8_t)*MAX_ROM_NIBBLES_BUFFER);
	
	// clear the ui8_rom_title and the ui8_themes_titles_arr to '\0'
	memset (ui8_rom_title,'\0',sizeof(ui8_rom_title));
	AUX_FUNCS_lstrcpy(ui8_rom_title,"Info not initialized",MAX_ROM_TITLE_BUFFER);
	memset (ui8_themes_titles_arr,'\0',sizeof(ui8_themes_titles_arr));

	// clear the file name and path buffers used to store current songs file path	
	memset (ui8_file_name,'\0',sizeof(uint8_t)*MAX_ROM_FILE_NAME);
	memset (ui8_file_path,'\0',sizeof(uint8_t)*MAX_ROM_FILE_PATH);
	
	// ui8_nibbles_buffer does NOT contain valid ROM data yet
	ui8_nibbles_buffer_initialized = FALSE;
	
}//DPACK_CTRL_Init


int8_t DPACK_CTRL_check_buffer(){
    int8_t i8_ret_val = 0;
	int16_t i16_ret_val=0;
	int32_t i32_aux = 0;
	uint8_t ui8_expected_header[] = ROMPACK_HEADER_NIBBLES;

	// print current header bytes and compare them with expected
	i32_aux=0;
	while ( (i32_aux<ROMPACK_HEADER_NUM_BYTES)&&(i8_ret_val==0)){
					
		if (ui8_nibbles_buffer[i32_aux]!=ui8_expected_header[i32_aux]) i8_ret_val = -1;
		i32_aux++;
		
	}//while
		
	return i8_ret_val;

}//DPACK_CTRL_check_buffer


int32_t DPACK_CTRL_get_size_rom_in_buffer(){
	int32_t i32_ret_val=0;
    uint8_t ui8_found_end = FALSE;
    int32_t i32_aux = 0;

    if (ui8_nibbles_buffer_initialized==FALSE){
		
		i32_ret_val = -1;
		
	}else{

		i32_aux = 0;
		while ( (ui8_found_end==FALSE) && (i32_aux<MAX_ROM_NIBBLES_BUFFER-8) ){
	
		   // despite all the ROM PACKs should end with the byte sequence "0x23 0x83 0x93 0x06 0x47 0x83 0xAB 0x02 0x63 0x27 
		   // 0x4B 0x27 0x47 0x93 0x2B 0x83 0xFF 0xFF 0xFF 0xFF 0x00 0x00 0x00 0x00", there is no need to search for all them
		   // and only search for "0xFF 0xFF 0xFF 0xFF 0x00 0x00 0x00 0x00" to detect the end of the ROM PACK cartridge
	
		   if ( (ui8_nibbles_buffer[i32_aux]  ==0xFF) && (ui8_nibbles_buffer[i32_aux+1]==0xFF) &&
				(ui8_nibbles_buffer[i32_aux+2]==0xFF) && (ui8_nibbles_buffer[i32_aux+3]==0xFF)  &&
				(ui8_nibbles_buffer[i32_aux+4]==0x00) && (ui8_nibbles_buffer[i32_aux+5]==0x00)  &&
				(ui8_nibbles_buffer[i32_aux+6]==0x00) && (ui8_nibbles_buffer[i32_aux+7]==0x00) ){
		     
				 ui8_found_end = TRUE;
				 i32_ret_val = i32_aux + 8; // 8 is the number of the bytes "0xFF 0xFF 0xFF 0xFF 0x00 0x00 0x00 0x00"
			 
			}else{
				i32_aux++;
			}
		}//while
    
	}//if
	
	return i32_ret_val;

}//DPACK_CTRL_get_size_rom_in_buffer
