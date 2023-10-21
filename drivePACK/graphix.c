// *****************************************************************
// ****                       drivePACK                         ****
// ****                  www.tolaemon.com/dpack                 ****
// ****                       Source code                       ****
// ****                       05/05/2021                        ****
// ****                     Jordi Bartolome                     ****
// ****                                                         ****
// **** IMPORTANT:                                              ****
// **** Using this code or any part of it means accepting all   ****
// **** conditions exposed in: http://www.tolaemon.com/dpack    ****
// *****************************************************************

#include "graphix.h"

struct ref_text_buffer text_buffer;

// font description into the PROGRAM FLASH. Each byte corresponds
// to a column of the character
const uint8_t GRAPHIX_font_symbols[][8] ={
		// ASCII CODES BEGIN #######################################################
		{0x00,0x00,0x00,0x00,0x00,0x00,0x00},//***                = 000
		{0x00,0x00,0x00,0x00,0x00,0x00,0x00},//***                = 001
		{0x00,0x00,0x00,0x00,0x00,0x00,0x00},//***                = 002
		{0x00,0x00,0x00,0x00,0x00,0x00,0x00},//***                = 003
		{0x00,0x00,0x00,0x00,0x00,0x00,0x00},//***                = 004
		{0x00,0x00,0x00,0x00,0x00,0x00,0x00},//***                = 005
		{0x00,0x00,0x00,0x00,0x00,0x00,0x00},//***                = 006
		{0x00,0x00,0x00,0x00,0x00,0x00,0x00},//***                = 007
		{0x00,0x00,0x00,0x00,0x00,0x00,0x00},//***                = 008
		{0x00,0x00,0x00,0x00,0x00,0x00,0x00},//***                = 009
		{0x00,0x00,0x00,0x00,0x00,0x00,0x00},//***                = 010
		{0x00,0x00,0x00,0x00,0x00,0x00,0x00},//***                = 011
		{0x00,0x00,0x00,0x00,0x00,0x00,0x00},//***                = 012
		{0x00,0x00,0x00,0x00,0x00,0x00,0x00},//***                = 013
		{0x00,0x00,0x00,0x00,0x00,0x00,0x00},//***                = 014
		{0x00,0x00,0x00,0x00,0x00,0x00,0x00},//***                = 015
		{0x00,0x00,0x00,0x00,0x00,0x00,0x00},//***                = 016
		{0x00,0x00,0x00,0x00,0x00,0x00,0x00},//***                = 017
		{0x00,0x00,0x00,0x00,0x00,0x00,0x00},//***                = 018
		{0x00,0x00,0x00,0x00,0x00,0x00,0x00},//***                = 019
		{0x00,0x00,0x00,0x00,0x00,0x00,0x00},//***                = 020
		{0x00,0x00,0x00,0x00,0x00,0x00,0x00},//***                = 021
		{0x00,0x00,0x00,0x00,0x00,0x00,0x00},//***                = 022
		{0x00,0x00,0x00,0x00,0x00,0x00,0x00},//***                = 023
		{0x00,0x00,0x00,0x00,0x00,0x00,0x00},//***                = 024
		{0x00,0x00,0x00,0x00,0x00,0x00,0x00},//***                = 025
		{0x00,0x00,0x00,0x00,0x00,0x00,0x00},//***                = 026
		{0x00,0x00,0x00,0x00,0x00,0x00,0x00},//***                = 027
		{0x00,0x00,0x00,0x00,0x00,0x00,0x00},//***                = 028
		{0x00,0x00,0x00,0x00,0x00,0x00,0x00},//***                = 029
		{0x00,0x00,0x00,0x00,0x00,0x00,0x00},//***                = 030
		{0x00,0x00,0x00,0x00,0x00,0x00,0x00},//***                = 031
		{0x00,0x00,0x00,0x00,0x00,0x00,0x00},//' '                = 032
		{0x00,0x00,0x00,0xFA,0x00,0x00,0x00},//'!'                = 033
		{0x00,0x00,0x60,0x00,0x60,0x00,0x00},//'"'                = 034
		{0x00,0x24,0x7E,0x24,0x7E,0x24,0x00},//'#'                = 035
		{0x00,0x24,0x54,0xFE,0x54,0x48,0x00},//'$'                = 036
		{0xC4,0xC8,0x10,0x20,0x46,0x86,0x00},//'%'                = 037
		{0x00,0x6C,0x92,0xAA,0x44,0x0A,0x00},//'&'                = 038
		{0x00,0x00,0x00,0x50,0x60,0x00,0x00},//'''                = 039
		{0x00,0x00,0x7C,0x82,0x00,0x00,0x00},//'('                = 040
		{0x00,0x00,0x00,0x82,0x7C,0x00,0x00},//')'                = 041
		{0x00,0x28,0x10,0x7C,0x10,0x28,0x00},//'*'                = 042
		{0x00,0x10,0x10,0x7C,0x10,0x10,0x00},//'+'                = 043
		{0x00,0x00,0x05,0x06,0x00,0x00,0x00},//**                 = 044
		{0x00,0x10,0x10,0x10,0x10,0x10,0x00},//'-'                = 045
		{0x00,0x00,0x06,0x06,0x00,0x00,0x00},//'.'                = 046
		{0x02,0x04,0x08,0x10,0x20,0x40,0x80},//'/'                = 047
		{0x00,0x7C,0x8A,0x92,0xA2,0x7C,0x00},//'0'                = 048
		{0x00,0x00,0x42,0xFE,0x02,0x00,0x00},//'1'                = 049
		{0x00,0x42,0x86,0x8A,0x92,0x62,0x00},//'2'                = 050
		{0x00,0x84,0x82,0xA2,0xD2,0x8C,0x00},//'3'                = 051
		{0x00,0x18,0x28,0x48,0xFE,0x08,0x00},//'4'                = 052
		{0x00,0xE4,0xA2,0xA2,0xA2,0x9C,0x00},//'5'                = 053
		{0x00,0x3C,0x52,0x92,0x92,0x0C,0x00},//'6'                = 054
		{0x00,0x80,0x8E,0x90,0xA0,0xC0,0x00},//'7'                = 055
		{0x00,0x6C,0x92,0x92,0x92,0x6C,0x00},//'8'                = 056
		{0x00,0x60,0x92,0x92,0x92,0x7C,0x00},//'9'                = 057
		{0x00,0x00,0x6C,0x6C,0x00,0x00,0x00},//':'                = 058
		{0x00,0x00,0x6A,0x6C,0x00,0x00,0x00},//';'                = 059
		{0x00,0x10,0x28,0x44,0x82,0x00,0x00},//'<'                = 060
		{0x00,0x28,0x28,0x28,0x28,0x28,0x00},//'='                = 061
		{0x00,0x00,0x82,0x44,0x28,0x10,0x00},//'>'                = 062
		{0x00,0x40,0x80,0x8A,0x90,0x60,0x00},//'?'                = 063
		{0x00,0x4C,0x92,0x9A,0x82,0x7C,0x00},//'@'                = 064
		{0x00,0x7E,0x88,0x88,0x88,0x7E,0x00},//'A'                = 065
		{0x00,0xFE,0x92,0x92,0x92,0x6C,0x00},//'B'                = 066
		{0x00,0x7C,0x82,0x82,0x82,0x44,0x00},//'C'                = 067
		{0x00,0xFE,0x82,0x82,0x44,0x38,0x00},//'D'                = 068
		{0x00,0xFE,0x92,0x92,0x92,0x82,0x00},//'E'                = 069
		{0x00,0xFE,0x90,0x90,0x90,0x80,0x00},//'F'                = 070
		{0x00,0x7C,0x82,0x92,0x92,0x5E,0x00},//'G'                = 071
		{0x00,0xFE,0x10,0x10,0x10,0xFE,0x00},//'H'                = 072
		{0x00,0x00,0x82,0xFE,0x82,0x00,0x00},//'I'                = 073
		{0x00,0x04,0x02,0x82,0xFC,0x80,0x00},//'J'                = 074
		{0x00,0xFE,0x10,0x28,0x44,0x82,0x00},//'K'                = 075
		{0x00,0xFE,0x02,0x02,0x02,0x02,0x00},//'L'                = 076
		{0x00,0xFE,0x40,0x30,0x40,0xFE,0x00},//'M'                = 077
		{0x00,0xFE,0x20,0x30,0x08,0xFE,0x00},//'N'                = 078
		{0x00,0x7C,0x82,0x82,0x82,0x7C,0x00},//'O'                = 079
		{0x00,0xFE,0x90,0x90,0x90,0x60,0x00},//'P'                = 080
		{0x00,0x7C,0x82,0x8A,0x84,0x7A,0x00},//'Q'                = 081
		{0x00,0xFE,0x90,0x98,0x94,0x62,0x00},//'R'                = 082
		{0x00,0x62,0x92,0x92,0x92,0x8C,0x00},//'S'                = 083
		{0x00,0x80,0x80,0xFE,0x80,0x80,0x00},//'T'                = 084
		{0x00,0xFC,0x02,0x02,0x02,0xFC,0x00},//'U'                = 085
		{0x00,0xF8,0x04,0x02,0x04,0xF8,0x00},//'V'                = 086
		{0x00,0xFC,0x02,0x1C,0x02,0xFC,0x00},//'W'                = 087
		{0x00,0xC6,0x28,0x10,0x28,0xC6,0x00},//'X'                = 088
		{0x00,0xE0,0x10,0x0E,0x10,0xE0,0x00},//'Y'                = 089
		{0x00,0x86,0x8A,0x92,0xA2,0xC2,0x00},//'Z'                = 090
		{0x00,0x00,0xFE,0x82,0x82,0x00,0x00},//'['                = 091
		{0x00,0x00,0x00,0x00,0x00,0x00,0x00},//**                 = 092
		{0x00,0x00,0x82,0x82,0xFE,0x00,0x00},//']'                = 093
		{0x00,0x00,0x00,0x00,0x00,0x00,0x00},//**                 = 094
		{0x00,0x02,0x02,0x02,0x02,0x02,0x00},//'_'                = 095
		{0x00,0x00,0x00,0x00,0x00,0x00,0x00},//**                 = 096
		{0x00,0x04,0x2A,0x2A,0x2A,0x1E,0x00},//'a'                = 097
		{0x00,0x7E,0x0A,0x12,0x12,0x0C,0x00},//'b'                = 098
		{0x00,0x1C,0x22,0x22,0x22,0x04,0x00},//'c'                = 099
		{0x00,0x0C,0x12,0x12,0x0A,0x7E,0x00},//'d'                = 100
		{0x00,0x1C,0x2A,0x2A,0x2A,0x18,0x00},//'e'                = 101
		{0x00,0x08,0x3E,0x48,0x40,0x20,0x00},//'f'                = 102
		{0x00,0x18,0x25,0x25,0x25,0x3E,0x00},//'g'                = 103
		{0x00,0x7E,0x08,0x10,0x10,0x0E,0x00},//'h'                = 104
		{0x00,0x00,0x00,0x5E,0x00,0x00,0x00},//'i'                = 105
		{0x00,0x02,0x01,0x11,0x5E,0x00,0x00},//'j'                = 106
		{0x00,0x7E,0x08,0x14,0x22,0x00,0x00},//'k'                = 107
		{0x00,0x00,0x42,0x7E,0x02,0x00,0x00},//'l'                = 108
		{0x00,0x3E,0x20,0x18,0x20,0x1E,0x00},//'m'                = 109
		{0x00,0x3E,0x10,0x20,0x20,0x1E,0x00},//'n'                = 110
		{0x00,0x1C,0x22,0x22,0x22,0x1C,0x00},//'o'                = 111
		{0x00,0x3F,0x24,0x24,0x24,0x18,0x00},//'p'                = 112
		{0x00,0x18,0x24,0x24,0x14,0x3F,0x00},//'q'                = 113
		{0x00,0x3E,0x10,0x20,0x20,0x10,0x00},//'r'                = 114
		{0x00,0x12,0x2A,0x2A,0x2A,0x04,0x00},//'s'                = 115
		{0x00,0x20,0x7C,0x22,0x02,0x04,0x00},//'t'                = 116
		{0x00,0x3C,0x02,0x02,0x04,0x3E,0x00},//'u'                = 117
		{0x00,0x38,0x04,0x02,0x04,0x38,0x00},//'v'                = 118
		{0x00,0x3C,0x02,0x0C,0x02,0x3C,0x00},//'w'                = 119
		{0x00,0x22,0x14,0x08,0x14,0x22,0x00},//'x'                = 120
		{0x00,0x38,0x05,0x05,0x05,0x3E,0x00},//'y'                = 121
		{0x00,0x22,0x26,0x2A,0x32,0x22,0x00},//'z'                = 122
		{0x00,0x10,0x6C,0x82,0x00,0x00,0x00},//'{'                = 123
		{0x00,0x00,0x7E,0x00,0x00,0x00,0x00},//'|'                = 124
		{0x00,0x82,0x6C,0x10,0x00,0x00,0x00},//'},'               = 125
        {0x00,0x00,0x00,0x88,0x00,0x00,0x00},//small dots         = 126
		{0x00,0x00,0x00,0x00,0x00,0x00,0x00},//***                = 127
		{0x38,0x44,0xAA,0x8A,0xAA,0x44,0x38},//carita             = 128
		{0x7F,0xDE,0xCF,0xFE,0xDF,0xCE,0x7F},//fantasma pac       = 129
		{0x7C,0xFE,0xFE,0xBE,0xF6,0xE6,0x44},//pacman             = 130
		{0x20,0x40,0x40,0x20,0x20,0x40,0x00},//~                  = 131
		{0x00,0x06,0x0E,0xFC,0x40,0x38,0x00},//corchea            = 132
		{0x3E,0x52,0x92,0xF2,0x82,0x82,0xFE},//file               = 133
		{0x7E,0x82,0x82,0x82,0x42,0x42,0x7E},//folder             = 134
		{0x00,0x00,0x00,0x18,0x18,0x00,0x00},//center point       = 135
		{0x7F,0xB2,0xA1,0x82,0xB1,0xA2,0x7F},//fantasma 2         = 136
		{0x44,0xE6,0xF6,0xBE,0xFE,0xFE,0x7C},//pacman 2           = 137
		{0x10,0x10,0x10,0x10,0x10,0x10,0x10},//separador1         = 138
		{0x10,0x00,0x00,0x00,0x10,0x00,0x00},//separador2         = 139
		{0x00,0x10,0x20,0x7E,0x20,0x10,0x00},//up                 = 140
		{0x00,0x08,0x04,0x7E,0x04,0x08,0x00},//down               = 141
		{0x00,0x78,0x08,0x2A,0x1C,0x08,0x00},//enter              = 142
		{0x00,0x22,0x14,0x08,0x14,0x22,0x00},//exit               = 143
		{0x00,0x00,0xFE,0x7C,0x38,0x10,0x00},//play               = 144
		{0x82,0x44,0x28,0x92,0x44,0x28,0x10},//foward             = 145
		{0x10,0x28,0x44,0x92,0x28,0x44,0x82},//rewind             = 146
		{0x00,0x7E,0x7E,0x7E,0x7E,0x7E,0x00},//stop               = 147
		{0x00,0x7E,0x7E,0x00,0x7E,0x7E,0x00},//pause              = 148
		{0x82,0xC6,0xAE,0x96,0xAE,0xC6,0x82},//clock              = 149
		{0x49,0x92,0x24,0x49,0x92,0x24,0x49},//pattern1           = 150
		{0x49,0x24,0x92,0x48,0x25,0x92,0x49},//pattern2           = 151
		{0x11,0x22,0x44,0x88,0x44,0x22,0x11},//pattern3           = 152
		{0x18,0x24,0x42,0x81,0x42,0x24,0x18},//pattern4           = 153
		{0xFF,0x80,0x80,0x80,0x80,0x80,0x80},//corner topleft     = 154
		{0x80,0x80,0x80,0x80,0x80,0x80,0xFF},//corner topright    = 155
		{0x80,0x80,0x80,0x80,0x80,0x80,0x80},//line top           = 156
		{0x00,0x00,0x00,0x00,0x00,0x00,0xFF},//line right         = 157
		{0xFF,0x01,0x01,0x01,0x01,0x01,0x01},//corner bottomleft  = 158
		{0x01,0x01,0x01,0x01,0x01,0x01,0xFF},//corner bottomright = 159
		{0x01,0x01,0x01,0x01,0x01,0x01,0x01},//line bottom        = 160
		{0xFF,0x00,0x00,0x00,0x00,0x00,0x00},//line left          = 161
		{0xFF,0x80,0x80,0x80,0x80,0x80,0x80},//corner topleft     = 162
		{0x80,0x80,0x80,0x80,0x80,0x80,0xFF},//corner topright    = 163
		{0x55,0x55,0x55,0x55,0x55,0x55,0x55},//hor. lines         = 164
		{0x04,0x0C,0x1C,0x3C,0x1C,0x0C,0x04},//solid up arrow     = 165
		{0x20,0x30,0x38,0x3C,0x38,0x30,0x20},//solid down arrow   = 166
		{0x08,0x08,0x2A,0x1C,0x08,0x00,0x00},//right arrow        = 167
		{0x08,0x1C,0x2A,0x08,0x08,0x00,0x00},//left arrow         = 168
		{0x00,0x00,0xFF,0xAA,0xFF,0x00,0x00},//|| with pattern    = 169
		{0x00,0x00,0xFF,0x00,0xFF,0x00,0x00},//|| empty           = 170
		{0x00,0xFF,0xAA,0x55,0xAA,0xFF,0x00},//| | empty          = 171
		{0x00,0xFF,0x00,0x00,0x00,0xFF,0x00},//| | with pattern   = 172
		{0x00,0xFF,0x00,0x55,0x00,0xFF,0x00},//| | with dots      = 173
		{0x00,0x18,0x18,0x18,0x18,0x18,0x00},//-  thick horz.bar  = 174
		{0x00,0x22,0x14,0x08,0x14,0x22,0x40},//cancel symbol      = 175
		{0x08,0x04,0x02,0x04,0x08,0x10,0x20}//accept symbol      = 176
		// remeber to update the define GRAPHIX_MAX_FONT_SYMBOLS !
};//GRAPHIX_font



