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

#ifdef LCD_IS_ST7735

#ifndef LCD_ST7735_H_
#define LCD_ST7735_H_

// IMPORTANT: these DEFINES DEPPEND on the GPIO/S CONFIGURATION:
#define P1_LCD_MASK_LCD_RS      0x00000040 // PORTB-1: PB06 - LCD RS
#define P1_LCD_MASK_LCD_CS      0x00000020 // PORTB-1: PB05 - LCD CS
#define P1_LCD_MASK_LCD_WR      0x00000000 // NOT USED in ST7735
#define P1_LCD_MASK_LCD_RST     0x00000080 // PORTB-1: PB07 - RESET
#define P1_SPI_LCD_MOSI_MASK    0x00000010 // PORTB-1: PB08(SERCOM4 PAD0) - MOSI - LCD PIN5 SDA
#define P1_SPI_LCD_SCK_MASK     0x00000020 // PORTB-1: PB09(SERCOM4 PAD1) - SCK  - LCD PIN6 SCL

#define LCD_ST7735_SIZE_X 160
#define LCD_ST7735_SIZE_Y 128

#define LCD_ST7735_PAUSE_NOPS(nops_number)   __asm__ ("nop;"); // for (uint32_t ui32_def=0;ui32_def<nops_number;ui32_def++){__asm__ ("nop;");}
#define LCD_ST7735_PAUSE_TICKS(ticks_number) tticks_timer = TIMING_SetTicksDelay(ticks_number); while (TIMING_CheckTicksDelayReached(tticks_timer)==FALSE){1;};

#define LCD_ST7735_RS_SET  PORT->Group[1].OUTSET.reg = P1_LCD_MASK_LCD_RS
#define LCD_ST7735_RS_CLR  PORT->Group[1].OUTCLR.reg = P1_LCD_MASK_LCD_RS
// #define LCD_ST7735_WR_SET  PORT->Group[1].OUTSET.reg = P1_LCD_MASK_LCD_WR
// #define LCD_ST7735_WR_CLR  PORT->Group[1].OUTCLR.reg = P1_LCD_MASK_LCD_WR
#define LCD_ST7735_CS_SET  PORT->Group[1].OUTSET.reg = P1_LCD_MASK_LCD_CS
#define LCD_ST7735_CS_CLR  PORT->Group[1].OUTCLR.reg = P1_LCD_MASK_LCD_CS
#define LCD_ST7735_RST_SET PORT->Group[1].OUTSET.reg = P1_LCD_MASK_LCD_RST
#define LCD_ST7735_RST_CLR PORT->Group[1].OUTCLR.reg = P1_LCD_MASK_LCD_RST

#define LCD_ST7735_TRANSPARENT	         0xFFFE
#define LCD_ST7735_WHITE                 0x0000
#define LCD_ST7735_BLACK                 0x0001

// Description:
// Receives:
// Returns:
//   By value:
// Note:
void LCD_ST7735_init();

// Description:
// Receives:
// Returns:
//   By value:
// Note:
uint8_t LCD_ST7735_write_data(uint8_t ui8_data_to_send);

// Description:
// Receives:
// Returns:
//   By value:
// Note:
uint8_t LCD_ST7735_write_command(uint8_t ui8_command_to_send);

// Description:
// Receives:
// Returns:
//   By value:
// Note:
void LCD_ST7735_set_xy(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);

// Description:
// Receives:
// Returns:
//   By value:
// Note:
void LCD_ST7735_set_pixel(uint16_t x, uint16_t y, uint16_t ui16_color);

#endif /* LCD_ST7735_H_ */

#endif //#ifdef LCD_IS_ST7735
