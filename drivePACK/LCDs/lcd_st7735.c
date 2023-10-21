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

#include "lcd_st7735.h"
#include "../timing.h"

#ifdef LCD_IS_ST7735

// some models of ST7735 based LCD displays seem to vertically and horizontally shift the image
// and an extra offset vertical and horizontal offset must be added to compensate this shifting
int LCD_ST7735_x_offset = 0;
int LCD_ST7735_y_offset = 0;

void LCD_ST7735_init_1(){
	TIMING_TICKS tticks_timer;

	//ST7735R Frame Rate
	LCD_ST7735_write_command(0xB1); // Command: FRMCTR1 B1h: Frame Rate Control (In normal mode/ Full colors)
	LCD_ST7735_write_data(0x01);
	LCD_ST7735_write_data(0x2C);
	LCD_ST7735_write_data(0x2D);
	
	LCD_ST7735_write_command(0xB2); // Command: FRMCTR2 B2h: Frame Rate Control (In Idle mode/ 8-colors)
	LCD_ST7735_write_data(0x01);
	LCD_ST7735_write_data(0x2C);
	LCD_ST7735_write_data(0x2D);
	
	LCD_ST7735_write_command(0xB3); // Command: FRMCTR3 B3h: Frame Rate Control (In Partial mode/ full colors)
	LCD_ST7735_write_data(0x01);
	LCD_ST7735_write_data(0x2C);
	LCD_ST7735_write_data(0x2D);
	LCD_ST7735_write_data(0x01);
	LCD_ST7735_write_data(0x2C);
	LCD_ST7735_write_data(0x2D);

	LCD_ST7735_write_command(0xB4); //Command: INVCTR B4h: Display Inversion Control
	LCD_ST7735_write_data(0x07);

	//ST7735R Power Sequence
	LCD_ST7735_write_command(0xC0); // Command: PWCTR1 C0h: Power Control 1
	LCD_ST7735_write_data(0xA2);
	LCD_ST7735_write_data(0x02);
	// ??? LCD_ST7735_write_data(0x84);
	
	LCD_ST7735_write_command(0xC1); // Command: PWCTR2 C1h: Power Control 2
	LCD_ST7735_write_data(0xC5);
	
	LCD_ST7735_write_command(0xC2); // Command: PWCTR3 C2h: Power Control 3 (in Normal mode/ Full colors)
	LCD_ST7735_write_data(0x0A);
	LCD_ST7735_write_data(0x00);
	
	LCD_ST7735_write_command(0xC3); // Command: PWCTR4 C3h: Power Control 4 (in Idle mode/ 8-colors)
	LCD_ST7735_write_data(0x8A);
	LCD_ST7735_write_data(0x2A);

	LCD_ST7735_write_command(0xC4); // Command: PWCTR5 C4h: Power Control 5 (in Partial mode/ full-colors)
	LCD_ST7735_write_data(0x8A);
	LCD_ST7735_write_data(0xEE);

	LCD_ST7735_write_command(0xC5); // Command: VMCTR1 C5h: VCOM Control 1
	LCD_ST7735_write_data(0x0E);

	LCD_ST7735_write_command(0x36); // Command: MADCTL 36h: Memory Data Access Control
	LCD_ST7735_write_data(0x84);
	
	// B7 MY  Page Address Order <<<! modificar aqui para reverse / invertir orientacion
	// B6 MX  Column Address Order <<<! modificar aqui para reverse / invertir orientacion
	// B5 MV  Page/Column Selection <<<! modificar aqui para reverse / invertir orientacion
	// B4 ML  Row/Column Exchange <<<! modificar aqui para reverse / invertir orientacion
	// B3 RGB RGB/BGR Order
	// B2 MH  Horizontal Refresh Order
	// B1 --
	// B0 --

	//ST7735R Gamma Sequence
	LCD_ST7735_write_command(0xe0); // Command: GMCTRP1 E0h: Gamma (‘+’polarity) Correction Characteristics Setting
	LCD_ST7735_write_data(0x0f);
	LCD_ST7735_write_data(0x1a);
	LCD_ST7735_write_data(0x0f);
	LCD_ST7735_write_data(0x18);
	LCD_ST7735_write_data(0x2f);
	LCD_ST7735_write_data(0x28);
	LCD_ST7735_write_data(0x20);
	LCD_ST7735_write_data(0x22);
	LCD_ST7735_write_data(0x1f);
	LCD_ST7735_write_data(0x1b);
	LCD_ST7735_write_data(0x23);
	LCD_ST7735_write_data(0x37);
	LCD_ST7735_write_data(0x00);
	LCD_ST7735_write_data(0x07);
	LCD_ST7735_write_data(0x02);
	LCD_ST7735_write_data(0x10);
	
	LCD_ST7735_write_command(0xe1); // Command: GMCTRN1 E1h:  Gamma ‘-’polarity Correction Characteristics Setting
	LCD_ST7735_write_data(0x0f);
	LCD_ST7735_write_data(0x1b);
	LCD_ST7735_write_data(0x0f);
	LCD_ST7735_write_data(0x17);
	LCD_ST7735_write_data(0x33);
	LCD_ST7735_write_data(0x2c);
	LCD_ST7735_write_data(0x29);
	LCD_ST7735_write_data(0x2e);
	LCD_ST7735_write_data(0x30);
	LCD_ST7735_write_data(0x30);
	LCD_ST7735_write_data(0x39);
	LCD_ST7735_write_data(0x3f);
	LCD_ST7735_write_data(0x00);
	LCD_ST7735_write_data(0x07);
	LCD_ST7735_write_data(0x03);
	LCD_ST7735_write_data(0x10);

	LCD_ST7735_write_command(0x2a); // Command: CASET 2Ah: Column Address Set, the LCD_ST7735_set_xy() function to may ovewrite this command
	LCD_ST7735_write_data(0x00);
	LCD_ST7735_write_data(0x00);
	LCD_ST7735_write_data(0x00);
	LCD_ST7735_write_data(0x7f+LCD_ST7735_x_offset); // 127 + extra offset
	
	LCD_ST7735_write_command(0x2b); // Command: RASET 2Bh: Row Address Set, the LCD_ST7735_set_xy() function to may ovewrite this command
	LCD_ST7735_write_data(0x00);
	LCD_ST7735_write_data(0x00);
	LCD_ST7735_write_data(0x00);
	LCD_ST7735_write_data(0x9f+LCD_ST7735_y_offset); // 159 + extra offset

	LCD_ST7735_write_command(0xF0); // Command: EXTCTRL F0h: Extension Command Control
	LCD_ST7735_write_data(0x01);
	
	LCD_ST7735_write_command(0xF6); // Command: Disable ram power save mode
	LCD_ST7735_write_data(0x00);

	LCD_ST7735_write_command(0x3A); // Command: 3Ah interface pixel format
	LCD_ST7735_write_data(0x05);    // 05h: 65k mode: RGB 5bit 6bit 5bits
	
	LCD_ST7735_write_command(0x29); // Command: DISPON 29h: Display On

}//LCD_ST7735_init_1


