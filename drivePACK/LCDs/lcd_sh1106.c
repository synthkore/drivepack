// *****************************************************************
// ****                       drivePACK                         ****
// ****                  www.tolaemon.com/dpack                 ****
// ****                       Source code                       ****
// ****                       25/07/2021                        ****
// ****                     Jordi Bartolome                     ****
// ****                                                         ****
// **** IMPORTANT:                                              ****
// **** Using this code or any part of it means accepting all   ****
// **** conditions exposed in: http://www.tolaemon.com/dpack    ****
// *****************************************************************

#include "lcd_sh1106.h"

#ifdef LCD_IS_SH1106

struct{
    uint8_t ui8_address;
	uint8_t ui8_frame_buffer[LCD_SH1106_COLS_X][LCD_SH1106_PAGES_Y];
}LCD_SH1106_frame_buffer;

void LCD_SH1106_init(uint8_t ui8_address){
	// TIMING_TICKS tticks_timer;


    LCD_SH1106_frame_buffer.ui8_address = ui8_address;

	LCD_SH1106_write_command(LCD_SH1106_CMD_DISPLAYOFF);
	LCD_SH1106_write_command(LCD_SH1106_CMD_NORMALDISPLAY);
	LCD_SH1106_write_command(LCD_SH1106_CMD_SETDISPLAYCLOCKDIV);
	LCD_SH1106_write_command(0x80);
	LCD_SH1106_write_command(LCD_SH1106_CMD_SETMULTIPLEX);
	LCD_SH1106_write_command(0x3F);
	LCD_SH1106_write_command(LCD_SH1106_CMD_SETDISPLAYOFFSET);
	LCD_SH1106_write_command(0x00);
	LCD_SH1106_write_command(LCD_SH1106_CMD_SETSTARTLINE | 0x00);
	LCD_SH1106_write_command(LCD_SH1106_CMD_CHARGEPUMP);
	LCD_SH1106_write_command(0x14);
	LCD_SH1106_write_command(LCD_SH1106_CMD_MEMORYMODE);
	LCD_SH1106_write_command(0x00);
	LCD_SH1106_write_command(LCD_SH1106_CMD_SEGREMAP);
	LCD_SH1106_write_command(LCD_SH1106_CMD_COMSCANDEC);
	LCD_SH1106_write_command(LCD_SH1106_CMD_SETCOMPINS);
	LCD_SH1106_write_command(0x12);
	LCD_SH1106_write_command(LCD_SH1106_CMD_SETCONTRAST);
	LCD_SH1106_write_command(0xCF);
	LCD_SH1106_write_command(LCD_SH1106_CMD_SETPRECHARGE);
	LCD_SH1106_write_command(0xF1);
	LCD_SH1106_write_command(LCD_SH1106_CMD_SETVCOMDETECT);
	LCD_SH1106_write_command(0x40);
	LCD_SH1106_write_command(LCD_SH1106_CMD_DISPLAYALLON_RESUME);
	LCD_SH1106_write_command(LCD_SH1106_CMD_NORMALDISPLAY);
	LCD_SH1106_write_command(0x2e);            // stop scroll
	LCD_SH1106_write_command(LCD_SH1106_CMD_DISPLAYON);	

}//LCD_SH1106_init


int16_t LCD_SH1106_write_I2C_data_frame(uint8_t ui8_data_to_send){
	int16_t ret_val = 0;


	SERCOM4->I2CM.CTRLB.bit.ACKACT = 1; // set NACK for high speed code
	SERCOM4->I2CM.DATA.reg = ui8_data_to_send;
	while (SERCOM4->I2CM.SYNCBUSY.bit.SYSOP == 1){1;};
	
	// If the I2C master receives an acknowledge response from the I2C slave,
	// INTFLAG.MB will be set and STATUS.RXNACK will be cleared.
	while (SERCOM4->I2CM.INTFLAG.bit.MB==0){1;}
	SERCOM4->I2CM.INTFLAG.bit.MB=0;// clear the interrupt flag	
	if(SERCOM4->I2CM.STATUS.bit.RXNACK == 1){
		// If there is no I2C slave device responding to the address packet, then
		// the INTFLAG.MB Interrupt flag and STATUS.RXNACK will be set.
		ret_val = -1;
	}else{
		// If the I2C master receives an acknowledge response from the I2C slave,
		// INTFLAG.MB will be set and STATUS.RXNACK will be cleared.
		ret_val = 0;
	}//if

	return ret_val;
	
}// LCD_SH1106_write_I2C_data_frame


