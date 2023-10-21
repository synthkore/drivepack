// *****************************************************************
// ****                       drivePACK                         ****
// ****                  www.tolaemon.com/dpack                 ****
// ****                       Source code                       ****
// ****                       12/07/2021                        ****
// ****                     Jordi Bartolome                     ****
// ****                                                         ****
// **** IMPORTANT:                                              ****
// **** Using this code or any part of it means accepting all   ****
// **** conditions exposed in: http://www.tolaemon.com/dpack    ****
// *****************************************************************

#include "lcd_ili9481.h"

#ifdef LCD_IS_ILI9481

void LCD_ILI9481_init() {
	TIMING_TICKS tticks_timer;
	
	// start with all lines to '1'
	LCD_ILI9481_CS_SET;
	LCD_ILI9481_RST_SET;
	LCD_ILI9481_WR_SET;
	LCD_ILI9481_RS_SET;

	// RESET
	LCD_ILI9481_PAUSE_TICKS(2000);
	LCD_ILI9481_RST_CLR;
	LCD_ILI9481_PAUSE_TICKS(2000);
	LCD_ILI9481_RST_SET;
	LCD_ILI9481_PAUSE_TICKS(2000);

	LCD_ILI9481_write_command(0x11); // Exit Sleep Mode
	LCD_ILI9481_PAUSE_TICKS(2000);
	
	// WriteCommand(0x28); // Turn Off Display
	
	LCD_ILI9481_write_command(0xD0); // Power Setting
	LCD_ILI9481_write_data_8(0x07);// 1st Parameter: 0 0  0 0 0 VC[2] VC[1] VC[0]
	LCD_ILI9481_write_data_8(0x42);// 2nd Parameter: 0 PON 0 0 0 BT[2] BT[1] BT[0]
	LCD_ILI9481_write_data_8(0x18);// 3rd Parameter: 0 0 0 VCIRE VRH[3] VRH[2] VRH[1] VRH[0]
	// VC[2:0] Sets the ratio factor of Vci to generate the reference voltages Vci1.
	// BT[2:0] Sets the Step up factor and output voltage level from the reference voltages Vci1
	// PON is used to control the operation to generate VLOUT3.
	// VRH[3:0]: Sets the factor to generate VREG1OUT from VCILVL.
	// VCIRE: Select the external reference voltage Vci or internal reference voltage VCIR.

	LCD_ILI9481_write_command(0xD1); // VCOM Control
	LCD_ILI9481_write_data_8(0x00);// 1st Parameter 0 0 0 0 0 0 0 SEL/VCM
	LCD_ILI9481_write_data_8(0x07);// 2nd Parameter 0 0 VCM[5] VCM[4] VCM[3] VCM[2] VCM[1] VCM[0]
	LCD_ILI9481_write_data_8(0x10);// 3rd Parameter 0 0 0 VDV[4] VDV[3] VDV[2] VDV[1] VDV[0]
	// VCM [6:0] is used to set factor to generate VCOMH voltage from the reference voltage VREG1OUT
	// VDV[4:0] is used to set the VCOM alternating amplitude in the range of VREG1OUT x 0.70 to VREG1OUT x
	// SELVCM: Selection the VCM setting.

	LCD_ILI9481_write_command(0xD2); // Power_Setting for Normal Mode
	LCD_ILI9481_write_data_8(0x01);
	LCD_ILI9481_write_data_8(0x02);

	LCD_ILI9481_write_command(0xC0); //Panel Driving Setting
	LCD_ILI9481_write_data_8(0x10);
	LCD_ILI9481_write_data_8(0x3B);
	LCD_ILI9481_write_data_8(0x00);
	LCD_ILI9481_write_data_8(0x02);
	LCD_ILI9481_write_data_8(0x11);
	
	LCD_ILI9481_write_command(0xC5); //Frame Rate and Inversion Control
	LCD_ILI9481_write_data_8(0x03);

	LCD_ILI9481_write_command(0xC8);//Gamma Setting
	LCD_ILI9481_write_data_8(0x00);
	LCD_ILI9481_write_data_8(0x32);
	LCD_ILI9481_write_data_8(0x36);
	LCD_ILI9481_write_data_8(0x45);
	LCD_ILI9481_write_data_8(0x06);
	LCD_ILI9481_write_data_8(0x16);
	LCD_ILI9481_write_data_8(0x37);
	LCD_ILI9481_write_data_8(0x75);
	LCD_ILI9481_write_data_8(0x77);
	LCD_ILI9481_write_data_8(0x54);
	LCD_ILI9481_write_data_8(0x0C);
	LCD_ILI9481_write_data_8(0x00);

	//WriteCommand(0xB3); //Frame Memory Access and Interface Setting
	//WriteData08(0x02);
	//WriteData08(0x05);
	//WriteData08(0x07);
	//WriteData08(0x00);

	LCD_ILI9481_write_command(0x36); //Set_address_mode
	LCD_ILI9481_write_data_8(0x0B); // B7 B6 B5 B4 B3 0 B1 B0
	// B7 Page Address Order <<<! modificar aqui para reverse / invertir orientacion
	// B6 Column Address Order <<<! modificar aqui para reverse / invertir orientacion
	// B5 Page/Column Selection <<<! modificar aqui para reverse / invertir orientacion
	// B4 Vertical Order <<<! modificar aqui para reverse / invertir orientacion
	// B3 RGB/BGR Order
	// B2 Display data latch data order Set to ‘0’
	// B1 Horizontal Flip <<<! modificar aqui para reverse / invertir orientacion
	// B0 Vertical Flip <<<! modificar aqui para reverse / invertir orientacion

	LCD_ILI9481_write_command(0x3A); // Set_pixel_format
	LCD_ILI9481_write_data_8(0x55);

	LCD_ILI9481_write_command(0x2A); // Set_column_address
	LCD_ILI9481_write_data_8(0x00);
	LCD_ILI9481_write_data_8(0x00);
	LCD_ILI9481_write_data_8(0x01);
	LCD_ILI9481_write_data_8(0x3F);

	LCD_ILI9481_write_command(0x2B); // Set_page_address  SET TO 0 - 320
	LCD_ILI9481_write_data_8(0x00);
	LCD_ILI9481_write_data_8(0x00);
	LCD_ILI9481_write_data_8(0x01);
	LCD_ILI9481_write_data_8(0xE0);
	
	LCD_ILI9481_PAUSE_TICKS(2000);

	LCD_ILI9481_write_command(0x29); //DISPLAY ON

}//LCD_ILI9481_init



