// *****************************************************************
// ****                       drivePACK                         ****
// ****                  www.tolaemon.com/dpack                 ****
// ****                       Source code                       ****
// ****                       10/07/2020                        ****
// ****                     Jordi Bartolome                     ****
// ****                                                         ****
// **** IMPORTANT:                                              ****
// **** Using this code or any part of it means accepting all   ****
// **** conditions exposed in: http://www.tolaemon.com/dpack    ****
// **** This code is part of the bareSAM platform. All info at: ****
// **** http://www.tolaemon.com/baresam                         ****
// *****************************************************************
#ifndef _GPIO_H_
#define _GPIO_H_

#include "sam.h"

// ATSAMD51    -   drivePACK function
// PORTA - 0:
// PA00        -     
// PA01        -   
// PA02        -   
// PA03        -   
// PA04        -   
// PA05        -   
// PA06        -   
// PA07        -  ENC_A 
// PA08        -  ENC_B
// PA09        -  ENC_PUSHB
// PA10        -  PUSHB_A
// PA11        -  PUSHB_B 
// PA12        -  PUSHB_C
// PA13        -  PUSHB_D
// PA14        -   
// PA15        -   
// PA16        -   
// PA17        -   
// PA18        -   DIR_DATA
// PA19        -   DIR_CTR 
// PA20        -   
// PA21        -   
// PA22        -   SERCOM PAD0 -*Tx>(1)- USART TX PIN
// PA23        -   SERCOM PAD1 -*Rx<(0)- USART RX PIN
// PA24        -   
// PA25        -   
// PA26        -   
// PA27        -   
// PA28        -   
// PA29        -   
// PA30        -   
// PA31        -   
// PORTB - 1:
// PB00        -   
// PB01        -   
// PB02        -   
// PB03        -   
// PB04        -   
// PB05        -   
// PB06        -   
// PB07        -   
// PB08        -   
// PB09        -   
// PB10        -   
// PB11        -   
// PB12        -   
// PB13        -   
// PB14        -   
// PB15        -   
// PB16        -   
// PB17        -   
// PB18        -   
// PB19        -   
// PB20        -   
// PB21        -   
// PB22        -   
// PB23        -   
// PB24        -   
// PB25        -   
// PB26        -   
// PB27        -   
// PB28        -   
// PB29        -   
// PB30        -   
// PB31        -   

// MASKS
// EIC: External Interrupt and NMI Controller flags
#define EIC_INTFLAG_EIC0_MASK    0x00000001   // Mask for the INTFLAG for the Event Interrupt Controller 0
#define EIC_INTFLAG_EIC1_MASK    0x00000002   // Mask for the INTFLAG for for the Event Interrupt Controller 1
#define EIC_INTENSET_EIC0_MASK   0x00000001   // Mask for the INTFLAG for the Event Interrupt Controller 0
#define EIC_INTENSET_EIC1_MASK   0x00000002   // Mask for the INTFLAG for for the Event Interrupt Controller 1
#define EIC_INTENCLR_EIC0_MASK   0x00000001   // Mask for the INTFLAG for the Event Interrupt Controller 0
#define EIC_INTENCLR_EIC1_MASK   0x00000002   // Mask for the INTFLAG for for the Event Interrupt Controller 1


