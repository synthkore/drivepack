// *****************************************************************
// ****                       drivePACK                         ****
// ****                  www.tolaemon.com/dpack                 ****
// ****                       Source code                       ****
// ****                       5/08/2021                        ****
// ****                     Jordi Bartolome                     ****
// ****                                                         ****
// **** IMPORTANT:                                              ****
// **** Using this code or any part of it means accepting all   ****
// **** conditions exposed in: http://www.tolaemon.com/dpack    ****
// *****************************************************************

#include "lcd_st7565.h"

#ifdef LCD_IS_ST7565

struct{
     uint8_t ui8_frame_buffer[LCD_ST7565_COLS_X][LCD_ST7565_PAGES_Y];
}LCD_ST7565_frame_buffer;

void LCD_ST7565_init(){
	TIMING_TICKS tticks_timer;
	
	// start with all lines to '1'
	LCD_ST7565_CS_SET;
	LCD_ST7565_RST_SET;
	LCD_ST7565_WR_SET;
	LCD_ST7565_RS_SET;	
			
	// RESET
	LCD_ST7565_PAUSE_TICKS(2000);
	LCD_ST7565_RST_CLR;
	LCD_ST7565_PAUSE_TICKS(2000);
	LCD_ST7565_RST_SET;
	LCD_ST7565_PAUSE_TICKS(2000);

	LCD_ST7565_write_SPI_command(0xaf);//DISPLAY_ON          = 0xaf;
	LCD_ST7565_write_SPI_command(0x2c | 0x2a | 0x29); //  PCTRL_BOOSTER_ON    = 0x2c,  PCTRL_REGULATOR_ON  = 0x2a, PCTRL_VFOLLOWER_ON  = 0x29
	LCD_ST7565_write_SPI_command(0x20 | 0x7 );// V0_INTERNAL_R_0     = 0x20; V0_INTERNAL_R_0 + (v0 & 0x07));
	LCD_ST7565_write_SPI_command(0x81);// ELECTRONIC_VOL_MODE = 0x81;
			
	LCD_ST7565_write_SPI_command(0x01 - 1 + (1 & 0x3f)); //ELECTRONIC_VOL_1    = 0x01;
	
	LCD_ST7565_write_SPI_command(0xa3); //BIAS_1_7 = 0xa3;
	
    LCD_ST7565_write_SPI_command(0xc0|8);// COMMON OUTPUT MODE SELECT = COMMON_ASCENDING    = 0xc0
    LCD_ST7565_write_SPI_command(0xa0); // SEGMENT_ASCENDING   = 0xa0;
    LCD_ST7565_write_SPI_command(0xa4); // ENTIRE_DISPLAY_OFF  = 0xa4;
    LCD_ST7565_write_SPI_command(0xa6); // INVERT_DISPLAY_OFF  = 0xa6;
    LCD_ST7565_write_SPI_command(0x40); // COMMON_OFFSET       = 0x40;
	// LCD_ST7565_write_command(0xA2); // LCD_BIAS_SET        = 0x42;

	LCD_ST7565_buffer_fill(0x00);
	LCD_ST7565_buffer_refresh();

}//LCD_ST7565_init



