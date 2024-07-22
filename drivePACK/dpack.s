// *****************************************************************
// ****                     www.tolaemon.com                    ****
// ****                       Source code                       ****
// ****                        28/03/2020                       ****
// ****                     Jordi Bartolome                     ****
// ****                                                         ****
// **** IMPORTANT:                                              ****
// **** Using this code or any part of it means accepting all   ****
// **** conditions exposed in: http://www.tolaemon.com/dpack    ****
// *****************************************************************

  // #include "dpack_ctrl.h" // to include the constants of the .h file in the assembly source code, the name of the file must end wiht uppercase '.S', not in lowercase '.s'. If the file ends in lowercase '.s' then the .h is not included.
 
 .syntax unified // This turns on Unified Assembly Language which is required to get all the features of Thumb-2. I.e. instruction and  r1,r0,#0x0F wont work without this directive

// REGISTERS
.equ REG_EIC_BASE_ADDRESS,     0x40002800   // \brief (EIC) APB Base Address
.equ REG_EIC_INTFLAG,          0x40002814   // \brief (EIC_INTFLAG offset) Interrupt Flag Status and Clear = REG_EIC_BASE_ADDRESS + EIC_INTFLAG_OFFSET = 0x40002800 + 0x14 = 0x40002814
.equ EIC_INTFLAG_EIC0_MASK,    0x00000001   // Mask for the INTFLAG for the Event Interrupt Controller 0
.equ EIC_INTFLAG_EIC1_MASK,    0x00000002   // Mask for the INTFLAG for for the Event Interrupt Controller 1
 
.equ REG_EIC_INTENSET,         0x40002810   // \brief (EIC_INTENSET offset) Interrupt Enable Set = REG_EIC_BASE_ADDRESS + EIC_INTENSET_OFFSET = 0x40002800 + 0x10 = 0x40002810
.equ EIC_INTENSET_EIC0_MASK,   0x00000001   // Mask for the INTENSET for the Event Interrupt Controller 0
.equ EIC_INTENSET_EIC1_MASK,   0x00000002   // Mask for the INTENSET for for the Event Interrupt Controller 1

.equ REG_EIC_INTENCLR,         0x4000280C   // \brief (EIC_INTENCLR offset) Interrupt Enable Clear = REG_EIC_BASE_ADDRESS + EIC_INTENCLR_OFFSET = 0x40002800 + 0x0C = 0x4000280C
.equ EIC_INTENCLR_EIC0_MASK,   0x00000001   // Mask for the INTFLAG for the Event Interrupt Controller 0
.equ EIC_INTENCLR_EIC1_MASK,   0x00000002   // Mask for the INTFLAG for for the Event Interrupt Controller 1

// PORT 0 (A) registers
.equ REG_PORT_DIRCLR0,         0x41008004   // \brief (PORT) Data Direction Clear 0 
.equ REG_PORT_DIRSET0,         0x41008008   // \brief (PORT) Data Direction Set 0
.equ REG_PORT_OUTSET0,         0x41008018   // \brief (PORT) Data Output Value Set 1
.equ REG_PORT_OUTCLR0,         0x41008014   // \brief (PORT) Data Output Value Clear 0
.equ REG_PORT_IN0,             0x41008020   // \brief (PORT) Data Input Value 0
.equ REG_PORT_OUT0,            0x41008010   // \brief (PORT) Data Output Value 0
// PORT 0 (A) PINCFG PIN specific registers         
.equ REG_PORT_0_PINCFG_00,  0x41008040                   // \brief (PORT) Pin Configuration Port 0: Pin 0
.equ REG_PORT_0_PINCFG_02, (REG_PORT_0_PINCFG_00 + (1*2))  // \brief (PORT) Pin Configuration Port 0: Pin 2
.equ REG_PORT_0_PINCFG_05, (REG_PORT_0_PINCFG_00 + (1*5))  // \brief (PORT) Pin Configuration Port 0: Pin 5
.equ REG_PORT_0_PINCFG_06, (REG_PORT_0_PINCFG_00 + (1*6))  // \brief (PORT) Pin Configuration Port 0: Pin 6

// PORT 1 (B) registers
.equ REG_PORT_DIRCLR1,           0x4001080C // \brief (PORT) Data Direction Clear 1 
.equ REG_PORT_DIRSET1,           0x41008088 // \brief (PORT) Data Direction Set 1 
.equ REG_PORT_OUTSET1,           0x41008098 // \brief (PORT) Data Output Value Set 1
.equ REG_PORT_OUTCLR1,           0x41008094 // \brief (PORT) Data Output Value Clear 1
.equ REG_PORT_IN1,               0x410080A0 // \brief (PORT) Data Input Value 1
.equ REG_PORT_OUT1,              0x41008090 // \brief (PORT) Data Output Value 1

// SERCOM3 base address
.equ REG_SERCOM3,                0x41014000 // \brief (SERCOM3) APB Base Address
.equ REG_SERCOM3_USART_INTFLAG,  0x41014018 // \brief (SERCOM3) Interrupt Flag Status and Clear
.equ REG_SERCOM3_USART_DATA,     0x41014028 // \brief (SERCOM3) USART Data

// DRE Data Register Empty, This flag is cleared by writing new data to DATA.This flag is set when DATA is empty and ready to be written

// PORT 0 (A) PINCFG PIN specific registers 
.equ REG_PORT_1_PINCFG_00, 0x410080C0                     // \brief (PORT) Pin Configuration 1
.equ REG_PORT_1_PINCFG_10, (REG_PORT_1_PINCFG_00 + (1*10))  // \brief (PORT) Pin Configuration Port 1: Pin 10
.equ REG_PORT_1_PINCFG_11, (REG_PORT_1_PINCFG_00 + (1*11))  // \brief (PORT) Pin Configuration Port 1: Pin 11
.equ REG_PORT_1_PINCFG_12, (REG_PORT_1_PINCFG_00 + (1*12))  // \brief (PORT) Pin Configuration Port 1: Pin 12
.equ REG_PORT_1_PINCFG_13, (REG_PORT_1_PINCFG_00 + (1*13))  // \brief (PORT) Pin Configuration Port 1: Pin 13
.equ REG_PORT_1_PINCFG_14, (REG_PORT_1_PINCFG_00 + (1*14))  // \brief (PORT) Pin Configuration Port 1: Pin 14
.equ REG_PORT_1_PINCFG_15, (REG_PORT_1_PINCFG_00 + (1*15))  // \brief (PORT) Pin Configuration Port 1: Pin 15
.equ REG_PORT_1_PINCFG_16, (REG_PORT_1_PINCFG_00 + (1*16))  // \brief (PORT) Pin Configuration Port 1: Pin 16
.equ REG_PORT_1_PINCFG_17, (REG_PORT_1_PINCFG_00 + (1*17))  // \brief (PORT) Pin Configuration Port 1: Pin 17
// CACHE MONITOR STATUS REGISTER
.equ REG_CMCC_MSR,         0x41006034 // \brief (CMCC) Cache Monitor Status Register

// MASKS
// PORT 0 MASKS
.equ P0_DIR_CTRL_DATA_MASK,   0x000C0000 // PORTA-0: PA19-(D11)-DIR_CTRL and PA18-(D10)-DIR_DATA bits in PORTA
.equ P0_DIR_CTRL_BIT_MASK,    0x00080000 // PORTA-0: PA19-(D11)-DIR_CTR bit in PORTA
.equ P0_DIR_DATA_BIT_MASK,    0x00040000 // PORTA-0: PA18-(D10)-DIR_DATA bit in PORTA
.equ P0_TEST_PIN_MASK,        0x00200000 // PORTA-0: PA21-(A8)-TEST PIN
.equ P0_UNKNWN_CTRL_BIT_MASK, 0x00000004 // PORTA-0: PA02 '?' bit in PORTA
.equ P0_ROM_CON_GPIO01_MASK,  0x00004000 // PORTA-0: PA14 - ROM connector GPIO1
.equ P0_ROM_CON_GPIO02_MASK,  0x00008000 // PORTA-0: PA15 - ROM connector GPIO2
.equ P0_ROM_CON_GPIO03_MASK,  0x00010000 // PORTA-0: PA16 - ROM connector GPIO3
.equ P0_USER_IFACE_PUSHB_ANY, 0x00003E00 // PORTA-0: (P0_USER_IFACE_PUSHB_ENC1|P0_USER_IFACE_PUSHB_1|P0_USER_IFACE_PUSHB_2|P0_USER_IFACE_PUSHB_3|P0_USER_IFACE_PUSHB_4): Mask to detect any of the push buttons mapped in P0

// PORT 1 MASKS
.equ P1_nCS_BIT_MASK,         0x00000400 // PORTB-1: PB10 - ROM connector !CS
.equ P1_RnW_BIT_MASK,         0x00000800 // PORTB-1: PB11 - ROM connector R\!W
.equ P1_nDATA0_BIT_MASK,      0x00001000 // PORTB-1: PB12 - ROM connector !D0
.equ P1_nDATA1_BIT_MASK,      0x00002000 // PORTB-1: PB13 - ROM connector !D1
.equ P1_nDATA2_BIT_MASK,      0x00004000 // PORTB-1: PB14 - ROM connector !D2
.equ P1_nDATA3_BIT_MASK,      0x00008000 // PORTB-1: PB15 - ROM connector !D3
.equ P1_nCLCK1_BIT_MASK,      0x00010000 // PORTB-1: PB16 - ROM connector !CLCK1
.equ P1_nCLCK2_BIT_MASK,      0x00020000 // PORTB-1: PB17 - ROM connector !CLCK2

.equ P1_CTRL_AND_DATA_MASK,   0x0003FC00 // PORTB-1: PB17 'CLCK2' , PB16 'CLCK1' , PB15 '!D3' , PB14 '!D2' , PB13 '!D1' , PB12 '!D0' , PB11 '!W/R' , PB10 '!CS'
.equ P1_DATA_MASK,            0x0000F000 // PORTB-1: PB15 '!D3' , PB14 '!D2' , PB13 '!D1' , PB12 '!D0'
.equ P1_CTRL_MASK,            0x00030C00 // PORTB-1: PB17 'CLCK2' , PB16 'CLCK1' , PB11 '!W/R' , PB10 '!CS'

// CTRL bits VARIABLE MASKS: 7:x 6:x 5:x 4:'?' 3:CLCK2 2:CLCK1 1:R/!W 0:!CS
.equ VAR_CTRL_nCS_MASK,                 0x01
.equ VAR_CTRL_RnW_MASK,                 0x02
.equ VAR_CTRL_nCLCK1_MASK,              0x04
.equ VAR_CTRL_nCLCK2_MASK,              0x08
.equ VAR_CTRL_nUNKNOWN_MASK,            0x10

// SERCOM bits MASKS:
.equ VAR_SERCOM_INTFLAG_DRE_MASK,    0x01

// clck12_state transitions 
.equ CLCK12_STATE_TRANSITION_NONE,       0
.equ CLCK12_STATE_TRANSITION_0_TO_1,     1
.equ CLCK12_STATE_TRANSITION_1_TO_2,     2
.equ CLCK12_STATE_TRANSITION_2_TO_3,     3
.equ CLCK12_STATE_TRANSITION_3_TO_4,     4
.equ CLCK12_STATE_TRANSITION_4_TO_5,     5
.equ CLCK12_STATE_TRANSITION_5_TO_6,     6
.equ CLCK12_STATE_TRANSITION_6_TO_7,     7
.equ CLCK12_STATE_TRANSITION_7_TO_0,     8

// numbber of loop cycles to wait on each clck12 state
.equ CLCK12_WAIT_CYCLES_STATE0,          2
.equ CLCK12_WAIT_CYCLES_STATE1,          10
.equ CLCK12_WAIT_CYCLES_STATE2,          2
.equ CLCK12_WAIT_CYCLES_STATE3,          2
.equ CLCK12_WAIT_CYCLES_STATE4,          2
.equ CLCK12_WAIT_CYCLES_STATE5,          2
.equ CLCK12_WAIT_CYCLES_STATE6,          2
.equ CLCK12_WAIT_CYCLES_STATE7,          2

// VARIABLE bus states:
.equ VAR_BUS_STATE_INVALID,                      0x00 // 0 not valid
.equ VAR_BUS_STATE_HIGH_IMPEDANCE,               0x01 // 1 high impedance                     - ctrl bits:INPUT  data bits:INPUT 
.equ VAR_BUS_STATE_READ_ADDR_WRITEN_BY_KEYB,     0x02 // 2 read address requested by keyboard - ctrl bits:INPUT  data bits:INPUT  - ROM EMULATION MODE
.equ VAR_BUS_STATE_WRITE_DATA_REQ_BY_KEYB,       0x03 // 3 write data requested by keyboard   - ctrl bits:INPUT  data bits:OUTPUT - ROM EMULATION MODE 
.equ VAR_BUS_STATE_WRITE_ADDR_REQ_TO_ROMPACK,    0x04 // 4 write address requested to ROMPACK - ctrl bits:OUTPUT data bits:OUTPUT - KEYBOARD EMULATION MODE 
.equ VAR_BUS_STATE_READ_DATA_REQ_TO_ROMPACK,     0x05 // 5 read data requested to ROMPACK     - ctrl bits:OUTPUT data bits:INPUT  - KEYBOARD EMULATION MODE 

// Other VARIABLES
.equ VAR_NUM_NIBBLES_ADDR,                       0x05
.equ VAR_ROMPACK_DEVICE_CODE,                    0x03

// DPACK DUMPER state machine states:
.equ DPACK_DUMPER_STATE_IDLE_CFG,                 0
.equ DPACK_DUMPER_STATE_IDLE_WAIT,                1
.equ DPACK_DUMPER_STATE_IDLE_CNT_CLCK,            2
.equ DPACK_DUMPER_STATE_WRITE_ADD_CFG,            3
.equ DPACK_DUMPER_STATE_WRITE_ADD_WAIT,           4
.equ DPACK_DUMPER_STATE_WRITE_ADD,                5
.equ DPACK_DUMPER_STATE_READ_DATA_CFG,            6
.equ DPACK_DUMPER_STATE_READ_DATA_WAIT,           7
.equ DPACK_DUMPER_STATE_READ_DATA,                8
.equ DPACK_DUMPER_STATE_READ_DATA_STORE,          9
.equ DPACK_DUMPER_STATE_RESET_CURR_ADDRESS,      10

.equ DPACK_DUMPER_CS_WAIT_CLOCK_CYCLES,           5
.equ DPACK_DUMPER_READ_DATA_NIBBLES_MAX,          0x10 // as each RAM byte contains 2 nibbles use always an odd value for this constant ( that is because the 0x00 also counts, so it will force reading an EVEN number of nibbles )
.equ DPACK_DUMPER_WRITE_ADDR_NIBBLES_MAX,         5
.equ DPACK_DUMPER_MAX_ADDRESS,                    0xFFFF // !! REMEMBER TO ALSO UPDATE THE #define DPACK_DUMPER_MAX_ADDRESS with the same value in dpack_ctrl.h

// PINCFG registers
//       PMUXEN:1 bit:      0  Peripheral Multiplexer Enable     
//         INEN:1 bit:      1  Input Enable                      
//       PULLEN:1 bit:      2  Pull Enable                       
//     reserved:3 bit:  3.. 5  Reserved                          
//       DRVSTR:1 bit:      6  Output Driver Strength Selection  
//     reserved:1 bit:      7  Reserved                          
.equ PINCFG_INEN_BIT_MASK,           0x0002 // enable sampling of pin value, must be set to 1 if pin is set as input
.equ PINCFG_PULLEN_BIT_MASK,         0x0004


// ATSAMD51 - Metro M4 -  CasioROM 
//  PB15        (6)         PIN1  - !Data3    
//  PB14        (5)         PIN2  - !Data2
//  PB13        (4)         PIN3  - !Data1
//  PB12        (7)         PIN4  - !Data0
//  PB11        (A5)        PIN5  - R/!W
//  PB16        (3)         PIN6  - CLCK1
//  PB17        (2)         PIN7  - CLCK2
//  PA02        (A0)        PIN8  - ?      < UNKNOWN < NOT USED
//  ----        (GND)       PIN9  - Gnd
//  PB10        (A4)        PIN10 - !CS
//  ----        (5V)        PIN11 - Vcc
//  SN74LVC4245A signals:  0 = OUTPUT (B 3.3V to A 5.0V ) , 1 = INPUT (A 5.0V to B 3.3V)
//  PA19        (D11)        DIR_CTRL
//  PA18        (D10)        DIR_DATA
//  USART3 (SERCOM3) signals:
//  PA22(SERCOM PAD0)    *Tx>(1)      FTDI PIN 5-RXD
//  PA23(SERCOM PAD1)    *Rx<(0)      FTDI PIN 1-TXD


//###################################################################################################### 
// EIC_0_Handler ( conected to CLCK1 line )
//   Interrupt handler for the Event Interrupt Controller 0 used to detect changes on the GPIO line PB16
//   connected to the ROM Pack CLCK1 line. The interrupt also sets the IEC_FLAG_nW interrupt flag indicating
//   that the clock changed its state when the R/!W was '0'
//######################################################################################################
// PRE: before calling this function these conditions must be fulfilled:
//     interrupts for EIC controller 0 on PB16 must have been configured
//     interrupts for EIC controller 0 on PB16 must have been previously enabled
//.text
//.global EIC_0_Handler
//
//.thumb_func
//EIC_0_Handler:	 
//   push {r2-r6} // push the value of the registers used in the subroutine
// 
//   .thumb_func
//   EIC_0_Handler_cancel_int:
// 
//     // the interrupt flag bit EIC0 is cleared by writing a '1' to it.
//     movw r3,#:lower16:REG_EIC_INTFLAG 
//     movt r3,#:upper16:REG_EIC_INTFLAG
//     ldr  r4,[r3]	  	  
//     movw r5,#:lower16:EIC_INTFLAG_EIC0_MASK 
//     movt r5,#:upper16:EIC_INTFLAG_EIC0_MASK
//     orr  r4,r5
//        str  r4,[r3]
// 
//  .thumb_func
//  EIC_0_Handler_end:
// 
//     pop {r2-r6} // pop the value of the registers used in the subroutine
// 
//   bx lr



//###################################################################################################### 
// EIC_1_Handler ( connected to CLCK2 line )
//   Interrupt handler for the Event Interrupt Controller 1 used to detect changes on the GPIO line PB17
//   which is connected to the ROM Pack CLCK2 line. The interrupt also sets the IEC_FLAG_R interrupt flag 
//   indicating that the clock changed its state when the R/!W was '1'
//######################################################################################################
// PRE: before calling this function these conditions must be fulfilled:
//     interrupts for EIC controller 1 on PB17 must have been configured
//     interrupts for EIC controller 1 on PB17 must have been previously enabled
.text
.global EIC_1_Handler

