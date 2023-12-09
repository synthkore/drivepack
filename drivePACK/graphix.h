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
	uint8_t  ui8_x_origin;// screen X coordinate where the text buffer starts
	uint8_t  ui8_y_origin;// screen Y coordinate where the text buffer starts
	uint8_t  ui8_pixel_sizeX;// X size of each pixel
    uint8_t  ui8_pixel_sizeY;// Y size of each pixel
	uint8_t  ui8_backgr_char;// the char code used last time the text buffer was filled
	uint8_t  ui8_backgr_front_color_idx;// front color index of the char used to fill the text buffer
	uint8_t  ui8_backgr_back_color_idx;// background color index of the char used to fill the  of the buffer
	uint16_t ui16_numb_cols;// number of used columns of the text buffer ( from 0 to GRAPHIX_TEXT_BUFFER_MAX_COLUMNS - 1)
	uint16_t ui16_numb_rows;// number of used rows of the text buffer ( from 0 to GRAPHIX_TEXT_BUFFER_MAX_ROWS - 1)
	uint16_t ui16_color_idx[GRAPHIX_TEXT_BUFFER_MAX_COLORS];//transparent, black, white, blue, yellow ...
	t_text_buffer_row  row[GRAPHIX_TEXT_BUFFER_MAX_ROWS];
};

/*********************************************************************************************
* @brief initializes the graphic display, the text buffer and other variables used by the 
* graphics module.
*********************************************************************************************/
void GRAPHIX_Init();

/*********************************************************************************************
* @brief initializes the text buffer used to enter and show text information in the screen.
* @param[in] ui16_num_cols the number of characters that will be on each text buffer row ( x 
* size in characters ). It may never be greater than GRAPHIX_TEXT_BUFFER_MAX_COLUMNS
* @param[in] ui16_num_rows the number of character rows that will be on the text buffer ( y size
* in characters ). It may never be greater than GRAPHIX_TEXT_BUFFER_MAX_ROWS
* @param[in] ui16_origX x coordinate in the screen at which the content of the text buffer will
* be rendered.
* @param[in] ui16_origY y coordinate in the screen at which the content of the text buffer will
* be rendered.
* @param[in] ui8_size_pixelX number of pixels on X direction that will take each characters 
* pixel when printed in the screen.
* @param[in] ui8_size_pixelY  number of pixels on Y direction that will take each characters
* pixel when printed in the screen.
* @param[in] ui8_idx_front_color default color used to print the characters of the text 
* buffer in the screen ( the colors can be different for each character ).
* @param[in] ui8_idx_back_color default color INDEX used as background color in the text buffer
* ( the colors can be different for each character ).
* @param[in] ui8_idx_shadow_color color INDEX used to paint the shadow of the characters when 
* the shadow effect is enabled.
*********************************************************************************************/
void GRAPHIX_text_buffer_init(uint16_t ui16_num_cols, uint16_t ui16_num_rows, uint8_t ui16_origX, uint8_t ui16_origY, uint8_t ui8_size_pixelX, uint8_t ui8_size_pixelY, uint8_t ui8_idx_front_color, uint8_t ui8_idx_back_color , uint8_t ui8_idx_shadow_color );

/*********************************************************************************************
* @brief prints the current text buffer content in the screen
*********************************************************************************************/
void GRAPHIX_text_buffer_refresh();

/*********************************************************************************************
* @brief places the received char with the specified attributes and colors at the specified
* row and column of the text buffer.
* @param[in] ui16_col column ( x of the buffer ) at which the received char will be placed
* @param[in] ui16_row row ( y of the buffer ) at which the received char will be placed
* @param[in] ui8_char the character to place at the specified (ui16_row, ui16_col) position
* @param[in] ui8_attribs attributes for the received character in the text bfufer.
* @param[in] ui8_idx_front_color color INDEX assigned to the received char in the text buffer.
* @param[in] ui8_idx_back_color background color INDEX assigned to the received char in the text 
* buffer.
* @param[in] ui8_idx_shadow_color if shadow is enabled then this color INDEX be used to print the
* shadow of the received character.
*********************************************************************************************/
void GRAPHIX_text_buffer_set_char(uint16_t ui16_col , uint16_t ui16_row, uint8_t ui8_char,  uint8_t ui8_attribs, uint8_t ui8_idx_front_color, uint8_t ui8_idx_back_color, uint8_t ui8_idx_shadow_color);