// PORT 0 MASKS
// masks to control direction in SN74LVC4245 5v - 3.3V bus drivers 
#define P0_DIR_CTRL_DATA_MASK   0x000C0000 // PORTA-0: PA19 and PA18 - (D11) and (D10)  - DIR_CTRL and DIR_DATA
#define P0_DIR_CTRL_BIT_MASK    0x00080000 // PORTA-0: PA19 - (D11)  - DIR_CTR bit in PORTA
#define P0_DIR_DATA_BIT_MASK    0x00040000 // PORTA-0: PA18 - (D10)  - DIR_DATA bit in PORTA
#define P0_TEST_PIN_MASK        0x00200000 // PORTA-0: PA21-(A8)-TEST PIN
// masks to control SERCOM 0
#define P0_USART_TX_MASK        0x00400000 // PORTA-0: PA22(SERCOM PAD0) -*Tx>(1)- USART TX PIN
#define P0_USART_RX_MASK        0x00800000 // PORTA-0: PA23(SERCOM PAD1) -*Rx<(0)- USART RX PIN
#define P0_SPI_SD_MOSI_MASK     0x00000010 // PORTA-0: PA04(SERCOM0 PAD0) - (A3) - MOSI - SD PIN2 DATA_IN
#define P0_SPI_SD_SCK_MASK      0x00000020 // PORTA-0: PA05(SERCOM0 PAD1) - (A1) - SCK  - SD PIN5 SCK_IN
#define P0_SPI_SD_MISO_MASK     0x00000040 // PORTA-0: PA06(SERCOM0 PAD2) - (A2) - MISO - SD PIN7 DATA_OUT
#define P0_SPI_SD_nCS_MASK      0x00100000 // PORTA-0: PA20 - (D9) - SD PIN1 !CS
// masks to ROM connector 
#define P0_UNKNWN_CTRL_BIT_MASK 0x00000004 // PORTA-02: PA02 '?' bit in PORTA
#define P0_ROM_CON_GPIO01       0x00004000 // PORTA-14: PA14 - ROM connector GPIO1
#define P0_ROM_CON_GPIO02       0x00008000 // PORTA-15: PA15 - ROM connector GPIO2
#define P0_ROM_CON_GPIO03       0x00010000 // PORTA-16: PA16 - ROM connector GPIO3
// masks to control the user interface push buttons and encoder
#define P0_USER_IFACE_ENC1_A      0x00000080 // PORTA-07: PA07  -  ENC_A
#define P0_USER_IFACE_ENC1_B      0x00000100 // PORTA-08: PA08  -  ENC_B
#define P0_USER_IFACE_PUSHB_ENC1  0x00000200 // PORTA-09: PA09  -  ENC_PUSHB
#define P0_USER_IFACE_PUSHB_1     0x00002000 // PORTA-13: PA13  -  PUSHB_D in schematic
#define P0_USER_IFACE_PUSHB_2     0x00000400 // PORTA-10: PA10  -  PUSHB_A in schematic
#define P0_USER_IFACE_PUSHB_3     0x00001000 // PORTA-12: PA12  -  PUSHB_C in schematic
#define P0_USER_IFACE_PUSHB_4     0x00000800 // PORTA-11: PA11  -  PUSHB_B in schematic
#define P0_USER_IFACE_PUSHB_ANY   (P0_USER_IFACE_PUSHB_ENC1|P0_USER_IFACE_PUSHB_1|P0_USER_IFACE_PUSHB_2|P0_USER_IFACE_PUSHB_3|P0_USER_IFACE_PUSHB_4) // Mask to detect any of the push buttons mapped in P0

// NOT USED MASKS LCD ...
#define P0_LCD_MASK_DB00        0x00000000 // PORTA-0: - NOT USED - DB00
#define P0_LCD_MASK_DB01        0x00000000 // PORTA-0: - NOT USED - DB01
#define P0_LCD_MASK_DB02        0x00000000 // PORTA-0: - NOT USED - DB02
#define P0_LCD_MASK_DB03        0x00000000 // PORTA-0: - NOT USED - DB03
#define P0_LCD_MASK_DB04        0x00000000 // PORTA-0: - NOT USED - DB04
#define P0_LCD_MASK_DB05        0x00000000 // PORTA-0: - NOT USED - DB05
#define P0_LCD_MASK_DB06        0x00000000 // PORTA-0: - NOT USED - DB06
#define P0_LCD_MASK_DB07        0x00000000 // PORTA-0: - NOT USED - DB07
#define P0_LCD_MASK_DB08        0x00000000 // PORTA-0: - NOT USED - DB08
#define P0_LCD_MASK_DB09        0x00000000 // PORTA-0: - NOT USED - DB09


// PORT 1 MASKS
// masks to ROM connector: single pins
#define P1_nCS_BIT_MASK         0x00000400 // PORTB-1: PB10 - ROM connector !CS   * JBR 2022-01-08 CAMBIADO, revisar en ASM
#define P1_RnW_BIT_MASK         0x00000800 // PORTB-1: PB11 - ROM connector R\!W  * JBR 2022-01-08 CAMBIADO, revisar en ASM
#define P1_nDATA0_BIT_MASK      0x00001000 // PORTB-1: PB12 - ROM connector !D0
#define P1_nDATA1_BIT_MASK      0x00002000 // PORTB-1: PB13 - ROM connector !D1
#define P1_nDATA2_BIT_MASK      0x00004000 // PORTB-1: PB14 - ROM connector !D2
#define P1_nDATA3_BIT_MASK      0x00008000 // PORTB-1: PB15 - ROM connector !D3
#define P1_nCLCK1_BIT_MASK      0x00010000 // PORTB-1: PB16 - ROM connector !CLCK1
#define P1_nCLCK2_BIT_MASK      0x00020000 // PORTB-1: PB17 - ROM connector !CLCK2
// masks to ROM connector: combination of pins
#define P1_CTRL_AND_DATA_MASK   0x0003FC00 // PORTB-1: PB17 'CLCK2' , PB16 'CLCK1' , PB15 '!D3' , PB14 '!D2' , PB13 '!D1' , PB12 '!D0' , PB11 '!W/R' , PB10 '!CS'
#define P1_DATA_MASK            0x0000F000 // PORTB-1: PB15 '!D3' , PB14 '!D2' , PB13 '!D1' , PB12 '!D0'
#define P1_CTRL_MASK            0x00030C00 // PORTB-1: PB17 'CLCK2' , PB16 'CLCK1' , PB11 '!W/R' , PB10 '!CS'
// masks for the LCD 		    
#define P1_LCD_MASK_LCD_RS      0x00000040 // PORTB-1: PB06 - LCD RS
#define P1_LCD_MASK_LCD_CS      0x00000020 // PORTB-1: PB05 - LCD CS
#define P1_LCD_MASK_LCD_WR      0x00000000 //  NOT USED in ST7735
#define P1_LCD_MASK_LCD_RST     0x00000080 // PORTB-1: PB07 - RESET
#define P1_SPI_LCD_MOSI_MASK    0x00000010 // PORTB-1: PB08(SERCOM4 PAD0) - MOSI - LCD PIN5 SDA
#define P1_SPI_LCD_SCK_MASK     0x00000020 // PORTB-1: PB09(SERCOM4 PAD1) - SCK  - LCD PIN6 SCL