.thumb_func
EIC_1_Handler:	 
    push {r2-r6} // push the value of the registers used in the subroutine

    mov  r0,#0                                   // clear all the content of r0 because it will be used to store the values of the ctrl lines

  	// get R/!W and !CS lines bits state and store them into r2
	movw r3,#:lower16:(P1_RnW_BIT_MASK|P1_nCS_BIT_MASK) // set r3 to current ( P1_RnW_BIT_MASK | P1_nCS_BIT_MASK ) bits mask value
	movt r3,#:upper16:(P1_RnW_BIT_MASK|P1_nCS_BIT_MASK)
	movw r4,#:lower16:REG_PORT_IN1            // set r4 to REG_PORT_IN1 register address
	movt r4,#:upper16:REG_PORT_IN1 
	ldr  r5,[r4]                              // load in r5 the content of REG_PORT_IN1 and apply ( P1_RnW_BIT_MASK | P1_nCS_BIT_MASK ) on it
	and  r3,r5                                // keep the value of the ( P1_RnW_BIT_MASK | P1_nCS_BIT_MASK ) bit in r3
	lsr  r5,r3,#10                            // move the read R/!W and !CS bit values ( P1_RnW_BIT_MASK | P1_nCS_BIT_MASK ) to the position it will have in the ui8_ctrl_bits variable
	orr  r0,r5                                // once in their position, store the bits in r0
	
	// JBR 2023-07-10 Commented as an optimization because CLCK1 and CLCK2 are no longer needed in the ctrl_bits ( CLCK changes are detected by the ingrrupt ).
	// get CLCK1 and CLCK2 lines  bits state and store them into r2
	// movw r3,#:lower16:(P1_nCLCK1_BIT_MASK|P1_nCLCK2_BIT_MASK) // set r3 to current ( P1_nCLCK1_BIT_MASK | P1_nCLCK2_BIT_MASK ) bits mask value
	// movt r3,#:upper16:(P1_nCLCK1_BIT_MASK|P1_nCLCK2_BIT_MASK)
	// movw r5,#:lower16:REG_PORT_IN1         // set r5 to REG_PORT_IN1 register address
	// movt r5,#:upper16:REG_PORT_IN1 
	// ldr  r5,[r4]                           // set in r5 currrent REG_PORT_IN1 register value
	// and  r3,r5                             // store the value of the ( P1_nCLCK1_BIT_MASK | P1_nCLCK2_BIT_MASK ) bit in r3
	// lsr  r5,r3,#14                         // move the ( P1_nCLCK1_BIT_MASK | P1_nCLCK2_BIT_MASK ) bit to the position it will have in the ui8_ctrl_bits variable
	// orr  r0,r5                             // once on its position, update the bits in the r2 register used to store into the ctrl bits 
		                                      // at this point r2 contains the new state of the ctrl bits ( UNKNWN_CTRL, !CS, R!W, CLCK1, CLCK2 ... )  

	// check the state of the !CS, the R/!W signals and the value of the last read address device code nibble. If the !CS is 1 then go to HIGH_IMPEDANCE_STATE, 
	// if the !CS is 0 and the R/!W signal is '0', then the keyboard is writting the address nibbles in the address/data bus so jump to the READ_ADDR_WRITEN_BY_KEYB
	// state if R/!W is '1' and the last read address device code nibble is 0x3 then keyboard is reading the the data from the ROM PACK so jump to WRITE_DATA_REQ_BY_KEYB, 
	// but if it is not 0x3 then it is readding that data from other device so keep in HIGH IMPEDANCE STATE state
	and r3,r0,#VAR_CTRL_nCS_MASK               
	cmp r3,#0	
	bne EIC_1_Handler_nCS_1                   // if nCS==1 jump to EIC_1_Handler_nCS_1 ( HIGH_IMPEDANCE_STATE )             
	and r3,r0,#VAR_CTRL_RnW_MASK
	cmp r3,#0
	beq EIC_1_Handler_RnW_0                   // if RnW==0 jump to EIC_1_Handler_RnW_0 ( READ_ADDR_WRITEN_BY_KEYB )
	movw r2,#:lower16:ui8_read_device_code
	movt r2,#:upper16:ui8_read_device_code
	ldrb r3,[r2]
	cmp r3,#3                                 
	bne EIC_1_Handler_nCS_1                   // if RnW==1 and ui8_read_device_code!=3 jump to EIC_1_Handler_nCS_1 ( HIGH_IMPEDANCE_STATE )  
	b EIC_1_Handler_RnW_1                     // if RnW==1 and ui8_read_device_code==3 jump to EIC_1_Handler_RnW_1 ( WRITE_DATA_REQ_BY_KEYB )

		// ##################### RnW = 0 READ ADDRESS WRITTEN BY KEYBOARD ##########################################
		.thumb_func
        EIC_1_Handler_RnW_0:

			movw r2,#:lower16:ui8_bus_state                              // check if bus is already configured in HIGH IMPEDANCE mode or not and if bus is already configured in HIGH IMPEDANCE then go to end
			movt r2,#:upper16:ui8_bus_state
			ldrb r3,[r2]
			cmp  r3,#VAR_BUS_STATE_READ_ADDR_WRITEN_BY_KEYB                 
		    beq  EIC_1_Handler_RnW_0_check_cycle

			.thumb_func
			EIC_1_Handler_RnW_0_init:
				movw r3,#:lower16:VAR_BUS_STATE_READ_ADDR_WRITEN_BY_KEYB // - update the bus state variable indicating that now is configured in READ ADDRESS WRITTEN BY KEYBOARD
				movt r3,#:upper16:VAR_BUS_STATE_READ_ADDR_WRITEN_BY_KEYB		  
				strb r3,[r2]

				// JBR 2023-07-10 Optimization: as we always arrive/enter into READ ADDRESS WRITTEN BY KEYBOARD  state from the BUS_STATE_HIGH_IMPEDANCE and in that state the 
				// DATA lines are configured as input there is no need to configure them as input again
				//// Configure DATA lines as INPUT:
				//// ########## PORT B
                //// configure INPUT DIRECTION of uC I/O pins used for 'DATA' signals: DIR bit = 1 OUTPUT , DIR bit = 0 INPUT
				//movw r2,#:lower16:P1_DATA_MASK             
				//movt r2,#:upper16:P1_DATA_MASK
				//movw r3,#:lower16:REG_PORT_DIRCLR1         
				//movt r3,#:upper16:REG_PORT_DIRCLR1
				//str  r2,[r3]                               
				//
				//// Enable INEN bits 
				//// PB12 - PIN1  - !Data0
				//movw r2,#:lower16:REG_PORT_1_PINCFG_12     
				//movt r2,#:upper16:REG_PORT_1_PINCFG_12
				//movw r3,#:lower16:PINCFG_INEN_BIT_MASK     
				//movt r3,#:upper16:PINCFG_INEN_BIT_MASK
				//ldr  r4,[r2]                               
				//orr  r4,r3                                 
				//str  r4,[r2]   
				//
				//// PB13 - PIN?  - !Data1
				//movw r2,#:lower16:REG_PORT_1_PINCFG_13     
				//movt r2,#:upper16:REG_PORT_1_PINCFG_13
				//movw r3,#:lower16:PINCFG_INEN_BIT_MASK     
				//movt r3,#:upper16:PINCFG_INEN_BIT_MASK
				//ldr  r4,[r2]                               
				//orr  r4,r3
				//str  r4,[r2] 
				//
				//// PB14 - PIN?  - !Data2
				//movw r2,#:lower16:REG_PORT_1_PINCFG_14     
				//movt r2,#:upper16:REG_PORT_1_PINCFG_14
				//movw r3,#:lower16:PINCFG_INEN_BIT_MASK     
				//movt r3,#:upper16:PINCFG_INEN_BIT_MASK
				//ldr  r4,[r2]                               
				//orr  r4,r3                                 
				//str  r4,[r2] 
				//
				//// PB15 - PIN?  - !Data3
				//movw r2,#:lower16:REG_PORT_1_PINCFG_15     
				//movt r2,#:upper16:REG_PORT_1_PINCFG_15
				//movw r3,#:lower16:PINCFG_INEN_BIT_MASK     
				//movt r3,#:upper16:PINCFG_INEN_BIT_MASK
				//ldr  r4,[r2]                               
				//orr  r4,r3                                 
				//str  r4,[r2] 
				//
				//// set to '1' the VALUE of I/O line  used to set '5-3.3V TRANSCEIVERs DIR' direction in order to
				//// configure them in INPUT direction (A 5.0V to B 3.3V)  0 = OUTPUT (B 3.3V to A 5.0V ) , 1 = INPUT (A 5.0V to B 3.3V)
				//movw r2,#:lower16:P0_DIR_DATA_BIT_MASK  
				//movt r2,#:upper16:P0_DIR_DATA_BIT_MASK
				//movw r3,#:lower16:REG_PORT_OUTSET0      
				//movt r3,#:upper16:REG_PORT_OUTSET0
				//str  r2,[r3]
				//// END configure DATA lines as input

				// JBR 2023-07-10 Optimization: as we always arrive to READ ADDRESS WRITTEN BY KEYBOARD  state from the BUS_STATE_HIGH_IMPEDANCE and in that state ui8_clck_cycle is forced to #1 there is no need to execute this initialization here
				// mov  r3,#1 
				// movw r2,#:lower16:ui8_clck_cycle                    // initialize to #1 the ui8_clck_cycle variable used to keep the cycle (even, odd) of the currently used CLCKx signal
				// movt r2,#:upper16:ui8_clck_cycle
				// strb r3,[r2]

				mov  r3,#0
				movw r2,#:lower16:ui8_read_add_nibble_numb          // initialize to #0 the variable used to count the number of address nibbles written by the keyboard and read from the bus
				movt r2,#:upper16:ui8_read_add_nibble_numb	  	   					  
				strb r3,[r2]									     

				movw r2,#:lower16:ui8_read_device_code             // initialize to #0 the variable used to store the read address device code
				movt r2,#:upper16:ui8_read_device_code	  	   					  
				strb r3,[r2]									     
														       
				movw r2,#:lower16:ui32_read_nibble_address          // initialize to 0 the variable used to store the address requested by the keyboard
				movt r2,#:upper16:ui32_read_nibble_address	       
				str  r3,[r2]		

			.thumb_func
			EIC_1_Handler_RnW_0_check_cycle:

				// if the CPU is writting the address nibble in the bus, first thing to do is read it from the bus as soon as possible 
				// to avoid being to late get the address nibble and loosing it. Once it has been read it is temporary stored in 
				// ui8_read_nibble_val to process it later.
				movw r3,#:lower16:REG_PORT_IN1
				movt r3,#:upper16:REG_PORT_IN1
				ldr  r2,[r3]
				and  r2,#P1_DATA_MASK
	            movw r3,#:lower16:ui8_read_nibble_val                 // take the read address nibble stored ui8_read_nibble_val
				movt r3,#:upper16:ui8_read_nibble_val					
				lsr  r2,#12                                           // r2 lowest 4 bits temporary store the read address nibble from the bus
				strb r2,[r3]

				// check if ui8_clck_cycle is #1 or #2
				movw r3,#:lower16:ui8_clck_cycle           
				movt r3,#:upper16:ui8_clck_cycle
				ldrb r2,[r3]
				cmp  r2,#2 
				beq EIC_1_Handler_RnW_0_clck_cycle_2

				.thumb_func
				EIC_1_Handler_RnW_0_clck_cycle_1:

					// if ui8_clck_cycle is #1 update it to #2 for the next interrupt
					// movw r3,#:lower16:ui8_clck_cycle           
				    // movt r3,#:upper16:ui8_clck_cycle
					mov r2,#2
					strb r2,[r3]

					// blink read LED
					movw r2,#:lower16:REG_PORT_OUT0
					movt r2,#:upper16:REG_PORT_OUT0
					ldr  r3,[r2]
  					movw r4,#:lower16:(P0_ROM_CON_GPIO01_MASK)
					movt r4,#:upper16:(P0_ROM_CON_GPIO01_MASK)
					eor  r3,r4                                             // apply an XOR '1' over the P0_ROM_CON_GPIO02_MASK to blink READ LED
					str r3,[r2]																	
					// END blink read LED
					b EIC_1_Handler_cancel_int // cancel and leave the interrupt

				.thumb_func
				 EIC_1_Handler_RnW_0_clck_cycle_2:

					// as ui8_clck_cycle is #2 then update it to #1 for the next interrupt
					movw r3,#:lower16:ui8_clck_cycle                       
					movt r3,#:upper16:ui8_clck_cycle
					mov r2,#1
					strb r2,[r3]

					// before processing the current read nibble check if all the VAR_NUM_NIBBLES_ADDR nibbles have been already read
					movw r3,#:lower16:ui8_read_add_nibble_numb                // calculate the number of bits to shift the read nibble to place it in the right position in the ui32_read_nibble_address according to the current number of read address nibbles till the moment.
					movt r3,#:upper16:ui8_read_add_nibble_numb
					ldrb r4,[r3]
					cmp  r4,#VAR_NUM_NIBBLES_ADDR                             // check if all the address nibbles have been read and continue if not or leave if affirmative
					beq  EIC_1_Handler_cancel_int
                        
	                    movw r3,#:lower16:ui8_read_nibble_val                 // take the read address nibble stored ui8_read_nibble_val
						movt r3,#:upper16:ui8_read_nibble_val					
						ldrb r2,[r3]
																		       
					    // set the read adress nibble bits in the right position inside the whole address					   					
						movw r5,#:lower16:ui32_read_nibble_address            // take the address with the nibbles that have been stored in the previous cycles
						movt r5,#:upper16:ui32_read_nibble_address
						ldr  r6,[r5]
						                                          
						lsl  r4,#2                                            // shift 2 bits to the left the number of nibbles ( mutliply by 4 ) to get the number of bits to shift ( 1 nibble to shift are 4 bits to shift ): niblle 0 shift 0 bits, nibble 1 shift 1x4 bits, nibble 2 shift 2x4=8 bits, nibble 3 shift 3x4=12 bits ...

						movw r5,#:lower16:0x0000000F                          // prepare the mask used to modify only the 4 bits of the whole address that correspond to the current processed nibble position
						movt r5,#:upper16:0x0000000F
						lsl  r5,r4                                            // shift the mask used to modify only the bits that correspond to the current processed address nibble position
						lsl  r2,r4                                            // shift the the read nibble value to place it in the bits corresponding to the current processed address nibble position

						bic  r6,r5                                            // clean the bits of the 'ui32_read_nibble_address' that are going to be updated: bic r6,r5 is an 'and' with r5 complements 0x0000000F > 0xFFFFFFF0 & r6
						orr  r6,r2                                            // place the the read nibble in the bits corresponding to the current processed address nibble position

						movw r4,#:lower16:ui32_read_nibble_address            // store in memory the address nibbbles read till the moment
						movt r4,#:upper16:ui32_read_nibble_address
						str  r6,[r4]

						// increase the number of read nibbles counter
						movw r3,#:lower16:ui8_read_add_nibble_numb		      // take the current number of address nibbles that have been written by the keyboard
						movt r3,#:upper16:ui8_read_add_nibble_numb		    
						ldrb r2,[r3]									    											    
						add  r2,#1									          // if there are still nibbles to read increase the read address nibbles counter 
						strb r2,[r3]								    
						cmp  r2,#VAR_NUM_NIBBLES_ADDR                         // on access to ROM PACKs last nibble of the address requested by the keyboard should be '0xC' and it corresponds to the device ID
						bne EIC_1_Handler_cancel_int
			  							
							// all address nibbles have been received so it is time to process them to get the device code and the complete final ROM address value and					
							movw r3,#:lower16:0x00FF0000                     // get the address device code nibble
							movt r3,#:upper16:0x00FF0000
							and  r3,r6                                       // AND the value of the device code mask and the address nibbles to get the device code nibble
							lsr  r3,#16                                    
							movw r5,#:lower16:ui8_read_device_code           // storte in memory the read device code nibble
					        movt r5,#:upper16:ui8_read_device_code
							strb r3,[r5]

							movw r3,#:lower16:0x0000FFFF                    // get the 4 address nibbles
							movt r3,#:upper16:0x0000FFFF
							and  r6, r3										// take only the 4 lowest nibbles and discard 5th ( 5th nibble corresponds to the  device code)
							eor  r6, r3										// all bits are read from the bus in inverse logic, so they must be reversed to get the right value
							str  r6,[r4]                                    // store in memory the 4 address nibbles once processed

				b EIC_1_Handler_cancel_int // cancel and leave the interrupt

		// ##################### RnW = 1 WRITE DATA REQUESTED BY KEYBOARD STATE ##########################################
		.thumb_func
         EIC_1_Handler_RnW_1:

			movw r2,#:lower16:ui8_bus_state                             // check if bus is already configured in HIGH IMPEDANCE mode or not and if bus is already configured in HIGH IMPEDANCE then go to end
			movt r2,#:upper16:ui8_bus_state
			ldrb r3,[r2]
			cmp  r3,#VAR_BUS_STATE_WRITE_DATA_REQ_BY_KEYB                 
		    beq  EIC_1_Handler_RnW_1_check_cycle

			.thumb_func
			EIC_1_Handler_RnW_1_init:

   			    movw r3,#:lower16:VAR_BUS_STATE_WRITE_DATA_REQ_BY_KEYB  // update the bus state variable indicating that now is configured in READ ADDRESS WRITTEN BY KEYBOARD
				movt r3,#:upper16:VAR_BUS_STATE_WRITE_DATA_REQ_BY_KEYB		  
				strb r3,[r2] 

				// Configure DATA lines as output
				// ########## PORT B
				// configure DIRECTION of I/O pins used for 'DATA'  signals: DIR bit = 1 OUTPUT , DIR bit = 0 INPUT
				movw r2,#:lower16:P1_DATA_MASK       
				movt r2,#:upper16:P1_DATA_MASK
				movw r3,#:lower16:REG_PORT_DIRSET1   
				movt r3,#:upper16:REG_PORT_DIRSET1
				str  r2,[r3]                         

				// set to '1' the VALUE of I/O line used to set data lines ( start with data lines to 0xF )
				movw r2,#:lower16:P1_DATA_MASK   
				movt r2,#:upper16:P1_DATA_MASK
				movw r3,#:lower16:REG_PORT_OUTSET1
				movt r3,#:upper16:REG_PORT_OUTSET1
				str  r2,[r3]     

				// Disable INEN bits 
				// PB12 - PIN?  - !Data0
				movw r2,#:lower16:REG_PORT_1_PINCFG_12     
				movt r2,#:upper16:REG_PORT_1_PINCFG_12
				movw r3,#:lower16:(~PINCFG_INEN_BIT_MASK)  
				movt r3,#:upper16:(~PINCFG_INEN_BIT_MASK)
				ldr  r4,[r2]                               
				and  r4,r3                                 
				str  r4,[r2]   

				// PB13 - PIN?  - !Data1
				movw r2,#:lower16:REG_PORT_1_PINCFG_13     
				movt r2,#:upper16:REG_PORT_1_PINCFG_13
				movw r3,#:lower16:(~PINCFG_INEN_BIT_MASK)  
				movt r3,#:upper16:(~PINCFG_INEN_BIT_MASK)
				ldr  r4,[r2]                               
				and  r4,r3                                 
				str  r4,[r2] 

				// PB14 - PIN?  - !Data2
				movw r2,#:lower16:REG_PORT_1_PINCFG_14     
				movt r2,#:upper16:REG_PORT_1_PINCFG_14
				movw r3,#:lower16:(~PINCFG_INEN_BIT_MASK)  
				movt r3,#:upper16:(~PINCFG_INEN_BIT_MASK)
				ldr  r4,[r2]                               
				and  r4,r3                                 
				str  r4,[r2] 

				// PB15 - PIN?  - !Data3
				movw r2,#:lower16:REG_PORT_1_PINCFG_15     
				movt r2,#:upper16:REG_PORT_1_PINCFG_15
				movw r3,#:lower16:(~PINCFG_INEN_BIT_MASK)  
				movt r3,#:upper16:(~PINCFG_INEN_BIT_MASK)
				ldr  r4,[r2]                               
				and  r4,r3                                 
				str  r4,[r2] 

				// set to '0' the VALUE of I/O line  used to set '5-3.3V TRANSCEIVERs DIR' direction in order to configure
				// them in OUTPUT direction (B 3.3V to A 5.0V)  0 = OUTPUT (B 3.3V to A 5.0V ) , 1 = INPUT (A 5.0V to B 3.3V)
				movw r2,#:lower16:P0_DIR_DATA_BIT_MASK   
				movt r2,#:upper16:P0_DIR_DATA_BIT_MASK
				movw r3,#:lower16:REG_PORT_OUTCLR0    
				movt r3,#:upper16:REG_PORT_OUTCLR0
				str  r2,[r3]                             
				// END configure DATA lines as output

				// JBR 2023-07-10 Optimization: as we always arrive toWRITE DATA REQUESTED BY KEYBOARD state from the BUS_STATE_HIGH_IMPEDANCE and in that state ui8_clck_cycle is forced to #1 there is no need to execute this initialization here
				// mov  r3,#1 
				// movw r2,#:lower16:ui8_clck_cycle                         // initialize to #1 the ui8_clck_cycle variable used to keep the cycle (even, odd) of the currently used CLCKx signal
				// movt r2,#:upper16:ui8_clck_cycle
				// strb r3,[r2]

			.thumb_func
			EIC_1_Handler_RnW_1_check_cycle:

				// check if ui8_clck_cycle is #1 or #2
				movw r3,#:lower16:ui8_clck_cycle           
				movt r3,#:upper16:ui8_clck_cycle
				ldrb r2,[r3]
				cmp  r2,#2 
				bne EIC_1_Handler_RnW_1_clck_cycle_1

				.thumb_func
				EIC_1_Handler_RnW_1_clck_cycle_2:

				   	// if ui8_clck_cycle is #1 update it to #2 for the next interrupt
				   	// movw r3,#:lower16:ui8_clck_cycle           
				   	// movt r3,#:upper16:ui8_clck_cycle
				   	mov r2,#1
				   	strb r2,[r3]
				   	b EIC_1_Handler_cancel_int // cancel and leave the interrupt

				.thumb_func
				 EIC_1_Handler_RnW_1_clck_cycle_1:

					// write the date nibble in the bus
					movw r4,#:lower16:REG_PORT_OUT1              // get the port to write the data nibble in the data bus
					movt r4,#:upper16:REG_PORT_OUT1			    
					ldr  r2,[r4]							    
					movw r5,#:lower16:(~P1_DATA_MASK)            // clear the bits of the port that correspond to the data bits				      
					movt r5,#:upper16:(~P1_DATA_MASK)			  
					and  r2,r5
					movw r5,#:lower16:ui8_write_nibble_val       // get the data nibble to write to the bus
					movt r5,#:upper16:ui8_write_nibble_val	    
					ldrb r6,[r5]							    
					lsl  r6,#12                                  // shift the data nibble value to place the bits in the right PORT0 positions before writting: 0x0000000F > 0x0000F000                 
					orr  r2,r6                                   // set the bits to write				
					str  r2,[r4]                                 // write the value into the port to place it into the data bus

 					// if ui8_clck_cycle is #2 update it to #1 for the next interrupt
					// movw r3,#:lower16:ui8_clck_cycle           
					// movt r3,#:upper16:ui8_clck_cycle
					mov r2,#2
					strb r2,[r3]

					// increase the address of next data nibble to provide to the keyboard ( it is used as an offset inside the ROM dump content )
					movw r3,#:lower16:ui32_read_nibble_address 
					movt r3,#:upper16:ui32_read_nibble_address								
					ldr  r2,[r3]                                 // take current data nibble read address and increase it +1  for the next data nibble write cycle
					add  r2,#1                          	    
					str  r2,[r3]                                 // store new address value in the "ui32_read_nibble_address" variable

					// prepare the next data nibble to write     // TODO: JBR 2023-07-06 si el valor de ui32_read_nibble_address ya lo teniamos arriba, quizas no haga falta ni esa variable y baste con un registro temporal
					lsrs r2,#1                                   // shift 1 bit to the right to divide requested ROMPack address by 2 ( the C flag is updated to the last bit shifted out,
																 // except when the shift length is 0). ROMPack are addressed at nibble level while uC memory is adressed at byte level 
																 // ( each byte in the uC memory contains two nibbles ). To convert the address of the nibble requested by the keyboard
																 // to uC memory address, it is divided by 2. 
	   				bcs EIC_1_Handler_RnW_1_data_nibble_is_odd   // if, before dividing, the nibble address is ODD ( carry=1 so cc=false ) then low 4 bits of the byte are taken, and if the  
																 // address befor dividing is EVEN the high 4 bits ( carry=0 so cc=true ) of the byte are taken.
					.thumb_func 
					EIC_1_Handler_RnW_1_data_nibble_is_even:     // take the nible from an EVEN address
						 movw r3,#:lower16:ui8_nibbles_buffer // to access flash hardcoded ROMPACKS use: #:lower16:casio_rompack              
						 movt r3,#:upper16:ui8_nibbles_buffer // to access flash hardcoded ROMPACKS use: #:upper16:casio_rompack              
						 add  r3,r2                              // add the current ROM Pack address to the base address of the ROM Pack dump in the uC flash to get the address of the nibble in the uC flash
						 ldrb r4,[r3] 
						 lsr  r4,#4                              // as it is an even address value, nibble corresponds to the highest 4 bits                
						 b EIC_1_Handler_RnW_1_prepare_data_nibble

					.thumb_func 
					EIC_1_Handler_RnW_1_data_nibble_is_odd:       // take the nible from an ODD address
						movw r3,#:lower16:ui8_nibbles_buffer  // to access flash hardcoded ROMPACKS use: #:lower16:casio_rompack        
						movt r3,#:upper16:ui8_nibbles_buffer  // to access flash hardcoded ROMPACKS use: #:upper16:casio_rompack        
						add  r3,r2                                // add the current ROM Pack address to the base address of the ROM dump in the uC flash to get the address of the nibble in the uC flash        
						ldrb r4,[r3]                              // as it is an odd address value, nibble corresponds to the lowest 4 bits  	
	       
					.thumb_func 
					EIC_1_Handler_RnW_1_prepare_data_nibble:      // write the data nibble corresponding to the current address into the data bus
						and  r4,#0x0F
						eor  r4,#0x0F

						movw r3,#:lower16:ui8_write_nibble_val    // prepare next nibble to write into the data bus
						movt r3,#:upper16:ui8_write_nibble_val
						strb r4,[r3]

					    b EIC_1_Handler_cancel_int // cancel and leave the interrupt

		// ##################### nCS = 1 HIGH IMPEDANCE STATE ##########################################
		.thumb_func
         EIC_1_Handler_nCS_1:

		    movw r2,#:lower16:ui8_bus_state                        // check if bus is already configured in HIGH IMPEDANCE mode or not and if bus is already configured in HIGH IMPEDANCE then go to end
			movt r2,#:upper16:ui8_bus_state
			ldrb r3,[r2]
			cmp  r3,#VAR_BUS_STATE_HIGH_IMPEDANCE                 
		    bne  EIC_1_Handler_nCS_1_init
			b EIC_1_Handler_cancel_int // cancel and leave the interrupt

			.thumb_func
			EIC_1_Handler_nCS_1_init:
			   
				// Configure DATA lines as INPUT
				// ########## PORT B	 
				// configure INPUT DIRECTION of uC I/O pins used for 'DATA' signals: DIR bit = 1 OUTPUT , DIR bit = 0 INPUT
				movw r2,#:lower16:P1_DATA_MASK             
				movt r2,#:upper16:P1_DATA_MASK
				movw r3,#:lower16:REG_PORT_DIRCLR1         
				movt r3,#:upper16:REG_PORT_DIRCLR1
				str  r2,[r3]                               

				// Enable INEN bits 
				// PB12 - PIN1  - !Data0
				movw r2,#:lower16:REG_PORT_1_PINCFG_12     
				movt r2,#:upper16:REG_PORT_1_PINCFG_12
				movw r3,#:lower16:PINCFG_INEN_BIT_MASK     
				movt r3,#:upper16:PINCFG_INEN_BIT_MASK
				ldr  r4,[r2]                               
				orr  r4,r3                                 
				str  r4,[r2]   

				// PB13 - PIN?  - !Data1
				movw r2,#:lower16:REG_PORT_1_PINCFG_13     
				movt r2,#:upper16:REG_PORT_1_PINCFG_13
				movw r3,#:lower16:PINCFG_INEN_BIT_MASK     
				movt r3,#:upper16:PINCFG_INEN_BIT_MASK
				ldr  r4,[r2]                               
				orr  r4,r3
				str  r4,[r2] 

				// PB14 - PIN?  - !Data2
				movw r2,#:lower16:REG_PORT_1_PINCFG_14     
				movt r2,#:upper16:REG_PORT_1_PINCFG_14
				movw r3,#:lower16:PINCFG_INEN_BIT_MASK     
				movt r3,#:upper16:PINCFG_INEN_BIT_MASK
				ldr  r4,[r2]                               
				orr  r4,r3                                 
				str  r4,[r2] 

				// PB15 - PIN?  - !Data3
				movw r2,#:lower16:REG_PORT_1_PINCFG_15     
				movt r2,#:upper16:REG_PORT_1_PINCFG_15
				movw r3,#:lower16:PINCFG_INEN_BIT_MASK     
				movt r3,#:upper16:PINCFG_INEN_BIT_MASK
				ldr  r4,[r2]                               
				orr  r4,r3                                 
				str  r4,[r2] 

				// set to '1' the VALUE of I/O line  used to set '5-3.3V TRANSCEIVERs DIR' direction in order to
				// configure them in INPUT direction (A 5.0V to B 3.3V)  0 = OUTPUT (B 3.3V to A 5.0V ) , 1 = INPUT (A 5.0V to B 3.3V)
				movw r2,#:lower16:P0_DIR_DATA_BIT_MASK  
				movt r2,#:upper16:P0_DIR_DATA_BIT_MASK
				movw r3,#:lower16:REG_PORT_OUTSET0      
				movt r3,#:upper16:REG_PORT_OUTSET0
				str  r2,[r3]
				// END Configure DATA lines as input
		  
				movw r2,#:lower16:ui8_bus_state                      // update the bus state variable indicating that the bus is now configured in HIGH IMPEDANCE
				movt r2,#:upper16:ui8_bus_state
				movw r3,#:lower16:VAR_BUS_STATE_HIGH_IMPEDANCE
				movt r3,#:upper16:VAR_BUS_STATE_HIGH_IMPEDANCE		  
				strb r3,[r2]	  
  
				mov  r3,#1 
				movw r2,#:lower16:ui8_clck_cycle                     // initialize to #1 the ui8_clck_cycle variable used to keep the cycle (even, odd) of the currently used CLCKx signal
				movt r2,#:upper16:ui8_clck_cycle
				strb r3,[r2]

				// mov  r3,#0
				// movw r2,#:lower16:ui8_read_add_nibble_numb          // initialize to #0 the variable used to count the number of address nibbles written by the keyboard and read from the bus
				// movt r2,#:upper16:ui8_read_add_nibble_numb	  	   					  
				// strb r3,[r2]									     
		  											       
				// JBR 2023-07-06 Comentado pq ya no se usan las submaquinas de estado
				// mov  r3,#0 
				// movw r2,#:lower16:ui8_read_add_writ_by_keyb_state    // initialize to #0 the variable used to keep the state of the "read_addr_written_by_keyboard" substate machine
				// movt r2,#:upper16:ui8_read_add_writ_by_keyb_state									  
				// strb r3,[r2]
	
				// movw r2,#:lower16:ui8_write_data_req_by_keyb_state   // initialize to #0 the variable used to keep the state of the "write_data_read_by_keyboard" substate machine
				// movt r2,#:upper16:ui8_write_data_req_by_keyb_state									  
				// strb r3,[r2]		   
				// FIN JBR 2023-07-06 Comentado pq ya no se usan las submaquinas de estado

				// bl dpack_int_blink_read_led                       // invert the state of the READ LED. As an optimization, instead of in SET_BUS_READ_ADDR_WRIT_BY_KEYB, the GREEN READ LED state is inverted every time state machine enters into SET_BUS_HIGH_Z state

				// get the address of next data nibble to provide to the keyboard ( it is used as an offset inside the ROM dump content )
				movw r3,#:lower16:ui32_read_nibble_address 
				movt r3,#:upper16:ui32_read_nibble_address								
				ldr  r2,[r3]                                 // take current data nibble read address and increase it +1  for the next data nibble write cycle

				// prepare the next data nibble to write     // TODO: JBR 2023-07-06 si el valor de ui32_read_nibble_address ya lo teniamos arriba, quizas no haga falta ni esa variable y baste con un registro temporal
				lsrs r2,#1                                   // shift 1 bit to the right to divide requested ROMPack address by 2 ( the C flag is updated to the last bit shifted out,
																// except when the shift length is 0). ROMPack are addressed at nibble level while uC memory is adressed at byte level 
																// ( each byte in the uC memory contains two nibbles ). To convert the address of the nibble requested by the keyboard
																// to uC memory address, it is divided by 2. 
	   			bcs EIC_1_Handler_nCS_1_is_odd               // if, before dividing, the nibble address is ODD ( carry=1 so cc=false ) then low 4 bits of the byte are taken, and if the  
																// address befor dividing is EVEN the high 4 bits ( carry=0 so cc=true ) of the byte are taken.
				.thumb_func 
				EIC_1_Handler_nCS_1_is_even:                 // take the nible from an EVEN address
						movw r3,#:lower16:ui8_nibbles_buffer // to access flash hardcoded ROMPACKS use: #:lower16:casio_rompack              
						movt r3,#:upper16:ui8_nibbles_buffer // to access flash hardcoded ROMPACKS use: #:upper16:casio_rompack              
						add  r3,r2                              // add the current ROM Pack address to the base address of the ROM Pack dump in the uC flash to get the address of the nibble in the uC flash
						ldrb r4,[r3] 
						lsr  r4,#4                              // as it is an even address value, nibble corresponds to the highest 4 bits                
						b EIC_1_Handler_nCS_1_prepare_data_nibble

				.thumb_func 
				EIC_1_Handler_nCS_1_is_odd:                  // take the nible from an ODD address
					movw r3,#:lower16:ui8_nibbles_buffer  // to access flash hardcoded ROMPACKS use: #:lower16:casio_rompack        
					movt r3,#:upper16:ui8_nibbles_buffer  // to access flash hardcoded ROMPACKS use: #:upper16:casio_rompack        
					add  r3,r2                                // add the current ROM Pack address to the base address of the ROM dump in the uC flash to get the address of the nibble in the uC flash        
					ldrb r4,[r3]                              // as it is an odd address value, nibble corresponds to the lowest 4 bits  	
	       
				.thumb_func 
				EIC_1_Handler_nCS_1_prepare_data_nibble:      // write the data nibble corresponding to the current address into the data bus
					and  r4,#0x0F
					eor  r4,#0x0F

					movw r3,#:lower16:ui8_write_nibble_val    // prepare next nibble to write into the data bus
					movt r3,#:upper16:ui8_write_nibble_val
					strb r4,[r3]


			    b EIC_1_Handler_cancel_int // cancel and leave the interrupt

    .thumb_func
    EIC_1_Handler_cancel_int:

		// store the state of the ctrl bits in the ui8_ctrl_bits variable
		movw r3,#:lower16:ui8_ctrl_bits           
		movt r3,#:upper16:ui8_ctrl_bits
		strb r0,[r3]

		// the interrupt flag bit EIC1 is cleared by writing a '1' to it.
		movw r3,#:lower16:REG_EIC_INTFLAG 
		movt r3,#:upper16:REG_EIC_INTFLAG
		ldr  r4,[r3]	  
		movw r5,#:lower16:EIC_INTFLAG_EIC1_MASK 
		movt r5,#:upper16:EIC_INTFLAG_EIC1_MASK
		orr r4,r5

	   str  r4,[r3]

   .thumb_func
   EIC_1_Handler_end:

      pop {r2-r6} // pop the value of the registers used in the subroutine

	  bx lr



