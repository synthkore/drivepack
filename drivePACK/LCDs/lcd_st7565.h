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


/*********************************************************************************************
* @brief initializes the ST7565 compatible graphic display hardware
*********************************************************************************************/
void LCD_ST7565_init();

/*********************************************************************************************
* @brief sends a data byte to the ST7565 compatible graphic display through the SPI applying
* the right SEND DATA byte signals sequence:
*   clear LCD_ST7565_CS to '0'
*   set LCD_ST7565_RS to '1'
*   send received command byte through SPI
*   wait for command byte transmission to complete
*   set LCD_ST7565_CS to '1'
* @param[in] ui8_data_to_send with the data value to send
* @return
*********************************************************************************************/
uint8_t LCD_ST7565_write_SPI_data(uint8_t ui8_data_to_send);

/*********************************************************************************************
* @brief sends a command byte to the ST7565 compatible graphic display through the SPI applying
* the right SEND COMMAND byte signals sequence:
*   clear LCD_ST7565_CS to '0'
*   clear LCD_ST7565_RS to '0'
*   send received command byte through SPI
*   wait for command byte transmission to complete
*   set LCD_ST7565_CS to '1'
* @param[in] ui8_command_to_send with the command value to send
* @return 
*********************************************************************************************/
uint8_t LCD_ST7565_write_SPI_command(uint8_t ui8_command_to_send);

/*********************************************************************************************
* @brief fills the internal buffer ( not the ST7565 display  ) with the received ui8_page_pattern.
* To see the result in the display screen the buffer must be refreshed.
* @param[in] ui8_page_pattern the bit pattern used to fill the internal buffer
*********************************************************************************************/
void LCD_ST7565_buffer_fill(uint8_t ui8_page_pattern);

/*********************************************************************************************
* @brief fills the rectangular area defined by the x1,y1 and x2,y2 in the internal graphic 
* buffer used to update  the ST7565 display with the received bit pattern. To see the result in
* the display screen the buffer must be refreshed.
* @param[in] ui16_pattern pattern applied into the display buffer.
* @param[in] ui16_x1 X coordinate of the top left corner of the rectangular area to fill
* @param[in] ui16_y1 Y coordinate of the top left corner of the rectangular area to fill
* @param[in] ui16_x2 X coordinate of the bottom right corner of the rectangular area to fill
* @param[in] ui16_y2 Y coordinate of the bottom right corner of the rectangular area to fill
*********************************************************************************************/
void LCD_ST7565_buffer_area_fast_fill(uint16_t ui16_pattern, uint16_t ui16_x1, uint16_t ui16_y1, uint16_t ui16_x2, uint16_t ui16_y2);

/*********************************************************************************************
* @brief Sets the specified pixel in the internal graphic buffer to the specified color ( 1=black,
* 0=white). To see the result in the display screen the buffer must be refreshed.
* @param[in] ui8_x coordinate of the pixel to paint
* @param[in] ui8_y coordinate of the pixel to paint
* @param[in] ui8_bw desired color for the specified pixel ( 0 or 1 ).
*********************************************************************************************/
void LCD_ST7565_buffer_set_pixel(uint8_t ui8_x, uint8_t ui8_y, uint8_t ui8_bw);

/*********************************************************************************************
* @brief repaints the content of the internal graphic buffer in the ST7565 display.
*********************************************************************************************/
void LCD_ST7565_buffer_refresh();

/*********************************************************************************************
* @brief fills the ST7565 compatible display ( not the buffer ) with the received ui8_page_pattern.
* @param[in] ui8_page_pattern the bit pattern used to fill the display.
*********************************************************************************************/
void LCD_ST7565_clear(uint8_t ui8_page_pattern);

#endif /* LCD_ST7565_H_ */

#endif //#ifdef LCD_IS_ST7565