void LCD_ST7735_init_COMANDS_1A(){

	LCD_ST7735_write_command(0x3A); // Command: COLMOD 3Ah: Interface Pixel Format
    LCD_ST7735_write_data(0x05);

	LCD_ST7735_write_command(0xB1); // Command: FRMCTR1 B1h: Frame Rate Control (In normal mode/ Full colors)
	LCD_ST7735_write_data(0x00);    // fastest refresh
	LCD_ST7735_write_data(0x06);    // 6 lines front porch
	LCD_ST7735_write_data(0x03);    // 3 lines back porch
	
	LCD_ST7735_write_command(0x36); // Command: MADCTL 36h: Memory Data Access Control
	LCD_ST7735_write_data(0x84);
	// B7 MY  Page Address Order <<<! modificar aqui para reverse / invertir orientacion
	// B6 MX  Column Address Order <<<! modificar aqui para reverse / invertir orientacion
	// B5 MV  Page/Column Selection <<<! modificar aqui para reverse / invertir orientacion
	// B4 ML  Row/Column Exchange <<<! modificar aqui para reverse / invertir orientacion
	// B3 RGB RGB/BGR Order
	// B2 MH  Horizontal Refresh Order
	// B1 --
	// B0 --	

	LCD_ST7735_write_command(0xB6); // Command: DISSET5 (B6h): Display Function set 5
	LCD_ST7735_write_data(0x15);    // 1 clk cycle nonoverlap, 2 cycle gate  rise, 3 cycle osc equalize
	LCD_ST7735_write_data(0x02);    // Fix on VTL

	LCD_ST7735_write_command(0xB4); // Command: INVCTR B4h: Display Inversion Control
	LCD_ST7735_write_data(0x00);    // Line inversion

	//ST7735R Power Sequence
	LCD_ST7735_write_command(0xC0); // Command: PWCTR1 C0h: Power Control 1
	LCD_ST7735_write_data(0x02);    //  GVDD = 4.7V
	LCD_ST7735_write_data(0x70);    //  1.0uA
		
	LCD_ST7735_write_command(0xC1); // Command: PWCTR2 C1h: Power Control 2
	LCD_ST7735_write_data(0x05);    //  VGH = 14.7V, VGL = -7.35V
	
	LCD_ST7735_write_command(0xC2); // Command: PWCTR3 C2h: Power Control 3 (in Normal mode/ Full colors)
	LCD_ST7735_write_data(0x01);    // Opamp current small
	LCD_ST7735_write_data(0x02);    // Boost frequency

	LCD_ST7735_write_command(0xC5); // Command: VMCTR1 C5h: VCOM Control 1
	LCD_ST7735_write_data(0x3C);    // VCOMH = 4V
    LCD_ST7735_write_data(0x38);    // VCOML = -1.1V

	//ST7735R Gamma Sequence
	LCD_ST7735_write_command(0xe0); // Command: GMCTRP1 E0h: Gamma (‘+’polarity) Correction Characteristics Setting
	LCD_ST7735_write_data(0x09);    // Not sure what these config values mean
	LCD_ST7735_write_data(0x16);
	LCD_ST7735_write_data(0x09);
	LCD_ST7735_write_data(0x20);
	LCD_ST7735_write_data(0x21);
	LCD_ST7735_write_data(0x1B);
	LCD_ST7735_write_data(0x13);
	LCD_ST7735_write_data(0x19);
	LCD_ST7735_write_data(0x17);
	LCD_ST7735_write_data(0x15);
	LCD_ST7735_write_data(0x1E);
	LCD_ST7735_write_data(0x2B);
	LCD_ST7735_write_data(0x04);
	LCD_ST7735_write_data(0x05);
	LCD_ST7735_write_data(0x02);
	LCD_ST7735_write_data(0x0E);
		
	LCD_ST7735_write_command(0xe1); // Command: GMCTRN1 E1h:  Gamma ‘-’polarity Correction Characteristics Setting 
	LCD_ST7735_write_data(0x0B);    // Not sure what these config values mean
	LCD_ST7735_write_data(0x14);
	LCD_ST7735_write_data(0x08);
	LCD_ST7735_write_data(0x1E);
	LCD_ST7735_write_data(0x22);
	LCD_ST7735_write_data(0x1D);
	LCD_ST7735_write_data(0x18);
	LCD_ST7735_write_data(0x1E);
	LCD_ST7735_write_data(0x1B);
	LCD_ST7735_write_data(0x1A);
	LCD_ST7735_write_data(0x24);
	LCD_ST7735_write_data(0x2B);
	LCD_ST7735_write_data(0x06);
	LCD_ST7735_write_data(0x06);
	LCD_ST7735_write_data(0x02);
	LCD_ST7735_write_data(0x0F);

	LCD_ST7735_write_command(0x2a); // Command: CASET 2Ah: Column Address Set, the LCD_ST7735_set_xy() function to may ovewrite this command
	LCD_ST7735_write_data(0x00);
	LCD_ST7735_write_data(0x02);
	LCD_ST7735_write_data(0x00);
	LCD_ST7735_write_data(0x81); // 128-1=127=0x7f
		
	LCD_ST7735_write_command(0x2b); // Command: RASET 2Bh: Row Address Set, the LCD_ST7735_set_xy() function to may ovewrite this command
	LCD_ST7735_write_data(0x00);
	LCD_ST7735_write_data(0x02);
	LCD_ST7735_write_data(0x00);
	LCD_ST7735_write_data(0x81); // 160-1=159=0x9f

    LCD_ST7735_write_command(0x13); // Command: NORON  13h: Normal Display Mode On
		
	LCD_ST7735_write_command(0x29); // Command: DISPON 29h: Display On

}//LCD_ST7735_init_COMANDS_1A