// READ AND WIRTE DATA FROM BUS:
//             _________   
//   !CS                |_______________________________________________________________________________________________________________________ ...
//             111111111100000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000
// 
//             __     ____________     ________     ____________     ________     ____________     ________     ____________     ________     __.... 
//   CLCK2       |___|            |___|        |___|            |___|        |___|            |___|        |___|            |___|        |___|
//             11100001111111111111000011111111100001111111111111000011111111100001111111111111000011111111100001111111111111000011111111100001
//
//            _________     ____________     ________     ____________     ________     ____________     ________     ____________     ________      
//   CLCK1             |___|            |___|        |___|            |___|        |___|            |___|        |___|            |___|        |_...
//            111111111100001111111111111000011111111100001111111111111000011111111100001111111111111000011111111100001111111111111000011111111100   
//
//  !CS        111111111100000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000//
//  CLCK2      11100001111111111111000011111111100001111111111111000011111111100001111111111111000011111111100001111111111111000011111111100001
//  CLCK1      11111111100001111111111111000011111111100001111111111111000011111111100001111111111111000011111111100001111111111111000011111111100   
//
//   WRITE DATA              write                        write                          write                         write  
//                             |XXXXXXXXXXXXXXXXXXXXXXXXXXXXX|YYYYYYYYYYYYYYYYYYYYYYYYYYYYY|ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ|XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//   READ DATA                 |        ^ read               |         ^ read              |        ^ read               |        ^ read  
//   
//                     11111111111000011111111100001111111111111000011111111100001111111111111000011111111100001111111111111000011111111100001...
//                     10000111111111111100001111111110000111111111111100001111111110000111111111111100001111111110000111111111111100001111111...
//
// Option 1             1   1  |  0   1  1   1 0   1  1   1  |  0   1  1   1 0   1  1   1  |  0   1  1   1 0   1  1   1  |  0   1  1   1 0   1  1   1  |
//                      0   1  |  1   1  0   1 1   1  0   1  |  1   1  0   1 1   1  0   1  |  1   1  0   1 1   1  0   1  |  1   1  0   1 1   1  0   1  | 
//
// Option 2            |1   1     0   1  1   1 0   1 |1   1     0   1  1   1 0   1 |1   1     0   1  1   1 0   1 |1   1     0   1  1   1 0   1 |1   1 ...  
//                     |0   1     1   1  0   1 1   1 |0   1     1   1  0   1 1   1 |0   1     1   1  0   1 1   1 |0   1     1   1  0   1 1   1 |0   1 ...   
// 
//    clck12_state =    0   1     2   3  4   5 6   7 |0   1     2   3  4   5 6   7 |0   1     2   3  4   5 6   7 |0   1     2   3  4   5 6   7 |0   1 ...    
// 

//###################################################################################################### 
// TEST GPIO
//   Infinte loop that keeps toggling the data lines to check that data line connections are OK
//######################################################################################################
.text
.global test_gpio_data_lines

.thumb_func
test_gpio_data_lines:	 	
    push {lr}

	bl config_data_lines_as_output
	bl config_ctrl_lines_as_output

	movw r4,#0
				 	
	.thumb_func 
	test_gpio_data_lines_loop:
		
		movw r0,#:lower16:REG_PORT_OUT1      // get the content of the port of the data bits
		movt r0,#:upper16:REG_PORT_OUT1
		ldr  r1,[r0]
		movw r2,#:lower16:(~P1_DATA_MASK)            
		movt r2,#:upper16:(~P1_DATA_MASK)
		and  r1,r2                            // clear the bits of the port that correspond to the data bits
					
		add r4,#1							  // increase the counter used to control the duration of each cycle of the square signals
		
		movw r5,#:lower16:60000
		movt r5,#:upper16:60000         
		cmp r4,r5                             // check if the counter has reached the 1st cycle value
		ble test_gpio_data_lines_a            // if it still has not reached the first value then set 0xa in the data lines 
		movw r5,#:lower16:120000
		movt r5,#:upper16:120000
		cmp r4,r5                             // check if the counter has reached the 2nd cycle value
		ble test_gpio_data_lines_b            // if has reached the first value but still has not reached the second value then set 0x5 in the data lines
		movw r4,#0                            
		b test_gpio_data_lines_loop

	.thumb_func 
	test_gpio_data_lines_a:

		movw r5,#:lower16:0xA000
		movt r5,#:upper16:0xA000
		orr  r1,r5
		str r1,[r0]
		b test_gpio_data_lines_loop

	.thumb_func 
	test_gpio_data_lines_b:

		movw r5,#:lower16:0x5000
		movt r5,#:upper16:0x5000
		orr  r1,r5
		str r1,[r0]
		b test_gpio_data_lines_loop
		
    pop {pc} // return from call ( place the LR previously stored in the stack in the PC )



//###################################################################################################### 
// SET_TEST_PIN
//    simply sets to '1' state of the TEST PIN
// WARNING!!!: THIS FUNCTIONS MODIFIES R4, R5, and R6, have this present before calling the function
//######################################################################################################
.text
.global set_test_pin

.thumb_func
set_test_pin:	 	
    push {lr}
	push {r4-r6}

	// get P0 ctrl bits state
	movw r4,#:lower16:REG_PORT_OUT0             
	movt r4,#:upper16:REG_PORT_OUT0 
	ldr  r5,[r4]                                // set in r5 current REG_PORT_OUT0 register value
	// set to '1' the P0_TEST_PIN_MASK pin
	movw r6,#:lower16:(P0_TEST_PIN_MASK)        
	movt r6,#:upper16:(P0_TEST_PIN_MASK)
	orr  r5,r6                                  // set the state of the bit before updating it with the OR with its new value
	str  r5,[r4]                                // update the port 0 value
   	
	pop {r4-r6}
    pop {pc} // return from call ( place the LR previously stored in the stack in the PC )



//###################################################################################################### 
// CLEAR_TEST_PIN
//    simply clears to '0' state of the TEST PIN
// WARNING!!!: THIS FUNCTIONS MODIFIES R4, R5, and R6, have this present before calling the function
//######################################################################################################
.text
.global clear_test_pin

.thumb_func
clear_test_pin:	 	
    push {lr}
	push {r4-r6}

	// get P0 ctrl bits state
	movw r4,#:lower16:REG_PORT_OUT0             
	movt r4,#:upper16:REG_PORT_OUT0 
	ldr  r5,[r4]                                // set in r5 current REG_PORT_OUT0 register value
	// clear to '0' the P0_TEST_PIN_MASK pin
	movw r6,#:lower16:(~P0_TEST_PIN_MASK)       
	movt r6,#:upper16:(~P0_TEST_PIN_MASK)
	and  r5,r6                                  // clear the state of the bit before updating it with the AND with its new value
	str  r5,[r4]                                // update the port 0 value
	
	pop {r4-r6}
    pop {pc} // return from call ( place the LR previously stored in the stack in the PC )



