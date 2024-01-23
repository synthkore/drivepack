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
// **** This code is part of the bareSAM platform. All info at: ****
// **** http://www.tolaemon.com/baresam                         ****
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

/*********************************************************************************************
* @brief initializes the ILI9481 compatible graphic display hardware
*********************************************************************************************/
void LCD_ILI9481_init();

/*********************************************************************************************
* @brief sets the received command to the IO lines used to interface the ILI9481 compatible graphic
* display
* @param[in] ui8_command the command to send to the ILI9481 compatible graphic display
* @return
*********************************************************************************************/
void LCD_ILI9481_write_command(uint8_t ui8_command);

/*********************************************************************************************
* @brief sets the received 8 bit data value to the IO lines used to interface the ILI9481 
* compatible graphic display
* @param[in] ui8_data the 8 bits data value to send to the ILI9481 compatible graphic display
*********************************************************************************************/
void LCD_ILI9481_write_data_8(uint8_t ui8_data);

/*********************************************************************************************
* @brief sets the received 16 bit data value to the IO lines used to interface the ILI9481
* compatible graphic display
* @param[in] ui16_data the 16 bits data value to send to the ILI9481 compatible graphic display
*********************************************************************************************/
void LCD_ILI9481_write_data_16(uint16_t ui16_data);

/*********************************************************************************************
* @brief fills the rectangular area defined by the x1,y1 and x2,y2 with the received ui16_color
* It is done applying a fast algorithm that consists on, first setting the address where the
* rectangular area starts, and then by sequentially writting the color code into the ILI9481 
* compatible graphic display memory.
* @param[in] ui16_color the RGB 5bit 6bit 5bits color code used to fill the specified rectangle.
* @param[in] ui16_x1 X coordinate of the top left corner of the rectangular area to fill
* @param[in] ui16_y1 Y coordinate of the top left corner of the rectangular area to fill
* @param[in] ui16_x2 X coordinate of the bottom right corner of the rectangular area to fill
* @param[in] ui16_y2 Y coordinate of the bottom right corner of the rectangular area to fill
*********************************************************************************************/
void LCD_ILI9481_area_fast_fill(uint16_t ui16_color, uint16_t ui16_x1, uint16_t ui16_y1, uint16_t ui16_x2, uint16_t ui16_y2);

/*********************************************************************************************
* @brief sets the draw cursor at the memory row and page extent of the graphic ILI9481 display 
* that corresponds to area delimited by the received x1,y1 and x2,y2 coordinates.
* @param[in] ui16_x1 X coordinate of the top left corner of the rectangular extent area
* @param[in] ui16_y1 Y coordinate of the top left corner of the rectangular extent area
* @param[in] ui16_x2 X coordinate of the bottom right corner of the rectangular extent area
* @param[in] ui16_y2 Y coordinate of the bottom right corner of the rectangular extent area
*********************************************************************************************/
void LCD_ILI9481_set_xy(uint16_t ui16_x1, uint16_t ui16_y1, uint16_t ui16_x2, uint16_t ui16_y2);

/*********************************************************************************************
* @brief sets the received color code in the pixel at the received x,y coordinates of the  ILI9481 
* display 
* @param[in] x coordinate of the pixel to paint
* @param[in] y coordinate of the pixel to paint
* @param[in] ui16_color the RGB 5bit 6bit 5bits color code used to paint the pixel
*********************************************************************************************/
void LCD_ILI9481_set_pixel(uint16_t x, uint16_t y, uint16_t ui16_color);

/*********************************************************************************************
* @brief draws a line of the specified color between the received x0,y0 point and the x1,y1 point.
* @param[in] i16_x0 x coordinate in pixels of the point that defines the first end point of the line
* @param[in] i16_y0 y coordinate in pixels of the point that defines the first end point of the line
* @param[in] i16_x1 x coordinate in pixels of the point that defines the second end point of the line
* @param[in] i16_y1 y coordinate in pixels of the point that defines the second end point of the line
* @param[in] ui16_color the RGB 5bit 6bit 5bits color code used to draw the line
* @note from: https://gist.github.com/bert/1085538 
*********************************************************************************************/
void LCD_ILI9481_line(int16_t i16_x0, int16_t i16_y0, int16_t i16_x1, int16_t i16_y1, uint16_t ui16_color);

