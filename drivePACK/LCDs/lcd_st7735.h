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
// **** This code is part of the bareSAM platform. All info at: ****
// **** http://www.tolaemon.com/baresam                         ****
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

/*********************************************************************************************
* @brief initializes the ST7735 compatible graphic display hardware
*********************************************************************************************/
void LCD_ST7735_init();

/*********************************************************************************************
* @brief sends a data byte to the ST7735 compatible graphic display through the SPI applying
* the right SEND DATA byte signals sequence:
*   clear LCD_ST7735_CS to '0'
*   set LCD_ST7735_RS to '1'
*   send received command byte through SPI
*   wait for command byte transmission to complete
*   set LCD_ST7735_CS to '1'
* @param[in] ui8_data_to_send with the data value to send
* @return
*********************************************************************************************/
uint8_t LCD_ST7735_write_SPI_data(uint8_t ui8_data_to_send);

/*********************************************************************************************
* @brief sends a command byte to the ST7735 compatible graphic display through the SPI applying
* the right SEND COMMAND byte signals sequence:
*   clear LCD_ST7735_CS to '0'
*   clear LCD_ST7735_RS to '0'
*   send received command byte through SPI
*   wait for command byte transmission to complete
*   set LCD_ST7735_CS to '1'
* @param[in] ui8_command_to_send with the command value to send
* @return
*********************************************************************************************/
uint8_t LCD_ST7735_write_SPI_command(uint8_t ui8_command_to_send);

/*********************************************************************************************
* @brief fills the rectangular area defined by the x1,y1 and x2,y2 with the received ui16_color
* It is done applying a fast algorithm that consists on, first setting the address where the
* rectangular area starts, and then by sequentially writing the color code into the ST7735
* compatible graphic display memory.
* @param[in] ui16_color the RGB 5bit 6bit 5bits color code used to fill the specified rectangle.
* @param[in] ui16_x1 X coordinate of the top left corner of the rectangular area to fill
* @param[in] ui16_y1 Y coordinate of the top left corner of the rectangular area to fill
* @param[in] ui16_x2 X coordinate of the bottom right corner of the rectangular area to fill
* @param[in] ui16_y2 Y coordinate of the bottom right corner of the rectangular area to fill
*********************************************************************************************/
void LCD_ST7735_fast_fill(uint16_t ui16_color, uint16_t ui16_x1, uint16_t ui16_y1, uint16_t ui16_x2, uint16_t ui16_y2);

/*********************************************************************************************
* @brief sets the draw cursor at the memory row and page extent of the graphic ST7735 compatible
* display that corresponds to area delimited by the received x1,y1 and x2,y2 coordinates.
* @param[in] x1 X coordinate of the top left corner of the rectangular extent area
* @param[in] y1 Y coordinate of the top left corner of the rectangular extent area
* @param[in] x2 X coordinate of the bottom right corner of the rectangular extent area
* @param[in] y2 Y coordinate of the bottom right corner of the rectangular extent area
*********************************************************************************************/
void LCD_ST7735_set_xy(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);

/*********************************************************************************************
* @brief sets the received color code in the pixel at the received x,y coordinates of the ST7735
* compatible display
* @param[in] x coordinate of the pixel to paint
* @param[in] y coordinate of the pixel to paint
* @param[in] ui16_color the RGB 5bit 6bit 5bits color code used to paint the pixel
*********************************************************************************************/
void LCD_ST7735_set_pixel(uint16_t x, uint16_t y, uint16_t ui16_color);

#endif /* LCD_ST7735_H_ */

#endif //#ifdef LCD_IS_ST7735
