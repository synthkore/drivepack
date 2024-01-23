// *****************************************************************
// ****                       drivePACK                         ****
// ****                  www.tolaemon.com/dpack                 ****
// ****                       Source code                       ****
// ****                       25/07/2021                        ****
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

#ifdef LCD_IS_SH1106

#ifndef LCD_SH1106_H_
#define LCD_SH1106_H_

#define LCD_SH1106_SIZE_X 128
#define LCD_SH1106_SIZE_Y 64

#define LCD_SH1106_COLS_X LCD_SH1106_SIZE_X
#if ((LCD_SH1106_SIZE_Y%8)!=0)
#define LCD_SH1106_PAGES_Y (LCD_SH1106_SIZE_Y/8)+1
#else
#define LCD_SH1106_PAGES_Y (LCD_SH1106_SIZE_Y/8)
#endif


#define LCD_SH1106_I2C_ADDRESS 0x3C

#define LCD_SH1106_READ  0x01
#define LCD_SH1106_WRITE 0x00

#define LCD_SH1106_CMD_CHARGEPUMP          0x8D
#define LCD_SH1106_CMD_COMSCANDEC          0xC8
#define LCD_SH1106_CMD_COMSCANINC          0xC0
#define LCD_SH1106_CMD_DISPLAYALLON        0xA5
#define LCD_SH1106_CMD_DISPLAYALLON_RESUME 0xA4
#define LCD_SH1106_CMD_DISPLAYOFF          0xAE
#define LCD_SH1106_CMD_DISPLAYON           0xAF
#define LCD_SH1106_CMD_EXTERNALVCC         0x01
#define LCD_SH1106_CMD_INVERTDISPLAY       0xA7
#define LCD_SH1106_CMD_MEMORYMODE          0x20
#define LCD_SH1106_CMD_NORMALDISPLAY       0xA6
#define LCD_SH1106_CMD_COLUMNADDR          0x21
#define LCD_SH1106_CMD_PAGEADDR            0x22
#define LCD_SH1106_CMD_PAGESTARTADDRESS    0xB0
#define LCD_SH1106_CMD_SEGREMAP            0xA1
#define LCD_SH1106_CMD_SETCOMPINS          0xDA
#define LCD_SH1106_CMD_SETCONTRAST         0x81
#define LCD_SH1106_CMD_SETDISPLAYCLOCKDIV  0xD5
#define LCD_SH1106_CMD_SETDISPLAYOFFSET    0xD3
#define LCD_SH1106_CMD_SETHIGHCOLUMN       0x10
#define LCD_SH1106_CMD_SETLOWCOLUMN        0x00
#define LCD_SH1106_CMD_SETMULTIPLEX        0xA8
#define LCD_SH1106_CMD_SETPRECHARGE        0xD9
#define LCD_SH1106_CMD_SETSEGMENTREMAP     0xA1
#define LCD_SH1106_CMD_SETSTARTLINE        0x40
#define LCD_SH1106_CMD_SETVCOMDETECT       0xDB
#define LCD_SH1106_CMD_SWITCHCAPVCC        0x02

#define LCD_SH1106_PAUSE_NOPS(nops_number)   __asm__ __volatile__ ("nop;"); 
#define LCD_SH1106_PAUSE_TICKS(ticks_number) tticks_timer = TIMING_SetTicksDelay(ticks_number); while (TIMING_CheckTicksDelayReached(tticks_timer)==FALSE){1;};

// #define LCD_SH1106_RS_SET  PORT->Group[1].OUTSET.reg = P1_LCD_MASK_LCD_RS
// #define LCD_SH1106_RS_CLR  PORT->Group[1].OUTCLR.reg = P1_LCD_MASK_LCD_RS
// #define LCD_SH1106_WR_SET  PORT->Group[1].OUTSET.reg = P1_LCD_MASK_LCD_WR
// #define LCD_SH1106_WR_CLR  PORT->Group[1].OUTCLR.reg = P1_LCD_MASK_LCD_WR
// #define LCD_SH1106_CS_SET  PORT->Group[1].OUTSET.reg = P1_LCD_MASK_LCD_CS
// #define LCD_SH1106_CS_CLR  PORT->Group[1].OUTCLR.reg = P1_LCD_MASK_LCD_CS
// #define LCD_SH1106_RST_SET PORT->Group[1].OUTSET.reg = P1_LCD_MASK_LCD_RST
// #define LCD_SH1106_RST_CLR PORT->Group[1].OUTCLR.reg = P1_LCD_MASK_LCD_RST

#define LCD_SH1106_TRANSPARENT	         0xFFFE
#define LCD_SH1106_WHITE                 0x0000
#define LCD_SH1106_BLACK                 0x0001