//###################################################################################################### 
// PULSE_TEST_PIN
//    simply sets to '1' and then to '0' the state of the TEST PIN
// WARNING!!!: THIS FUNCTIONS MODIFIES R4, R5, and R6, have this present before calling the function
//######################################################################################################
.text
.global pulse_test_pin

.thumb_func
pulse_test_pin:	 	
    push {lr}
	push {r4-r6}

	// get P0 ctrl bits state
	movw r4,#:lower16:REG_PORT_OUT0             
	movt r4,#:upper16:REG_PORT_OUT0 
	ldr  r5,[r4]                                // set in r5 current REG_PORT_OUT0 register value
	// set to '1' the P0_TEST_PIN_MASK pin
	movw r6,#:lower16:(P0_TEST_PIN_MASK)        
	movt r6,#:upper16:(P0_TEST_PIN_MASK)
	orr  r5,r6                                  // set the state of the bit before updating it with the OR with its new value
	str  r5,[r4]                                // update the port 0 value	
	// clear to '0' the P0_TEST_PIN_MASK pin
	movw r6,#:lower16:(~P0_TEST_PIN_MASK)       
	movt r6,#:upper16:(~P0_TEST_PIN_MASK)
	and  r5,r6                                  // clear the state of the bit before updating it with the AND with its new value
	str  r5,[r4]                                // update the port 0 value   	

	pop {r4-r6}
    pop {pc} // return from call ( place the LR previously stored in the stack in the PC )



//###################################################################################################### 
// TOGLE_TEST_PIN
//    simply inverts the state of the TEST PIN
// WARNING!!!: THIS FUNCTIONS MODIFIES R4, R5, and R6, have this present before calling the function
//######################################################################################################
.text
.global toggle_test_pin

.thumb_func
toggle_test_pin:	 	
    push {lr}
	push {r4-r6}

	// get P0 ctrl bits state
	movw r4,#:lower16:REG_PORT_OUT0             
	movt r4,#:upper16:REG_PORT_OUT0 
	ldr  r5,[r4]                                // set in r5 current REG_PORT_OUT0 register value
	// get the state of the P0_TEST_PIN_MASK pin
	movw r6,#:lower16:(P0_TEST_PIN_MASK)       
	movt r6,#:upper16:(P0_TEST_PIN_MASK)
	eor  r5,r6                                  // apply an XOR '1' over the TEST PIN.
	str  r5,[r4]                                // update the port 0 value

	pop {r4-r6}
    pop {pc} // return from call ( place the LR previously stored in the stack in the PC )



//###################################################################################################### 
// CONFIG DATA LINES AS INPUT
//   Configures DATA uC I/O pins as INPUT, and also changes the value of DIR_DATA line to '1' to set the 
// DATA TRANSCEIVER in INPUT direction (A 5.0V to B 3.3V) 
// Set Data I/O pins DIRECTION to:
//  PB15        (6)         PIN1  - !Data3    
//  PB14        (5)         PIN2  - !Data2
//  PB13        (4)         PIN3  - !Data1
//  PB12        (7)         PIN4  - !Data0
// Set DIR_DATA I/O line VALUE to change TRANSCEIVER to INPUT direction:
//  PA18       (D10)        DIR_DATA = 1  INPUT direction (A 5.0V to B 3.3V) 
//######################################################################################################
.global config_data_lines_as_input
.thumb_func 
config_data_lines_as_input:
     push {lr}

	 // ########## PORT B
	 
	// configure INPUT DIRECTION of uC I/O pins used for 'DATA' signals: DIR bit = 1 OUTPUT , DIR bit = 0 INPUT
	movw r2,#:lower16:P1_DATA_MASK             
	movt r2,#:upper16:P1_DATA_MASK
	movw r3,#:lower16:REG_PORT_DIRCLR1         
	movt r3,#:upper16:REG_PORT_DIRCLR1
	str  r2,[r3]                               

	// Enable INEN bits 
	// PB12 - PIN1  - !Data0
	movw r2,#:lower16:REG_PORT_1_PINCFG_12     
	movt r2,#:upper16:REG_PORT_1_PINCFG_12
	movw r3,#:lower16:PINCFG_INEN_BIT_MASK     
	movt r3,#:upper16:PINCFG_INEN_BIT_MASK
	ldr  r4,[r2]                               
	orr  r4,r3                                 
	str  r4,[r2]   

	// PB13 - PIN?  - !Data1
	movw r2,#:lower16:REG_PORT_1_PINCFG_13     
	movt r2,#:upper16:REG_PORT_1_PINCFG_13
	movw r3,#:lower16:PINCFG_INEN_BIT_MASK     
	movt r3,#:upper16:PINCFG_INEN_BIT_MASK
	ldr  r4,[r2]                               
	orr  r4,r3
	str  r4,[r2] 

	// PB14 - PIN?  - !Data2
	movw r2,#:lower16:REG_PORT_1_PINCFG_14     
	movt r2,#:upper16:REG_PORT_1_PINCFG_14
	movw r3,#:lower16:PINCFG_INEN_BIT_MASK     
	movt r3,#:upper16:PINCFG_INEN_BIT_MASK
	ldr  r4,[r2]                               
	orr  r4,r3                                 
	str  r4,[r2] 

	// PB15 - PIN?  - !Data3
	movw r2,#:lower16:REG_PORT_1_PINCFG_15     
	movt r2,#:upper16:REG_PORT_1_PINCFG_15
	movw r3,#:lower16:PINCFG_INEN_BIT_MASK     
	movt r3,#:upper16:PINCFG_INEN_BIT_MASK
	ldr  r4,[r2]                               
	orr  r4,r3                                 
	str  r4,[r2] 

	// set to '1' the VALUE of I/O line  used to set '5-3.3V TRANSCEIVERs DIR' direction in order to
	// configure them in INPUT direction (A 5.0V to B 3.3V)  0 = OUTPUT (B 3.3V to A 5.0V ) , 1 = INPUT (A 5.0V to B 3.3V)
	movw r2,#:lower16:P0_DIR_DATA_BIT_MASK  
	movt r2,#:upper16:P0_DIR_DATA_BIT_MASK
	movw r3,#:lower16:REG_PORT_OUTSET0      
	movt r3,#:upper16:REG_PORT_OUTSET0
	str  r2,[r3]

    pop {pc} // return from call ( place the LR previously stored in the stack in the PC )



//###################################################################################################### 
// CONFIG DATA LINES AS OUTPUT
//   Configures DATA uC I/O pins as OUTPUT, and also changes the value of DIR_DATA line to '0' to set the 
// DATA TRANSCEIVER in OUTPUT direction (B 3.3V to A 5.0V) 
// Set Data I/O pins DIRECTION to:
//  PB15        (6)         PIN1  - !Data3    
//  PB14        (5)         PIN2  - !Data2
//  PB13        (4)         PIN3  - !Data1
//  PB12        (7)         PIN4  - !Data0
// Set DIR_DATA I/O line VALUE to change TRANSCEIVER to INPUT direction:
//  PA18        (D10)       DIR_DATA = 0  OUTPUT direction ( B 3.3V to A 5.0V ) 
// POST:
// Registers modified: 
//      r2, r3, r4
//######################################################################################################
.global config_data_lines_as_output

.thumb_func 
config_data_lines_as_output:
     push {lr}

    // configure all used IO lines:
	 
	// ########## PORT B

	// configure DIRECTION of I/O pins used for 'DATA'  signals: DIR bit = 1 OUTPUT , DIR bit = 0 INPUT
	movw r2,#:lower16:P1_DATA_MASK       
	movt r2,#:upper16:P1_DATA_MASK
	movw r3,#:lower16:REG_PORT_DIRSET1   
	movt r3,#:upper16:REG_PORT_DIRSET1
	str  r2,[r3]                         

	// set to '1' the VALUE of I/O line used to set data lines ( start with data lines to 0xF )
	movw r2,#:lower16:P1_DATA_MASK   
	movt r2,#:upper16:P1_DATA_MASK
	movw r3,#:lower16:REG_PORT_OUTSET1
	movt r3,#:upper16:REG_PORT_OUTSET1
	str  r2,[r3]     

	// Disable INEN bits 
	// PB12 - PIN?  - !Data0
	movw r2,#:lower16:REG_PORT_1_PINCFG_12     
	movt r2,#:upper16:REG_PORT_1_PINCFG_12
	movw r3,#:lower16:(~PINCFG_INEN_BIT_MASK)  
	movt r3,#:upper16:(~PINCFG_INEN_BIT_MASK)
	ldr  r4,[r2]                               
	and  r4,r3                                 
	str  r4,[r2]   

	// PB13 - PIN?  - !Data1
	movw r2,#:lower16:REG_PORT_1_PINCFG_13     
	movt r2,#:upper16:REG_PORT_1_PINCFG_13
	movw r3,#:lower16:(~PINCFG_INEN_BIT_MASK)  
	movt r3,#:upper16:(~PINCFG_INEN_BIT_MASK)
	ldr  r4,[r2]                               
	and  r4,r3                                 
	str  r4,[r2] 

	// PB14 - PIN?  - !Data2
	movw r2,#:lower16:REG_PORT_1_PINCFG_14     
	movt r2,#:upper16:REG_PORT_1_PINCFG_14
	movw r3,#:lower16:(~PINCFG_INEN_BIT_MASK)  
	movt r3,#:upper16:(~PINCFG_INEN_BIT_MASK)
	ldr  r4,[r2]                               
	and  r4,r3                                 
	str  r4,[r2] 

	// PB15 - PIN?  - !Data3
	movw r2,#:lower16:REG_PORT_1_PINCFG_15     
	movt r2,#:upper16:REG_PORT_1_PINCFG_15
	movw r3,#:lower16:(~PINCFG_INEN_BIT_MASK)  
	movt r3,#:upper16:(~PINCFG_INEN_BIT_MASK)
	ldr  r4,[r2]                               
	and  r4,r3                                 
	str  r4,[r2] 

	// set to '0' the VALUE of I/O line  used to set '5-3.3V TRANSCEIVERs DIR' direction in order to configure
	// them in OUTPUT direction (B 3.3V to A 5.0V)  0 = OUTPUT (B 3.3V to A 5.0V ) , 1 = INPUT (A 5.0V to B 3.3V)
	movw r2,#:lower16:P0_DIR_DATA_BIT_MASK   
	movt r2,#:upper16:P0_DIR_DATA_BIT_MASK
	movw r3,#:lower16:REG_PORT_OUTCLR0    
	movt r3,#:upper16:REG_PORT_OUTCLR0
	str  r2,[r3]                             

    pop {pc} // return from call ( place the LR previously stored in the stack in the PC )



//###################################################################################################### 
// CONFIG CTRL LINES AS INPUT
//   Configures CTRL uC I/O pins as INPUT, and also changes the value of DIR_CTRL line to '1' to set the 
// CTRL TRANSCEIVER in INPUT direction (A 5.0V to B 3.3V) 
//  PB11        (A5)        PIN5  - R/!W
//  PB16        (3)         PIN6  - CLCK1
//  PB17        (2)         PIN7  - CLCK2
//  PA02        (A0)        PIN8  - ?
//  PB10        (A4)        PIN10 - !CS
// Set DIR_CTRL I/O line VALUE to change TRANSCEIVER to INPUT direction:
//  PA06        (A2)        DIR_CTRL = 1  INPUT direction (A 5.0V to B 3.3V) 
//######################################################################################################
.global config_ctrl_lines_as_input
.thumb_func 
config_ctrl_lines_as_input:
     push {lr}

    // ########## PORT A

	// configure direction of PORTA CTRL DATA bits  DIR bit = 1 OUTPUT , DIR bit = 0 INPUT
	movw r0,#:lower16:P0_UNKNWN_CTRL_BIT_MASK  
	movt r0,#:upper16:P0_UNKNWN_CTRL_BIT_MASK
	movw r1,#:lower16:REG_PORT_DIRCLR0         
	movt r1,#:upper16:REG_PORT_DIRCLR0
	str  r0,[r1]                               

	// configure Port A input bits INEN bit to enable sampling of INPUT pin
	// PA02 - '?'
	movw r0,#:lower16:REG_PORT_0_PINCFG_02     
	movt r0,#:upper16:REG_PORT_0_PINCFG_02
	movw r1,#:lower16:PINCFG_INEN_BIT_MASK     
	movt r1,#:upper16:PINCFG_INEN_BIT_MASK
	ldr  r2,[r0]                               
	orr  r2,r1                                 
	str  r2,[r0]                               

	 // ########## PORT B

	// configure direction of PORTB CTRL DATA bits DIR bit = 1 OUTPUT , DIR bit = 0 INPUT
	movw r0,#:lower16:P1_CTRL_MASK           
	movt r0,#:upper16:P1_CTRL_MASK
	movw r1,#:lower16:REG_PORT_DIRCLR1       
	movt r1,#:upper16:REG_PORT_DIRCLR1
	str  r0,[r1]                        

	// configure Port B input bits INEN bits to enable sampling of INPUT pins
	// PB10 - !CS
	movw r0,#:lower16:REG_PORT_1_PINCFG_10   
	movt r0,#:upper16:REG_PORT_1_PINCFG_10
	movw r1,#:lower16:PINCFG_INEN_BIT_MASK     
	movt r1,#:upper16:PINCFG_INEN_BIT_MASK
	ldr  r2,[r0]                               
	orr  r2,r1                                 
	str  r2,[r0]        

	// PB11 - R/!W
	movw r0,#:lower16:REG_PORT_1_PINCFG_11     
	movt r0,#:upper16:REG_PORT_1_PINCFG_11
	movw r1,#:lower16:PINCFG_INEN_BIT_MASK     
	movt r1,#:upper16:PINCFG_INEN_BIT_MASK
	ldr  r2,[r0]                               
	orr  r2,r1                                 
	str  r2,[r0]        

	// PB16 - CLCK1
	movw r0,#:lower16:REG_PORT_1_PINCFG_16    
	movt r0,#:upper16:REG_PORT_1_PINCFG_16
	movw r1,#:lower16:PINCFG_INEN_BIT_MASK    
	movt r1,#:upper16:PINCFG_INEN_BIT_MASK
	ldr  r2,[r0]                              
	orr  r2,r1                                
	str  r2,[r0] 

	// PB17 - CLCK2
	movw r0,#:lower16:REG_PORT_1_PINCFG_17     
	movt r0,#:upper16:REG_PORT_1_PINCFG_17
	movw r1,#:lower16:PINCFG_INEN_BIT_MASK     
	movt r1,#:upper16:PINCFG_INEN_BIT_MASK
	ldr  r2,[r0]                               
	orr  r2,r1                                 
	str  r2,[r0] 

	// set to '1' the VALUE of I/O line  used to set '5-3.3V TRANSCEIVERs DIR' direction in order to
	// configure them in INPUT direction (A 5.0V to B 3.3V)  0 = OUTPUT (B 3.3V to A 5.0V ) , 1 = INPUT (A 5.0V to B 3.3V)
	movw r0,#:lower16:P0_DIR_CTRL_BIT_MASK
	movt r0,#:upper16:P0_DIR_CTRL_BIT_MASK
	movw r1,#:lower16:REG_PORT_OUTSET0    
	movt r1,#:upper16:REG_PORT_OUTSET0
	str  r0,[r1]                          
	
    pop {pc} // return from call ( place the LR previously stored in the stack in the PC )



//###################################################################################################### 
// CONFIG CTRL LINES AS OUTPUT
//   Configures CTRL uC I/O pins as OUTPUT, and also changes the value of DIR_CTRL line to '0' to set the 
// CTRL TRANSCEIVER in OUTPUT direction ( B 3.3V to A 5.0V )
//  PB11        (A5)        PIN5  - R/!W
//  PB16        (3)         PIN6  - CLCK1
//  PB17        (2)         PIN7  - CLCK2
//  PA02        (A0)        PIN8  - ?
//  PB10        (A4)        PIN10 - !CS
// Clear DIR_CTRL I/O line VALUE to change TRANSCEIVER to OUTPUT direction:
//  PA06        (A2)        DIR_CTRL = 1  INPUT direction  ( B 3.3V to A 5.0V )
//######################################################################################################
.global config_ctrl_lines_as_output
.thumb_func 
config_ctrl_lines_as_output:
     push {lr}

    // ########## PORT A

	// configure direction of PORTA CTRL DATA bits DIR bit = 1 OUTPUT , DIR bit = 0 INPUT
	movw r2,#:lower16:P0_UNKNWN_CTRL_BIT_MASK  
	movt r2,#:upper16:P0_UNKNWN_CTRL_BIT_MASK
	movw r3,#:lower16:REG_PORT_DIRSET0         
	movt r3,#:upper16:REG_PORT_DIRSET0
	str  r2,[r3]                               

	// configure Port A input bits INEN bit to disable sampling of INPUT pin
	// PA02 - '?'
	movw r2,#:lower16:REG_PORT_0_PINCFG_02     
	movt r2,#:upper16:REG_PORT_0_PINCFG_02
	movw r3,#:lower16:(~PINCFG_INEN_BIT_MASK)  
	movt r3,#:upper16:(~PINCFG_INEN_BIT_MASK)
	ldr  r4,[r2]                               
	and  r4,r3                                 
	str  r4,[r2]                               

	 // ########## PORT B

	// configure direction of PORTA CTRL DATA bits DIR bit = 1 OUTPUT , DIR bit = 0 INPUT
	movw r2,#:lower16:P1_CTRL_MASK            
	movt r2,#:upper16:P1_CTRL_MASK
	movw r3,#:lower16:REG_PORT_DIRSET1        
	movt r3,#:upper16:REG_PORT_DIRSET1
	str  r2,[r3]  
	  
	// configure Port B input bits INEN bits to disable sampling of INPUT pins
	// PB10 - !CS
	movw r2,#:lower16:REG_PORT_1_PINCFG_10    
	movt r2,#:upper16:REG_PORT_1_PINCFG_10
	movw r3,#:lower16:(~PINCFG_INEN_BIT_MASK)  
	movt r3,#:upper16:(~PINCFG_INEN_BIT_MASK)
	ldr  r4,[r2]                               
	and  r4,r3                                 
	str  r4,[r2]        

	// PB11 - R/!W
	movw r2,#:lower16:REG_PORT_1_PINCFG_11     
	movt r2,#:upper16:REG_PORT_1_PINCFG_11
	movw r3,#:lower16:(~PINCFG_INEN_BIT_MASK)  
	movt r3,#:upper16:(~PINCFG_INEN_BIT_MASK)
	ldr  r4,[r2]                               
	and  r4,r3                                 
	str  r4,[r2]        

	// PB16 - CLCK1
	movw r2,#:lower16:REG_PORT_1_PINCFG_16     
	movt r2,#:upper16:REG_PORT_1_PINCFG_16
	movw r3,#:lower16:(~PINCFG_INEN_BIT_MASK)  
	movt r3,#:upper16:(~PINCFG_INEN_BIT_MASK)
	ldr  r4,[r2]                               
	and  r4,r3                                 
	str  r4,[r2] 

	// PB17 - CLCK2
	movw r2,#:lower16:REG_PORT_1_PINCFG_17     
	movt r2,#:upper16:REG_PORT_1_PINCFG_17
	movw r3,#:lower16:(~PINCFG_INEN_BIT_MASK)  
	movt r3,#:upper16:(~PINCFG_INEN_BIT_MASK)
	ldr  r4,[r2]                               
	and  r4,r3                                 
	str  r4,[r2] 
	
	// set to '0' the VALUE of I/O line  used to set '5-3.3V TRANSCEIVERs DIR' direction in order to configure
	// them in OUTPUT direction (A 5.0V to B 3.3V)  0 = OUTPUT (B 3.3V to A 5.0V ) , 1 = INPUT (A 5.0V to B 3.3V)
	movw r2,#:lower16:P0_DIR_CTRL_BIT_MASK
	movt r2,#:upper16:P0_DIR_CTRL_BIT_MASK
	movw r3,#:lower16:REG_PORT_OUTCLR0    
	movt r3,#:upper16:REG_PORT_OUTCLR0
	str  r2,[r3] 

    pop {pc} // return from call ( place the LR previously stored in the stack in the PC )