void LCD_ILI9481_write_command(uint8_t ui8_command) {
	uint32_t ui32_aux = 0;


	// LCD_ILI9481_CS_SET;
	// LCD_ILI9481_PAUSE_NOPS(1);

	LCD_ILI9481_CS_CLR;
	LCD_ILI9481_PAUSE_NOPS(1);
	  
	LCD_ILI9481_RS_CLR;
	LCD_ILI9481_PAUSE_NOPS(1);
	
	// clear all PORTA bits used to set the DB bits
	PORT->Group[0].OUTCLR.reg = ( P0_LCD_MASK_DB00 | P0_LCD_MASK_DB01 | P0_LCD_MASK_DB02 | P0_LCD_MASK_DB03 | P0_LCD_MASK_DB04 | P0_LCD_MASK_DB05 | P0_LCD_MASK_DB06 | P0_LCD_MASK_DB07 | P0_LCD_MASK_DB08 | P0_LCD_MASK_DB09 );
	ui32_aux = 0x00000000;
	if ((ui8_command&0x01)!=0) ui32_aux = ui32_aux | P0_LCD_MASK_DB00;
	if ((ui8_command&0x02)!=0) ui32_aux = ui32_aux | P0_LCD_MASK_DB01;
	if ((ui8_command&0x04)!=0) ui32_aux = ui32_aux | P0_LCD_MASK_DB02;
	if ((ui8_command&0x08)!=0) ui32_aux = ui32_aux | P0_LCD_MASK_DB03;
	if ((ui8_command&0x10)!=0) ui32_aux = ui32_aux | P0_LCD_MASK_DB04;
	if ((ui8_command&0x20)!=0) ui32_aux = ui32_aux | P0_LCD_MASK_DB05;
	if ((ui8_command&0x40)!=0) ui32_aux = ui32_aux | P0_LCD_MASK_DB06;
	if ((ui8_command&0x80)!=0) ui32_aux = ui32_aux | P0_LCD_MASK_DB07;
	// if ((ui16_db_value&0x0100)!=0) ui32_aux = ui32_aux | P0_LCD_MASK_DB08;
	// if ((ui16_db_value&0x0200)!=0) ui32_aux = ui32_aux | P0_LCD_MASK_DB09;
	PORT->Group[0].OUTSET.reg = ui32_aux;
	LCD_ILI9481_PAUSE_NOPS(1);

	LCD_ILI9481_WR_CLR;
	LCD_ILI9481_PAUSE_NOPS(1);
	  
	LCD_ILI9481_WR_SET;
	LCD_ILI9481_PAUSE_NOPS(1);
	  
	LCD_ILI9481_RS_SET;
	LCD_ILI9481_PAUSE_NOPS(1);
	  
}//LCD_ILI9481_write_command