int16_t LCD_SH1106_write_I2C_address_frame(uint8_t ui8_read_write){
	int16_t ret_val = 0;


	SERCOM4->I2CM.CTRLB.bit.ACKACT = 1; // set NACK for high speed code
	SERCOM4->I2CM.ADDR.reg = LCD_SH1106_frame_buffer.ui8_address<<1 | ui8_read_write;
	while (SERCOM4->I2CM.SYNCBUSY.bit.SYSOP == 1){1;};
		
	// If the I2C master receives an acknowledge response from the I2C slave,
	// INTFLAG.MB will be set and STATUS.RXNACK will be cleared.		
	while (SERCOM4->I2CM.INTFLAG.bit.MB==0){1;}
    SERCOM4->I2CM.INTFLAG.bit.MB=0;// clear the interrupt flag		
	if(SERCOM4->I2CM.STATUS.bit.RXNACK == 1){	
		// If there is no I2C slave device responding to the address packet, then
		// the INTFLAG.MB Interrupt flag and STATUS.RXNACK will be set.
		ret_val = -1;			
	}else{
		// If the I2C master receives an acknowledge response from the I2C slave,
		// INTFLAG.MB will be set and STATUS.RXNACK will be cleared.
		ret_val = 0;
	}//if

    return ret_val;

}// LCD_SH1106_write_I2C_address_frame


int16_t LCD_SH1106_write_command(uint8_t ui8_command){
	int16_t ret_val = 0;


	// after the I2C master has been enabled, the Bus state is UNKNOWN (0b00). From
	// the UNKNOWN state, the bus will transition to IDLE (0b01) by either forcing
	// by writing 0b01 to STATUS.BUSSTATE
	SERCOM4->I2CM.STATUS.bit.BUSSTATE=1;

	// begin transmitting
	LCD_SH1106_write_I2C_address_frame(LCD_SH1106_WRITE);
	
	// send command mode and then command
	LCD_SH1106_write_I2C_data_frame(0x00);
	LCD_SH1106_write_I2C_data_frame(ui8_command);

	// stop transmitting
	SERCOM4->I2CM.CTRLB.bit.CMD = 0x3 ;
	while (SERCOM4->I2CM.SYNCBUSY.bit.SYSOP == 1){1;};

	return ret_val;

}// LCD_SH1106_write_command


int16_t LCD_SH1106_write_data( uint8_t ui8_data){
	int16_t ret_val = 0;


	// after the I2C master has been enabled, the Bus state is UNKNOWN (0b00). From
	// the UNKNOWN state, the bus will transition to IDLE (0b01) by either forcing
	// by writing 0b01 to STATUS.BUSSTATE
	SERCOM4->I2CM.STATUS.bit.BUSSTATE=1;

	// begin transmitting
	LCD_SH1106_write_I2C_address_frame(LCD_SH1106_WRITE);
	
	// send command mode and then command
	LCD_SH1106_write_I2C_data_frame(0x40);
	LCD_SH1106_write_I2C_data_frame(ui8_data);

	// stop transmiting
	SERCOM4->I2CM.CTRLB.bit.CMD = 0x3 ;
	while (SERCOM4->I2CM.SYNCBUSY.bit.SYSOP == 1){1;};

	return ret_val;

}// LCD_SH1106_write_data


void LCD_SH1106_buffer_area_fast_fill(uint16_t ui16_pattern, uint16_t ui16_x1, uint16_t ui16_y1, uint16_t ui16_x2, uint16_t ui16_y2){
	uint8_t ui8_column_start = 0;
	uint8_t ui8_column_end = 0;
	uint8_t ui8_column = 0;
	uint8_t ui8_page_start = 0;
	uint8_t ui8_page_end = 0;
	uint8_t ui8_page = 0;
	uint8_t ui8_col_value = 0;
	uint8_t ui8_mask_start = 0;
	uint8_t ui8_mask_end = 0;
	uint8_t ui8_mask = 0;
	uint8_t ui8_num_bits = 0;
	uint8_t ui16_aux = 0;


	// correct the order of X coordinates if necessary
	if (ui16_x1>ui16_x2){
		ui16_aux = ui16_x1;
		ui16_x1 = ui16_x2;
		ui16_x2 = ui16_aux;
	}
	// correct the order of Y coordinates if necessary
	if (ui16_y1>ui16_y2){
		ui16_aux = ui16_y1;
		ui16_y1 = ui16_y2;
		ui16_y2 = ui16_aux;
	}

	// get the page and column to draw the rectangle pixels
	ui8_column_start = ui16_x1;
	ui8_column_end = ui16_x2;
	ui8_page_start = ui16_y1 / 8;
	ui8_page_end = ui16_y2 / 8;

	// fill the pages and columns with the right bit patterns in order to
	// fill the specified area
	for (ui8_page=ui8_page_start;ui8_page<=ui8_page_end;ui8_page++){

		ui8_mask_start = 0;
		ui8_mask_end = 0;
		ui8_mask = 0;
		
		// if current processed page corresponds to the top part of the filled area
		if (ui8_page==ui8_page_start){
			// in the case of the first page apply the mask only in the last bits
			// of the column byte
			ui8_num_bits = ui16_y1 % 8;
			for (ui16_aux=0;ui16_aux<ui8_num_bits;ui16_aux++){
				ui8_mask_start = ui8_mask_start | (0x01<<ui16_aux);
			}
			ui8_mask_start = ~ui8_mask_start;
			ui8_mask = ui8_mask_start;
		}//if
		
		// if current processed page corresponds to the bottom part of the filled area
		if (ui8_page==ui8_page_end){
			// in the case of the last page apply the mask only in the first bits
			// of the column byte
			ui8_num_bits = ui16_y2 % 8;
			for (ui16_aux=0;ui16_aux<ui8_num_bits;ui16_aux++){
				ui8_mask_end = ui8_mask_end | (0x01<<ui16_aux);
			}
			ui8_mask = ui8_mask_end;
		}//if
		
		if ( (ui8_page==ui8_page_start)&&(ui8_page==ui8_page_end)){
			// if start and end pages are the same then masks are ANDed
			ui8_mask = ui8_mask_end & ui8_mask_start;
			}else if ((ui8_page!=ui8_page_start)&&(ui8_page!=ui8_page_end)){
			// in the middle pages apply the mask in all the bits
			ui8_mask = 0xFF;
		}//if
		
		// apply the pattern and the mask over the buffer
		for (ui8_column=ui8_column_start;ui8_column<ui8_column_end;ui8_column++){
			
			// apply the pattern with the corresponding mask to fill only the pixels
			// of the frame buffer that correspond to the specified coordinates
			ui8_col_value = LCD_SH1106_frame_buffer.ui8_frame_buffer[ui8_column][ui8_page];
			ui8_col_value = ui8_col_value & (~ui8_mask);
			ui8_col_value = ui8_col_value | (((uint8_t)ui16_pattern) & ui8_mask);
			LCD_SH1106_frame_buffer.ui8_frame_buffer[ui8_column][ui8_page]=ui8_col_value;

		}//for
		
	}//for
	
}//LCD_SH1106_buffer_area_fast_fill