/*********************************************************************************************
* @brief draws a circle of the specified radius and color centered at the cx,cy coordinates.
* @param[in] cx X coordinate in pixels of the circle center from the origi
* @param[in] cy Y coordinate in pixels of the circle center from the origin.
* @param[in] i16_radius radius of the circle in pixels
* @param[in] ui16_color the RGB 5bit 6bit 5bits color code used to paint the pixel
* @note from: https://gist.github.com/bert/1085538  : raster_circle (int x0, int y0, int radius)
*********************************************************************************************/
void LCD_ILI9481_circle (int16_t i16_cx, int16_t i16_cy, int16_t i16_radius, uint16_t ui16_color);

/*********************************************************************************************
* @brief draws a circle of the specified radius and color centered at the cx,cy coordinates.
* @param[in] i16_x0 X coordinate in pixels of the circle center from the origin
* @param[in] i16_y0 Y coordinate in pixels of the circle center from the origin.
* @param[in] i16_radius radius of the circle in pixels
* @param[in] ui16_color the RGB 5bit 6bit 5bits color code used to paint the pixel
* @note from: https://gist.github.com/bert/1085538  : raster_circle (int x0, int y0, int radius)
*********************************************************************************************/
void LCD_ILI9481_raster_circle (int16_t i16_x0, int16_t i16_y0, int16_t i16_radius, uint16_t ui16_color);

/*********************************************************************************************
* @brief prints the received character at the specified coordinates of the screen with
* the specified pixel sizes and colors.
* @param[in] ui16_x_orig x coordinate in pixels at which the received character will be printed.
* @param[in] ui16_y_orig y coordinate in pixels at which the received character will be printed.
* @param[in] ui8_x_pixel_size number of pixels on x direction that will take each characters
* pixel when printed in the screen.
* @param[in] ui8_y_pixel_size number of pixels on y direction that will take each characters
* pixel when printed in the screen.
* @param[in] ui16_color the RGB 5bit 6bit 5bits color code used to print the character
* @param[in] ui16_back_color color the RGB 5bit 6bit 5bits color code used to paint the background 
* of the character.
* @param[in] ui8_ascii the ASCII code of the character to print in the screen.
*********************************************************************************************/
void LCD_ILI9481_print_char(uint16_t ui16_x_orig, uint16_t ui16_y_orig, uint8_t ui8_x_pixel_size, uint8_t ui8_y_pixel_size, uint16_t ui16_color, uint16_t ui16_back_color, uint8_t ui8_ascii );

/*********************************************************************************************
* @brief prints the received string at the specified coordinates of the screen with
* the specified pixel sizes and colors.
* @param[in] ui16_x_orig x coordinate in pixels at which the received string will be printed.
* @param[in] ui16_y_orig y coordinate in pixels at which the received character will be printed.
* @param[in] ui8_x_pixel_size number of pixels on X direction that will take each characters
* pixel when printed in the screen 
* @param[in] ui8_y_pixel_size number of pixels on Y direction that will take each characters
* pixel when printed in the screen.
* @param[in] ui16_color the RGB 5bit 6bit 5bits color code used to print the string
* @param[in] ui16_back_color the RGB 5bit 6bit 5bits color code used to paint the background of the string
* @param[in] ui8_string the array of uint8s with the asccii codes to print in the screen. It
* must end with '\0'
* @param[in] ui16_MAX_LEN
*********************************************************************************************/
void LCD_ILI9481_print_string(uint16_t ui16_x_orig, uint16_t ui16_y_orig, uint8_t ui8_x_pixel_size, uint8_t ui8_y_pixel_size, uint16_t ui16_color, uint16_t ui16_back_color, uint8_t * ui8_string, uint16_t ui16_MAX_LEN );

#endif /* LCD_ILI9481_H_ */

#endif // #ifdef LCD_IS_ILI9481