//###################################################################################################### 
// PREPARE NEXT NIBBLE TO WRITE
// Procedure that prepares the data nibble to write into the bus on the next data write iteration. It
// uses the last address value requested by the keyboard ui32_read_nibble_address to access to the 
// ui8_nibbles_buffer then it takes the corresponding nibble places it into the variable 
// ui8_write_nibble_val 
// POST:
//   r4,r,r6,r7 MODIFIED
//######################################################################################################
.text
.global prepare_next_data_nibble_to_write

.thumb_func
prepare_next_data_nibble_to_write:
    push {lr}
					
		movw r4,#:lower16:ui32_read_nibble_address   // get the address of next data nibble to provide to the keyboard, the address is used as an offset inside the ROM dump content
		movt r4,#:upper16:ui32_read_nibble_address
		ldr  r5,[r4]
		lsrs r5,#1                                   // shift 1 bit to the right to divide requested ROMPack address by 2 ( the C flag is updated to the last bit shifted out,
                                                     // except when the shift length is 0). ROMPack are addressed at nibble level while uC memory is adressed at byte level 
                                                     // ( each byte in the uC memory contains two nibbles ). To convert the address of the nibble requested by the keyboard
                                                     // to uC memory address, it is divided by 2. 
		bcs prepare_next_data_nibble_is_odd          // if, before dividing, the nibble address is ODD ( carry=1 so cc=false ) then low 4 bits of the byte are taken, and if the  
                                                     // address befor dividing is EVEN the high 4 bits ( carry=0 so cc=true ) of the byte are taken.
		.thumb_func 
		prepare_next_data_nibble_is_even:            // take the nible from an EVEN address
				movw r6,#:lower16:ui8_nibbles_buffer // to access flash hardcoded ROMPACKS use: #:lower16:casio_rompack              
				movt r6,#:upper16:ui8_nibbles_buffer // to access flash hardcoded ROMPACKS use: #:upper16:casio_rompack              
				add  r6,r5                           // add the current ROM Pack address to the base address of the ROM Pack dump in the uC flash to get the address of the nibble in the uC flash
				ldrb r7,[r6] 
				lsr  r7,#4                           // as it is an even address value, nibble corresponds to the highest 4 bits                
				b prepare_next_data_nibble
		.thumb_func 
		prepare_next_data_nibble_is_odd:             // take the nible from an ODD address
				movw r6,#:lower16:ui8_nibbles_buffer      // to access flash hardcoded ROMPACKS use: #:lower16:casio_rompack        
				movt r6,#:upper16:ui8_nibbles_buffer      // to access flash hardcoded ROMPACKS use: #:upper16:casio_rompack        
				add  r6,r5                           // add the current ROM Pack address to the base address of the ROM dump in the uC flash to get the address of the nibble in the uC flash        
				ldrb r7,[r6]                         // as it is an odd address value, nibble corresponds to the lowest 4 bits  	
		.thumb_func 
		prepare_next_data_nibble:                    // write the data nibble corresponding to the current address into the data bus
				and  r7,#0x0F
				eor  r7,#0x0F

				movw r6,#:lower16:ui8_write_nibble_val  // prepare next nibble to write into the data bus
				movt r6,#:upper16:ui8_write_nibble_val
				strb r7,[r6]

		.thumb_func 
		prepare_next_data_nibble_to_write_end:

	pop {pc} // return from call ( place the LR previously stored in the stack in the PC )



//###################################################################################################### 
// ENABLE EIC0 INTERRUPT ( connected to CLCK1 )
// Enable the EIC 0 interruption on clck1 line to capture all the transitions without loosing any
// of them ( disable the EIC 1 interruption on clck2 line before enabling the EIC0 interrupt )
//###################################################################################################### 
// PRE: 
.global enable_clck1_interrupt

.thumb_func
enable_clck1_interrupt:
		push {lr}

		// enable EIC0 interrupt ( CLCK1 line triggers the EIC 0 ). EICx interrupt is enabled by writting '1' on the corresponding REG_EIC_INTENSET bit
		movw r2,#:lower16:REG_EIC_INTENSET
		movt r2,#:upper16:REG_EIC_INTENSET
		movw r3,#:lower16:EIC_INTENCLR_EIC0_MASK 
		movt r3,#:upper16:EIC_INTENCLR_EIC0_MASK
		str  r3,[r2]

		.thumb_func
		enable_clck1_interrupt_end:

           pop {pc} // return from call ( place the LR previously stored in the stack in the PC )


//###################################################################################################### 
// DISABLE EIC0 INTERRUPT ( connected to CLCK1 )
// Disable the EIC 0 interruption on clck1 line used to capture all the CLCK1 transitions 
//###################################################################################################### 
// PRE: 
.global disable_clck1_interrupt

.thumb_func
disable_clck1_interrupt:
		push {lr}

		// disable EIC0 interrupt ( CLCK1 line triggers the EIC 0 ). EICx interrupt is disabled by writting '1' on the corresponding REG_EIC_INTENCLR bit
		movw r2,#:lower16:REG_EIC_INTENCLR 
		movt r2,#:upper16:REG_EIC_INTENCLR
		movw r3,#:lower16:EIC_INTENCLR_EIC0_MASK 
		movt r3,#:upper16:EIC_INTENCLR_EIC0_MASK
		str  r3,[r2]

		.thumb_func
		disable_clck1_interrupt_end:

           pop {pc} // return from call ( place the LR previously stored in the stack in the PC )


//###################################################################################################### 
// ENABLE EIC1 INTERRUPT ( connected to CLCK2 )
// Enable the EIC 1 interruption on clck2 line to capture all the transitions without loosing any
// of them ( disable the EIC 0 interruption on clck1 line before enabling the EIC1 interrupt )
//###################################################################################################### 
// PRE: 
.global enable_clck2_interrupt

.thumb_func
enable_clck2_interrupt:
		push {lr}

		// enable EIC1 interrupt ( CLCK2 line triggers the EIC 1 ). EICx interrupt is enabled by writting '1' on the corresponding REG_EIC_INTENSET bit
		movw r2,#:lower16:REG_EIC_INTENSET
		movt r2,#:upper16:REG_EIC_INTENSET
		movw r3,#:lower16:EIC_INTENCLR_EIC1_MASK 
		movt r3,#:upper16:EIC_INTENCLR_EIC1_MASK
		str  r3,[r2]

		.thumb_func
		enable_clck2_interrupt_end:

			pop {pc} // return from call ( place the LR previously stored in the stack in the PC )


//###################################################################################################### 
// DISABLE EIC1 INTERRUPT ( connected to CLCK2 )
// Disable the EIC 1 interruption on clck2 line used to capture all the CLCK2 transitions 
//###################################################################################################### 
// PRE: 
.global disable_clck2_interrupt

.thumb_func
disable_clck2_interrupt:
		push {lr}

		// disable EIC1 interrupt ( CLCK2 line triggers the EIC 1 ). EICx interrupt is disabled by writting '1' on the corresponding REG_EIC_INTENCLR bit
		movw r2,#:lower16:REG_EIC_INTENCLR 
		movt r2,#:upper16:REG_EIC_INTENCLR
		movw r3,#:lower16:EIC_INTENCLR_EIC1_MASK 
		movt r3,#:upper16:EIC_INTENCLR_EIC1_MASK
		str  r3,[r2]

		.thumb_func
		disable_clck2_interrupt_end:

			pop {pc} // return from call ( place the LR previously stored in the stack in the PC )


		
//###################################################################################################### 
// READ ADDRESS WRITTEN BY KEYBOARD SUBSTATE MACHINE
// The KEYBOARD writes into the bus the address of the data to read.
// Takes the value of each nibble written by the keyboard to get the data address requested by the 
// keyboard.
//###################################################################################################### 
.global read_address_written_by_keyboard

	.thumb_func 
	read_address_written_by_keyboard:
		push {lr}

        movw r5,#:lower16:ui8_read_add_writ_by_keyb_state
		movt r5,#:upper16:ui8_read_add_writ_by_keyb_state
		ldrb r4,[r5]
        cmp r4,#0 
		beq read_address_written_by_keyboard_substate_0
        cmp r4,#1 
		beq read_address_written_by_keyboard_substate_1
        cmp r4,#2
		beq read_address_written_by_keyboard_substate_2
        cmp r4,#3
		beq read_address_written_by_keyboard_substate_3
        cmp r4,#4
		beq read_address_written_by_keyboard_substate_4
        b read_address_written_by_keyboard_end

	.thumb_func 
	read_address_written_by_keyboard_substate_0:

	    movw r6,#:lower16:ui8_clck_cycle
		movt r6,#:upper16:ui8_clck_cycle    
		ldrb r7,[r6]
		cmp  r7,#2
		bne  read_address_written_by_keyboard_end     // check if the ui8_nW_clck_cycle is 2 and in that case move to the next substate 
		
		add  r4,#1                                    // add #1 to the ui8_read_add_writ_by_keyb_state to move to the next substate
		strb r4,[r5]                                  // update theui8_read_add_writ_by_keyb_state with the new susbtate value
		b read_address_written_by_keyboard_end

	.thumb_func 
	read_address_written_by_keyboard_substate_1:
        		
	    movw r6,#:lower16:REG_PORT_IN1                // read the current address nibble in the bus written by the keyboard
		movt r6,#:upper16:REG_PORT_IN1
        ldr  r7,[r6]
		and  r7,#P1_DATA_MASK                         // get only the address bits

		movw r6,#:lower16:ui8_read_nibble_val         // store in ui8_read_nibble_val the current address nibble value
		movt r6,#:upper16:ui8_read_nibble_val			
		lsr  r7,#12
		strb  r7,[r6]

		add  r4,#1                                    // add #1 to the ui8_read_add_writ_by_keyb_state to move to the next substate
		strb r4,[r5]                                  // update theui8_read_add_writ_by_keyb_state with the new susbtate value
		b read_address_written_by_keyboard_end

	.thumb_func 
	read_address_written_by_keyboard_substate_2:
	
        movw r6,#:lower16:ui8_read_add_nibble_numb                // calculate the number of bits to shift the read nibble to place it in the right position in the ui32_read_nibble_address according to the current number of read address nibbles till the moment.
		movt r6,#:upper16:ui8_read_add_nibble_numb
		ldrb r7,[r6]
		cmp  r7,#VAR_NUM_NIBBLES_ADDR                             // check if all the address nibbles have been read and continue if not
		beq  read_address_written_by_keyboard_substate_2_upd_state

		movw r6,#:lower16:ui8_read_nibble_val                     // take the nibble read in the previous 'ui8_read_add_writ_by_keyb_state_0' state
		movt r6,#:upper16:ui8_read_nibble_val
		ldrb r8,[r6]
				
		movw r6,#:lower16:ui32_read_nibble_address                // take the address with the nibbles that have been stored in the previous cycles
		movt r6,#:upper16:ui32_read_nibble_address
		ldr  r9,[r6]

		lsl  r7,#2                                                // shift 2 bits to left to multipy by 4 the number of nibbles to get the number of bits to shift: nibble 0:(0x4)=0 bits, nibble 1:(1x4)=4bits, nibble 2:(2x4)=8bits, nibble 3:(3x4)=12bits, nibble 4:(4x4)=16bits,

		movw r6,#:lower16:0x0000000F                              // prepare the mask used to modify only the bits corresponding to the current processed address nibble position
		movt r6,#:upper16:0x0000000F

		lsl  r6,r7                                                // shift the mask used to modify only the bits corresponding to the current processed address nibble position
		lsl  r8,r7                                                // shift the the nibble read on previous substate to place it in the bits corresponding to the current processed address nibble position

		bic  r9,r6                                                // clean the bits of the bits of the 'ui32_read_nibble_address' that we are going to initialize: bic rX,rY is an 'and' with rY complements 0x0000000F > 0xFFFFFFF0 & rX
		orr  r9,r8                                                // place the the nibble read on previous 'ui8_read_add_writ_by_keyb_state_0' state in the bits corresponding to the current processed address nibble position

		movw r6,#:lower16:ui32_read_nibble_address                // store the address nibbbles that have been stored into 'ui32_read_nibble_address' till the moment
		movt r6,#:upper16:ui32_read_nibble_address
		str  r9,[r6]

	.thumb_func 
	read_address_written_by_keyboard_substate_2_upd_state:

		add  r4,#1                                    // add #1 to the ui8_read_add_writ_by_keyb_state to move to the next substate
		strb r4,[r5]                                  // update theui8_read_add_writ_by_keyb_state with the new susbtate value
		b read_address_written_by_keyboard_end	

	.thumb_func 
	read_address_written_by_keyboard_substate_3:

		movw r6,#:lower16:ui8_read_add_nibble_numb		// take the current number of address nibbles that have been written by the keyboard
		movt r6,#:upper16:ui8_read_add_nibble_numb
		ldrb r7,[r6]
		cmp  r7,#VAR_NUM_NIBBLES_ADDR        			// check if all address nibbles written by the keyboard have been read, and continue if not
		beq read_address_written_by_keyboard_substate_3_upd_state
		add  r7,#1										// if there are still nibbles to read increase the read address nibbles counter 
		strb r7,[r6]
		cmp  r7,#VAR_NUM_NIBBLES_ADDR                   // last nibble of the address requested by the keyboard is always a '0xC' but it ¿ does not contain information ?
		bne read_address_written_by_keyboard_substate_3_upd_state
		movw r6,#:lower16:ui32_read_nibble_address		// all address nibbles have been received so it is time to process them to get the right ROM address value
		movt r6,#:upper16:ui32_read_nibble_address
		ldr  r7,[r6]
		movw r8,#:lower16:0x0000FFFF
		movt r8,#:upper16:0x0000FFFF
		and  r7, r8										// take only the 4 lowest nibbles and discard 5th ( 5th nibble corresponds to the 0xC )
		eor  r7, r8										// all bits are read from the bus in inverse logic, so they must be reversed to get the right value
		str  r7,[r6]                                    // store the address once processed

	.thumb_func 
	read_address_written_by_keyboard_substate_3_upd_state:

		add  r4,#1                                    // add #1 to the ui8_read_add_writ_by_keyb_state to move to the next substate
		strb r4,[r5]                                  // update theui8_read_add_writ_by_keyb_state with the new susbtate value
		b read_address_written_by_keyboard_end	

	.thumb_func 
	read_address_written_by_keyboard_substate_4:

	    movw r6,#:lower16:ui8_clck_cycle
		movt r6,#:upper16:ui8_clck_cycle    
		ldrb r7,[r6]
		cmp  r7,#1
		bne  read_address_written_by_keyboard_end     // check if the ui8_nW_clck_cycle is 1 and in that case move to the next substate 
		mov  r4,#0                                    // reset ui8_read_add_writ_by_keyb_state substate to "0"
		strb r4,[r5]                                  // update theui8_read_add_writ_by_keyb_state with the new susbtate value
		b read_address_written_by_keyboard_end

	.thumb_func 
	read_address_written_by_keyboard_end:
			
		pop {pc} // return from call ( place the LR previously stored in the stack in the PC )


//###################################################################################################### 
// WRITE DATA READ BY KEYBOARD SUBSTATE MACHINE
// The ROM writes in the bus the data nibble of the last address requested by the keyboard. Once the 
// keyboard has read it, then the ROM increases its internal counter and repeats the process with the
// following address.
//###################################################################################################### 
.global write_data_read_by_keyboard

	.thumb_func 
	write_data_read_by_keyboard:
		push {lr}
		
        movw r3,#:lower16:ui8_write_data_req_by_keyb_state
		movt r3,#:upper16:ui8_write_data_req_by_keyb_state
		ldrb r2,[r3]
        cmp r2,#0 
		beq write_data_read_by_keyboard_substate_0
        cmp r2,#1 
		beq write_data_read_by_keyboard_substate_1
        cmp r2,#2
		beq write_data_read_by_keyboard_substate_2
        cmp r2,#3
		beq write_data_read_by_keyboard_substate_3
		b write_data_read_by_keyboard_end

	.thumb_func 
	write_data_read_by_keyboard_substate_0:

	    movw r5,#:lower16:ui8_clck_cycle
		movt r5,#:upper16:ui8_clck_cycle    
		ldrb r4,[r5]
		cmp  r4,#2
		bne  write_data_read_by_keyboard_end     // check if the ui8_R_clck_cycle is 1 and in that case restart the state machine

		add  r2,#1                               // add #1 to the ui8_write_data_req_by_keyb_state to move to the next substate
		strb r2,[r3]                             // update the ui8_write_data_req_by_keyb_state value in memory with the new susbtate value
		b write_data_read_by_keyboard_end

	.thumb_func 
	write_data_read_by_keyboard_substate_1:

	    movw r4,#:lower16:REG_PORT_OUT1            // get the port to write the data nibble in the data bus
		movt r4,#:upper16:REG_PORT_OUT1
		ldr  r5,[r4]
		movw r6,#:lower16:(~P1_DATA_MASK)          // clear the bits of the port that correspond to the data bits				      
		movt r6,#:upper16:(~P1_DATA_MASK)			
		and  r5,r6                             				
		movw r6,#:lower16:ui8_write_nibble_val     // get the data nibble to write to the bus
		movt r6,#:upper16:ui8_write_nibble_val
		ldrb r7,[r6]
		lsl  r7,#12                                // shift the data nibble value to place the bits in the right PORT0 positions before writting: 0x0000000F > 0x0000F000                 
		orr  r5,r7                                 // set the bits to write				
		str  r5,[r4]                               // write the value into the port to place it into the data bus

		movw r4,#:lower16:ui32_read_nibble_address // increase the address of next data nibble to provide to the keyboard ( it is used as an offset inside the ROM dump content )
		movt r4,#:upper16:ui32_read_nibble_address								
		ldr  r5,[r4]                               // take current data nibble read address and increase it +1  for the next data nibble write cycle
		add  r5,#1                          
		str  r5,[r4]                               // store new address value in the "ui32_read_nibble_address" variable

		add  r2,#1                               // add #1 to the ui8_write_data_req_by_keyb_state to move to the next substate
		strb r2,[r3]                             // update the ui8_write_data_req_by_keyb_state value in memory with the new susbtate value
		b write_data_read_by_keyboard_end

	.thumb_func 
	write_data_read_by_keyboard_substate_2:

		bl prepare_next_data_nibble_to_write       
			
		add  r2,#1                               // add #1 to the ui8_write_data_req_by_keyb_state to move to the next substate
		strb r2,[r3]                             // update the ui8_write_data_req_by_keyb_state value in memory with the new susbtate value
		b write_data_read_by_keyboard_end

	.thumb_func 
	write_data_read_by_keyboard_substate_3:

	    movw r5,#:lower16:ui8_clck_cycle
		movt r5,#:upper16:ui8_clck_cycle    
		ldrb r4,[r5]
		cmp  r4,#1
		bne  write_data_read_by_keyboard_end     // check if the ui8_R_clck_cycle is 1 and in that case restart the state machine
			
		mov  r2,#0                               // reset the substate machine by setting the  0 the r2 and ui8_write_data_req_by_keyb_state values
		strb r2,[r3]                             // update the ui8_write_data_req_by_keyb_state value in memory with the new susbtate value
        
	.thumb_func 
	write_data_read_by_keyboard_end:
			
		pop {pc} // return from call ( place the LR previously stored in the stack in the PC )



//###################################################################################################### 
// DPACK MAIN ROUTINE: ROM PACK cart emulator
//######################################################################################################
// PRE: before calling this function FOR FIRST time in the loop these conditions must be fulfilled:
//     ctr_lines (CLCK1, CLCK2, !CS, R!W ... ) must be configured as input
//     data_lines (D0,D1,D2,D3) must be configured as input
.text
.global dpack_int_main

.thumb_func
dpack_int_main:

    push {lr}
    // ARM ABI: a subroutine must preserve the contents of the registers r4-r8, r10, r11 and SP
	push {r4-r11}

	.thumb_func
    dpack_int_start:             

	// ALL PROCESSING IS DONE INSIDE THE EIC1 INTERRUPT ON CLCK2 STATE CHANGES

	.thumb_func  
    dpack_int_main_end:  

	// ARM ABI: a subroutine must preserve the contents of the registers r4-r8, r10, r11 and SP
	pop {r4-r11}

        pop {pc} // return from call ( place the LR previously stored in the stack in the PC )