/*********************************************************************************************
* @brief initializes the SH1106 compatible graphic display hardware
*********************************************************************************************/
void LCD_SH1106_init(uint8_t ui8_address);

/*********************************************************************************************
* @brief sends the data frame through the I2C and returns if that frame
* has been accepted or not by the destination.
* @param[in] ui8_read_write R/!W 1 if it corresponds to a read command, 0 if it
*  corresponds to a write command.
* @reutrn >=0 the data frame has been received and acknowledged by the destination
* <0 the data frame has not been acknowledged by the destination
*********************************************************************************************/
int16_t LCD_SH1106_write_I2C_data_frame(uint8_t ui8_data_to_send);

/*********************************************************************************************
* @brief sends the address frame through the I2C to the the SH1106 compatible graphic display 
* and returns if that address has been accepted or not. The right address must have been properly
* configured in the init function before calling this function.
* @param[in] ui8_read_write: R/!W 1 if it corresponds to a read command, 0 if it 
* corresponds to a write command.
* @return >=0 the address has been received and acknowledged by the destination
* <0 the address has not been acknowledged by any destination
*********************************************************************************************/
int16_t LCD_SH1106_write_I2C_address_frame(uint8_t ui8_read_write);

/*********************************************************************************************
* @brief Sends an LCD COMMAND message to the SH1106 compatible graphic display. The LCD COMMAND message
* is sent by sending the following SPI frames:
*   write_SPI_address_frame(LCD_SH1106_WRITE);
*   write_SPI_data_frame(0x00);
*   write_SPI_data_frame(ui8_command);
* @param[in] ui8_command the command to send to the SH1106 compatible graphic display
* @return 
*********************************************************************************************/
int16_t LCD_SH1106_write_command( uint8_t ui8_command);

/*********************************************************************************************
* @brief Sends an LCD DATA message to the SH1106 compatible graphic display. The LCD DATA message
* is sent by sending the following SPI frames:
*   write_SPI_address_frame(LCD_SH1106_WRITE);
*   write_SPI_data_frame(0x40);
*   write_SPI_data_frame(ui8_data);
* @param[in] ui8_data the data to send to the SH1106 compatible graphic display
* @return
*********************************************************************************************/
int16_t LCD_SH1106_write_data(uint8_t ui8_data);

/*********************************************************************************************
* @brief fills the SH1106 display ( not the buffer ) with the received ui8_page_pattern.
* @param[in] ui8_page_pattern the bit pattern used to fill the SH1106 compatible display.
*********************************************************************************************/
void LCD_SH1106_clear(uint8_t ui8_page_pattern);

/*********************************************************************************************
* @brief fills the internal buffer ( not the SH1106 display  ) with the received ui8_page_pattern.
* To see the result in the display screen the buffer must be refreshed.
* @param[in] ui8_page_pattern the bit pattern used to fill the internal buffer
*********************************************************************************************/
void LCD_SH1106_buffer_fill(uint8_t ui8_page_pattern);

/*********************************************************************************************
* @brief repaints the content of the internal graphic buffer in the SH1106 display.
*********************************************************************************************/
void LCD_SH1106_buffer_refresh();

/*********************************************************************************************
* @brief sets the pixel at the received x,y coordinates in the internal buffer at the received
* color ( 0 or 1 ). To see the result in the display screen the buffer must be refreshed.
* @param[in] ui8_x x coordinate of the pixel to paint
* @param[in] ui8_y y coordinate of the pixel to paint
* @param[in] ui8_bw desired color for the pixel ( 0 or 1 ).
*********************************************************************************************/
void LCD_SH1106_buffer_set_pixel(uint8_t ui8_x, uint8_t ui8_y, uint8_t ui8_bw);

/*********************************************************************************************
* @brief fills the rectangular area defined by the x1,y1 and x2,y2 in the internal buffer used
* to update  the SH1106 display with the received bit pattern. To see the result in the display 
* screen the buffer must be refreshed.
* @param[in] ui16_pattern pattern applied into the display buffer.
* @param[in] ui16_x1 X coordinate of the top left corner of the rectangular area to fill
* @param[in] ui16_y1 Y coordinate of the top left corner of the rectangular area to fill
* @param[in] ui16_x2 X coordinate of the bottom right corner of the rectangular area to fill
* @param[in] ui16_y2 Y coordinate of the bottom right corner of the rectangular area to fill
*********************************************************************************************/
void LCD_SH1106_buffer_area_fast_fill(uint16_t ui16_pattern, uint16_t ui16_x1, uint16_t ui16_y1, uint16_t ui16_x2, uint16_t ui16_y2);

#endif /* LCD_SH1106_H_ */

#endif // #ifdef LCD_IS_SH1106