void LCD_ST7735_init_COMANDS_1B(){

	LCD_ST7735_write_command(0xB1); // Command: FRMCTR1 B1h: Frame Rate Control (In normal mode/ Full colors)
	LCD_ST7735_write_data(0x01);    // Rate = fosc/(1x2+40) * (LINE+2C+2D)
	LCD_ST7735_write_data(0x2C);    
	LCD_ST7735_write_data(0x2D);    
	
	LCD_ST7735_write_command(0xB2); // Command: FRMCTR2 B2h: Frame Rate Control (In Idle mode/ 8-colors)
	LCD_ST7735_write_data(0x01);    // Rate = fosc/(1x2+40) * (LINE+2C+2D)
	LCD_ST7735_write_data(0x2C);
	LCD_ST7735_write_data(0x2D);	
	
	LCD_ST7735_write_command(0xB3); // Command: FRMCTR3 B3h: Frame Rate Control (In Partial mode/ full colors)
	LCD_ST7735_write_data(0x01);    //  Dot inversion mode
	LCD_ST7735_write_data(0x2C);
	LCD_ST7735_write_data(0x2D);
	LCD_ST7735_write_data(0x01);    // Line inversion mode
	LCD_ST7735_write_data(0x2C);
	LCD_ST7735_write_data(0x2D);

	LCD_ST7735_write_command(0xB4); // Command: INVCTR B4h: Display Inversion Control
	LCD_ST7735_write_data(0x07);    // No inversion

	LCD_ST7735_write_command(0xC0); // Command: PWCTR1 C0h: Power Control 1
	LCD_ST7735_write_data(0xA2);    // 
	LCD_ST7735_write_data(0x02);    // -4.6V
	LCD_ST7735_write_data(0x84);    // AUTO mode
	
	LCD_ST7735_write_command(0xC1); // Command: PWCTR2 C1h: Power Control 2
	LCD_ST7735_write_data(0xC5);    // VGH25 = 2.4C VGSEL = -10 VGH = 3 * AVDD
	
	LCD_ST7735_write_command(0xC2); // Command: PWCTR3 C2h: Power Control 3 (in Normal mode/ Full colors)
	LCD_ST7735_write_data(0x0A);    // Opamp current small
	LCD_ST7735_write_data(0x00);    // Boost frequency

	LCD_ST7735_write_command(0xC3); // Command: PWCTR4 C3h: Power Control 4 (in Idle mode/ 8-colors)
	LCD_ST7735_write_data(0x8A);    // BCLK/2, Opamp current small & Medium low
	LCD_ST7735_write_data(0x2A);	
	
	LCD_ST7735_write_command(0xC4); // Command: PWCTR5 C4h: Power Control 5 (in Partial mode/ full-colors)
	LCD_ST7735_write_data(0x8A);
	LCD_ST7735_write_data(0xEE);	
	
	LCD_ST7735_write_command(0xC5); // Command: VMCTR1 C5h: VCOM Control 1
	LCD_ST7735_write_data(0x0E);

	LCD_ST7735_write_command(0x20); // Command: INVOFF (20h): Display Inversion Off

	LCD_ST7735_write_command(0x36); // Command: MADCTL 36h: Memory Data Access Control
	LCD_ST7735_write_data(0x84);	
	// B7 MY  Page Address Order <<<! modificar aqui para reverse / invertir orientacion
	// B6 MX  Column Address Order <<<! modificar aqui para reverse / invertir orientacion
	// B5 MV  Page/Column Selection <<<! modificar aqui para reverse / invertir orientacion
	// B4 ML  Row/Column Exchange <<<! modificar aqui para reverse / invertir orientacion
	// B3 RGB RGB/BGR Order
	// B2 MH  Horizontal Refresh Order
	// B1 --
	// B0 --

	LCD_ST7735_write_command(0x3A); // Command: COLMOD 3Ah: Interface Pixel Format
	LCD_ST7735_write_data(0x05);

}//LCD_ST7735_init_COMANDS_1B