uint8_t LCD_ST7565_write_SPI_command(uint8_t ui8_command_to_send){

	LCD_ST7565_CS_CLR;
	LCD_ST7565_PAUSE_NOPS(1);
	
	LCD_ST7565_RS_CLR;
	LCD_ST7565_PAUSE_NOPS(1);

	// "...When writing a character to the Data register (DATA),  the character will be transferred to the Shift
	// register. Once the content of TxDATA has been transferred to the Shift register,  the Data Register Empty
	// flag in the Interrupt Flag Status and Clear register (INTFLAG.DRE) will be set ..."
	while (SERCOM4->SPI.INTFLAG.bit.DRE!=1){};
	//if ( SERCOM4->SPI.INTFLAG.bit.DRE == 1 ) {
	// SERCOM4->USART.INTENSET.bit.TXC == 1;
	SERCOM4->SPI.DATA.bit.DATA = ui8_command_to_send;
	//}//if
	while (SERCOM4->SPI.INTFLAG.bit.TXC!=1){};
	SERCOM4->SPI.INTFLAG.bit.TXC=0;
	
	// "...Each time one character is shifted out from the master,  another character will be shifted in from the
	// slave simultaneously. If the receiver is enabled (CTRLA.RXEN=1), the contents of the Shift register will be
	// transferred to the two-level receive buffer. The transfer takes place in the same clock cycle as the last
	// data bit is shifted in. And the Receive Complete Interrupt flag in the Interrupt Flag Status and Clear
	// register (INTFLAG.RXC) will be set. The received data can be retrieved by reading DATA..."
	// while (SERCOM4->SPI.INTFLAG.bit.RXC != 1 ){};
	// ui8_data_to_send = SERCOM4->SPI.DATA.bit.DATA;
	
	LCD_ST7565_CS_SET;
	
	return ui8_command_to_send; // return the value read during transmission
	
}// LCD_ST7565_write_SPI_command



uint8_t LCD_ST7565_write_SPI_data(uint8_t ui8_data_to_send){

	LCD_ST7565_CS_CLR;
	LCD_ST7565_PAUSE_NOPS(1);
	
	LCD_ST7565_RS_SET;
	LCD_ST7565_PAUSE_NOPS(1);

	// "...When writing a character to the Data register (DATA),  the character will be transferred to the Shift
	// register. Once the content of TxDATA has been transferred to the Shift register,  the Data Register Empty
	// flag in the Interrupt Flag Status and Clear register (INTFLAG.DRE) will be set ..."
	while (SERCOM4->SPI.INTFLAG.bit.DRE!=1){};
	//if ( SERCOM4->SPI.INTFLAG.bit.DRE == 1 ) {
	// SERCOM4->USART.INTENSET.bit.TXC == 1;
	SERCOM4->SPI.DATA.bit.DATA = ui8_data_to_send;
	//}//if
	while (SERCOM4->SPI.INTFLAG.bit.TXC!=1){};
	SERCOM4->SPI.INTFLAG.bit.TXC=0;
	
	// "...Each time one character is shifted out from the master,  another character will be shifted in from the
	// slave simultaneously. If the receiver is enabled (CTRLA.RXEN=1), the contents of the Shift register will be
	// transferred to the two-level receive buffer. The transfer takes place in the same clock cycle as the last
	// data bit is shifted in. And the Receive Complete Interrupt flag in the Interrupt Flag Status and Clear
	// register (INTFLAG.RXC) will be set. The received data can be retrieved by reading DATA..."
	// while (SERCOM4->SPI.INTFLAG.bit.RXC != 1 ){};
	// ui8_data_to_send = SERCOM4->SPI.DATA.bit.DATA;

	LCD_ST7565_CS_SET;
	LCD_ST7565_PAUSE_NOPS(1);
	
	return ui8_data_to_send; // return the value read during transmission
	
}// LCD_ST7565_write_SPI_data



void LCD_ST7565_buffer_area_fast_fill(uint16_t ui16_pattern, uint16_t ui16_x1, uint16_t ui16_y1, uint16_t ui16_x2, uint16_t ui16_y2){
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
		}
		
		// if current processed page corresponds to the bottom part of the filled area
		if (ui8_page==ui8_page_end){
			// in the case of the last page apply the mask only in the first bits
			// of the column byte
			ui8_num_bits = ui16_y2 % 8;
			for (ui16_aux=0;ui16_aux<ui8_num_bits;ui16_aux++){
				ui8_mask_end = ui8_mask_end | (0x01<<ui16_aux);
			}
			ui8_mask = ui8_mask_end;
		}
		
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
			ui8_col_value = LCD_ST7565_frame_buffer.ui8_frame_buffer[ui8_column][ui8_page];
			ui8_col_value = ui8_col_value & (~ui8_mask);
			ui8_col_value = ui8_col_value | (((uint8_t)ui16_pattern) & ui8_mask);
			LCD_ST7565_frame_buffer.ui8_frame_buffer[ui8_column][ui8_page]=ui8_col_value;

		}//for
		
	}//for
	
}//LCD_ST7565_buffer_area_fast_fill