void GRAPHIX_Init(){


    // Initialize the configured graphics device and routines
    LCD_INIT;
    
	// Update text buffer color indexes
	text_buffer.ui16_color_idx[GRAPHIX_TEXT_COL_IDX_TRANSPARENT] = LCD_COLOR_TRANSPARENT;
	text_buffer.ui16_color_idx[GRAPHIX_TEXT_COL_IDX_BLACK] = LCD_COLOR_BLACK;
	text_buffer.ui16_color_idx[GRAPHIX_TEXT_COL_IDX_WHITE] = LCD_COLOR_WHITE;
	text_buffer.ui16_color_idx[GRAPHIX_TEXT_COL_IDX_GREY] = LCD_COLOR_GREY;
	text_buffer.ui16_color_idx[GRAPHIX_TEXT_COL_IDX_DARK_GREY] = LCD_COLOR_DARK_GREY;
	text_buffer.ui16_color_idx[GRAPHIX_TEXT_COL_IDX_RED] = LCD_COLOR_BRIGHT_RED;
	text_buffer.ui16_color_idx[GRAPHIX_TEXT_COL_IDX_DARK_RED] = LCD_COLOR_RED;
	text_buffer.ui16_color_idx[GRAPHIX_TEXT_COL_IDX_ORANGE] = LCD_COLOR_ORANGE;
	text_buffer.ui16_color_idx[GRAPHIX_TEXT_COL_IDX_YELLOW] = LCD_COLOR_YELLOW;
	text_buffer.ui16_color_idx[GRAPHIX_TEXT_COL_IDX_DARK_YELLOW] = LCD_COLOR_DARK_YELLOW;
	text_buffer.ui16_color_idx[GRAPHIX_TEXT_COL_IDX_GREEN] = LCD_COLOR_GREEN;
	text_buffer.ui16_color_idx[GRAPHIX_TEXT_COL_IDX_DARK_GREEN] = LCD_COLOR_DARK_GREEN;
	text_buffer.ui16_color_idx[GRAPHIX_TEXT_COL_IDX_BRIGHT_BLUE] = LCD_COLOR_BRIGHT_BLUE;
	text_buffer.ui16_color_idx[GRAPHIX_TEXT_COL_IDX_BLUE] = LCD_COLOR_BLUE;
	text_buffer.ui16_color_idx[GRAPHIX_TEXT_COL_IDX_DARK_BLUE] = LCD_COLOR_DARK_BLUE;

    // Clean the display
	LCD_FAST_FILL(LCD_COLOR_BLACK, 0, 0, LCD_MAX_SIZE_X-1 , LCD_MAX_SIZE_Y-1);

    // Initialize the text buffer
	GRAPHIX_text_buffer_init(22, 12, 5, 11, 1,1, GRAPHIX_TEXT_COL_IDX_ORANGE, GRAPHIX_TEXT_COL_IDX_BLACK, GRAPHIX_TEXT_COL_IDX_YELLOW);
	 	
}//GRAPHIX_init