/*********************************************************************************************
* @brief laces the received string with the specified attributes and colors at the specified
* row and column of the text buffer. The string is clipped in order it does not go beyond
* the X limits of the text buffer.
* @param[in] ui16_col column ( x of the buffer ) at which the received string will be placed
* @param[in] ui16_row row ( y of the buffer ) at which the received string will be placed
* @param[in] pui8_string the array of uint8s containing the ASCII codes that are going to be placed
* in the text buffer. It must end with '\0'.
* @param[in] ui8_attribs attributes for the characters of the received string in the text buffer.
* @param[in] ui8_idx_front_color color INDEX assigned in the text buffer to the chars of the received 
* string.
* @param[in] ui8_idx_back_color background color INDEX assigned in the text buffer to the chars of the
* received string.
* @param[in] ui8_idx_shadow_color INDEX if shadow is enabled then this color be used to print the
* shadow of the characters of the received string.
*********************************************************************************************/
void GRAPHIX_text_buffer_set_string(uint16_t ui16_col , uint16_t ui16_row, uint8_t * pui8_string,  uint8_t ui8_attribs, uint8_t ui8_idx_front_color, uint8_t ui8_idx_back_color, uint8_t ui8_idx_shadow_color);

/*********************************************************************************************
* @brief fills the text buffer ( from 0,0 to ui16_numb_rows, ui16_numb_cols ) with the received
* ui8_char with the specified specified attributes and colors.
* @param[in] ui8_char the character used to fill the text buffer.
* @param[in] ui8_attribs default attributes for all the characters of the text buffer.
* @param[in] ui8_idx_front_color default color INDEX used to print the characters of the text
* buffer in the screen ( the colors can be different for each character ).
* @param[in] ui8_idx_back_color default color INDEX used as background color in the text buffer
* ( the colors can be different for each character ).
* @param[in] ui8_idx_shadow_color default color INDEX used to paint the shadow of the characters when
* the shadow effect is enabled.
*********************************************************************************************/
void GRAPHIX_text_buffer_fill(uint8_t ui8_char, uint8_t ui8_attribs, uint8_t ui8_idx_front_color, uint8_t ui8_idx_back_color, uint8_t ui8_idx_shadow_color);

/*********************************************************************************************
* @brief the same than GRAPHIX_text_buffer_fill() function but only affects the attributes
* field of all characters of the text_buffer. It allows to set the received ui8_attribs to all
* characters of the text buffer.
* @param[in] ui8_attribs default attributes for all the characters of the text buffer.
* @note 
*  The GRAPHIX_text_buffer_refresh() function only updates in the screem those characters
* that have been modified on last write operation in the text buffer ( those who have
* its ATTR_REFRESH attribute set to 'true' ). So this function GRAPHIX_text_buffer_fill_attr()
* may be useful to force to update in screen all characters.
*********************************************************************************************/
void GRAPHIX_text_buffer_fill_attr(uint8_t ui8_attribs);

/*********************************************************************************************
* @brief implements the Bresenham algorithm to draw a line from x0,y0 to x1,y1
* @param[in] i16_x0 X coordinate in pixels of the line origin point
* @param[in] i16_y0 Y coordinate in pixels of the line origin point
* @param[in] i16_x1 X coordinate in pixels of the line end point
* @param[in] i16_y1 Y coordinate in pixels of the line end point
* @param[in] ui16_color RGB color code used to draw the line in the screen.
*********************************************************************************************/
void GRAPHIX_line(int16_t i16_x0, int16_t i16_y0, int16_t i16_x1, int16_t i16_y1, uint16_t ui16_color);

/*********************************************************************************************
* @brief Implements the algorithm to draw a circle of the specified radius and color centered 
* at the x0,y0 coordinates.
* @param[in] i16_x0 X coordinate in pixels of the center of the circle.
* @param[in] i16_y0 Y coordinate in pixels of the center of the circle.
* @param[in] i16_radius radius of the circle to draw.
* @param[in] ui16_color RGB color code used to draw the circle in the screen.
*********************************************************************************************/
void GRAPHIX_raster_circle (int16_t i16_x0, int16_t i16_y0, int16_t i16_radius, uint16_t ui16_color);

/*********************************************************************************************
* @brief implements the algorithm to draw a circle of the specified radius and color centered
* at the x0,y0 coordinates.
* @param[in] i16_x0 X coordinate in pixels of the center of the circle.
* @param[in] i16_y0 Y coordinate in pixels of the center of the circle.
* @param[in] i16_radius radius of the circle to draw.
* @param[in] ui16_color RGB color code used to draw the circle in the screen.
*********************************************************************************************/
void GRAPHIX_circle (int16_t i16_cx, int16_t i16_cy, int16_t i16_radius, uint16_t ui16_color);