void LCD_ST7565_buffer_fill(uint8_t ui8_page_pattern){
	uint8_t ui8_column = 0;
	uint8_t ui8_page = 0;

	for (ui8_page=0;ui8_page<LCD_ST7565_PAGES_Y;ui8_page++){
		
		for (ui8_column=0;ui8_column<LCD_ST7565_COLS_X;ui8_column++){

			LCD_ST7565_frame_buffer.ui8_frame_buffer[ui8_column][ui8_page]=ui8_page_pattern;

		}//for
		
	}//for
	
}//LCD_ST7565_buffer_fill


void LCD_ST7565_clear(uint8_t ui8_page_pattern){
	uint8_t ui8_column = 0;
	uint8_t ui8_page = 0;

	for (ui8_page=0;ui8_page<8;ui8_page++){
		
		LCD_ST7565_write_SPI_command(LCD_ST7565_CMD_SET_PAGE|ui8_page);
		LCD_ST7565_write_SPI_command(LCD_ST7565_CMD_SET_COL_LSB | 0x00);
		LCD_ST7565_write_SPI_command(LCD_ST7565_CMD_SET_COL_MSB | 0x00);
		
		for (ui8_column=0;ui8_column<128;ui8_column++){
			LCD_ST7565_write_SPI_data((uint8_t)ui8_page_pattern);
		}//for
		
	}//for

}//LCD_ST7565_clear


void LCD_ST7565_buffer_set_pixel(uint8_t ui8_x, uint8_t ui8_y, uint8_t ui8_bw){
	uint8_t ui8_page = 0;
	uint8_t ui8_col_value = 0;

	
	if ( (ui8_x<LCD_ST7565_SIZE_X) && (ui8_y<LCD_ST7565_SIZE_Y) ){
		
		ui8_page = (ui8_y/8);
		
		// get the current value of the group of 8 pixels
		ui8_col_value = LCD_ST7565_frame_buffer.ui8_frame_buffer[ui8_x][ui8_page];
		
		// update the value of the corresponding pixel in the group of 8 bits
		if (ui8_bw==0){
			ui8_col_value = ui8_col_value & (~(1<<(ui8_y%8)));
		}else{
			ui8_col_value = ui8_col_value | (1<<(ui8_y%8));
		}//if
		
		// set the new value for the group of 8 pixes with the updated pixel
		LCD_ST7565_frame_buffer.ui8_frame_buffer[ui8_x][ui8_page] = ui8_col_value;
		
	}//if

}//LCD_ST7565_buffer_set_pixel


void LCD_ST7565_buffer_refresh(){
	uint8_t ui8_column = 0;
	uint8_t ui8_page = 0;


	for (ui8_page=0;ui8_page<LCD_ST7565_PAGES_Y;ui8_page++){
		
		LCD_ST7565_write_SPI_command(LCD_ST7565_CMD_SET_PAGE|ui8_page);
		LCD_ST7565_write_SPI_command(LCD_ST7565_CMD_SET_COL_LSB | 0x00);
		LCD_ST7565_write_SPI_command(LCD_ST7565_CMD_SET_COL_MSB | 0x00);	
		
		for (ui8_column=0;ui8_column<LCD_ST7565_COLS_X;ui8_column++){
			LCD_ST7565_write_SPI_data(LCD_ST7565_frame_buffer.ui8_frame_buffer[ui8_column][ui8_page]);
		}//for
		
	}//for
	
}//LCD_ST7565_buffer_refresh

#endif //#ifdef LCD_IS_ST7565



	