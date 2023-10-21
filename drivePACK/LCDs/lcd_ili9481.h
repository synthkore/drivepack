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

#include "sam.h"
#include "lcdio.h"

#ifdef LCD_IS_ILI9481

#ifndef LCD_ILI9481_H_
#define LCD_ILI9481_H_

#define LCD_ILI9481_SIZE_X 480
#define LCD_ILI9481_SIZE_Y 320

#define LCD_ILI9481_PAUSE_NOPS(nops_number)   __asm__ __volatile__ ("nop;"); // JBR TBD 2022-24-03 Check that macro, nops_number is useless here
#define LCD_ILI9481_PAUSE_TICKS(ticks_number) tticks_timer = TIMING_SetTicksDelay(ticks_number); while (TIMING_CheckTicksDelayReached(tticks_timer)==FALSE){1;};

#define LCD_ILI9481_RS_SET  PORT->Group[1].OUTSET.reg = P1_LCD_MASK_LCD_RS
#define LCD_ILI9481_RS_CLR  PORT->Group[1].OUTCLR.reg = P1_LCD_MASK_LCD_RS
#define LCD_ILI9481_WR_SET  PORT->Group[1].OUTSET.reg = P1_LCD_MASK_LCD_WR
#define LCD_ILI9481_WR_CLR  PORT->Group[1].OUTCLR.reg = P1_LCD_MASK_LCD_WR
#define LCD_ILI9481_CS_SET  PORT->Group[1].OUTSET.reg = P1_LCD_MASK_LCD_CS
#define LCD_ILI9481_CS_CLR  PORT->Group[1].OUTCLR.reg = P1_LCD_MASK_LCD_CS
#define LCD_ILI9481_RST_SET PORT->Group[1].OUTSET.reg = P1_LCD_MASK_LCD_RST
#define LCD_ILI9481_RST_CLR PORT->Group[1].OUTCLR.reg = P1_LCD_MASK_LCD_RST


//*********************************
// COLORS
//*********************************
// VGA color palette
#define LCD_ILI9481_VGA_BLACK		0x0000
#define LCD_ILI9481_VGA_WHITE		0xFFFF
#define LCD_ILI9481_VGA_RED			0xF800
#define LCD_ILI9481_VGA_GREEN		0x0400
#define LCD_ILI9481_VGA_BLUE		0x001F
#define LCD_ILI9481_VGA_SILVER		0xC618
#define LCD_ILI9481_VGA_GRAY		0x8410
#define LCD_ILI9481_VGA_MAROON		0x8000
#define LCD_ILI9481_VGA_YELLOW		0xFFE0
#define LCD_ILI9481_VGA_OLIVE		0x8400
#define LCD_ILI9481_VGA_LIME		0x07E0
#define LCD_ILI9481_VGA_AQUA		0x07FF
#define LCD_ILI9481_VGA_TEAL		0x0410
#define LCD_ILI9481_VGA_NAVY		0x0010
#define LCD_ILI9481_VGA_FUCHSIA		0xF81F
#define LCD_ILI9481_VGA_PURPLE		0x8010
#define LCD_ILI9481_VGA_TRANSPARENT	0xFFFE

// Description:
// Receives:
// Returns:
//   By value:
// Note:
void LCD_ILI9481_init();

// Description:
// Receives:
// Returns:
//   By value:
// Note:
void LCD_ILI9481_write_command(uint8_t ui8_command);

// Description:
// Receives:
// Returns:
//   By value:
// Note:
void LCD_ILI9481_write_data_8(uint8_t ui8_data);

// Description:
// Receives:
// Returns:
//   By value:
// Note:
void LCD_ILI9481_write_data_16(uint16_t ui16_data);

// Description:
// Receives:
// Returns:
//   By value:
// Note:
void LCD_ILI9481_fast_fill(uint16_t ui16_color, uint16_t ui16_x1, uint16_t ui16_y1, uint16_t ui16_x2, uint16_t ui16_y2);

// Description:
// Receives:
// Returns:
//   By value:
// Note:
void LCD_ILI9481_set_xy(uint16_t ui16_x1, uint16_t ui16_y1, uint16_t ui16_x2, uint16_t ui16_y2);

// Description:
// Receives:
// Returns:
//   By value:
// Note:
void LCD_ILI9481_init();

// Description:
// Receives:
// Returns:
//   By value:
// Note:
void LCD_ILI9481_set_pixel(uint16_t x, uint16_t y, uint16_t ui16_color);

// Description:
// Receives:
// Returns:
//   By value:
// Note:
// From: https://gist.github.com/bert/1085538 
void LCD_ILI9481_line(int16_t i16_x0, int16_t i16_y0, int16_t i16_x1, int16_t i16_y1, uint16_t ui16_color);

// Description:
// Receives:
//    cx, cy:denote the offset of the circle centre from the origin.
// Returns:
//   By value:
// Note:
// From: https://gist.github.com/bert/1085538 : circle (int cx, int cy, int radius)
void LCD_ILI9481_circle (int16_t i16_cx, int16_t i16_cy, int16_t i16_radius, uint16_t ui16_color);

// Description:
// Receives:
//    x0, y0:denote the offset of the circle centre from the origin.
// Returns:
//   By value:
// Note:
// From: https://gist.github.com/bert/1085538 : raster_circle (int x0, int y0, int radius)
void LCD_ILI9481_raster_circle (int16_t i16_x0, int16_t i16_y0, int16_t i16_radius, uint16_t ui16_color);

// Description:
// Receives:
// Returns:
//   By value:
// Note:
void LCD_ILI9481_print_char(uint16_t ui16_x_orig, uint16_t ui16_y_orig, uint8_t ui8_x_pixel_size, uint8_t ui8_y_pixel_size, uint16_t ui16_color, uint16_t ui16_back_color, uint8_t ui8_ascii );


// Description:
// Receives:
// Returns:
//   By value:
// Note:
void LCD_ILI9481_print_string(uint16_t ui16_x_orig, uint16_t ui16_y_orig, uint8_t ui8_x_pixel_size, uint8_t ui8_y_pixel_size, uint16_t ui16_color, uint16_t ui16_back_color, uint8_t * ui8_string, uint16_t ui16_MAX_LEN );

#endif /* LCD_ILI9481_H_ */

#endif // #ifdef LCD_IS_ILI9481