//###################################################################################################### 
// DPACK MAIN ROUTINE
// SUBROUTINE USED TO EMULATE/GENERATE CLOCK 1 2 SIGNAL
//             _________     ____________     ________     ____________     ________     ____________     ________     ____________     ________      
//   CLCK1              |___|            |___|        |___|            |___|        |___|            |___|        |___|            |___|        |_...
//             111111111100001111111111111000011111111100001111111111111000011111111100001111111111111000011111111100001111111111111000011111111100   
//   
//             __     ____________     ________     ____________     ________     ____________     ________     ____________     ________     __.... 
//   CLCK2       |___|            |___|        |___|            |___|        |___|            |___|        |___|            |___|        |___|
//             11100001111111111111000011111111100001111111111111000011111111100001111111111111000011111111100001111111111111000011111111100001
//
//   CLCK2       |1  1  0   1     1   1  0   1 |1  1  0   1     1   1  0   1 |1  1  0   1     1   1  0   1 |1  1  0   1     1   1  0   1 |
//   CLCK1       |0  1  1   1     0   1  1   1 |0  1  1   1     0   1  1   1 |0  1  1   1     0   1  1   1 |0  1  1   1     0   1  1   1 |
//  clck12_state |0  1  2   3     4   5  6   7 |0  1  2   3     4   5  6   7 |0  1  2   3     4   5  6   7 |0  1  2   3     4   5  6   7 |
//######################################################################################################
.text
.global dpack_dumper_clck12_state

.thumb_func
dpack_dumper_clck12_state:
		push {lr}

		movw r0,#:lower16:ui8_clck12_state_wait_cycles // check if ui8_clck12_state_wait_cycles has reached 0 
		movt r0,#:upper16:ui8_clck12_state_wait_cycles
        ldrb r1,[r0]
		cmp  r1,#0
		beq dpack_dumper_clck12_update_state        // if ui8_clck12_state_wait_cycles has reached 0 then it is time to update the clck12_state variable
		sub  r1,#1                                    // if ui8_clck12_state_wait_cycles has NOT reached 0 then decrease the counter and wait to next cycle to check again
		strb r1,[r0]
		movw r4,#:lower16:ui8_clck12_state_transition // the counter as been decreased and the state clck12_state is not going to be updated so there will be no transisition CLCK12_STATE_TRANSITION_NONE
		movt r4,#:upper16:ui8_clck12_state_transition
	    mov  r5,#CLCK12_STATE_TRANSITION_NONE
		strb r5,[r4]
		b dpack_dumper_clck12_state_end

	.thumb_func
    dpack_dumper_clck12_update_state:
		movw r0,#:lower16:ui8_clck12_state           
		movt r0,#:upper16:ui8_clck12_state
		ldrb r1,[r0]
		movw r2,#:lower16:ui8_ctrl_bits           
		movt r2,#:upper16:ui8_ctrl_bits
		ldrb r3,[r2]

	//       CLCK2  |1  
    //       CLCK1  |0 
    // clck12_state  0 
	.thumb_func
    dpack_dumper_clck12_state_0:
		  cmp  r1,#0
		  beq  dpack_dumper_clck12_update_0  // check if current clock12 state is 0 and update the bits in the ctrl bits variable if necessary
	      b    dpack_dumper_clck12_state_1
	.thumb_func
    dpack_dumper_clck12_update_0:
		  movw r4,#:lower16:ui8_clck12_state_transition // set the transition variable to the new value to notify the chanfe of the clck12_state
		  movt r4,#:upper16:ui8_clck12_state_transition
	      mov  r5,#CLCK12_STATE_TRANSITION_0_TO_1
		  strb r5,[r4]
		  movw r4,#:lower16:ui8_clck12_state_wait_cycles // update the duration in loop cycles of next clck12 state
		  movt r4,#:upper16:ui8_clck12_state_wait_cycles
	      mov  r5,#CLCK12_WAIT_CYCLES_STATE0
		  strb r5,[r4]
		  orr r3,#VAR_CTRL_nCLCK2_MASK       // set CLCK2
	      and r3,#(~VAR_CTRL_nCLCK1_MASK)    // clear CLCK1
		  strb r3,[r2]                       // store the new state of ui8_ctrl_bits with the state of the signals
		  add r1,#1                          // set clck12 to next state ( 0+1 )
		  strb r1,[r0]                       // store the new ui8_clck12_state value

	      // - Update new ctrl signals state (CLCK1, CLKC2 ...)  into the bus ( according to the state of the values set in ctr variables )
		  bl dpack_dumper_set_ctrl_bits
	      b   dpack_dumper_clck12_state_end

	//       CLCK2  |1   1 
    //       CLCK1  |0   1
    // clck12_state  0   1
	.thumb_func
    dpack_dumper_clck12_state_1:
		  cmp  r1,#1
		  beq  dpack_dumper_clck12_update_1 // check if current clock12 state is 1 and update the bits in the ctrl bits variable if necessary
		  b dpack_dumper_clck12_state_2
	.thumb_func
    dpack_dumper_clck12_update_1:
		  movw r4,#:lower16:ui8_clck12_state_transition // set the transition variable to the new value to notify the chanfe of the clck12_state
		  movt r4,#:upper16:ui8_clck12_state_transition
	      mov  r5,#CLCK12_STATE_TRANSITION_1_TO_2
		  strb r5,[r4]
		  movw r4,#:lower16:ui8_clck12_state_wait_cycles // update the duration in loop cycles of next clck12 state
		  movt r4,#:upper16:ui8_clck12_state_wait_cycles
	      mov  r5,#CLCK12_WAIT_CYCLES_STATE1
		  strb r5,[r4]
          orr r3,#VAR_CTRL_nCLCK2_MASK        // set CLCK2
		  orr r3,#VAR_CTRL_nCLCK1_MASK        // set CLCK1
		  strb r3,[r2]                       // store the new state of 'ui8_ctrl_bits' with the state of the signals
		  add r1,#1                          // set clck12 to next state ( 1+1 )
		  strb r1,[r0]                       // store the new 'ui8_clck12_state' value

	      // - Update new ctrl signals state (CLCK1, CLKC2 ...)  into the bus ( according to the state of the values set in ctr variables )
		  bl dpack_dumper_set_ctrl_bits
		  b   dpack_dumper_clck12_state_end

	//       CLCK2  |1   1     0 
    //       CLCK1  |0   1     1 
    // clck12_state  0   1     2 
	.thumb_func
    dpack_dumper_clck12_state_2:
		 cmp  r1,#2
	 	 beq  dpack_dumper_clck12_update_2 // check if current clock12 state is 2 and update the bits in the ctrl bits variable if necessary
		 b dpack_dumper_ck12_state_3
	.thumb_func
    dpack_dumper_clck12_update_2:
		  movw r4,#:lower16:ui8_clck12_state_transition // set the transition variable to the new value to notify the chanfe of the clck12_state
		  movt r4,#:upper16:ui8_clck12_state_transition
	      mov  r5,#CLCK12_STATE_TRANSITION_2_TO_3
		  strb r5,[r4]
		  movw r4,#:lower16:ui8_clck12_state_wait_cycles // update the duration in loop cycles of next clck12 state
		  movt r4,#:upper16:ui8_clck12_state_wait_cycles
	      mov  r5,#CLCK12_WAIT_CYCLES_STATE2
		  strb r5,[r4]
          and r3,#(~VAR_CTRL_nCLCK2_MASK)    // clear CLCK2
	 	  orr r3,#VAR_CTRL_nCLCK1_MASK       // set CLCK1
		  strb r3,[r2]                       // store the new state of 'ui8_ctrl_bits' with the state of the signals
		  add r1,#1                          // set clck12 to next state ( 2+1 )
		  strb r1,[r0]                       // store the new 'ui8_clck12_state' value

	      // - Update new ctrl signals state (CLCK1, CLKC2 ...)  into the bus ( according to the state of the values set in ctr variables )
		  bl dpack_dumper_set_ctrl_bits
		  b dpack_dumper_clck12_state_end

	//       CLCK2  |1   1     0   1 
    //       CLCK1  |0   1     1   1 
    // clck12_state  0   1     2   3 
	.thumb_func
    dpack_dumper_ck12_state_3:
		cmp  r1,#3
		beq  dpack_dumper_clck12_update_3 // check if current clock12 state is 3 and update the bits in the ctrl bits variable if necessary
		b dpack_dumper_clck12_state_4
	.thumb_func
    dpack_dumper_clck12_update_3:
		  movw r4,#:lower16:ui8_clck12_state_transition // set the transition variable to the new value to notify the chanfe of the clck12_state
		  movt r4,#:upper16:ui8_clck12_state_transition
	      mov  r5,#CLCK12_STATE_TRANSITION_3_TO_4
		  strb r5,[r4]
		  movw r4,#:lower16:ui8_clck12_state_wait_cycles // update the duration in loop cycles of next clck12 state
		  movt r4,#:upper16:ui8_clck12_state_wait_cycles
		  mov  r5,#CLCK12_WAIT_CYCLES_STATE3
		  strb r5,[r4]
		  orr r3,#VAR_CTRL_nCLCK2_MASK       // set CLCK2
		  orr r3,#VAR_CTRL_nCLCK1_MASK       // set CLCK1
		  strb r3,[r2]                       // store the new state of ui8_ctrl_bits with the state of the signals
		  add r1,#1                          // set clck12 to next state ( 3+1 )
		  strb r1,[r0]                       // store the new ui8_clck12_state value

	      // - Update new ctrl signals state (CLCK1, CLKC2 ...)  into the bus ( according to the state of the values set in ctr variables )
		  bl dpack_dumper_set_ctrl_bits
		  b dpack_dumper_clck12_state_end

	//       CLCK2  |1   1     0   1  1  
    //       CLCK1  |0   1     1   1  0  
    // clck12_state  0   1     2   3  4  
	.thumb_func
    dpack_dumper_clck12_state_4:
			cmp  r1,#4
			beq  dpack_dumper_clck12_update_4 // check if current clock12 state is 4 and update the bits in the ctrl bits variable if necessary
			b dpack_dumper_clck12_state_5
	.thumb_func
    dpack_dumper_clck12_update_4:
			movw r4,#:lower16:ui8_clck12_state_transition // set the transition variable to the new value to notify the chanfe of the clck12_state
			movt r4,#:upper16:ui8_clck12_state_transition
			mov  r5,#CLCK12_STATE_TRANSITION_4_TO_5
			strb r5,[r4]
			movw r4,#:lower16:ui8_clck12_state_wait_cycles // update the duration in loop cycles of next clck12 state
			movt r4,#:upper16:ui8_clck12_state_wait_cycles
			mov  r5,#CLCK12_WAIT_CYCLES_STATE4
			strb r5,[r4]
			orr r3,#VAR_CTRL_nCLCK2_MASK       // set CLCK2
			and r3,#(~VAR_CTRL_nCLCK1_MASK)    // clear CLCK1
		    strb r3,[r2]                       // store the new state of ui8_ctrl_bits with the state of the signals
		    add r1,#1                          // set clck12 to next state ( 4+1 )
		    strb r1,[r0]                       // store the new ui8_clck12_state value

	        // - Update new ctrl signals state (CLCK1, CLKC2 ...)  into the bus ( according to the state of the values set in ctr variables )
 		    bl dpack_dumper_set_ctrl_bits
			b dpack_dumper_clck12_state_end

	//       CLCK2  |1   1     0   1  1   1 
    //       CLCK1  |0   1     1   1  0   1 
    // clck12_state  0   1     2   3  4   5 
	.thumb_func
    dpack_dumper_clck12_state_5:
			cmp  r1,#5
			beq  dpack_dumper_clck12_update_5 // check if current clock12 state is 5 and update the bits in the ctrl bits variable if necessary
			b dpack_dumper_clck12_state_6
	.thumb_func
    dpack_dumper_clck12_update_5:
			movw r4,#:lower16:ui8_clck12_state_transition // set the transition variable to the new value to notify the chanfe of the clck12_state
			movt r4,#:upper16:ui8_clck12_state_transition
			mov  r5,#CLCK12_STATE_TRANSITION_5_TO_6
			strb r5,[r4]
			movw r4,#:lower16:ui8_clck12_state_wait_cycles // update the duration in loop cycles of next clck12 state
			movt r4,#:upper16:ui8_clck12_state_wait_cycles
			mov  r5,#CLCK12_WAIT_CYCLES_STATE5
			strb r5,[r4]
			orr  r3,#VAR_CTRL_nCLCK2_MASK       // set CLCK2
			orr  r3,#VAR_CTRL_nCLCK1_MASK       // set CLCK1
		    strb r3,[r2]                        // store the new state of ui8_ctrl_bits with the state of the signals
		    add  r1,#1                          // set clck12 to next state ( 5+1 )
		    strb r1,[r0]                        // store the new ui8_clck12_state value

	        // - Update new ctrl signals state (CLCK1, CLKC2 ...)  into the bus ( according to the state of the values set in ctr variables )
 		    bl dpack_dumper_set_ctrl_bits
			b dpack_dumper_clck12_state_end

	//       CLCK2  |1   1     0   1  1   1 0 
    //       CLCK1  |0   1     1   1  0   1 1 
    // clck12_state  0   1     2   3  4   5 6 
	.thumb_func
    dpack_dumper_clck12_state_6:
			cmp  r1,#6
			beq  dpack_dumper_clck12_update_6 // check if current clock12 state is 6 and update the bits in the ctrl bits variable if necessary
			b dpack_dumper_clck12_state_7
	.thumb_func
    dpack_dumper_clck12_update_6:
			movw r4,#:lower16:ui8_clck12_state_transition // set the transition variable to the new value to notify the chanfe of the clck12_state
			movt r4,#:upper16:ui8_clck12_state_transition
			mov  r5,#CLCK12_STATE_TRANSITION_6_TO_7
			strb r5,[r4]
			movw r4,#:lower16:ui8_clck12_state_wait_cycles // update the duration in loop cycles of next clck12 state
			movt r4,#:upper16:ui8_clck12_state_wait_cycles
			mov  r5,#CLCK12_WAIT_CYCLES_STATE6
			strb r5,[r4]
			and r3,#(~VAR_CTRL_nCLCK2_MASK)     // clear CLCK2
			orr r3,#VAR_CTRL_nCLCK1_MASK        // set CLCK1
		    strb r3,[r2]                        // store the new state of ui8_ctrl_bits with the state of the signals
		    add  r1,#1                          // set clck12 to next state ( 6+1 )
		    strb r1,[r0]                        // store the new ui8_clck12_state value

	        // - Update new ctrl signals state (CLCK1, CLKC2 ...)  into the bus ( according to the state of the values set in ctr variables )
 		    bl dpack_dumper_set_ctrl_bits
			b dpack_dumper_clck12_state_end

	//       CLCK2  |1   1     0   1  1   1 0   1
    //       CLCK1  |0   1     1   1  0   1 1   1
    // clck12_state  0   1     2   3  4   5 6   7
	.thumb_func
    dpack_dumper_clck12_state_7:
			// cmp  r1,#7
			// beq  dpack_dumper_clck12_update_7 // check if current clock12 state is 7 and update the bits in the ctrl bits variable if necessary
			// b dpack_dumper_clck12_state_end
	.thumb_func
    dpack_dumper_clck12_update_7:
			movw r4,#:lower16:ui8_clck12_state_transition // set the transition variable to the new value to notify the chanfe of the clck12_state
			movt r4,#:upper16:ui8_clck12_state_transition
			mov  r5,#CLCK12_STATE_TRANSITION_7_TO_0
			strb r5,[r4]
			movw r4,#:lower16:ui8_clck12_state_wait_cycles // update the duration in loop cycles of next clck12 state
			movt r4,#:upper16:ui8_clck12_state_wait_cycles
			mov  r5,#CLCK12_WAIT_CYCLES_STATE7
			strb r5,[r4]
			orr r3,#VAR_CTRL_nCLCK2_MASK        // set CLCK2
			orr r3,#VAR_CTRL_nCLCK1_MASK        // set CLCK1
		    strb r3,[r2]                        // store the new state of ui8_ctrl_bits with the state of the signals
		    mov  r1,#0                          // set clck12 state back to the begining 0
		    strb r1,[r0]                        // store the new ui8_clck12_state value

	        // - Update new ctrl signals state (CLCK1, CLKC2 ...)  into the bus ( according to the state of the values set in ctr variables )
 		    bl dpack_dumper_set_ctrl_bits
			// b dpack_dumper_clck12_state_end

	.thumb_func
    dpack_dumper_clck12_state_end:

    pop {pc} // return from call ( place the LR previously stored in the stack in the PC )


//######################################################################################################
// SUBROUTINE THAT TAKES THE STATE OF THE CTRL SIGNALS FROM THE ui8_ctrl_bits VARIRABLE, AND UPDATES THE
// STATE OF THE I/O LINES ACCORDING TO THAT VALUES
//######################################################################################################
.text
.global dpack_dumper_set_ctrl_bits

.thumb_func
dpack_dumper_set_ctrl_bits:
    push {lr}

	// set into the bus the new state of the ctrl bits ( UNKNWN_CTRL, !CS, R!W, CLCK1, CLCK2 ... )  from the CTRL BITS VARIABLE so first
	// thing to do is take current state of the bits in the variable (variable ctrl bits: 7:x 6:x 5:x 4:'?' 3:CLCK2 2:CLCK1 1:R/!W 0:!CS )
	movw r0,#:lower16:ui8_ctrl_bits           
	movt r0,#:upper16:ui8_ctrl_bits
	ldrb r1,[r0] 

	// set P0 ctrl bits:
	movw r2,#:lower16:REG_PORT_OUT0             
	movt r2,#:upper16:REG_PORT_OUT0 
	ldr  r3,[r2]                                // set in r2 current REG_PORT_OUT0 register value

	// set into the bus the value of the UNKNWN_CTRL bit
	movw r4,#:lower16:(~P0_UNKNWN_CTRL_BIT_MASK)// set in r3 P0_UNKNWN_CTRL_BIT_MASK inverted 
	movt r4,#:upper16:(~P0_UNKNWN_CTRL_BIT_MASK)
	and  r3,r4                                  // clear the state of the bit before updating it with the OR with its new value
	mov  r5,r1                                  // copy in r4 the value of  ctrl bits variable
	and  r5,#VAR_CTRL_nUNKNOWN_MASK             // apply the mask over a copy of the ctrl bits variable to keep only the UNKNWN_CTRL bit state
	lsr  r5,#2                                  // move the bit state from the position in the ctrl bits variable to its position in the port
	orr  r3,r5                                  // OR the variable bit value in its position in the port
		  
	str  r3,[r2]                                // update the port 0 value

	// set P1 ctrl bits:		  
	movw r2,#:lower16:REG_PORT_OUT1
	movt r2,#:upper16:REG_PORT_OUT1
	ldr  r3,[r2]                                // set in r2 current REG_PORT_OUT1 register value

	// set into the bus the value of the CLCK1 and CLCK2 bits
	movw r4,#:lower16:(~(P1_nCLCK1_BIT_MASK|P1_nCLCK2_BIT_MASK)) // set in r3 P1_nCLCK1_BIT_MASK and P1_nCLCK2_BIT_MASK inverted
	movt r4,#:upper16:(~(P1_nCLCK1_BIT_MASK|P1_nCLCK2_BIT_MASK))
	and  r3,r4                                  // clear the state of the bits before updating them with the OR with their new value
	mov  r5,r1                                  // copy in r4 the value of  ctrl bits variable
	and  r5,#(VAR_CTRL_nCLCK1_MASK|VAR_CTRL_nCLCK2_MASK)// apply the mask over a copy of the ctrl bits variable to keep only the CLCK1 and CLCK2 bits state
	lsl  r5,#14                                 // move the bit state from the position in the ctrl bits variable to its position in the port
	orr  r3,r5                                  // OR the variable bit value in its position in the port

	// set into the bus the value of the nCS and RnW
	movw r4,#:lower16:(~(P1_nCS_BIT_MASK|P1_RnW_BIT_MASK))// set in r3 P1_nCS_BIT_MASK and iP1_RnW_BIT_MASK nverted 
	movt r4,#:upper16:(~(P1_nCS_BIT_MASK|P1_RnW_BIT_MASK)) 
	and  r3,r4                                     // clear the state of the bits before updating them with the OR with their new value
	mov  r5,r1                                     // copy in r4 the value of  ctrl bits variable
	and  r5,#(VAR_CTRL_nCS_MASK|VAR_CTRL_RnW_MASK) // apply the mask over a copy of the ctrl bits variable to keep only the nCS and RnW bits state
	lsl  r5,#10                                    // move the bit state from the position in the ctrl bits variable to its position in the port
	orr  r3,r5                                     // OR the variable bit value in its position in the port
		  
	str  r3,[r2]                                  // update the port 1 value

	.thumb_func
    dpack_dumper_set_ctrl_bits_end:

    pop {pc} // return from call ( place the LR previously stored in the stack in the PC )