void GRAPHIX_text_buffer_init(uint16_t ui16_num_cols, uint16_t ui16_num_rows, uint8_t ui16_origX, uint8_t ui16_origY, uint8_t ui8_size_pixelX, uint8_t ui8_size_pixelY, uint8_t ui8_idx_front_color, uint8_t ui8_idx_back_color , uint8_t ui8_idx_shadow_color ){
	uint16_t ui16_row_aux = 0;
	uint16_t ui16_col_aux = 0;


    // protection to avoid color index out of range ( set default values )
	if (ui8_idx_front_color>=GRAPHIX_TEXT_BUFFER_MAX_COLORS) ui8_idx_front_color = GRAPHIX_TEXT_COL_IDX_WHITE;
	if (ui8_idx_back_color>=GRAPHIX_TEXT_BUFFER_MAX_COLORS) ui8_idx_back_color = GRAPHIX_TEXT_COL_IDX_BLACK;
	if (ui8_idx_shadow_color>=GRAPHIX_TEXT_BUFFER_MAX_COLORS) ui8_idx_shadow_color = GRAPHIX_TEXT_COL_IDX_GREY;

    // protection to avoid columns and rows initialization out of range
    if (ui16_num_cols>GRAPHIX_TEXT_BUFFER_MAX_COLUMNS) ui16_num_cols = GRAPHIX_TEXT_BUFFER_MAX_COLUMNS;
    text_buffer.ui16_numb_cols = ui16_num_cols;
    if (ui16_num_rows>GRAPHIX_TEXT_BUFFER_MAX_ROWS) ui16_num_rows = GRAPHIX_TEXT_BUFFER_MAX_ROWS;
	text_buffer.ui16_numb_rows = ui16_num_rows;
	
	text_buffer.ui8_x_origin = ui16_origX;
	text_buffer.ui8_y_origin = ui16_origY;

	text_buffer.ui8_pixel_sizeX = ui8_size_pixelX;
	text_buffer.ui8_pixel_sizeY = ui8_size_pixelY;
    
	// initialize background image properties
	text_buffer.ui8_backgr_char = ' ';
	text_buffer.ui8_backgr_front_color_idx = ui8_idx_front_color;
	text_buffer.ui8_backgr_back_color_idx = ui8_idx_back_color;
	
    // initialize each row of the text buffer
	for (ui16_row_aux=0;ui16_row_aux<GRAPHIX_TEXT_BUFFER_MAX_ROWS;ui16_row_aux++){
		
	    // initialize each char of the text buffer row
		for (ui16_col_aux=0;ui16_col_aux<GRAPHIX_TEXT_BUFFER_MAX_COLUMNS;ui16_col_aux++){
		   	   
			text_buffer.row[ui16_row_aux].ui8_text[ui16_col_aux] =' ';
			text_buffer.row[ui16_row_aux].ui8_color_idx_front[ui16_col_aux] = ui8_idx_front_color;
			text_buffer.row[ui16_row_aux].ui8_color_idx_back[ui16_col_aux] = ui8_idx_back_color;
			text_buffer.row[ui16_row_aux].ui8_color_idx_shadow[ui16_col_aux] = ui8_idx_shadow_color;
			text_buffer.row[ui16_row_aux].ui8_attributes[ui16_col_aux]  = ATTR_REFRESH;
			
		}//for
	   
	}//for

}//GRAPHIX_text_buffer_init