void LCD_ST7735_init_COMANDS_2A(){
	TIMING_TICKS tticks_timer;

	LCD_ST7735_write_command(0x2a); // Command: CASET 2Ah: Column Address Set, the LCD_ST7735_set_xy() function to may ovewrite this command
	LCD_ST7735_write_data(0x00);
	LCD_ST7735_write_data(0x02);
	LCD_ST7735_write_data(0x00);
	LCD_ST7735_write_data(0x7F+2); // 128-1=127=0x7f
	
	LCD_ST7735_write_command(0x2b); // Command: RASET 2Bh: Row Address Set, the LCD_ST7735_set_xy() function to may ovewrite this command
	LCD_ST7735_write_data(0x00);
	LCD_ST7735_write_data(0x01);
	LCD_ST7735_write_data(0x00);
	LCD_ST7735_write_data(0x9F+1); // 160-1=159=0x9f

}//LCD_ST7735_init_COMANDS_2A



void LCD_ST7735_init_COMANDS_2B(){
	TIMING_TICKS tticks_timer;

	LCD_ST7735_write_command(0x2a); // Command: CASET 2Ah: Column Address Set, the LCD_ST7735_set_xy() function to may ovewrite this command
	LCD_ST7735_write_data(0x00);
	LCD_ST7735_write_data(0x00);
	LCD_ST7735_write_data(0x00);
	LCD_ST7735_write_data(0x7F);
	
	LCD_ST7735_write_command(0x2b); // Command: RASET 2Bh: Row Address Set, the LCD_ST7735_set_xy() function to may ovewrite this command
	LCD_ST7735_write_data(0x00);
	LCD_ST7735_write_data(0x01);
	LCD_ST7735_write_data(0x00);
	LCD_ST7735_write_data(0x9F);

}//LCD_ST7735_init_COMANDS_2B