//######################################################################################################
// SUBROUTINE THAT TAKES LAST READ NIBBLE VALUE FROM ui8_dpack_dumper_last_data_nibble_read and STORES
// IT IN THE 4 LOWEST BITS OR IN THE 4 HIGHEST BITS OF THE BYTE IN THE BUFFER AND MOVES THE POINTER TO
// THE NEXT BUFFER POSITION WHEN THE BYTE HAS BEEN FILLED
//######################################################################################################
.text
.global dpack_dumper_store_in_buffer

.thumb_func
dpack_dumper_store_in_buffer:
    push {lr}


	movw r0,#:lower16:ui8_dpack_dumper_last_data_nibble_read  // get the last read nibble value ( 4 lowest bits )
	movt r0,#:upper16:ui8_dpack_dumper_last_data_nibble_read
	ldrb r1,[r0]

	movw r0,#:lower16:pui8_nibbles_buffer        // get the address of the next position to update in the buffer
	movt r0,#:upper16:pui8_nibbles_buffer
	ldr  r5,[r0]

	movw r0,#:lower16:ui8_dpack_dumper_data_nibbles_read      // get the current number of nibbles read from the current data block
	movt r0,#:upper16:ui8_dpack_dumper_data_nibbles_read
	ldrb r3,[r0]
	and  r0,r3,#0x00000001                                      // check if the count of read nibbles is Odd or Even. Each byte stores 2 nibbles, so if read address is even nibble is stored in the 4 lowest bits of the byte 
	cmp  r0,#0x00000001                                         // but if the the read address is odd then nibble is stored in the highest 4 bits of the byte, and the pointer to the buffer is increased to next position

	bne  dpack_dumper_store_in_buffer_is_even
	
	.thumb_func
	dpack_dumper_store_in_buffer_is_odd:

		// address ui8_dpack_dumper_data_nibbles_read is odd
		ldrb r2,[r5]                                                // take the value of the buffer byte to which the variable 'pui8_nibbles_buffer' is pointing to 		
		and  r3,r2,#0xFFFFFF0F                                      // and update the 4 highest bits of the buffer position with the last read nibble value
		lsl  r1,#4                                                  // shift the value 4 bits to the left to place the nibble in the 4 highest bits of the byte
		and  r1,#0x000000F0                                         // clear all other bits that are not 4 highest bits of the byte before 'oring'
		orr  r3,r1			
		strb r3,[r5]                                                // after updating the 4 highest bits of the byte with last read nibble value, update the new value in the buffer
		b dpack_dumper_store_in_buffer_end
	
	.thumb_func
	dpack_dumper_store_in_buffer_is_even:

	    // address ui8_dpack_dumper_data_nibbles_read is even
		ldrb r2,[r5]                                                // take the byte to which the variable 'pui8_nibbles_buffer' is pointing to
		and  r3,r2,#0xFFFFFFF0                                      // and update the 4 lowest bits of the buffer position with the last read nibble value
		and  r1,#0x000000F                                          // clear all other bits that are not 4 lowest bits of the byte before 'oring'
		orr  r3,r1			
		strb r3,[r5]                                                // after updating the 4 lowest bits of the byte with last read nibble value, update the new value in the buffer	   strb r3,[r5]                                                // after updating the 4 highest bits of the byte with last read nibble value, then update the new value in the buffer and increase the pointer

		add r5,#1                                                   // after processing every odd byte then set the buffer pointer to next position
		movw r0,#:lower16:pui8_nibbles_buffer         
		movt r0,#:upper16:pui8_nibbles_buffer
		str  r5,[r0]

	.thumb_func
	dpack_dumper_store_in_buffer_end:

    pop {pc} // return from call ( place the LR previously stored in the stack in the PC )

//######################################################################################################
// SUBROUTINE THAT TURNS ON OR OFF THE RED LED ( WRITE ) ACCORDING TO THE STATE OF THE SPECIFIED
// BIT OF THE CURRENTLY DUMPED ADDRESS OF THE ROM PACK. IF THE SPECIFIED ADDRESS BIT STATE IS '1' THEN
// THE LED WILL BE TURNED ON , AND IF THE STATE IS '0' IT WILL BE TURNED OFF.
//######################################################################################################
.text
.global dpack_dumper_blink_write_led

.thumb_func
dpack_dumper_blink_write_led:
    push {lr}

	movw r2,#:lower16:ui32_dpack_dumper_current_address
	movt r2,#:upper16:ui32_dpack_dumper_current_address
	ldr  r3,[r2]
	movw r2,#:lower16:0x00000800                          // the WRITE LED (RED) state is inverted in the IDLE_CFG STATE according to the state of the specified address bit the
	movt r2,#:upper16:0x00000800                          // highest the specified address bit is the slower it will blink, an the lowest the bit is the faster it will blink
	and  r3,r2										    
	cmp  r3,#0x00000000                                   // cmp uses <flexible operand2>, and allowed imediate values for that kind of operands are #0xXY, #0x00XY00XY , #0xXY00XY00 o #0xXYXYXYXY
	bne dpack_dumper_blink_write_led_end     		    
													    
	movw r2,#:lower16:REG_PORT_OUT0                       // the WRITE LED (RED) state is inverted every time state machine enters into IDLE_CFG STATE 
	movt r2,#:upper16:REG_PORT_OUT0					    
	ldr  r3,[r2]									    
	movw r4,#:lower16:(P0_ROM_CON_GPIO02_MASK)		    
	movt r4,#:upper16:(P0_ROM_CON_GPIO02_MASK)		    
	eor  r3,r4                                            // apply an XOR '1' over the P0_ROM_CON_GPIO02_MASK to blink READ LED
	str  r3,[r2]									    
	
	.thumb_func
    dpack_dumper_blink_write_led_end:

    pop {pc} // return from call ( place the LR previously stored in the stack in the PC )
	
//######################################################################################################
// SUBROUTINE THAT SWAPS THE STATE ( FROM ON TO OFF or FROM OFF TO ON ) OF THE GREEN LED ( READ ). 
// SO EACH TIME THIS FUNCTION IS CALLED THE GREEN ( READ ) LED INVERTS ITS STATE
//######################################################################################################
.text
.global dpack_blink_read_led

.thumb_func
dpack_blink_read_led:
    push {lr}

	movw r2,#:lower16:REG_PORT_OUT0
	movt r2,#:upper16:REG_PORT_OUT0
	ldr  r3,[r2]
  	movw r4,#:lower16:(P0_ROM_CON_GPIO01_MASK)
	movt r4,#:upper16:(P0_ROM_CON_GPIO01_MASK)
	eor  r3,r4                                             // apply an XOR '1' over the P0_ROM_CON_GPIO02_MASK to blink READ LED
	str r3,[r2]
	
	.thumb_func
    dpack_blink_read_led_end:

    pop {pc} // return from call ( place the LR previously stored in the stack in the PC )



//###################################################################################################### 
// DPACK DUMPER MAIN ROUTINE: ROM PACK cart content dumper 
//######################################################################################################
.text
.global dpack_dumper_main