void GRAPHIX_text_buffer_refresh(){
	uint16_t ui16_row_aux = 0;
	uint16_t ui16_col_aux = 0;
    uint16_t ui16_x = 0;
	uint16_t ui16_y = 0;
    uint16_t ui16_front_color = 0;
	uint16_t ui16_back_color = 0;
	uint16_t ui16_shadow_color = 0;
	uint8_t  ui8_idx = 0;
    uint8_t  ui8_char_pixel_sizeX;
	uint8_t  ui8_char_pixel_sizeY;
	uint8_t  ui8_aux;


	ui8_char_pixel_sizeX = GRAPHIX_FONT1_PIXELS_X * text_buffer.ui8_pixel_sizeX;
	ui8_char_pixel_sizeY = GRAPHIX_FONT1_PIXELS_Y * text_buffer.ui8_pixel_sizeY;

	ui16_y = (uint16_t) text_buffer.ui8_y_origin;
	for (ui16_row_aux=0;ui16_row_aux<text_buffer.ui16_numb_rows;ui16_row_aux++){
		
		ui16_x = (uint16_t) text_buffer.ui8_x_origin;
		for (ui16_col_aux=0;ui16_col_aux<text_buffer.ui16_numb_cols;ui16_col_aux++){
					
			// check if the ATTR_REFRESH flag is set and paint the character on screen if affirmative
			if ((text_buffer.row[ui16_row_aux].ui8_attributes[ui16_col_aux]&ATTR_REFRESH)!=0){
				
				// clear the refresh flag
				text_buffer.row[ui16_row_aux].ui8_attributes[ui16_col_aux]= text_buffer.row[ui16_row_aux].ui8_attributes[ui16_col_aux] & (~ATTR_REFRESH);
				
				// print current processed position character in screen
				ui8_idx = text_buffer.row[ui16_row_aux].ui8_color_idx_front[ui16_col_aux];
				ui16_front_color = text_buffer.ui16_color_idx[ui8_idx];
				ui8_idx = text_buffer.row[ui16_row_aux].ui8_color_idx_back[ui16_col_aux];
				ui16_back_color = text_buffer.ui16_color_idx[ui8_idx];
				
				ui8_aux = text_buffer.row[ui16_row_aux].ui8_text[ui16_col_aux];
				if (ui8_aux==' '){
					
					// blank space ' ' character display management
					
					if ((text_buffer.row[ui16_row_aux].ui8_attributes[ui16_col_aux]&ATTR_SPACE_BACKSYMBOL)!=0){
						// if the fill blank spaces with background char flag is active, replace all the blank spaces with
						// the background char set last time the text buffer fill command was called
						ui8_aux = text_buffer.ui8_backgr_char;
						ui16_front_color = text_buffer.ui16_color_idx[text_buffer.ui8_backgr_front_color_idx];
						ui16_back_color = text_buffer.ui16_color_idx[text_buffer.ui8_backgr_back_color_idx];
						GRAPHIX_print_char(ui16_x, ui16_y, text_buffer.ui8_pixel_sizeX , text_buffer.ui8_pixel_sizeY, ui16_front_color, ui16_back_color, ui8_aux);	
					}else{
						// if the fill blank spaces with background char flag is NOT active then print the standard ' ' code.
						GRAPHIX_print_char(ui16_x, ui16_y, text_buffer.ui8_pixel_sizeX , text_buffer.ui8_pixel_sizeY, ui16_front_color, ui16_back_color, ui8_aux);	
					}//if
									
				}else{
					
					// all other character display management	
					
					if ((text_buffer.row[ui16_row_aux].ui8_attributes[ui16_col_aux]&ATTR_SHADOW)!=0){
					     // if shadow flag is active, first print the character shifted 1 pixel and then, the caracter over it
						ui8_idx = text_buffer.row[ui16_row_aux].ui8_color_idx_shadow[ui16_col_aux];
						ui16_shadow_color = text_buffer.ui16_color_idx[ui8_idx];
						GRAPHIX_print_char(ui16_x + text_buffer.ui8_pixel_sizeX , ui16_y + text_buffer.ui8_pixel_sizeY , text_buffer.ui8_pixel_sizeX , text_buffer.ui8_pixel_sizeY, ui16_shadow_color, ui16_back_color, ui8_aux);
						ui16_back_color = text_buffer.ui16_color_idx[GRAPHIX_TEXT_COL_IDX_TRANSPARENT];
					}//if
					GRAPHIX_print_char(ui16_x, ui16_y, text_buffer.ui8_pixel_sizeX , text_buffer.ui8_pixel_sizeY, ui16_front_color, ui16_back_color, ui8_aux);
				
				}//if
			
			}//if
	
			ui16_x = ui16_x + ui8_char_pixel_sizeX;		
			
		}//for
		
		ui16_y = ui16_y + ui8_char_pixel_sizeY;
		
	}//for

}//GRAPHIX_text_buffer_refresh