/*********************************************************************************************
* @brief implements the algorithm to draw a rectangle with the specified border and fill colors
* at the specified coordinates. The rectangle is drawn between the top left corner coordinates 
* specified with the i16_x0,i16_y0 and the bottom right corner coordinates specified with the 
* 16_x1,i16_y1:
* @param[in] i16_x0 X coordinate in pixels of top left corner of the rectangle.
* @param[in] i16_y0 Y coordinate in pixels of top left corner of the rectangle.
* @param[in] i16_x1 X coordinate in pixels of bottom right corner of the rectangle.
* @param[in] i16_y1 Y coordinate in pixels of bottom right corner of the rectangle.
* @param[in] ui16_border_color when ui8_border is set, that is the RGB color code used to draw 
* the border of the rectangle.
* @param[in] ui16_back_color when ui8_solid is set, that is the RGB color code used to fill the
* rectangle.
* @param[in] ui8_border boolean to force drawing the border of the received rectangle with the 
received RGB color code.
* @param[in] ui8_solid ui8_solid boolean to force filling the drawn rectangle with the received
* RGB color code
*********************************************************************************************/
void GRAPHIX_rectangle(int16_t i16_x0, int16_t i16_y0, int16_t i16_x1, int16_t i16_y1, uint16_t ui16_border_color, uint16_t ui16_back_color, uint8_t ui8_border, uint8_t ui8_solid);

/*********************************************************************************************
* @brief sets the received color RGB code in the pixel at screen coordinates x,y
* @param[in] x x coordinate in pixels of the pixel
* @param[in] y x coordinate in pixels of the pixel
* @param[in] ui16_color desired RGB color code for the pixel at X,Y coordinates.
*********************************************************************************************/
void GRAPHIX_set_pixel(uint16_t x, uint16_t y, uint16_t ui16_color);

/*********************************************************************************************
* @brief prints the received character at the specified coordinates of the screen with 
* the specified pixel sizes and colors.
* @param[in] ui16_x_orig x coordinate in pixels at which the received character will be painted.
* @param[in] ui16_y_orig y coordinate in pixels at which the received character will be painted.
* @param[in] ui8_x_pixel_size number of pixels on X direction that will take each characters
* pixel when printed in the screen.
* @param[in] ui8_y_pixel_size  number of pixels on Y direction that will take each characters
* pixel when printed in the screen.
* @param[in] ui16_color RGB color code used to print the character
* buffer in the screen ( the colors can be different for each character ).
* @param[in] ui16_back_color RGB color code for the character background.
* @param[in] ui8_ascii the ASCII code of the character to print in the screen.
*********************************************************************************************/
void GRAPHIX_print_char(uint16_t ui16_x_orig, uint16_t ui16_y_orig, uint8_t ui8_x_pixel_size, uint8_t ui8_y_pixel_size, uint16_t ui16_color, uint16_t ui16_back_color, uint8_t ui8_ascii );

/*********************************************************************************************
* @brief prints the received string at the specified coordinates of the screen with
* the specified pixel sizes and colors.
* @param[in] ui16_x_orig X coordinate in pixels at which the received string will be printed.
* @param[in] ui16_y_orig Y coordinate in pixels at which the received string will be printed.
* @param[in] ui8_x_pixel_size number of pixels on X direction that will take each characters
* pixel when printed in the screen 
* @param[in] ui8_y_pixel_size number of pixels on Y direction that will take each characters
* pixel when printed in the screen.
* @param[in] ui16_color RGB color code used to print the string characters
* @param[in] ui16_back_color RGB color code for the string characters background.
* @param[in] ui8_string the array of uint8s with the asccii codes to print in the screen. It
* must end with '\0'
*********************************************************************************************/
void GRAPHIX_print_string(uint16_t ui16_x_orig, uint16_t ui16_y_orig, uint8_t ui8_x_pixel_size, uint8_t ui8_y_pixel_size, uint16_t ui16_color, uint16_t ui16_back_color, uint8_t * ui8_string );

/*********************************************************************************************
* @brief receives an array with the pixels of an image and paints it into the screen.
* @param[in] i16_x0 X coordinate in pixels at which the image will be rendered.
* @param[in] i16_y0 y coordinate in pixels at which the image will be rendered.
* @param[in] ui32_width original horizontal size of the image in pixels
* @param[in] ui32_height original vertical size of the image in pixels
* @param[in] ui8_pix_size number of screen pixels (XxY) used to paint each image pixel in the 
* screen.
* @param[in] pui16_mem_img array with the RGB color codes of the images pixels to paint in the
* screen.
* @param[in] ui32_mem_img_size size of the arrya with the RGB color codes.
*********************************************************************************************/
void GRAPHIX_print_img_from_mem(int16_t i16_x0, int16_t i16_y0, uint32_t ui32_width, uint32_t ui32_height, uint8_t ui8_pix_size, uint16_t * pui16_mem_img, uint32_t ui32_mem_img_size);

#endif // _GRAPHIX_H_