void LCD_ILI9481_write_data_8(uint8_t ui8_data) {
	uint32_t ui32_aux = 0;
	

	// LCD_ILI9481_CS_CLR;
	// LCD_ILI9481_PAUSE_NOPS(1);
	
	LCD_ILI9481_RS_SET;
	LCD_ILI9481_PAUSE_NOPS(1);

	// clear all PORTA bits used to set the DB bits
	PORT->Group[0].OUTCLR.reg = ( P0_LCD_MASK_DB00 | P0_LCD_MASK_DB01 | P0_LCD_MASK_DB02 | P0_LCD_MASK_DB03 | P0_LCD_MASK_DB04 | P0_LCD_MASK_DB05 | P0_LCD_MASK_DB06 | P0_LCD_MASK_DB07 | P0_LCD_MASK_DB08 | P0_LCD_MASK_DB09 );
	ui32_aux = 0x00000000;
	if ((ui8_data&0x01)!=0) ui32_aux = ui32_aux | P0_LCD_MASK_DB00;
	if ((ui8_data&0x02)!=0) ui32_aux = ui32_aux | P0_LCD_MASK_DB01;
	if ((ui8_data&0x04)!=0) ui32_aux = ui32_aux | P0_LCD_MASK_DB02;
	if ((ui8_data&0x08)!=0) ui32_aux = ui32_aux | P0_LCD_MASK_DB03;
	if ((ui8_data&0x10)!=0) ui32_aux = ui32_aux | P0_LCD_MASK_DB04;
	if ((ui8_data&0x20)!=0) ui32_aux = ui32_aux | P0_LCD_MASK_DB05;
	if ((ui8_data&0x40)!=0) ui32_aux = ui32_aux | P0_LCD_MASK_DB06;
	if ((ui8_data&0x80)!=0) ui32_aux = ui32_aux | P0_LCD_MASK_DB07;
	// if ((ui16_db_value&0x0100)!=0) ui32_aux = ui32_aux | P0_LCD_MASK_DB08;
	// if ((ui16_db_value&0x0200)!=0) ui32_aux = ui32_aux | P0_LCD_MASK_DB09;
	PORT->Group[0].OUTSET.reg = ui32_aux;
	LCD_ILI9481_PAUSE_NOPS(1);

	LCD_ILI9481_WR_CLR;
	LCD_ILI9481_PAUSE_NOPS(1);
	
	LCD_ILI9481_WR_SET;
	LCD_ILI9481_PAUSE_NOPS(1);
	
}//LCD_ILI9481_write_data_8



