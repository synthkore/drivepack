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
#include "sam.h"
#include "lcdio.h"

#ifdef LCD_IS_ST7565

#ifndef LCD_ST7565_H_
#define LCD_ST7565_H_

#define LCD_ST7565_SIZE_X 128
#define LCD_ST7565_SIZE_Y 64

#define LCD_ST7565_COLS_X LCD_ST7565_SIZE_X
#if ((LCD_ST7565_SIZE_Y%8)!=0)
    #define LCD_ST7565_PAGES_Y (LCD_ST7565_SIZE_Y/8)+1
#else
    #define LCD_ST7565_PAGES_Y (LCD_ST7565_SIZE_Y/8)
#endif


#define LCD_ST7565_VC_ON 0x2C       //1010
#define LCD_ST7565_VR_ON 0x2A
#define LCD_ST7565_VF_ON 0x29       //1001

#define UNKNOWN_ASCII_SYMBOL '?'

#define LCD_ST7565_PAUSE_NOPS(nops_number)   __asm__ __volatile__ ("nop;");  // JBR TBD 2022-24-03 Check that macro, nops_number is useless here
#define LCD_ST7565_PAUSE_TICKS(ticks_number) tticks_timer = TIMING_SetTicksDelay(ticks_number); while (TIMING_CheckTicksDelayReached(tticks_timer)==FALSE){1;};

#define LCD_ST7565_RS_SET  PORT->Group[1].OUTSET.reg = P1_LCD_MASK_LCD_RS
#define LCD_ST7565_RS_CLR  PORT->Group[1].OUTCLR.reg = P1_LCD_MASK_LCD_RS
#define LCD_ST7565_WR_SET  PORT->Group[1].OUTSET.reg = P1_LCD_MASK_LCD_WR
#define LCD_ST7565_WR_CLR  PORT->Group[1].OUTCLR.reg = P1_LCD_MASK_LCD_WR
#define LCD_ST7565_CS_SET  PORT->Group[1].OUTSET.reg = P1_LCD_MASK_LCD_CS
#define LCD_ST7565_CS_CLR  PORT->Group[1].OUTCLR.reg = P1_LCD_MASK_LCD_CS
#define LCD_ST7565_RST_SET PORT->Group[1].OUTSET.reg = P1_LCD_MASK_LCD_RST
#define LCD_ST7565_RST_CLR PORT->Group[1].OUTCLR.reg = P1_LCD_MASK_LCD_RST


#define LCD_ST7565_CMD_DISPLAY_OFF              0xAE //0b10101110
#define LCD_ST7565_CMD_DISPLAY_ON 				0xAF //0b10101111
#define LCD_ST7565_CMD_INITIAL_DISPLAY_LINE     0x40 //0b01000000
#define LCD_ST7565_CMD_SET_PAGE                 0xB0 //0b10110000
#define LCD_ST7565_CMD_SET_COL_MSB              0x10 //0b00010000
#define LCD_ST7565_CMD_SET_COL_LSB              0x00 //0b00000000
#define LCD_ST7565_CMD_ADC_SELECT				0xA0 //0b10100000
#define LCD_ST7565_CMD_REVERSE_DISPLAY_NORMAL	0xA6 //0b10100110
#define LCD_ST7565_CMD_REVERSE_DISPLAY_INVERT	0xA7 //0b10100111 invierte el negro por el blanco y el blanco por el negro
#define LCD_ST7565_CMD_ENTIRE_DISPLAY_OFF		0xA4 //0b10100100
#define LCD_ST7565_CMD_ENTIRE_DISPLAY_ON		0xA5 //0b10100101
#define LCD_ST7565_CMD_LCDG_BIAS_SELECT		    0xA2 //0b10100010
#define LCD_ST7565_CMD_POWER_CONTROL			0x28 //0x2F //0b00101000
#define LCD_ST7565_CMD_REGULATOR_RES_SELECT     0x23 //0b00100000
#define LCD_ST7565_CMD_SET_REF_VOLT_MODE		0x81 //0b10000001
#define LCD_ST7565_CMD_SET_REF_VOLT_REGISTER	0x20 //0b00000000
#define LCD_ST7565_CMD_LCDG_CONVERTER  	        0x2C //0b00101100
#define LCD_ST7565_CMD_LCDG_REGULATOR			0x2E //0b00101110
#define LCD_ST7565_CMD_LCDG_FOLLOWER			0x2F //0b00101111
#define LCD_ST7565_CMD_RESET					0xE2 //0b11100010
#define LCD_ST7565_CMD_SH_SELECT				0xC0 //0b11000000
#define LCD_ST7565_CMD_TEST_INSTR1				0x90 //0b10010000
#define LCD_ST7565_CMD_TEST_INSTR2				0xF0 //0b11110000
#define LCD_ST7565_CMD_RMW                      0xE0
#define LCD_ST7565_CMD_RMW_CLEAR                0xEE

#define LCD_ST7565_TRANSPARENT	                0xFFFE
#define LCD_ST7565_WHITE                        0x0000
#define LCD_ST7565_BLACK                        0x0001


// Description:
//  Sends the initialization commands sequence to the LCD 
// Receives:
// Returns:
//   By value:
// Note:
void LCD_ST7565_init();

// Description:
// Sends a data byte to the LCD through the SPI
// Receives:
// Returns:
//   By value:
// Note:
uint8_t LCD_ST7565_write_data(uint8_t ui8_data_to_send);

// Description:
// Sends a command byte to the LCD through the SPI
// Receives:
// Returns:
//   By value:
// Note:
uint8_t LCD_ST7565_write_command(uint8_t ui8_command_to_send);

// Description:
// Fills the frame buffer pages with the specified ui8_page_pattern
// Receives:
// Returns:
//   By value:
// Note:
void LCD_ST7565_buffer_fill(uint8_t ui8_page_pattern);

// Description:
// Fills the frame buffer pages with the specified ui8_page_pattern
// Receives:
// Returns:
//   By value:
// Note:
void LCD_ST7565_buffer_fast_fill(uint16_t ui16_pattern, uint16_t ui16_x1, uint16_t ui16_y1, uint16_t ui16_x2, uint16_t ui16_y2);

// Description:
// Sets the specified pixel in the frame buffer and then in the LCD with 
// the specified color ( 1=black, 0=white). Updates the 8 bits of the pixel
// page and then rewrites them in the LCD to update the screen.
// Receives:
// Returns:
//   By value:
// Note:
void LCD_ST7565_set_pixel(uint8_t ui8_x, uint8_t ui8_y, uint8_t ui8_bw);

// Description:
// Sets the specified pixel in the frame buffer with the specified color ( 1=black
// 0=white ).
// Receives:
// Returns:
//   By value:
// Note:
void LCD_ST7565_buffer_set_pixel(uint8_t ui8_x, uint8_t ui8_y, uint8_t ui8_bw);

// Description:
// Updates the the LCD with the content of the frame buffer.
// Receives:
// Returns:
//   By value:
// Note:
void LCD_ST7565_buffer_refresh();

// Description:
// Receives:
// Returns:
//   By value:
// Note:
void LCD_ST7565_clear(uint8_t ui8_page_pattern);

#endif /* LCD_ST7565_H_ */

#endif //#ifdef LCD_IS_ST7565