void GRAPHIX_text_buffer_set_char(uint16_t ui16_col , uint16_t ui16_row, uint8_t ui8_char,  uint8_t ui8_attribs, uint8_t ui8_idx_front_color, uint8_t ui8_idx_back_color, uint8_t ui8_idx_shadow_color){


	if ( (ui16_row<text_buffer.ui16_numb_rows)&&(ui16_col<text_buffer.ui16_numb_cols) ){

		// update the text in the buffer only if any of the fields has changed otherwise keep it
        if ( (text_buffer.row[ui16_row].ui8_text[ui16_col] !=ui8_char ) ||
			    (text_buffer.row[ui16_row].ui8_attributes[ui16_col] != ui8_attribs) ||
                (text_buffer.row[ui16_row].ui8_color_idx_front[ui16_col] != ui8_idx_front_color) ||
                (text_buffer.row[ui16_row].ui8_color_idx_back[ui16_col] != ui8_idx_back_color) ||
                (text_buffer.row[ui16_row].ui8_color_idx_shadow[ui16_col] != ui8_idx_shadow_color) ){
                    
				// update the fields of the buffer char with new received values
				text_buffer.row[ui16_row].ui8_text[ui16_col] = ui8_char;
				text_buffer.row[ui16_row].ui8_color_idx_front[ui16_col] = ui8_idx_front_color;
				text_buffer.row[ui16_row].ui8_color_idx_back[ui16_col] = ui8_idx_back_color;
				text_buffer.row[ui16_row].ui8_color_idx_shadow[ui16_col] = ui8_idx_shadow_color;
					
				// set the "modified" flag in order to force rewriting the updated character next time that ".refresh" function is called
				text_buffer.row[ui16_row].ui8_attributes[ui16_col] = ui8_attribs | ATTR_REFRESH;
			
		}//if			

	}//if

}//GRAPHIX_text_buffer_set_char



