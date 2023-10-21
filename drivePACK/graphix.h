// *****************************************************************
// ****                       drivePACK                         ****
// ****                  www.tolaemon.com/dpack                 ****
// ****                       Source code                       ****
// ****                       01/06/2021                        ****
// ****                     Jordi Bartolome                     ****
// ****                                                         ****
// **** IMPORTANT:                                              ****
// **** Using this code or any part of it means accepting all   ****
// **** conditions exposed in: http://www.tolaemon.com/dpack    ****
// *****************************************************************
#ifndef _GRAPHIX_H_
#define _GRAPHIX_H_

#include "sam.h"
#include "sys_defs.h"
#include "./LCDs/lcdio.h"

#define GRAPHIX_MAX_FONT_SYMBOLS         177 // is the number of characters in the font ( is the last symbol index + 1)

#define GRAPHIX_INV_CHAR_FONT_SYMBOL      63 // is the index of the character that is used when the character to print is not recognized

// some special char codes
#define GRAPHIX_CHAR_FILE        133
#define GRAPHIX_CHAR_FOLDER      134

// Font 1 parameters
#define GRAPHIX_FONT1_PIXELS_X   7
#define GRAPHIX_FONT1_PIXELS_Y   8

// Text buffer defines
#define GRAPHIX_TEXT_BUFFER_MAX_COLUMNS      22
#define GRAPHIX_TEXT_BUFFER_MAX_ROWS         15

// GRAPHIX_TEXT_BUFFER attributes
#define ATTR_NO_ATTRIBS            0x00
#define ATTR_REFRESH               0x01
#define ATTR_SHADOW                0x02 // print the character with shadow
#define ATTR_SPACE_BACKSYMBOL      0x04 // replace the space characters ' ' with the background symbol

// GRAPHIX_TEXT_BUFFER index colors
#define GRAPHIX_TEXT_COL_IDX_TRANSPARENT     0
#define GRAPHIX_TEXT_COL_IDX_BLACK           1
#define GRAPHIX_TEXT_COL_IDX_WHITE           2
#define GRAPHIX_TEXT_COL_IDX_GREY            3 
#define GRAPHIX_TEXT_COL_IDX_DARK_GREY       4 
#define GRAPHIX_TEXT_COL_IDX_RED             5
#define GRAPHIX_TEXT_COL_IDX_DARK_RED        6 
#define GRAPHIX_TEXT_COL_IDX_ORANGE          7
#define GRAPHIX_TEXT_COL_IDX_YELLOW          8
#define GRAPHIX_TEXT_COL_IDX_DARK_YELLOW     9
#define GRAPHIX_TEXT_COL_IDX_GREEN           10
#define GRAPHIX_TEXT_COL_IDX_DARK_GREEN      11
#define GRAPHIX_TEXT_COL_IDX_BRIGHT_BLUE     12
#define GRAPHIX_TEXT_COL_IDX_BLUE            13
#define GRAPHIX_TEXT_COL_IDX_DARK_BLUE       14
#define GRAPHIX_TEXT_BUFFER_MAX_COLORS       15

// GRAPHIX_TEXT_BUFFER COLOR CODES indexes
#define GRAPHIX_TEXT_COL_NEUTRAL             GRAPHIX_TEXT_COL_IDX_WHITE
#define GRAPHIX_TEXT_COL_SUCCESS             GRAPHIX_TEXT_COL_IDX_GREEN
#define GRAPHIX_TEXT_COL_ERROR               GRAPHIX_TEXT_COL_IDX_RED


#define SPECIAL_CODE_RUN_LENGTH  0xFFFE
#define COMPRESSED_HEADER_TAG    0x5A5A5A5A


typedef struct{
	uint8_t  ui8_text[GRAPHIX_TEXT_BUFFER_MAX_COLUMNS];// chars with the text of a row
	uint8_t  ui8_color_idx_front[GRAPHIX_TEXT_BUFFER_MAX_COLUMNS];// front color index of each chars of the row
	uint8_t  ui8_color_idx_shadow[GRAPHIX_TEXT_BUFFER_MAX_COLUMNS];// back color index of each chars of the row
	uint8_t  ui8_color_idx_back[GRAPHIX_TEXT_BUFFER_MAX_COLUMNS];// shadow color index of each char of the row 
	uint8_t  ui8_attributes[GRAPHIX_TEXT_BUFFER_MAX_COLUMNS];// each character attributes
}t_text_buffer_row;

struct ref_text_buffer{
	uint8_t  ui8_x_origin;// screen X cooordinate where the text buffer starts
	uint8_t  ui8_y_origin;// screen Y cooordinate where the text buffer starts
	uint8_t  ui8_pixel_sizeX;// X size of each pixel
    uint8_t  ui8_pixel_sizeY;// Y size of each pixel
	uint8_t  ui8_backgr_char;// the char code used last time the text buffer was filled
	uint8_t  ui8_backgr_front_color_idx;// front color index of the char used to fill the text buffer
	uint8_t  ui8_backgr_back_color_idx;// background color index of the char used to fill the  of the buffer
	uint16_t ui16_numb_cols;// number of used columns of the text buffer ( from 0 to GRAPHIX_TEXT_BUFFER_MAX_COLUMNS - 1)
	uint16_t ui16_numb_rows;// number of used rows of the text buffer ( from 0 to GRAPHIX_TEXT_BUFFER_MAX_ROWS - 1)
	uint16_t ui16_color_idx[GRAPHIX_TEXT_BUFFER_MAX_COLORS];//transaparent, black, white, blue, yellow ...
	t_text_buffer_row  row[GRAPHIX_TEXT_BUFFER_MAX_ROWS];
};