// NOT USED MASKS LCD ...
#define P1_LCD_MASK_DB10        0x00000000 // PORTA-0: - NOT USED - DB00
#define P1_LCD_MASK_DB11        0x00000000 // PORTA-0: - NOT USED - DB01
#define P1_LCD_MASK_DB12        0x00000000 // PORTA-0: - NOT USED - DB02
#define P1_LCD_MASK_DB13        0x00000000 // PORTA-0: - NOT USED - DB03
#define P1_LCD_MASK_DB14        0x00000000 // PORTA-0: - NOT USED - DB04
#define P1_LCD_MASK_DB15        0x00000000 // PORTA-0: - NOT USED - DB05


// CTRL bits VARIABLE MASKS: 7:x 6:x 5:x 4:'?' 3:CLCK2 2:CLCK1 1:R/!W 0:!CS
// this is the position at which different control bits are stored in the CTRL bits VARIABLE
#define VAR_CTRL_nCS_MASK       0x01
#define VAR_CTRL_RnW_MASK       0x02
#define VAR_CTRL_nCLCK1_MASK    0x04
#define VAR_CTRL_nCLCK2_MASK    0x08
#define VAR_CTRL_nUNKNOWN_MASK  0x10
#define VAR_CTRL_GPIO01         0x20
#define VAR_CTRL_GPIO02         0x40
#define VAR_CTRL_GPIO03         0x80

// TCx COMMAND CODES
#define CTRLB_CMD_NONE      0x0 // 0x0 NONE No action
#define CTRLB_CMD_RETRIGGER 0x1 // 0x1 RETRIGGER Force a start, restart or retrigger
#define CTRLB_CMD_STOP      0x2 // 0x2 STOP Force a stop
#define CTRLB_CMD_UPDATE    0x3 // 0x3 UPDATE Force update of double buffered registers
#define CTRLB_CMD_READSYNC  0x4 // 0x4 READSYNC Force a read synchronization of COUNT

/*********************************************************************************************
* @brief configures the uC peripherals ( GPIO, UARTs, TIMERs ) to match the application 
* requirements.
*********************************************************************************************/
void GPIO_Init();

/*********************************************************************************************
* @brief Configures the different CLCK peripherals to match the desired operational internal 
* clock frequency.
*********************************************************************************************/
void GPIO_InitClock();

/*********************************************************************************************
* @brief Reconfigures the different CLCK peripherals to set the CPU at 32.768kHz
*********************************************************************************************/
void GPIO_DeinitClock();

/*********************************************************************************************
* @brief Disables all interrupts in the application (timer, encoder IO ) and enables only the 
* interrupts needed to emulate the ROM PACK.
*********************************************************************************************/
void GPIO_KeepOnlyDrivePackInterrupts();

/*********************************************************************************************
* @brief Enables all the interrupts of the application (timer, encoder IO ), and disables the 
* interrupts needed to emulate the ROM PACK.
*********************************************************************************************/
void GPIO_RestoreAllSystemInterrupts();

/*********************************************************************************************
* @brief sets the data/addresses GPIOs and the INPUT/OUPTUT transceiver in output mode and sets
* the received value in the address/data lines.
* param[in] ui8_data_value the value to write into the output GPIO port
*********************************************************************************************/
void GPIO_SetDataLinesOutput(uint8_t ui8_data_value);

#endif // _GPIO_H_