void LCD_ILI9481_write_data_16(uint16_t ui16_data) {
	uint32_t ui32_aux = 0;
	
	
	// LCD_ILI9481_CS_CLR;
	// LCD_ILI9481_PAUSE_NOPS(1);
	
	LCD_ILI9481_RS_SET;
	LCD_ILI9481_PAUSE_NOPS(1);
	
	// set DB bits
	// clear all PORTA bits used to set the DB bits
	PORT->Group[0].OUTCLR.reg = ( P0_LCD_MASK_DB00 | P0_LCD_MASK_DB01 | P0_LCD_MASK_DB02 | P0_LCD_MASK_DB03 | P0_LCD_MASK_DB04 | P0_LCD_MASK_DB05 | P0_LCD_MASK_DB06 | P0_LCD_MASK_DB07 | P0_LCD_MASK_DB08 | P0_LCD_MASK_DB09 );
    ui32_aux = 0x00000000;
	if ((ui16_data&0x0001)!=0) ui32_aux = ui32_aux | P0_LCD_MASK_DB00;
	if ((ui16_data&0x0002)!=0) ui32_aux = ui32_aux | P0_LCD_MASK_DB01;
	if ((ui16_data&0x0004)!=0) ui32_aux = ui32_aux | P0_LCD_MASK_DB02;
	if ((ui16_data&0x0008)!=0) ui32_aux = ui32_aux | P0_LCD_MASK_DB03;
	if ((ui16_data&0x0010)!=0) ui32_aux = ui32_aux | P0_LCD_MASK_DB04;
	if ((ui16_data&0x0020)!=0) ui32_aux = ui32_aux | P0_LCD_MASK_DB05;
	if ((ui16_data&0x0040)!=0) ui32_aux = ui32_aux | P0_LCD_MASK_DB06;
	if ((ui16_data&0x0080)!=0) ui32_aux = ui32_aux | P0_LCD_MASK_DB07;
	if ((ui16_data&0x0100)!=0) ui32_aux = ui32_aux | P0_LCD_MASK_DB08;
	if ((ui16_data&0x0200)!=0) ui32_aux = ui32_aux | P0_LCD_MASK_DB09;
	PORT->Group[0].OUTSET.reg = ui32_aux;
					
	// clear all PORTB bits used to set the DB bits			
	PORT->Group[1].OUTCLR.reg = ( P1_LCD_MASK_DB10 | P1_LCD_MASK_DB11 | P1_LCD_MASK_DB12 | P1_LCD_MASK_DB13 | P1_LCD_MASK_DB14 | P1_LCD_MASK_DB15);	
	ui32_aux = 0x00000000;
	if ((ui16_data&0x0400)!=0) ui32_aux = ui32_aux | P1_LCD_MASK_DB10;
	if ((ui16_data&0x0800)!=0) ui32_aux = ui32_aux | P1_LCD_MASK_DB11;
	if ((ui16_data&0x1000)!=0) ui32_aux = ui32_aux | P1_LCD_MASK_DB12;
	if ((ui16_data&0x2000)!=0) ui32_aux = ui32_aux | P1_LCD_MASK_DB13;
	if ((ui16_data&0x4000)!=0) ui32_aux = ui32_aux | P1_LCD_MASK_DB14;
	if ((ui16_data&0x8000)!=0) ui32_aux = ui32_aux | P1_LCD_MASK_DB15;	
	PORT->Group[1].OUTSET.reg = ui32_aux;		
	
	LCD_ILI9481_PAUSE_NOPS(1);

	LCD_ILI9481_WR_CLR;
	LCD_ILI9481_PAUSE_NOPS(1);
	
	LCD_ILI9481_WR_SET;
	LCD_ILI9481_PAUSE_NOPS(1);
	
}//LCD_ILI9481_write_DB16