// Description:
// Receives:
// Returns:
//   By value:
// Note:
void GRAPHIX_Init();

// Description:
// Receives:
// Returns:
//   By value:
// Note:
void GRAPHIX_text_buffer_init(uint16_t ui16_num_cols, uint16_t ui16_num_rows, uint8_t ui16_origX, uint8_t ui16_origY, uint8_t ui8_size_pixelX, uint8_t ui8_size_pixelY, uint8_t ui8_idx_front_color, uint8_t ui8_idx_back_color , uint8_t ui8_idx_shadow_color );
// Description:
// Receives:
// Returns:
//   By value:
// Note:
void GRAPHIX_text_buffer_refresh();

// Description:
// Receives:
// Returns:
//   By value:
// Note:
void GRAPHIX_text_buffer_set_char(uint16_t ui16_col , uint16_t ui16_row, uint8_t ui8_char,  uint8_t ui8_attribs, uint8_t ui8_idx_front_color, uint8_t ui8_idx_back_color, uint8_t ui8_idx_shadow_color);

// Description:
// Receives:
// Returns:
//   By value:
// Note:
void GRAPHIX_text_buffer_set_string(uint16_t ui16_col , uint16_t ui16_row, uint8_t * pui8_string,  uint8_t ui8_attribs, uint8_t ui8_idx_front_color, uint8_t ui8_idx_back_color, uint8_t ui8_idx_shadow_color);

// Description:
// Receives:
// Returns:
//   By value:
// Note:
void GRAPHIX_text_buffer_fill(uint8_t ui8_char, uint8_t ui8_attribs, uint8_t ui8_idx_front_color, uint8_t ui8_idx_back_color, uint8_t ui8_idx_shadow_color);

// Description:
//   The same than GRAPHIX_text_buffer_fill() function but only affects the attributes 
//  field of all characters of the text_buffer. It allows to set the received ui8_attribs 
//  to all characters of the text buffer.
// Receives:
// Returns:
//   By value:
// Note:
//   The GRAPHIX_text_buffer_refresh() function only updates in the screem those characters
//  that have been modified on last write operation in the text buffer ( those who have
//  its ATTR_REFRESH attribute set to 'true' ). So this function GRAPHIX_text_buffer_fill_attr()
//  may be useful to force to update in screen all characters.
void GRAPHIX_text_buffer_fill_attr(uint8_t ui8_attribs);

// Description:
// Implements the Bresenham algorithm to draw a line from x0,y0 to x1,y1
// Receives:
// Returns:
//   By value:
// Note:
void GRAPHIX_line(int16_t i16_x0, int16_t i16_y0, int16_t i16_x1, int16_t i16_y1, uint16_t ui16_color);

// Description:
// Receives:
// Returns:
//   By value:
// Note:
void GRAPHIX_raster_circle (int16_t i16_x0, int16_t i16_y0, int16_t i16_radius, uint16_t ui16_color);

// Description:
// Receives:
// Returns:
//   By value:
// Note:
void GRAPHIX_circle (int16_t i16_cx, int16_t i16_cy, int16_t i16_radius, uint16_t ui16_color);

// Description:
// Receives:
// Returns:
//   By value:
// Note:

void GRAPHIX_rectangle(int16_t i16_x0, int16_t i16_y0, int16_t i16_x1, int16_t i16_y1, uint16_t ui16_border_color, uint16_t ui16_back_color, uint8_t ui8_border, uint8_t ui8_solid);

// Description:
// Receives:
// Returns:
//   By value:
// Note:
void GRAPHIX_set_pixel(uint16_t x, uint16_t y, uint16_t ui16_color);

// Description:
// Receives:
// Returns:
//   By value:
// Note:
void GRAPHIX_print_char(uint16_t ui16_x_orig, uint16_t ui16_y_orig, uint8_t ui8_x_pixel_size, uint8_t ui8_y_pixel_size, uint16_t ui16_color, uint16_t ui16_back_color, uint8_t ui8_ascii );

// Description:
// Receives:
// Returns:
//   By value:
// Note:
void GRAPHIX_print_string(uint16_t ui16_x_orig, uint16_t ui16_y_orig, uint8_t ui8_x_pixel_size, uint8_t ui8_y_pixel_size, uint16_t ui16_color, uint16_t ui16_back_color, uint8_t * ui8_string );

// Description:
// Receives:
// Returns:
//   By value:
// Note:
void GRAPHIX_print_img_from_mem(int16_t i16_x0, int16_t i16_y0, uint32_t ui32_width, uint32_t ui32_height, uint8_t ui8_pix_size, uint16_t * pui16_mem_img, uint32_t ui32_mem_img_size);

#endif // _GRAPHIX_H_
