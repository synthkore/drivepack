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

#include "../gpio.h"
#include "../sys_defs.h"
#include "../timing.h"
#ifndef _LCD_IO_H_
#define _LCD_IO_H_


// to select the LCD model, uncomment the driver of the used LCD display and 
// keep other LCD drivers commented: 
// #define LCD_IS_ILI9481
// #define LCD_IS_ST7565
#define LCD_IS_ST7735
// #define LCD_IS_SH1106

#ifdef LCD_IS_ILI9481
    
	#include "lcd_ili9481.h"
	
	#define LCD_INIT        LCD_ILI9481_init()
	#define LCD_SET_PIXEL   LCD_ILI9481_set_pixel
	#define LCD_FAST_FILL   LCD_ILI9481_fast_fill
	#define LCD_TRANSPARENT LCD_ILI9481_VGA_TRANSPARENT
	#define LCD_MAX_SIZE_X  LCD_ILI9481_SIZE_X
	#define LCD_MAX_SIZE_Y  LCD_ILI9481_SIZE_Y
	
#endif

#ifdef LCD_IS_ST7565

    #include "lcd_st7565.h"

    #define LCD_INIT        LCD_ST7565_init()
	#define LCD_SET_PIXEL   LCD_ST7565_buffer_set_pixel
	#define LCD_FAST_FILL   LCD_ST7565_buffer_fast_fill
	#define LCD_TRANSPARENT LCD_ST7565_TRANSPARENT
	#define LCD_MAX_SIZE_X  LCD_ST7565_SIZE_X
	#define LCD_MAX_SIZE_Y  LCD_ST7565_SIZE_Y
	
#endif


#ifdef LCD_IS_ST7735

    #include "lcd_st7735.h"

    #define LCD_INIT         LCD_ST7735_init()
	#define LCD_SET_PIXEL    LCD_ST7735_set_pixel
	#define LCD_FAST_FILL    LCD_ST7735_fast_fill
	#define LCD_TRANSPARENT  LCD_ST7735_TRANSPARENT
	#define LCD_MAX_SIZE_X   LCD_ST7735_SIZE_X
	#define LCD_MAX_SIZE_Y   LCD_ST7735_SIZE_Y

	//*********************************
	// COLORS ( to get a color search for rgb565 colour picker )
	//*********************************
	// 65K: RGB565 R:5bit G:6bit 5 B:bit palete
	#define LCD_COLOR_BLACK		    0x0000
	#define LCD_COLOR_WHITE		    0xFFFF
	#define LCD_COLOR_RED		    0xF800
	#define LCD_COLOR_BRIGHT_RED    0xF8C3
	#define LCD_COLOR_BRIGHT_GREEN  0xAFE7
	#define LCD_COLOR_GREEN         0x07E0
	#define LCD_COLOR_DARK_GREEN	0x2C02
	#define LCD_COLOR_BLUE		    0x001F
	#define LCD_COLOR_BRIGHT_BLUE   0x00DF
	#define LCD_COLOR_DARK_BLUE	    0x192E
	#define LCD_COLOR_SILVER	    0xC618
	#define LCD_COLOR_GREY		    0xD69A
	#define LCD_COLOR_DARK_GREY	    0x2965
	#define LCD_COLOR_MAROON	    0xA1A6
	#define LCD_COLOR_ORANGE	    0xFCA4
	#define LCD_COLOR_YELLOW	    0xFFE0
	#define LCD_COLOR_DARK_YELLOW   0x62C0
	#define LCD_COLOR_OLIVE		    0x8400
	#define LCD_COLOR_LIME		    0x07E0
	#define LCD_COLOR_AQUA		    0x07FF
	#define LCD_COLOR_TEAL		    0x0410
	#define LCD_COLOR_NAVY		    0x0010
	#define LCD_COLOR_FUCHSIA	    0xF81F
	#define LCD_COLOR_PURPLE	    0x8010
	#define LCD_COLOR_TRANSPARENT	0xFFFE	
	
#endif

#ifdef LCD_IS_SH1106

    #include "lcd_sh1106.h"

    #define LCD_INIT        LCD_SH1106_init(LCD_SH1106_I2C_ADDRESS)
	#define LCD_SET_PIXEL   LCD_SH1106_buffer_set_pixel
	#define LCD_FAST_FILL   LCD_SH1106_buffer_fast_fill
	#define LCD_TRANSPARENT LCD_SH1106_TRANSPARENT
	#define LCD_MAX_SIZE_X       LCD_SH1106_SIZE_X
	#define LCD_MAX_SIZE_Y       LCD_SH1106_SIZE_Y
	
#endif

#endif // _LCD_IO_H_