void LCD_ST7735_init_COMANDS_3(){
	TIMING_TICKS tticks_timer;
    // Rcmd3:

	//ST7735R Gamma Sequence
	LCD_ST7735_write_command(0xe0); // Command: GMCTRP1 E0h: Gamma (‘+’polarity) Correction Characteristics Setting
	LCD_ST7735_write_data(0x02);    // Not sure what these config values mean
	LCD_ST7735_write_data(0x1C);
	LCD_ST7735_write_data(0x07);
	LCD_ST7735_write_data(0x12);
	LCD_ST7735_write_data(0x37);
	LCD_ST7735_write_data(0x32);
	LCD_ST7735_write_data(0x29);
	LCD_ST7735_write_data(0x2D);
	LCD_ST7735_write_data(0x29);
	LCD_ST7735_write_data(0x25);
	LCD_ST7735_write_data(0x2B);
	LCD_ST7735_write_data(0x39);
	LCD_ST7735_write_data(0x00);
	LCD_ST7735_write_data(0x01);
	LCD_ST7735_write_data(0x03);
	LCD_ST7735_write_data(0x10);
	
	LCD_ST7735_write_command(0xe1); // Command: GMCTRN1 E1h:  Gamma ‘-’polarity Correction Characteristics Setting
	LCD_ST7735_write_data(0x03);    // Not sure what these config values mean
	LCD_ST7735_write_data(0x1d);
	LCD_ST7735_write_data(0x07);
	LCD_ST7735_write_data(0x06);
	LCD_ST7735_write_data(0x2E);
	LCD_ST7735_write_data(0x2C);
	LCD_ST7735_write_data(0x29);
	LCD_ST7735_write_data(0x2D);
	LCD_ST7735_write_data(0x2E);
	LCD_ST7735_write_data(0x2E);
	LCD_ST7735_write_data(0x37);
	LCD_ST7735_write_data(0x3F);
	LCD_ST7735_write_data(0x00);
	LCD_ST7735_write_data(0x00);
	LCD_ST7735_write_data(0x02);
	LCD_ST7735_write_data(0x10);

    LCD_ST7735_write_command(0x13); // Command: NORON  13h: Normal Display Mode On
    
    LCD_ST7735_write_command(0x29); // Command: DISPON 29h: Display On

}//LCD_ST7735_init_COMANDS_3