void GRAPHIX_text_buffer_set_string(uint16_t ui16_col , uint16_t ui16_row, uint8_t * pui8_string,  uint8_t ui8_attribs, uint8_t ui8_idx_front_color, uint8_t ui8_idx_back_color, uint8_t ui8_idx_shadow_color){
	uint16_t ui16_col_n =0;
	uint16_t ui16_char_n =0;


	if ( (ui16_row<text_buffer.ui16_numb_rows)&&(ui16_col<text_buffer.ui16_numb_cols) ){
		
		ui16_col_n = ui16_col;
		ui16_char_n = 0;
		while ( (ui16_col_n<text_buffer.ui16_numb_cols) && (pui8_string[ui16_char_n]!='\0') ){
			
			// update the text in the buffer only if any of the fields has changed otherwise keep it
            if ( (text_buffer.row[ui16_row].ui8_text[ui16_col_n] != pui8_string[ui16_char_n] ) ||
			     (text_buffer.row[ui16_row].ui8_attributes[ui16_col_n] != ui8_attribs) ||
                 (text_buffer.row[ui16_row].ui8_color_idx_front[ui16_col_n] != ui8_idx_front_color) ||
                 (text_buffer.row[ui16_row].ui8_color_idx_back[ui16_col_n] != ui8_idx_back_color) ||
                 (text_buffer.row[ui16_row].ui8_color_idx_shadow[ui16_col_n] != ui8_idx_shadow_color) ){
                    
					// update the fields of the buffer char with new received values
					text_buffer.row[ui16_row].ui8_text[ui16_col_n] = pui8_string[ui16_char_n];
					text_buffer.row[ui16_row].ui8_color_idx_front[ui16_col_n] = ui8_idx_front_color;
					text_buffer.row[ui16_row].ui8_color_idx_back[ui16_col_n] = ui8_idx_back_color;
					text_buffer.row[ui16_row].ui8_color_idx_shadow[ui16_col_n] = ui8_idx_shadow_color;
					
					// set the "modified" flag in order to force rewriting the updated character next time that ".refresh" function is called
					text_buffer.row[ui16_row].ui8_attributes[ui16_col_n] = ui8_attribs | ATTR_REFRESH;
			
			}//if			
			
			// set text buffer column cursor to next position
			ui16_col_n ++;
			// set string char index to next position
			ui16_char_n++;
			
		}//while

	}//if

}//GRAPHIX_text_buffer_set_string



void GRAPHIX_text_buffer_fill(uint8_t ui8_char, uint8_t ui8_attribs, uint8_t ui8_idx_front_color, uint8_t ui8_idx_back_color, uint8_t ui8_idx_shadow_color){
	uint16_t  ui16_row = 0;
	uint16_t  ui16_col = 0;


	// initialize background image properties
	text_buffer.ui8_backgr_char = ui8_char;
	text_buffer.ui8_backgr_front_color_idx = ui8_idx_front_color;
	text_buffer.ui8_backgr_back_color_idx = ui8_idx_back_color;

    // print all text buffer characters according to the attribute values	
	for (ui16_row=0;ui16_row<text_buffer.ui16_numb_rows;ui16_row++){
				
		for (ui16_col=0;ui16_col<text_buffer.ui16_numb_cols;ui16_col++){

			// update the text in the buffer only if any of the fields has changed otherwise keep it
            if ( (text_buffer.row[ui16_row].ui8_text[ui16_col] != ui8_char) ||
			     (text_buffer.row[ui16_row].ui8_attributes[ui16_col] != ui8_attribs) ||
                 (text_buffer.row[ui16_row].ui8_color_idx_front[ui16_col] != ui8_idx_front_color) ||
                 (text_buffer.row[ui16_row].ui8_color_idx_back[ui16_col] != ui8_idx_back_color) ||
                 (text_buffer.row[ui16_row].ui8_color_idx_shadow[ui16_col] != ui8_idx_shadow_color) ){
                    
					// update the fields of the buffer char with new received values
					text_buffer.row[ui16_row].ui8_text[ui16_col] = ui8_char;
					text_buffer.row[ui16_row].ui8_color_idx_front[ui16_col] = ui8_idx_front_color;
					text_buffer.row[ui16_row].ui8_color_idx_back[ui16_col] = ui8_idx_back_color;
					text_buffer.row[ui16_row].ui8_color_idx_shadow[ui16_col] = ui8_idx_shadow_color;
					
					// set the "modified" flag in order to force rewriting the updated character next time that ".refresh" function is called
					text_buffer.row[ui16_row].ui8_attributes[ui16_col] = ui8_attribs | ATTR_REFRESH;
			
			}//if
			
		}//for
		
	}//for

}//GRAPHIX_text_buffer_fill



void GRAPHIX_text_buffer_fill_attr(uint8_t ui8_attribs){
	uint16_t ui16_row_aux = 0;
	uint16_t ui16_col_aux = 0;
	uint8_t  ui8_aux;

	for (ui16_row_aux=0;ui16_row_aux<text_buffer.ui16_numb_rows;ui16_row_aux++){
		
		for (ui16_col_aux=0;ui16_col_aux<text_buffer.ui16_numb_cols;ui16_col_aux++){
			
			ui8_aux = text_buffer.row[ui16_row_aux].ui8_attributes[ui16_col_aux] | ui8_attribs;
			text_buffer.row[ui16_row_aux].ui8_attributes[ui16_col_aux] = ui8_aux;
			
		}//for
		
	}//for
	
}//GRAPHIX_text_buffer_fill_attr



void GRAPHIX_line(int16_t i16_x0, int16_t i16_y0, int16_t i16_x1, int16_t i16_y1, uint16_t ui16_color){
	int dx =  abs (i16_x1 - i16_x0), sx = i16_x0 < i16_x1 ? 1 : -1;
	int dy = -abs (i16_y1 - i16_y0), sy = i16_y0 < i16_y1 ? 1 : -1;
	int err = dx + dy, e2; /* error value e_xy */

	
	for (;;){  /* loop */
		
		LCD_SET_PIXEL(i16_x0, i16_y0, ui16_color);
		if (i16_x0 == i16_x1 && i16_y0 == i16_y1) break;
		e2 = 2 * err;
		if (e2 >= dy) { err += dy; i16_x0 += sx; } /* e_xy+e_x > 0 */
		if (e2 <= dx) { err += dx; i16_y0 += sy; } /* e_xy+e_y < 0 */
		
	}//for

}//GRAPHIX_line