void LCD_ILI9481_fast_fill(uint16_t ui16_color, uint16_t ui16_x1, uint16_t ui16_y1, uint16_t ui16_x2, uint16_t ui16_y2){
	// uint32_t ui32_total = 0;
	// uint32_t ui32_cter = 0;
	uint16_t ui16_ax = 0;
	uint16_t ui16_ay = 0;
	uint16_t ui16_aux = 0;

	
	if (ui16_x1>ui16_x2){
		ui16_aux = ui16_x1;
		ui16_x1 = ui16_x2;
		ui16_x2 = ui16_aux;
	}
	ui16_ax = ui16_x2 - ui16_x1;
	
	if (ui16_y1>ui16_y2){
		ui16_aux = ui16_y1;
		ui16_y1 = ui16_y2;
		ui16_y2 = ui16_aux;
	}
	ui16_ay = ui16_y2 - ui16_y1;
	
	// send all the pixels colors
	LCD_ILI9481_set_xy(ui16_x1, ui16_y1, ui16_x2, ui16_y2);
	// ui32_total = ui16_ax * ui16_ay;
		
	uint16_t xx;
	uint16_t yy;
	for (xx=0; xx<=ui16_ax; xx++){
		for (yy=0; yy<=ui16_ay; yy++){	
			LCD_ILI9481_write_data_16(ui16_color);
		}
	}
	
}//LCD_ILI9481_fast_fill



void LCD_ILI9481_set_xy(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2){

	// Graphic memory row and page extent
	//		     SC                 EC
	//		    8:0               8:0
	//		   -------------------------
	// SP 8:0  |  |  |  |  |  |  |  |  |
	//	       -------------------------
	//		   |  |  |  |  |  |  |  |  |
	//		   -------------------------
	//		   |  |  |  |  |  |  |  |  |
	//		   -------------------------
	//		   |  |  |  |  |  |  |  |  |
	//		   -------------------------
	//		   |  |  |  |  |  |  |  |  |
	//		   -------------------------
	//		   |  |  |  |  |  |  |  |  |
	//		   -------------------------
	//	EP 8:0 |  |  |  |  |  |  |  |  |
	//		   -------------------------

	LCD_ILI9481_write_command(0x2a);//2ah: set column address:
	LCD_ILI9481_write_data_8(0x01&(y1>>8));//1st Parameter 0 0 0 0 0 0 0 SC8
	LCD_ILI9481_write_data_8(0xFF&y1);//2nd Parameter SC7 SC6 SC5 SC4 SC3 SC2 SC1 SC0
	LCD_ILI9481_write_data_8(0x01&(y2>>8));//3rd Parameter 0 0 0 0 0 0 0 EC8
	LCD_ILI9481_write_data_8(0xFF&y2);//4th Parameter EC7 EC6 EC5 EC4 EC3 EC2 EC1 EC0
	//This command defines the column extent of the frame memory accessed by the host processor with the read_memory_continue and write_memory_continue commands. No status bits are changed.
	
	LCD_ILI9481_write_command(0x2b); // 2b: set page address
	LCD_ILI9481_write_data_8(0x01&(x1>>8));  // 1st Parameter 0 0 0 0 0 0 0 SP8
	LCD_ILI9481_write_data_8(0xFF&x1);	   // 2nd Parameter SP7 SP6 SP5 SP4 SP3 SP2 SP1 SP0
	LCD_ILI9481_write_data_8(0x01&(x2>>8));  // 3rd Parameter 0 0 0 0 0 0 0 EP8
	LCD_ILI9481_write_data_8(0xFF&x2);	   // 4th Parameter EP7 EP6 EP5 EP4 EP3 EP2 EP1 EP0
	// This command defines the page extent of the frame memory accessed by the host processor with the write_memory_continue and read_memory_continue command. No status bits are changed
	
	LCD_ILI9481_write_command(0x2c); // 2c: write memory start

}//LCD_ILI9481_set_xy



void LCD_ILI9481_set_pixel(uint16_t x, uint16_t y, uint16_t ui16_color){

	LCD_ILI9481_set_xy(x, y, x, y);
	LCD_ILI9481_write_data_16(ui16_color);

}//LCD_ILI9481_set_pixel

#endif //#ifdef LCD_IS_ILI9481
	