void LCD_ST7735_init(){
	TIMING_TICKS tticks_timer;
	
	// start with all lines to '1'
	LCD_ST7735_CS_SET;
	LCD_ST7735_RST_SET;
	// LCD_ST7735_WR_SET;
	LCD_ST7735_RS_SET;
	
	// RESET
	LCD_ST7735_PAUSE_TICKS(2000);
	LCD_ST7735_RST_CLR;
	LCD_ST7735_PAUSE_TICKS(2000);
	LCD_ST7735_RST_SET;
	LCD_ST7735_PAUSE_TICKS(2000);

	LCD_ST7735_write_command(0x11); // Command: SLPOUT 11h: Sleep OutExit Sleep Mode
	LCD_ST7735_PAUSE_TICKS(2000);

    // Option:INIT original initialization sequence: my own initialization sequence
    LCD_ST7735_x_offset = 1;
    LCD_ST7735_y_offset = 2;
	LCD_ST7735_init_1();

    // Option: INITB initialization sequence: based ingithub.com/Bodmer/ INITB sequence
    // LCD_ST7735_init_COMANDS_1A();
	
	// Option: INIT_GREENTAB initialization sequence: based in github.com/Bodmer/ GREENTAB sequence
	// LCD_ST7735_init_COMANDS_1B();
	// LCD_ST7735_init_COMANDS_2A();
	// LCD_ST7735_init_COMANDS_3();

	// Option: INIT_GREENTAB2 initialization sequence: based in github.com/Bodmer/ GREENTAB2 sequence
	// LCD_ST7735_init_COMANDS_1B();
	// LCD_ST7735_init_COMANDS_2A();
	// LCD_ST7735_write_command(0x36); // Command: MADCTL 36h: Memory Data Access Control
	// LCD_ST7735_write_data(0xC0);
	// LCD_ST7735_init_COMANDS_3();
    
	// Option: INITR_REDTAB initialization sequence: based in github.com/Bodmer/ REDTAB sequence
	// LCD_ST7735_init_COMANDS_1B();
	// LCD_ST7735_init_COMANDS_2B();
	// LCD_ST7735_init_COMANDS_3();

	// Option: INITR_BLACKTAB initialization sequence: based in github.com/Bodmer/ INITR_BLACKTAB sequence
	// LCD_ST7735_init_COMANDS_1B();
	// LCD_ST7735_write_command(0x36); // Command: MADCTL 36h: Memory Data Access Control
	// LCD_ST7735_write_data(0xC0);

}//LCD_ST7735_init



uint8_t LCD_ST7735_write_data(uint8_t ui8_data_to_send){

	LCD_ST7735_CS_CLR;
	LCD_ST7735_PAUSE_NOPS(1);
	
	LCD_ST7735_RS_SET;
	LCD_ST7735_PAUSE_NOPS(1);

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
	
	LCD_ST7735_CS_SET;
	LCD_ST7735_PAUSE_NOPS(1);
	
	return ui8_data_to_send; // return the value read during transmission
	
}// LCD_ST7735_write_data