void GRAPHIX_circle (int16_t i16_cx, int16_t i16_cy, int16_t i16_radius, uint16_t ui16_color){
	int16_t i16_error = -i16_radius;
	int16_t i16_x = i16_radius;
	int16_t i16_y = 0;


	// The following while loop may altered to 'while (x > y)' for a
	// performance benefit, as long as a call to 'plot4points' follows
	// the body of the loop. This allows for the elimination of the
	// '(x != y') test in 'plot8points', providing a further benefit.
	//
	// For the sake of clarity, this is not shown here.
	while (i16_x >= i16_y){
		
		// BEGIN plot 8 points:
		// BEGIN plot 4 points:
		LCD_SET_PIXEL(i16_cx + i16_x, i16_cy + i16_y,ui16_color);
		if (i16_x != 0) LCD_SET_PIXEL(i16_cx - i16_x, i16_cy + i16_y,ui16_color);
		if (i16_y != 0) LCD_SET_PIXEL(i16_cx + i16_x, i16_cy - i16_y,ui16_color);
		if (i16_x != 0 && i16_y != 0) LCD_SET_PIXEL(i16_cx - i16_x, i16_cy - i16_y, ui16_color);
		// END plot 4 points
		// BEGIN plot 4 points:
		if (i16_x != i16_y){
			LCD_SET_PIXEL(i16_cx + i16_y, i16_cy + i16_x,ui16_color);
			if (i16_y != 0) LCD_SET_PIXEL(i16_cx - i16_y, i16_cy + i16_x,ui16_color);
			if (i16_x != 0) LCD_SET_PIXEL(i16_cx + i16_y, i16_cy - i16_x,ui16_color);
			if (i16_y != 0 && i16_x != 0) LCD_SET_PIXEL(i16_cx - i16_y, i16_cy - i16_x, ui16_color);
		}
		// END plot 4 points
		// END plot 8 points:
		
		i16_error += i16_y;
		++i16_y;
		i16_error += i16_y;
		// The following test may be implemented in assembly language in
		// most machines by testing the carry flag after adding 'y' to
		// the value of 'error' in the previous step, since 'error'
		// nominally has a negative value.
		if (i16_error >= 0){
			--i16_x;
			i16_error -= i16_x;
			i16_error -= i16_x;
		}
		
	}//while
	
}//GRAPHIX_circle



void GRAPHIX_raster_circle(int16_t i16_x0, int16_t i16_y0, int16_t i16_radius, uint16_t ui16_color){
	int16_t i16_f = 1 - i16_radius;
	int16_t i16_ddF_x = 1;
	int16_t i16_ddF_y = -2 * i16_radius;
	int16_t i16_x = 0;
	int16_t i16_y = i16_radius;


	LCD_SET_PIXEL(i16_x0, i16_y0 + i16_radius, ui16_color);
	LCD_SET_PIXEL(i16_x0, i16_y0 - i16_radius, ui16_color);
	LCD_SET_PIXEL(i16_x0 + i16_radius, i16_y0, ui16_color);
	LCD_SET_PIXEL(i16_x0 - i16_radius, i16_y0, ui16_color);
	while (i16_x < i16_y){
		
		// ddF_x == 2 * x + 1;
		// ddF_y == -2 * y;
		// f == x*x + y*y - radius*radius + 2*x - y + 1;
		if (i16_f >= 0){
			i16_y--;
			i16_ddF_y += 2;
			i16_f += i16_ddF_y;
		}
		
		i16_x++;
		i16_ddF_x += 2;
		i16_f += i16_ddF_x;
		LCD_SET_PIXEL(i16_x0 + i16_x, i16_y0 + i16_y, ui16_color);
		LCD_SET_PIXEL(i16_x0 - i16_x, i16_y0 + i16_y, ui16_color);
		LCD_SET_PIXEL(i16_x0 + i16_x, i16_y0 - i16_y, ui16_color);
		LCD_SET_PIXEL(i16_x0 - i16_x, i16_y0 - i16_y, ui16_color);
		LCD_SET_PIXEL(i16_x0 + i16_y, i16_y0 + i16_x, ui16_color);
		LCD_SET_PIXEL(i16_x0 - i16_y, i16_y0 + i16_x, ui16_color);
		LCD_SET_PIXEL(i16_x0 + i16_y, i16_y0 - i16_x, ui16_color);
		LCD_SET_PIXEL(i16_x0 - i16_y, i16_y0 - i16_x, ui16_color);
		
	}//while
	
}//GRAPHIX_raster_circle



void GRAPHIX_rectangle(int16_t i16_x0, int16_t i16_y0, int16_t i16_x1, int16_t i16_y1, uint16_t ui16_border_color, uint16_t ui16_back_color, uint8_t ui8_border, uint8_t ui8_solid){
	
	
	// fill rectangle background
	if (ui8_solid==TRUE){
		LCD_FAST_FILL(ui16_back_color, i16_x0, i16_y0, i16_x1 , i16_y1);
	}//if
	
	// draw rectangle borders
	if (ui8_border==TRUE){
		// top horizontal line
		GRAPHIX_line(i16_x0, i16_y0, i16_x1, i16_y0,ui16_border_color);
		// bottom horizontal line
		GRAPHIX_line(i16_x0, i16_y1, i16_x1, i16_y1,ui16_border_color);
		// left vertical line
		GRAPHIX_line(i16_x0, i16_y0, i16_x0, i16_y1,ui16_border_color);
		// right vertical line
		GRAPHIX_line(i16_x1, i16_y0, i16_x1, i16_y1,ui16_border_color);
	}//if
	
}//GRAPHIX_rectangle