.thumb_func
dpack_dumper_main:
    push {lr}
	// ARM ABI: a subroutine must preserve the contents of the registers r4-r8, r10, r11 and SP
	push {r4-r11}

	// init variables

	movw r1,#:lower16:ui8_ctrl_bits  // ################################################################
	movt r1,#:upper16:ui8_ctrl_bits  // ctrl bits: 7:x 6:x 5:x 4:'?' 3:CLCK2 2:CLCK1 1:R/!W 0:!CS )
	mov  r0,#0
	strb r0,[r1] 

	 // init 'ui8_clck12_state' variable to 0
    movw r1,#:lower16:ui8_clck12_state                      // get 2 lower bytes of the variable address                    
	movt r1,#:upper16:ui8_clck12_state					    // get 2 hihgher bytes of the variable address
	mov  r0,#0											    
	strb r0,[r1]                                            // update the content of the variable address

	// init 'ui8_clck12_state_transition' variable to CLCK12_STATE_TRANSITION_NONE     
    movw r1,#:lower16:ui8_clck12_state_transition           // get 2 lower bytes of the variable address
	movt r1,#:upper16:ui8_clck12_state_transition           // get 2 hihgher bytes of the variable address
	mov  r0,#CLCK12_STATE_TRANSITION_NONE                    
	strb r0,[r1]                                            // update the content of the variable address

	// init 'ui8_dpack_dumper_data_nibbles_read' to DPACK_DUMPER_READ_DATA_NIBBLES_MAX to force 
	// transition from IDLE_WAIT to WRITE_ADD and start writting the address nibles of the next bytes to read
	// DPACK_DUMPER_READ_DATA_NIBBLES_MAX should correspond to the maximum number of nibbles that can be read
	// in a read sequence after having written the address
	movw r1,#:lower16:ui8_dpack_dumper_data_nibbles_read 
	movt r1,#:upper16:ui8_dpack_dumper_data_nibbles_read        
    movw r0,#:lower16:DPACK_DUMPER_READ_DATA_NIBBLES_MAX 
	movt r0,#:upper16:DPACK_DUMPER_READ_DATA_NIBBLES_MAX 
	str  r0,[r1]

	// init ui8_dpack_dumper_last_data_nibble_read variable to 0  
    movw r1,#:lower16:ui8_dpack_dumper_last_data_nibble_read    
	movt r1,#:upper16:ui8_dpack_dumper_last_data_nibble_read
	mov  r0,#0
	strb r0,[r1]
	 
	// init 'ui32_dpack_dumper_total_data_nibbles_read' variable to 0
    movw r1,#:lower16:ui32_dpack_dumper_total_data_nibbles_read 
	movt r1,#:upper16:ui32_dpack_dumper_total_data_nibbles_read
	mov  r0,#0
	strb r0,[r1]

	// init 'ui8_dpack_dumper_idle_clck_wait_cycles' variable to 0  
    movw r1,#:lower16:ui8_dpack_dumper_idle_clck_wait_cycles
	movt r1,#:upper16:ui8_dpack_dumper_idle_clck_wait_cycles
	mov  r0,#0
	strb r0,[r1]

	// set the pointer to the memory store buffer pointing to the beggining of the memory store
    movw r1,#:lower16:pui8_nibbles_buffer
	movt r1,#:upper16:pui8_nibbles_buffer
	movw r0,#:lower16:ui8_nibbles_buffer
	movt r0,#:upper16:ui8_nibbles_buffer
	str  r0,[r1]

	// init ui8_dpack_dumper_state variable to 0 start in IDLE_CFG state
    movw r1,#:lower16:ui8_dpack_dumper_state
	movt r1,#:upper16:ui8_dpack_dumper_state
	mov  r0,#DPACK_DUMPER_STATE_IDLE_CFG
	strb r0,[r1]

	.thumb_func      // ################################################################################
    dpack_dumper_main_start:

		  bl dpack_dumper_clck12_state // call the routine that updates the 'clck12_state' variable and the clck12 ctrl bits according to the new state

	.thumb_func      // #### DPACK DUMPER to MEMORY STATE MACHINE ##################################################################
	dpack_dumper_idle_cfg_state_check:

			// get current state of the DPACK DUMPER STATE MACHINE
			movw r2,#:lower16:ui8_dpack_dumper_state    
			movt r2,#:upper16:ui8_dpack_dumper_state
			ldrb r3,[r2] 

			// jump to the corresponding state
			cmp  r3,#DPACK_DUMPER_STATE_IDLE_CFG
			beq dpack_dumper_idle_cfg_state
			cmp  r3,#DPACK_DUMPER_STATE_IDLE_WAIT
			beq dpack_dumper_idle_wait_state
			cmp  r3,#DPACK_DUMPER_STATE_IDLE_CNT_CLCK
			beq dpack_dumper_idle_cnt_clck_state
			cmp  r3,#DPACK_DUMPER_STATE_WRITE_ADD_CFG
			beq dpack_dumper_write_addr_cfg_state
			cmp  r3,#DPACK_DUMPER_STATE_WRITE_ADD_WAIT
			beq dpack_dumper_write_addr_wait_state
			cmp  r3,#DPACK_DUMPER_STATE_WRITE_ADD
			beq dpack_dumper_write_addr_state
			cmp  r3,#DPACK_DUMPER_STATE_READ_DATA_CFG
			beq dpack_dumper_read_data_cfg_state
			cmp  r3,#DPACK_DUMPER_STATE_READ_DATA_WAIT
			beq dpack_dumper_read_data_wait_state
			cmp  r3,#DPACK_DUMPER_STATE_READ_DATA
			beq dpack_dumper_read_data_state
			cmp  r3,#DPACK_DUMPER_STATE_READ_DATA_STORE
			beq dpack_dumper_read_data_store_state
			cmp  r3,#DPACK_DUMPER_STATE_RESET_CURR_ADDRESS
			beq dpack_dumper_reset_current_address_state

			.thumb_func      // #### IDLE_CFG STATE ##################################################################
			dpack_dumper_idle_cfg_state:
			    // STATE DESCRIPTION: executes the entry/initialization actions of the IDLE WAIT states. In this state system keeps CS==1 
				 
			    // ACTIONS:
				// - Set DATA lines in input/high impedance mode:
				bl config_ctrl_lines_as_output   
				bl config_data_lines_as_output 		// JBR 2021-04-21 ¿ Para que sea high impedance no deberia ser as input ?
                // - Set the CS=1 and R!W=1: CTRL bits VARIABLE MASKS: 7:x 6:x 5:x 4:'?' 3:CLCK2 2:CLCK1 1:R/!W 0:!CS
				movw r0,#:lower16:ui8_ctrl_bits           
				movt r0,#:upper16:ui8_ctrl_bits
				ldrb r1,[r0] 				
				mov  r4,#(VAR_CTRL_nCS_MASK|VAR_CTRL_RnW_MASK)
				orr  r1,r4				
				strb r1,[r0]
				// - Update new ctrl signals state (CS, R/!W ...)  into the bus ( according to the state of the values set in ctr variables )
				bl dpack_dumper_set_ctrl_bits
				// - Reset the counter used to temporize IDLE state:
				movw r4,#:lower16:ui8_dpack_dumper_idle_clck_wait_cycles
				movt r4,#:upper16:ui8_dpack_dumper_idle_clck_wait_cycles
				mov  r5,#0
				strb r5,[r4]
				// - Invert the state of the  WRITE LED - RED LED according to currently dumped address
				bl dpack_dumper_blink_write_led
				
				// TRANSITIONS:
				// - IDLE_CFG -> IDLE_WAIT: direct transition:
				movw r2,#:lower16:ui8_dpack_dumper_state    
				movt r2,#:upper16:ui8_dpack_dumper_state
				mov r3,#DPACK_DUMPER_STATE_IDLE_WAIT
				strb r3,[r2]
				b dpack_dumper_main_end

			.thumb_func      // #### IDLE_WAIT STATE ##################################################################
			dpack_dumper_idle_wait_state:
				// STATE DESCRIPTION: stays in the IDLE WAIT state on which the CS==1

			    // ACTIONS:
				// - No actions, only wait for corresponding state transistions

				// TRANSITIONS:
				// TR 1 : IDLE_WAIT - > READ_DATA_CFG:  check if ( i8_dpack_dumper_clck_wait_cycles == DPACK_DUMPER_CS_WAIT_CLOCK_CYCLES ) &&  ( ui8_clck12_state_transition ==  ) && ( ui8_dpack_dumper_addres_nibbles_written == DPACK_DUMPER_WRITE_ADDR_NIBBLES_MAX ) jump to READ_DATA_CFG.
				//                                      idle time pause has passed, clock12 cycle state to check following state has also been reached all the address nibbles have been written into the bus in the WRITE_ADD_x states
				.thumb_func
				dpack_dumper_idle_wait_state_Tr_1:
					movw r4,#:lower16:ui8_dpack_dumper_idle_clck_wait_cycles // check if ui8_dpack_dumper_idle_clck_wait_cycles == DPACK_DUMPER_CS_WAIT_CLOCK_CYCLES
					movt r4,#:upper16:ui8_dpack_dumper_idle_clck_wait_cycles
					ldrb r5,[r4]
					cmp  r5,#DPACK_DUMPER_CS_WAIT_CLOCK_CYCLES // to control the duration of the IDLE periods where CS==1
					bne dpack_dumper_idle_wait_state_Tr_2      // this transition is not satisfied, jump to check next transition
					movw r4,#:lower16:ui8_clck12_state_transition
					movt r4,#:upper16:ui8_clck12_state_transition
					ldrb r5,[r4]
 					cmp  r5,#CLCK12_STATE_TRANSITION_6_TO_7     // check if 'ui8_clck12_state_transition' == 
					bne dpack_dumper_idle_wait_state_Tr_2     // this transition is not satisfied, jump to check next transition
					movw r4,#:lower16:ui8_dpack_dumper_addres_nibbles_written 
					movt r4,#:upper16:ui8_dpack_dumper_addres_nibbles_written
					ldrb r5,[r4]
					cmp r5,#DPACK_DUMPER_WRITE_ADDR_NIBBLES_MAX // check if ui8_dpack_dumper_data_nibbles_read == DPACK_DUMPER_READ_DATA_NIBBLES_MAX
					bne dpack_dumper_idle_wait_state_Tr_2       // this transition is not satisfied, jump to check next transition
					movw r2,#:lower16:ui8_dpack_dumper_state    
					movt r2,#:upper16:ui8_dpack_dumper_state				
					mov  r3,#DPACK_DUMPER_STATE_READ_DATA_CFG    // update state to DPACK_DUMPER_STATE_READ_DATA_CFG
					strb r3,[r2]
					b dpack_dumper_main_end

				// TR 2 : IDLE_WAIT - > WRITE_ADDR_CFG:  check if ( i8_dpack_dumper_clck_wait_cycles == DPACK_DUMPER_CS_WAIT_CLOCK_CYCLES ) &&  ( ui8_clck12_state_transition ==  ) && ( ui8_dpack_dumper_data_nibbles_read == DPACK_DUMPER_READ_DATA_NIBBLES_MAX ) then jump to WRITE_ADDR_CFG
				dpack_dumper_idle_wait_state_Tr_2:
					movw r4,#:lower16:ui8_dpack_dumper_idle_clck_wait_cycles // check if ui8_dpack_dumper_idle_clck_wait_cycles == DPACK_DUMPER_CS_WAIT_CLOCK_CYCLES
					movt r4,#:upper16:ui8_dpack_dumper_idle_clck_wait_cycles
					ldrb r5,[r4]
					cmp  r5,#DPACK_DUMPER_CS_WAIT_CLOCK_CYCLES    // to control the duration of the IDLE periods where CS==1
					bne dpack_dumper_idle_wait_state_Tr_3         // this transition is not satisfied, jump to check next
					movw r4,#:lower16:ui8_dpack_dumper_data_nibbles_read
					movt r4,#:upper16:ui8_dpack_dumper_data_nibbles_read
					ldrb r5,[r4]
					cmp r5,#DPACK_DUMPER_READ_DATA_NIBBLES_MAX    // this transition is not satisfied, jump to check next
					bne dpack_dumper_idle_wait_state_Tr_3       
					movw r4,#:lower16:ui8_clck12_state_transition
					movt r4,#:upper16:ui8_clck12_state_transition
					ldrb r5,[r4]
					cmp  r5,#CLCK12_STATE_TRANSITION_0_TO_1
					bne dpack_dumper_idle_wait_state_Tr_3         // this transition is not satisfied, jump to check next
					movw r2,#:lower16:ui8_dpack_dumper_state    
					movt r2,#:upper16:ui8_dpack_dumper_state			
					mov  r3,#DPACK_DUMPER_STATE_WRITE_ADD_CFG     // update state to DPACK_DUMPER_STATE_READ_DATA_CFG
					strb r3,[r2]
					b dpack_dumper_main_end

				// TR 3 : IDLE_WAIT - > IDLE_CNT_CLCK: when 'ui8_clck12_state_transition' is 6_7 the whole clock cycle has been executed so jump to IDE_CNT_CLCK to count completed clck12 cycles
				.thumb_func
				dpack_dumper_idle_wait_state_Tr_3:
					movw r4,#:lower16:ui8_dpack_dumper_idle_clck_wait_cycles // check if (ui8_dpack_dumper_idle_clck_wait_cycles != DPACK_DUMPER_CS_WAIT_CLOCK_CYCLES) && ( ui8_clck12_state_transition ==  )
					movt r4,#:upper16:ui8_dpack_dumper_idle_clck_wait_cycles
					ldrb r5,[r4]
					cmp  r5,#DPACK_DUMPER_CS_WAIT_CLOCK_CYCLES // to control the duration of the IDLE periods where CS==1
					bge dpack_dumper_main_end                  // this transition is not satisfied
					movw r4,#:lower16:ui8_clck12_state_transition
					movt r4,#:upper16:ui8_clck12_state_transition
					ldrb r5,[r4]
					cmp r5,#CLCK12_STATE_TRANSITION_6_TO_7
					bne dpack_dumper_main_end
					movw r2,#:lower16:ui8_dpack_dumper_state    
					movt r2,#:upper16:ui8_dpack_dumper_state
					mov  r3,#DPACK_DUMPER_STATE_IDLE_CNT_CLCK
					strb r3,[r2]
					b dpack_dumper_main_end

			.thumb_func      // #### IDLE_CNT_CLCK ##################################################################
			dpack_dumper_idle_cnt_clck_state:
				// STATE DESCRIPTION: simply increases the counter of completed clck12 cycles and then goes back to IDLE_WAIT

			    // ACTIONS:
			    // - Increase the clock cycles in the IDLE state
				movw r4,#:lower16:ui8_dpack_dumper_idle_clck_wait_cycles
				movt r4,#:upper16:ui8_dpack_dumper_idle_clck_wait_cycles
				ldrb r5,[r4]
				add  r5,#1
				strb r5,[r4]

				// TRANSITIONS
				// IDLE_CNT_CLCK -> IDLE_WAIT: direct transition
				.thumb_func 
				dpack_dumper_idle_cnt_clck_state_tr1:
					movw r2,#:lower16:ui8_dpack_dumper_state    
					movt r2,#:upper16:ui8_dpack_dumper_state
					mov  r3,#DPACK_DUMPER_STATE_IDLE_WAIT
					strb r3,[r2]
					b dpack_dumper_main_end

			.thumb_func      // #### WRITE_ADD_CFG ##################################################################
			dpack_dumper_write_addr_cfg_state:
				// STATE DESCRIPTION: executes the entry/initialization actions of the WRITE_ADD states

			    // ACTIONS:
				// - Set DATA lines in output mode to write address nibbles
				bl config_ctrl_lines_as_output
				bl config_data_lines_as_output			
                // - Clear the CS=0 and R!W=0: CTRL bits VARIABLE MASKS: 7:x 6:x 5:x 4:'?' 3:CLCK2 2:CLCK1 1:R/!W 0:!CS
				movw r0,#:lower16:ui8_ctrl_bits           
				movt r0,#:upper16:ui8_ctrl_bits
				ldrb r1,[r0] 
				mov  r4,#~(VAR_CTRL_nCS_MASK|VAR_CTRL_RnW_MASK)
				and  r1,r4				
				strb r1,[r0]
				// - Update new ctrl signals state (CS, R/!W ...)  into the bus ( according to the state of the values set in ctr variables )
				bl dpack_dumper_set_ctrl_bits
				// - Reset the the counter of number of read data nibbles block in the last READ_DATA cycles
				movw r4,#:lower16:ui8_dpack_dumper_data_nibbles_read
				movt r4,#:upper16:ui8_dpack_dumper_data_nibbles_read
				mov  r5,#0
				strb r5,[r4]				
				// - Reset the counter used to temporize IDLE state:
				movw r4,#:lower16:ui8_dpack_dumper_idle_clck_wait_cycles
				movt r4,#:upper16:ui8_dpack_dumper_idle_clck_wait_cycles
				mov  r5,#0
				strb r5,[r4]
				// - Prepare the value of the address to write into the DATA bus
				movw r4,#:lower16:ui32_dpack_dumper_current_address
				movt r4,#:upper16:ui32_dpack_dumper_current_address
				ldr  r5,[r4]
				movw r6,#:lower16:0x000C0000 // include the 0xC in the 5th position ( all address written in the bus have 0xC as last nibble, don't know why )
				movt r6,#:upper16:0x000C0000
				orr  r5,r6
				movw r4,#:lower16:ui32_dpack_dumper_address_to_write
				movt r4,#:upper16:ui32_dpack_dumper_address_to_write
				str  r5,[r4]

				// TRANSITIONS
				// - WRITE ADD CFG -> WRITE_ADD_WAIT: direct transition:
				.thumb_func 
				dpack_dumper_write_addr_cfg_state_tr1:
					movw r2,#:lower16:ui8_dpack_dumper_state    
					movt r2,#:upper16:ui8_dpack_dumper_state
					mov  r3,DPACK_DUMPER_STATE_WRITE_ADD_WAIT 
					strb r3,[r2]
					b dpack_dumper_main_end

			.thumb_func      // #### WRITE_ADD_WAIT ##################################################################
			dpack_dumper_write_addr_wait_state:
			    // STATE DESCRIPTION: state to wait before writing next address nibble in the bus, and if all 5 address nibbles 
				//                    have been written then go back to read the data corresponding to written address

				// ACTIONS:
				// No actions, only wait for transistions
				
				// TRANSITIONS:
				// TR 1 : WRITE_ADD_WAIT - > IDLE_CFG: // check if (ui8_dpack_dumper_addres_nibbles_written == DPACK_DUMPER_WRITE_ADDR_NIBBLES_MAX) && ( ui8_clck12_state_transition ==  ) 
				//                                        clock12 cycle state to check following state has been reached and all the address nibbles have been written into the bus
				.thumb_func
				dpack_dumper_write_addr_wait_state_Tr_1:
					movw r4,#:lower16:ui8_dpack_dumper_addres_nibbles_written // check if (ui8_dpack_dumper_addres_nibbles_written == DPACK_DUMPER_WRITE_ADDR_NIBBLES_MAX) && ( ui8_clck12_state_transition ==  )
					movt r4,#:upper16:ui8_dpack_dumper_addres_nibbles_written
					ldrb r5,[r4]
					cmp  r5,#DPACK_DUMPER_WRITE_ADDR_NIBBLES_MAX // check if ui32_dpack_dumper_address_to_write == DPACK_DUMPER_WRITE_ADDR_NIBBLES_MAX
					bne  dpack_dumper_write_addr_wait_state_Tr_2 // this transition is not satisfied
					movw r4,#:lower16:ui8_clck12_state_transition
					movt r4,#:upper16:ui8_clck12_state_transition
					ldrb r5,[r4]
					cmp r5,#CLCK12_STATE_TRANSITION_7_TO_0
					bne dpack_dumper_write_addr_wait_state_Tr_2
					movw r2,#:lower16:ui8_dpack_dumper_state    
					movt r2,#:upper16:ui8_dpack_dumper_state
					mov r3,#DPACK_DUMPER_STATE_IDLE_CFG
					strb r3,[r2]
					b dpack_dumper_main_end

				// TR 2 : WRITE_ADD_WAIT - > WRITE_ADD: jump when ui8_clck12_state is 7, the whole clock cycle has been executed
				//                                      all the address nibbles have NOT been yet written into the bus
				.thumb_func
				dpack_dumper_write_addr_wait_state_Tr_2:
					movw r4,#:lower16:ui8_clck12_state_transition
					movt r4,#:upper16:ui8_clck12_state_transition
					ldrb r5,[r4]
					cmp r5,#CLCK12_STATE_TRANSITION_1_TO_2
					bne dpack_dumper_main_end
					movw r2,#:lower16:ui8_dpack_dumper_state    
					movt r2,#:upper16:ui8_dpack_dumper_state
					mov  r3,#DPACK_DUMPER_STATE_WRITE_ADD         // update state to DPACK_DUMPER_STATE_WRITE_ADD
					strb r3,[r2]
					b dpack_dumper_main_end

			.thumb_func      // #### WRITE ADD #######################################################################
			dpack_dumper_write_addr_state:
				// STATE DESCRIPTION: state to write in the bus the next of the 5 address nibbles 

			    // ACTIONS:
				// - Write current address nibble into the DATA lines
				movw r4,#:lower16:ui32_dpack_dumper_address_to_write
				movt r4,#:upper16:ui32_dpack_dumper_address_to_write
				ldr  r5,[r4]
				mov  r6,r5
				lsr  r6,#4    // shift the address 4 bits to the right to prepare the address for next nibble address
				str  r6,[r4]  // write cycle and then store it into memory
				eor  r5,#0x0F // reverse the address nibble befor writting it into the data bus
				and  r5,#0x0F

				movw r4,#:lower16:REG_PORT_OUT1       // write into the proper bits of the port the previously prepared address nibble to
				movt r4,#:upper16:REG_PORT_OUT1       // write. First read the content of the data port, and clear the bits corresponding 
				ldr  r2,[r4]                          // to the DATA lines
				movw r6,#:lower16:(~P1_DATA_MASK)            
				movt r6,#:upper16:(~P1_DATA_MASK)					
				and  r2,r6                            // clear the bits of the port that correspond to the data bits
				lsl  r5,#12                           // shift the nibble value before writting to place the bits in the right PORT0 positions: 0x0000000F > 0x0000F000 
                orr  r2,r5                            // set the bits to write
				str  r2,[r4]                          // write the value into the bus 
				// - increase the current written address nibbles counter
				movw r4,#:lower16:ui8_dpack_dumper_addres_nibbles_written
				movt r4,#:upper16:ui8_dpack_dumper_addres_nibbles_written
				ldr  r5,[r4]
				add  r5,#1
				str  r5,[r4]

				// TRANSITIONS:
				// - WRITE ADD -> WRITE_ADD_WAIT: direct transition:
				.thumb_func 
				dpack_dumper_write_addr_state_tr1:
					movw r2,#:lower16:ui8_dpack_dumper_state    
					movt r2,#:upper16:ui8_dpack_dumper_state
					mov  r3,#DPACK_DUMPER_STATE_WRITE_ADD_WAIT 
					strb r3,[r2]
					b dpack_dumper_main_end

			.thumb_func      // #### READ DATA CFG ###################################################################
			dpack_dumper_read_data_cfg_state:
				// STATE DESCRIPTION: executes the entry/initialization actions of the READ_ADD states

			    // ACTIONS:
				// Set DATA lines in input mode to read data nibbles written by CASIO ROM PACK in the bus
				bl config_ctrl_lines_as_output
				bl config_data_lines_as_input		
                // - Clear the CS=0 and set R!W=1: CTRL bits VARIABLE MASKS: 7:x 6:x 5:x 4:'?' 3:CLCK2 2:CLCK1 1:R/!W 0:!CS
				movw r0,#:lower16:ui8_ctrl_bits           
				movt r0,#:upper16:ui8_ctrl_bits
				ldrb r1,[r0] 
				mov  r4,#~(VAR_CTRL_nCS_MASK)
				and  r1,r4 // clear the VAR_CTRL_nCS_MASK bit
				mov  r4,#VAR_CTRL_RnW_MASK
				orr  r1,r4 // set the VAR_CTRL_RnW_MASK bit
				strb r1,[r0]
				// - Update new ctrl signals state (CS, R/!W ...)  into the bus ( according to the state of the values set in ctr variables )
				bl dpack_dumper_set_ctrl_bits
				// - Reset the counter used to count the number of address nibbles written
				movw r4,#:lower16:ui8_dpack_dumper_addres_nibbles_written
				movt r4,#:upper16:ui8_dpack_dumper_addres_nibbles_written
				mov  r5,#0
				strb r5,[r4]
				// - Reset the counter used to temporize IDLE state:
				movw r4,#:lower16:ui8_dpack_dumper_idle_clck_wait_cycles
				movt r4,#:upper16:ui8_dpack_dumper_idle_clck_wait_cycles
				mov  r5,#0
				strb r5,[r4]

				// TRANSITIONS
				// - READ_DATA_CFG -> READ_DATA_WAIT: direct transition:
				.thumb_func 
				dpack_dumper_read_data_cfg_state_tr1:
					movw r2,#:lower16:ui8_dpack_dumper_state    
					movt r2,#:upper16:ui8_dpack_dumper_state
					mov  r3,#DPACK_DUMPER_STATE_READ_DATA_WAIT 
					strb r3,[r2]
					b dpack_dumper_main_end

			.thumb_func      // #### READ DATA WAIT ##################################################################
			dpack_dumper_read_data_wait_state:
			    // STATE DESCRIPTION: state to wait before reading next data nibble from the bus, and when DPACK_DUMPER_READ_DATA_NIBBLES_MAX
				//                    has been read then, go back to write next address nibbles

			    // ACTIONS:
				// No actions, only wait for transistions
				
				// TRANSITIONS:
				// TR 1 : READ_DATA_WAIT - > IDLE_CFG: // check if (ui8_dpack_dumper_data_nibbles_read == DPACK_DUMPER_READ_DATA_NIBBLES_MAX) && ( ui8_clck12_state == 7 ) 
				//                                        clock12 cycle state to check following state has been reached and all the data nibbles have been read from the bus
				.thumb_func
				dpack_dumper_read_data_wait_state_Tr_1:
					movw r4,#:lower16:ui8_dpack_dumper_data_nibbles_read // check if (ui8_dpack_dumper_data_nibbles_read == DPACK_DUMPER_READ_DATA_NIBBLES_MAX) && ( ui8_clck12_state == 7 )
					movt r4,#:upper16:ui8_dpack_dumper_data_nibbles_read
					ldrb r5,[r4]
					cmp  r5,#DPACK_DUMPER_READ_DATA_NIBBLES_MAX  // check if ui8_dpack_dumper_addres_nibbles_written == DPACK_DUMPER_READ_DATA_NIBBLES_MAX
					bne  dpack_dumper_read_data_wait_state_Tr_2  // this transition is not satisfied
					movw r4,#:lower16:ui8_clck12_state_transition
					movt r4,#:upper16:ui8_clck12_state_transition
					ldrb r5,[r4]
					cmp r5,#CLCK12_STATE_TRANSITION_6_TO_7
					bne dpack_dumper_read_data_wait_state_Tr_2
					movw r2,#:lower16:ui8_dpack_dumper_state    
					movt r2,#:upper16:ui8_dpack_dumper_state
					mov r3,#DPACK_DUMPER_STATE_IDLE_CFG
					strb r3,[r2]
					b dpack_dumper_main_end

				// TR 2 : READ_DATA_WAIT - > READ_DATA: jump when 'ui8_clck12_state' is 7, the whole clock cycle has been executed
				//                                      and all the data nibbles have NOT been yet read from the bus
				.thumb_func
				dpack_dumper_read_data_wait_state_Tr_2:
					movw r4,#:lower16:ui8_clck12_state_transition    // check if ui8_clck12_state == 7
					movt r4,#:upper16:ui8_clck12_state_transition
					ldrb r5,[r4]
					cmp r5,#CLCK12_STATE_TRANSITION_6_TO_7
					bne dpack_dumper_read_data_wait_state_Tr_3
					movw r2,#:lower16:ui8_dpack_dumper_state    
					movt r2,#:upper16:ui8_dpack_dumper_state
					mov  r3,#DPACK_DUMPER_STATE_READ_DATA   // update state to DPACK_DUMPER_STATE_READ_DATA
					strb r3,[r2]
					b dpack_dumper_main_end

				// TR 3 : READ_DATA_WAIT - > RESET_CURR_ADDRESS: check current address has reached the maximum configured allowed value
				//                                               if afirmative then jump to the RESET_CURR_ADDRESS to reset the address to 0
				.thumb_func
				dpack_dumper_read_data_wait_state_Tr_3:
					movw r4,#:lower16:ui32_dpack_dumper_current_address    // check if ui32_dpack_dumper_current_address == TOP_ADDRESS
					movt r4,#:upper16:ui32_dpack_dumper_current_address
					ldr  r5,[r4]
					mov  r6,#DPACK_DUMPER_MAX_ADDRESS
					cmp  r5,r6
					bne  dpack_dumper_main_end
					movw r2,#:lower16:ui8_dpack_dumper_state    
					movt r2,#:upper16:ui8_dpack_dumper_state
					mov  r3,#DPACK_DUMPER_STATE_RESET_CURR_ADDRESS   // update state to RESET_CURR_ADDRESS
					strb r3,[r2]
					b dpack_dumper_main_end

			.thumb_func      // #### READ DATA #######################################################################
			dpack_dumper_read_data_state:
				// STATE DESCRIPTION:

			    // ACTIONS:
				// - Get current data nibble from the data bus lines
				movw r4,#:lower16:REG_PORT_IN1      // get the content of the port that contains the data nibble
				movt r4,#:upper16:REG_PORT_IN1      
				ldr  r2,[r4]
				movw r4,#:lower16:P1_DATA_MASK
				movt r4,#:upper16:P1_DATA_MASK
				and  r2,r4                           // keep only the bits that correspond to the data nibble
				lsr  r2,#12                          // shift the data nible bits to the right to place them in the lowest 4 bits
				eor  r2,#0x0F						 // as bits are published in iverse logic in the data bus, to get the right value they are inverted again before sending them through USART
				and  r2,#0x0F						 // keep only the nibble bits, and set to 0 the others
				movw r4,#:lower16:ui8_dpack_dumper_last_data_nibble_read
				movt r4,#:upper16:ui8_dpack_dumper_last_data_nibble_read
				strb r2,[r4]
				// - Increase the ui8_dpack_dumper_data_nibbles_read counter
				movw r4,#:lower16:ui8_dpack_dumper_data_nibbles_read 
				movt r4,#:upper16:ui8_dpack_dumper_data_nibbles_read
				ldrb r5,[r4]
				add  r5,#1
				strb r5,[r4]
				// - Increase the absolute address counter 'ui32_dpack_dumper_current_address'
				movw r4,#:lower16:ui32_dpack_dumper_current_address 
				movt r4,#:upper16:ui32_dpack_dumper_current_address
				ldr  r5,[r4]
				add  r5,#1
				str  r5,[r4]

				// TRANSITIONS
				// - READ_DATA -> READ_DATA_STORE: direct transition:
				.thumb_func 
				dpack_dumper_read_data_state_tr1:
					movw r2,#:lower16:ui8_dpack_dumper_state    
					movt r2,#:upper16:ui8_dpack_dumper_state
					mov  r3,DPACK_DUMPER_STATE_READ_DATA_STORE 
					strb r3,[r2]
					b dpack_dumper_main_end

			.thumb_func      // #### READ DATA STORE #####################################################################
			dpack_dumper_read_data_store_state:
				// STATE DESCRIPTION:

			    // ACTIONS:
				// - Store previously read nibble into the nibbles to send buffer
				bl dpack_dumper_store_in_buffer

				// TRANSITIONS
				// - READ_DATA -> READ_DATA_WAIT: direct transition:
				.thumb_func 
				dpack_dumper_read_data_store_state_tr1:
					movw r2,#:lower16:ui8_dpack_dumper_state    
					movt r2,#:upper16:ui8_dpack_dumper_state
					mov  r3,DPACK_DUMPER_STATE_READ_DATA_WAIT     
					strb r3,[r2]
					b dpack_dumper_main_end

			.thumb_func     // #### RESET CURR ADDRESS #####################################################################
			dpack_dumper_reset_current_address_state:
				// STATE DESCRIPTION:

			    // ACTIONS:
				// - Reset the current address and restart reading form 0x00000
				movw r4,#:lower16:ui32_dpack_dumper_current_address
				movt r4,#:upper16:ui32_dpack_dumper_current_address
				mov  r5,#0x0000
				str  r5,[r4]

				// TRANSITIONS
				// - RESET_CURR_ADDRESS -> IDLE_CFG: direct transition:
				.thumb_func 
				dpack_dumper_reset_current_address_state_tr1:
					movw r2,#:lower16:ui8_dpack_dumper_data_nibbles_read // set 'ui8_dpack_dumper_data_nibbles_read' to DPACK_DUMPER_READ_DATA_NIBBLES_MAX as if the  
					movt r2,#:upper16:ui8_dpack_dumper_data_nibbles_read // READ_DATA cycles have finisehd otherway it will not move from state IDLE_WAIT to WRITE_ADD
					mov  r3,#DPACK_DUMPER_READ_DATA_NIBBLES_MAX
					strb r3,[r2]
					movw r2,#:lower16:ui8_dpack_dumper_state    
					movt r2,#:upper16:ui8_dpack_dumper_state
					mov  r3,#DPACK_DUMPER_STATE_IDLE_CFG
					strb r3,[r2]
					b dpack_dumper_main_exit                             // all the addresses have been dumped so leave the dumper routine

    .thumb_func
    dpack_dumper_main_end:  

	     b dpack_dumper_main_start

    .thumb_func
    dpack_dumper_main_exit:  

		// ARM ABI: a subroutine must preserve the contents of the registers r4-r8, r10, r11 and SP
		pop {r4-r11}
        // go back to C application code execution
        pop {pc} // return from call ( place the LR previously stored in the stack in the PC )