void LCD_SH1106_buffer_fill(uint8_t ui8_page_pattern){
	uint8_t ui8_column = 0;
	uint8_t ui8_page = 0;


	for (ui8_page=0;ui8_page<LCD_SH1106_PAGES_Y;ui8_page++){
		
		for (ui8_column=0;ui8_column<LCD_SH1106_COLS_X;ui8_column++){

			LCD_SH1106_frame_buffer.ui8_frame_buffer[ui8_column][ui8_page]=ui8_page_pattern;

		}//for
		
	}//for
	
}//LCD_SH1106_buffer_fill


void LCD_SH1106_clear(uint8_t ui8_page_pattern){
	uint8_t ui8_column = 0;
	uint8_t ui8_page = 0;

	for (ui8_page=0;ui8_page<8;ui8_page++){
		
		LCD_SH1106_write_command(LCD_SH1106_CMD_PAGESTARTADDRESS|ui8_page);
		LCD_SH1106_write_command(LCD_SH1106_CMD_SETLOWCOLUMN | 0x00);
		LCD_SH1106_write_command(LCD_SH1106_CMD_SETHIGHCOLUMN | 0x00);
		
		for (ui8_column=0;ui8_column<128;ui8_column++){
			LCD_SH1106_write_data((uint8_t)ui8_page_pattern);
		}//for
		
	}//for

}//LCD_SH1106_clear


void LCD_SH1106_buffer_set_pixel(uint8_t ui8_x, uint8_t ui8_y, uint8_t ui8_bw){
	uint8_t ui8_page = 0;
	uint8_t ui8_col_value = 0;

	
	if ( (ui8_x<LCD_SH1106_SIZE_X) && (ui8_y<LCD_SH1106_SIZE_Y) ){
		
		ui8_page = (ui8_y/8);
		
		// get the current value of the group of 8 pixels
		ui8_col_value = LCD_SH1106_frame_buffer.ui8_frame_buffer[ui8_x][ui8_page];
		
		// update the value of the corresponding pixel in the group of 8 bits
		if (ui8_bw==0){
			ui8_col_value = ui8_col_value & (~(1<<(ui8_y%8)));
		}else{
			ui8_col_value = ui8_col_value | (1<<(ui8_y%8));
		}//if
		
		// set the new value for the group of 8 pixes with the updated pixel
		LCD_SH1106_frame_buffer.ui8_frame_buffer[ui8_x][ui8_page] = ui8_col_value;
		
	}//if

}//LCD_SH1106_buffer_set_pixel


void LCD_SH1106_buffer_refresh(){
	uint8_t ui8_column = 0;
	uint8_t ui8_page = 0;


	for (ui8_page=0;ui8_page<LCD_SH1106_PAGES_Y;ui8_page++){
		
		LCD_SH1106_write_command(LCD_SH1106_CMD_PAGESTARTADDRESS|ui8_page);
		LCD_SH1106_write_command(LCD_SH1106_CMD_SETLOWCOLUMN | 0x00);
		LCD_SH1106_write_command(LCD_SH1106_CMD_SETHIGHCOLUMN | 0x00);
		
		for (ui8_column=0;ui8_column<LCD_SH1106_COLS_X;ui8_column++){
			LCD_SH1106_write_data(LCD_SH1106_frame_buffer.ui8_frame_buffer[ui8_column][ui8_page]);
		}//for
		
	}//for
	
}//LCD_SH1106_buffer_refresh

#endif //#ifdef LCD_IS_SH1106