void GRAPHIX_print_char(uint16_t ui16_x_orig, uint16_t ui16_y_orig, uint8_t ui8_x_pixel_size, uint8_t ui8_y_pixel_size, uint16_t ui16_color, uint16_t ui16_back_color, uint8_t ui8_char_code ){
	uint16_t ui8_char_col,ui8_char_row;
	uint8_t ui8_mask=0x00;
	uint16_t ui16_pix_x,ui16_pix_y;
	uint16_t ui16_pix_iter_x,ui16_pix_iter_y;

	
	// check the index, in the array of the font symbols, of the symbol that corresponds to the received 'ui8_char_code' 
	// and use the default invalid char font symbol if the received char is not valid or is out of range in the font
	if (ui8_char_code>=GRAPHIX_MAX_FONT_SYMBOLS){
		// invalid char code received, so use the font symbol used for invalid char codes
		ui8_char_code = GRAPHIX_INV_CHAR_FONT_SYMBOL;
	}//if
	
	// pixel iterations in x and in y
	ui16_pix_iter_x = ui8_x_pixel_size - 1;
	ui16_pix_iter_y = ui8_y_pixel_size - 1;
		
	// print the pixels of the character in the specified location with the specified pixel sizes and colors
	ui16_pix_x = ui16_x_orig;
	for (ui8_char_col=0;ui8_char_col<7;ui8_char_col++){
			
		ui8_mask=0x80;
		ui16_pix_y = ui16_y_orig;
		for (ui8_char_row=0;ui8_char_row<8;ui8_char_row++){
				
			if ((GRAPHIX_font_symbols[ui8_char_code][ui8_char_col]&ui8_mask)!=0){;
				// set pixel with received COLOR
				if (ui16_color!=LCD_TRANSPARENT) LCD_FAST_FILL(ui16_color, ui16_pix_x, ui16_pix_y, ui16_pix_x + ui16_pix_iter_x , ui16_pix_y + ui16_pix_iter_y);
			}else{
				// set pixel with received BACK_COLOR
				if (ui16_back_color!=LCD_TRANSPARENT) LCD_FAST_FILL(ui16_back_color, ui16_pix_x, ui16_pix_y, ui16_pix_x + ui16_pix_iter_x , ui16_pix_y + ui16_pix_iter_y);
			}
			ui16_pix_y=ui16_pix_y+ui8_y_pixel_size;
			ui8_mask=ui8_mask>>1;
				
		}//for
		ui16_pix_x = ui16_pix_x + ui8_x_pixel_size;
			
	}//for
		
}//GRAPHIX_print_char



void GRAPHIX_print_string(uint16_t ui16_x_orig, uint16_t ui16_y_orig, uint8_t ui8_x_pixel_size, uint8_t ui8_y_pixel_size, uint16_t ui16_color, uint16_t ui16_back_color, uint8_t * ui8_string){
	uint8_t  ui8_index = 0x00;
	uint16_t ui16_x_char = 0;
	uint16_t ui16_y_char = 0;
	uint16_t ui16_x_char_size = 0;
	// uint16_t ui16_y_char_size = 0;


	if (ui8_string!=NULL){
		
		ui8_index=0x00;
		
		ui16_x_char=ui16_x_orig;
		ui16_y_char=ui16_y_orig;
		
		ui16_x_char_size = 7*ui8_x_pixel_size;
		// ui16_y_char_size = 8*ui8_y_pixel_size;
		
		ui8_index=0x00;
		while (ui8_string[ui8_index]!='\0'){
			
			GRAPHIX_print_char(ui16_x_char, ui16_y_char, ui8_x_pixel_size, ui8_y_pixel_size, ui16_color, ui16_back_color, ui8_string[ui8_index] );

			ui16_x_char=ui16_x_char+ui16_x_char_size;
			// ui16_y_char=ui16_y_char+ui16_y_char_size;
			ui8_index++;
		}//while
		
	}//if
	
}//GRAPHIX_print_string



// Reads and plots the specified RUNING LENGTH compressed r5g6b5 file in 16 bits R5G6B5 compressed file format
void GRAPHIX_print_img_from_mem(int16_t i16_x0, int16_t i16_y0, uint32_t ui32_width, uint32_t ui32_height, uint8_t ui8_pix_size, uint16_t * pui16_mem_img, uint32_t ui32_mem_img_words){
	int i_ret_val = 0;
	uint16_t ui16_x;
	uint16_t ui16_y;
	uint16_t ui16_orig_x=(uint16_t)i16_x0;
	uint16_t ui16_orig_y=(uint16_t)i16_y0;
	uint16_t ui16_aux = 0;
	uint16_t ui16_RGB;// 5bits R, 6bits G, 5 bits B
	uint16_t ui16_RGB_num_repetitions;// 5bits R, 6bits G, 5 bits B
	uint16_t ui16_pixels_curr_row = 0;
    uint32_t ui32_word_index = 0;


	if (pui16_mem_img==NULL){
		i_ret_val = -1;
	}//if

	if (i_ret_val >= 0){

        // read and plot RUNING LENGTH compressed R5G6B5 file pixels
		ui16_pixels_curr_row = 0;
        ui16_x = ui16_orig_x;
        ui16_y = ui16_orig_y + (int)(ui32_height*ui8_pix_size);
		ui32_word_index = 0;
		// ui32_width = ui32_width * ui8_pix_size;
		// ui32_height = ui32_height * ui8_pix_size;
        while (ui32_word_index < ui32_mem_img_words){
	        
	        ui16_RGB = pui16_mem_img[ui32_word_index];
			ui32_word_index++;
	        
			if (ui16_RGB == SPECIAL_CODE_RUN_LENGTH){
				
		        //  SPECIAL_CODE_RUN_LENGTH is followed by color repetitions
		        //  number field (uint16) and by color code field (R5G6B5 uint16)
		        ui16_RGB_num_repetitions = pui16_mem_img[ui32_word_index];
				ui32_word_index++;
		        ui16_RGB  = pui16_mem_img[ui32_word_index];
				ui32_word_index++;
				
		    }else{
		        ui16_RGB_num_repetitions = 1;
	        }//if

	        // draw the corresponding number of pixels: it can be a single pixel or the
	        // value set in the SPECIAL_CODE_RUN_LENGTH
	        for (ui16_aux = 0; ui16_aux < ui16_RGB_num_repetitions; ui16_aux++){
		        
		        // set pixel color
				LCD_FAST_FILL(ui16_RGB, ui16_x, ui16_y, ui16_x + ui8_pix_size, ui16_y + ui8_pix_size);
				
		        // set next pixel coordinates
		        ui16_x=ui16_x + ui8_pix_size;
	            ui16_pixels_curr_row ++;				
		        if (ui16_pixels_curr_row >= ui32_width ) {
					ui16_pixels_curr_row = 0;
			        ui16_x = ui16_orig_x;
			        ui16_y=ui16_y - ui8_pix_size;
		        }//if
		        
	        }//for
   
       }//while
       
    }//if (i_ret_val>=0)

}//GRAPHIX_print_img_from_mem