uint8_t LCD_ST7735_write_command(uint8_t ui8_command_to_send){

	LCD_ST7735_CS_CLR;
	LCD_ST7735_PAUSE_NOPS(1);
	
	LCD_ST7735_RS_CLR;
	LCD_ST7735_PAUSE_NOPS(1);

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
	
	LCD_ST7735_CS_SET;
	
	return ui8_command_to_send; // return the value read during transmission
	
}// LCD_ST7735_write_data



void LCD_ST7735_fast_fill(uint16_t ui16_color, uint16_t ui16_x1, uint16_t ui16_y1, uint16_t ui16_x2, uint16_t ui16_y2){
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
	LCD_ST7735_set_xy(ui16_x1, ui16_y1, ui16_x2, ui16_y2);
	// ui32_total = ui16_ax * ui16_ay;
	
	uint16_t xx;
	uint16_t yy;
	for (xx=0; xx<=ui16_ax; xx++){
		for (yy=0; yy<=ui16_ay; yy++){			
		   LCD_ST7735_write_data((uint8_t)((ui16_color>>8)&0x00FF));
		   LCD_ST7735_write_data((uint8_t)(ui16_color&0x00FF));			
		}
	}
	
}//LCD_ST7735_fast_fill



void LCD_ST7735_set_xy(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2){

	
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

    // add the the configured extra offset to compensate he display shifting
    x1 = x1 + LCD_ST7735_x_offset;
    x2 = x2 + LCD_ST7735_x_offset;
    y1 = y1 + LCD_ST7735_y_offset;
    y2 = y2 + LCD_ST7735_y_offset;

	LCD_ST7735_write_command(0x2a); // Command: CASET 2Ah: Column Address Set, the LCD_ST7735_set_xy() function to may ovewrite this command
	LCD_ST7735_write_data(0x01&(y1>>8));//1st Parameter 0 0 0 0 0 0 0 SC8
	LCD_ST7735_write_data(0xFF&y1);//2nd Parameter SC7 SC6 SC5 SC4 SC3 SC2 SC1 SC0
	LCD_ST7735_write_data(0x01&(y2>>8));//3rd Parameter 0 0 0 0 0 0 0 EC8
	LCD_ST7735_write_data(0xFF&y2);//4th Parameter EC7 EC6 EC5 EC4 EC3 EC2 EC1 EC0
	//This command defines the column extent of the frame memory accessed by the host processor with the read_memory_continue and write_memory_continue commands. No status bits are changed.
	
	LCD_ST7735_write_command(0x2b); // Command: RASET 2Bh: Row Address Set, the LCD_ST7735_set_xy() function to may ovewrite this command
	LCD_ST7735_write_data(0x01&(x1>>8));// 1st Parameter 0 0 0 0 0 0 0 SP8
	LCD_ST7735_write_data(0xFF&x1);// 2nd Parameter SP7 SP6 SP5 SP4 SP3 SP2 SP1 SP0
	LCD_ST7735_write_data(0x01&(x2>>8));// 3rd Parameter 0 0 0 0 0 0 0 EP8
	LCD_ST7735_write_data(0xFF&x2);	// 4th Parameter EP7 EP6 EP5 EP4 EP3 EP2 EP1 EP0
	// This command defines the page extent of the frame memory accessed by the host processor with the write_memory_continue and read_memory_continue command. No status bits are changed
	
	LCD_ST7735_write_command(0x2c); // 2c: write memory start

}//LCD_ST7735_set_xy



void LCD_ST7735_set_pixel(uint16_t x, uint16_t y, uint16_t ui16_color){

	LCD_ST7735_set_xy(x, y, x, y);	
	LCD_ST7735_write_data((uint8_t)((ui16_color>>8)&0x00FF));
	LCD_ST7735_write_data((uint8_t)(ui16_color&0x00FF));

}//LCD_ST7735_set_pixel

#endif //#ifdef LCD_IS_ST7735