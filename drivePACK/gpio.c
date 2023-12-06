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
// *****************************************************************

#include "gpio.h"


void GPIO_InitClock(){
		
    //  Clock configuration:                                          EIC: External Interrupt and NMI Controller config
    //    +-------+   +--------+    +--------+   +--------+           TC0: Timer Counter 0
    //    |XCLCK32|-->|GenCLCK1|--> |DPLL0 1 |-->|GenCLCK0| - - - - > SERCOM0: Serial port 0
    //    +-------+   +--------+	+--------+   +--------+           SERCOM3: Serial port 3
    //                                                                SERCOM4: Serial port 4
    //                                                                ¿CPU?                                 
	// 
	// #### BEGIN : UNCOMMENT THIS CODE TO MONITOR GENERIC CLOCK CONTROLLER [0] SIGNAL TRHOUGH PB14 GCLK/IO[0] PIN
	// // Port A (0)  output bits configuration
	// PORT->Group[0].DIRSET.reg = ( P0_DIR_CTRL_DATA_MASK | P0_TEST_PIN_MASK ); // Set as OUTPUT: PA21 ( used as standar GPIO in output mode to toggle a bit ) , PA14 ( used used as GCLK/IO[0] output ),
	// PORT->Group[0].OUTCLR.reg = P0_DIR_CTRL_DATA_MASK;
	//
	// // Port B (1)  output bits configuration
	// PORT->Group[1].DIRSET.reg = P1_nDATA2_BIT_MASK;
	//
	// // Port B (1) multiplexing configuration:
	// // PINB14: GCLK/IO[0]
	// PORT->Group[1].PINCFG[14].bit.PMUXEN  = 1; // enable p.multiplexing on PINB14
	// PORT->Group[1].PMUX[7].bit.PMUXE = 0xC; // 14 is even, so use PMUXE to set p.multiplexing to function M ( GCLK/IO[0] )
	//
	/// // Enable the GCLKIO[0] clock output pin
	// GCLK->GENCTRL[0].bit.OE = 1;  // Enable Generic Clock output through configured GCLK/IO[0] IO pins
	//
	// #### END : UNCOMMENT THIS CODE TO MONITOR GENERIC CLOCK CONTROLLER [0] SIGNAL TRHOUGH PB14 GCLK/IO[0]
	
	// CODE COPIED FROM: https://www.avrfreaks.net/forum/how-do-i-get-120mhz-cpu-speed-using-crystal-atsamd51

	// CONFIGURE FLASH MEMORY WAIT STATES
	NVMCTRL->CTRLA.bit.RWS = 4; // 4WS allows up to 119 MHz, 5WS allows 120 MHz
	NVMCTRL->CTRLA.bit.AUTOWS = 0; // automatically determine the necessary wait states

	//NVMCTRL->CTRLA.bit.CACHEDIS0 = 1; // cache
	//NVMCTRL->CTRLA.bit.CACHEDIS1 = 1; // cache

	CMCC->CTRL.bit.CEN=0;			//Cache Disable //1=Enabled

	// CONFIGURE EXTERNAL 32K CRYSTAL OSCILLATOR
	OSC32KCTRL->XOSC32K.bit.CGM = 1; // control gain mode: 1 = standard/XT 2 = high-speed/HS
	OSC32KCTRL->XOSC32K.bit.STARTUP = 3;// oscillator startup time, 0 = 62 ms ( 62ms may not be enough when booting due to POWER-UP ), 0x1 = 125 ms, 0x2 = 500ms ... ( Table 29-2. Start-Up Time for 32KHz External Crystal Oscillator )
	OSC32KCTRL->XOSC32K.bit.ONDEMAND = 0; // 0 = always run ( "The On-Demand function can be disabled individually for each clock source by clearing the ONDEMAND bit located in each clock source controller" )
	OSC32KCTRL->XOSC32K.bit.XTALEN = 1; // enable crystal driver circuit for XIN32/XOUT32 pins
	OSC32KCTRL->XOSC32K.bit.EN32K = 1; // enable the 32 kHz output clock
	OSC32KCTRL->XOSC32K.bit.ENABLE = 1;
	while (!OSC32KCTRL->STATUS.bit.XOSC32KRDY); // wait until crystal oscillator is stable and ready to be used as a clock source
	OSC32KCTRL->RTCCTRL.bit.RTCSEL = OSC32KCTRL_RTCCTRL_RTCSEL_XOSC32K_Val; // RTC should use the external 32K crystal

	// CONFIGURE MASTER CLOCK
	MCLK->CPUDIV.bit.DIV = 1; // use a divisor of 1 for the master clock
    
 	//Configure ClockGen1 for the PLL GCLK_OSCCTRL_FDPLL0 ( Table 14-9. PCHCTRLm Mapping )
	GCLK->GENCTRL[1].bit.SRC = 5; // 0x05 = Internal 32.768kHz Oscillator. ( "Each Generator can individually select a clock source by setting the Source Select bit group in the Generator Control register (GENCTRLn.SRC)."  )
	GCLK->GENCTRL[1].bit.DIV = 1; // divisor 1 for the input clock from the PLL
	GCLK->GENCTRL[1].bit.GENEN = 1; // enable the clock generator 1.("Before a Peripheral Clock is enabled, one of the Generators must be enabled (GENCTRLn.GENEN) and selected as source for the Peripheral Channel by setting the Generator Selection bits in the Peripheral Channel Control register (PCHCTRL.GEN) ")

    // PCHCTRL Index = [1] = GCLK_OSCCTRL_FDPLL0 = Reference clock for FDPLL0 (Table 14-9. PCHCTRLm Mapping)
	//    GEN  Value =  1  = Generic Clock Generator 1 (Table 14-7. Generator Selection)
	GCLK->PCHCTRL[1].bit.GEN = 1; // "This bit field selects the Generator to be used as the source of a peripheral clock" So [1] Reference clock for FDPLL0 is '1'  = Generic Clock Generator 1 ,
	GCLK->PCHCTRL[1].bit.CHEN = 1; //ENABLE CHANNEL

	// CONFIGURE PLL0
    // REFCLK[2:0] bits:
    //  Value Name Description
    //   0x0  GCLK   Dedicated GCLK clock refer ( GCLK is the Generic Clock )
    //   0x1  XOSC32 XOSC32K clock reference
    //   0x2  XOSC0  XOSC0 clock reference
    //   0x3  XOSC1  XOSC1 clock reference	
	OSCCTRL->Dpll[0].DPLLCTRLB.bit.REFCLK= 0; // the clock is provided by the Generic Clock Controller ( note that GCLK->PCHCTRL[1] corresponds to the GCLK_OSCCTRL_FDPLL0 reference clock )
	// LDR register:
	// fCKR = 32.768k
	// fCLCK_DPLL = fCKR * ( LDR + 1 + (LDFRAC/32) )
	// fCLCK_DPLL = fCKR * ( LDR + 1 ) when LDFRAC == 0
	// LDR = ( fCLCK_DPLL / fCKR) - 1 when LDFRAC == 0
	// LDR:  fCLCK_DPLL:
	// 1000  32800768 Hz
	// 1200	 39354368 Hz
	// 1400	 45907968 Hz
	// 1600	 52461568 Hz
	// 1800	 59015168 Hz
	// 2000	 65568768 Hz
	// 2200	 72122368 Hz
	// 2400	 78675968 Hz
	// 2600	 85229568 Hz
	// 2800	 91783168 Hz
	// 3000	 98336768 Hz
	// 3200	104890368 Hz
	// 3400	111443968 Hz
	// 3600	117997568 Hz
	// 3800	124551168 Hz
	// 4000	131104768 Hz
	// 4200	137658368 Hz
	// 4400	144211968 Hz
	// 4600	150765568 Hz
	// 4800	157319168 Hz
	// 5000	163872768 Hz
	// 5200	170426368 Hz
	OSCCTRL->Dpll[0].DPLLRATIO.bit.LDR = 5000;
	OSCCTRL->Dpll[0].DPLLRATIO.bit.LDRFRAC = 0;
			
	// errata: When using a low-frequency input clock on FDPLLn, several FDPLL unlocks may occur while the output
	// frequency is stable. Workaround: when using a low-frequency input clock on FDPLLn, enable the lock bypass
	// feature to avoid FDPLL unlocks.
	OSCCTRL->Dpll[0].DPLLCTRLB.bit.LBYPASS = 1; // CLK_DPLL0 output clock is always on, and not dependent on frequency lock
	OSCCTRL->Dpll[0].DPLLCTRLA.bit.ONDEMAND = 0; // always run
	OSCCTRL->Dpll[0].DPLLCTRLA.bit.ENABLE = 1;

	while (!OSCCTRL->Dpll[0].DPLLSTATUS.bit.LOCK) ; // no point in checking DPLLSTATUS.bit.CLKRDY, because LBYPASS is enabled

	// CONFIGURE CLOCK GENERATOR 0
	GCLK->GENCTRL[0].bit.SRC = 7; // use PLL0 as the input
	GCLK->GENCTRL[0].bit.DIV = 1; // divisor 1 for the input clock from the PLL
	GCLK->GENCTRL[0].bit.GENEN = 1;

	// do peripheral clock initialization here...

}//GPIO_InitClock


void GPIO_DeinitClock(){

    //  Clock configuration:
    //    +-------+   +--------+           
    //    |XCLCK32|-->|GenCLCK0| - - - - > ¿CPU?
    //    +-------+   +--------+ 
    //
    //
	// RECONFIGURE GENERIC CLOCK GENERATOR 0 to 32.768kHz OSCILLATOR
	// configure external 32K crystal oscillator
	OSC32KCTRL->XOSC32K.bit.ENABLE = 0;
	OSC32KCTRL->XOSC32K.bit.CGM = 1; // control gain mode: 1 = standard/XT 2 = high-speed/HS
	OSC32KCTRL->XOSC32K.bit.STARTUP = 0; // oscillator startup time, 0 = 62 ms
	OSC32KCTRL->XOSC32K.bit.ONDEMAND = 0; // 0 = always run ( "The On-Demand function can be disabled individually for each clock source by clearing the ONDEMAND bit located in each clock source controller" )
	OSC32KCTRL->XOSC32K.bit.XTALEN = 1; // enable crystal driver circuit for XIN32/XOUT32 pins
	OSC32KCTRL->XOSC32K.bit.EN32K = 1; // enable the 32 kHz output clock
	OSC32KCTRL->XOSC32K.bit.ENABLE = 1;
	while (!OSC32KCTRL->STATUS.bit.XOSC32KRDY); // wait until crystal oscillator is stable and ready to be used as a clock source
	OSC32KCTRL->RTCCTRL.bit.RTCSEL = OSC32KCTRL_RTCCTRL_RTCSEL_XOSC32K_Val; // RTC should use the external 32K crystal		
	// reconfigure the external 32K crystal oscillator as source of generic clock generator 0
	GCLK->GENCTRL[0].bit.SRC = 5; // 0x05 = Internal 32.768kHz Oscillator. ( "Each Generator can individually select a clock source by setting the Source Select bit group in the Generator Control register (GENCTRLn.SRC)."  )
	GCLK->GENCTRL[0].bit.GENEN = 1;

    // DISABLE THE CLOCK IN ALL PERIPHERALS WHERE IT WAS ENABLED
    // Disable EIC External Interrupt and NMI Controller
    EIC->CTRLA.bit.ENABLE = 0; // Once the EIC has been configured, enable the EIC by writing a ‘1’ to CTRLA.ENABLE
    while(EIC->SYNCBUSY.reg == 1)
    // CLK_EIC (APBA): Peripheral Clock Masking
    MCLK->APBAMASK.bit.EIC_ = 0; // 1 = The APBA clock for the EIC is enabled, 0 = Disabled
    // GCLK_EIC: #4 is the peripheral channel control register for IEC: PCHCTRLm Mapping Index 4: 4 GCLK_EIC EIC
    GCLK->PCHCTRL[4].bit.CHEN = 0; //1 = clock CHANNEL ENABLE, 0 = DISABLE
	
	// Disable SERCOM0 
	SERCOM0->SPI.CTRLA.bit.ENABLE = 0;// 0 = Disable before configuring
	while (SERCOM0->SPI.SYNCBUSY.bit.ENABLE == 1); // wait for SPI to be disabled
    // CLK_SERCOM0_APB (APBB): Peripheral Clock Masking
	MCLK->APBAMASK.bit.SERCOM0_= 0; // 1 = The APBA clock for the SERCOM0 is enabled,  0 = Disabled
	// GCLK_SERCOM0_CORE: #7 is the peripheral channel control register for SERCOM0: PCHCTRLm Mapping Index 7: 7 GCLK_SERCOM0_CORE SERCOM0 Core
	GCLK->PCHCTRL[7].bit.CHEN = 0; //1 = clock CHANNEL ENABLE

	// Disable SERCOM3 
	SERCOM3->USART.CTRLA.bit.ENABLE = 0;// 0 = Disable before configuring
	while (SERCOM3->USART.SYNCBUSY.bit.ENABLE == 1); // wait for USART to be disabled	
	// CLK_SERCOM3_APB (APBB): Peripheral Clock Masking
	MCLK->APBBMASK.bit.SERCOM3_= 0; // 1 = The APBB clock for the SERCOM3 is enabled,  0 = Disabled
	// GCLK_SERCOM3_CORE: #24 is the peripheral channel control register for SERCOM3: PCHCTRLm Mapping Index 24: 24 GCLK_SERCOM3_CORE SERCOM3 Core
	GCLK->PCHCTRL[24].bit.CHEN = 0; //1 = clock CHANNEL ENABLE, 0 = DISABLE
	
	// Disable SERCOM4
	SERCOM4->SPI.CTRLA.bit.ENABLE = 0;// 0 = Disable before configuring
	while (SERCOM4->USART.SYNCBUSY.bit.ENABLE == 1); // wait for USART to be disabled	
	// CLK_SERCOM4_APB (APBA): Peripheral Clock Masking
	MCLK->APBDMASK.bit.SERCOM4_= 0; // 1 = The APBD clock for the SERCOM4 is enabled,  0 = Disabled
	// GCLK_SERCOM4_CORE: #34 is the peripheral channel control register for SERCOM4: PCHCTRLm Mapping Index 7: 7 GCLK_SERCOM4_CORE SERCOM4 Core
	GCLK->PCHCTRL[34].bit.CHEN = 0; //1 = clock CHANNEL ENABLE, 0 = DISABLE	
				
	// Disable TimerCounter0:
	TC0->COUNT16.CTRLA.bit.ENABLE = 0;// 0 = Disable before configuring. Some registers can only be written when TC is disabled
	while (TC0->COUNT16.SYNCBUSY.reg);	
	// TimerCounter 0 APB (APBA): Peripheral Clock Masking	
	MCLK->APBAMASK.bit.TC0_= 0; // 1 = The APBA clock for the TC0 is enabled, 0 = Disabled
	// GCLK_TC0_CORE: #9 is the peripheral channel control register for TC0 and TC1: PCHCTRLm Mapping Index 9: 9 GCLK_TC0, GCLK_TC1 TC0, TC1
	GCLK->PCHCTRL[9].bit.CHEN = 0; //1 = clock CHANNEL ENABLE, 0 = DISABLE
	
	// DISSABLE THE PLL0 
	OSCCTRL->Dpll[0].DPLLCTRLA.bit.ENABLE = 0;
    while (OSCCTRL->Dpll[0].DPLLSYNCBUSY.bit.ENABLE == 1)

	// DISSABLE THE GENERIC CLOCK GENERATOR 1
	GCLK->GENCTRL[1].bit.GENEN = 0; // 1 = enable the clock generator 1.("Before a Peripheral Clock is enabled, one of the Generators must be enabled (GENCTRLn.GENEN) and selected as source for the Peripheral Channel by setting the Generator Selection bits in the Peripheral Channel Control register (PCHCTRL.GEN) ")
	GCLK->PCHCTRL[1].bit.CHEN = 0; //1 = clock CHANNEL ENABLE, 0 = DISABLE

}//GPIO_DeinitClock


void GPIO_KeepOnlyDrivePackInterrupts(){

	// disable all IRQs while reconfiguring them
	__disable_irq();

	// disable Encoder and TimerCounter0 interrupts
	NVIC_DisableIRQ(EIC_7_IRQn); // disable EIC_7_IRQn interruption and vector line in the NVIC controller ( for the PA07 encoder line,  the PA08 interrupt is not maskable )
	NVIC_DisableIRQ(TC0_IRQn); // disable TC0 interruption and vector line in the NVIC controller  ( for the timer )

    // enable CLCK2 interrupt: reconfigure EIC (External Interrupt Controller ) for PB17 to generate interrupt on drivePACK CLCK2 signal changes:    
	// temporarily disable EIC peripheral before configuring it because some registers can only be written when the EIC is disabled (CTRLA.ENABLE=0)	  
	EIC->CTRLA.bit.ENABLE = 0; 
	while(EIC->SYNCBUSY.reg & EIC_SYNCBUSY_ENABLE); // wait for operation to complete
	
	// select EIC/EXTINT function for PB17 in the multiplexer 
  	PORT->Group[1].PINCFG[17].bit.PMUXEN  = 1; // enable p.multiplexing on PB17
  	PORT->Group[1].PMUX[8].bit.PMUXO = 0x0; // PB17, 17 is odd, so use PMUXO to set p.multiplexing to function (17/2=8), function A = EIC/EXTINT[1] ... set 0x0 ( 0x0 A Peripheral function A selected )
  	// as specified in the datasheet PINB17 is associated to EIC/EXTINT[>1<] so it is configured in 1/8=0 so CONFIG[0]->, and mod(1,8)=1 so .FILTEN1 and .SENSE1
  	EIC->CONFIG[0].bit.SENSE1 = 0x2; // EIC_CONFIG_SENSE0_RISE_Val 0x0 No detection , 0x1 RISE Rising-edge detection , 0x2 FALL Falling-edge detection, 0x3 BOTH Both-edge detection
  	EIC->CONFIG[0].bit.FILTEN1 = 0; // 0 = Filter is disabled for EXTINT[n*8+x] input.
  	// enable EIC1 to generate interrupts on changes of state of PB17. PB17 is connected to CLCK2 signal so that interrupt allows to detect CLCK2 cycles
	EIC->INTENSET.reg = EIC_INTFLAG_EIC1_MASK;// set bit 1 for EIC/EXTINT[1] to enable interrupt Writing a '0' to bit x has no effect.
  	EIC->INTFLAG.reg =  EIC_INTFLAG_EIC1_MASK;// the flag bit EIC/EXTINT[1] interrupt flag is cleared by writing a '1' to it.
  		  
    NVIC_EnableIRQ(EIC_1_IRQn); // enable EIC_1_IRQn interruption and vector line in the NVIC controller ( ROM pack CLCK2 line interrupt )
    
	// once configured enable the EIC peripheral
  	EIC->CTRLA.bit.ENABLE = 1; // Once the EIC has been configured, enable the EIC by writing a ‘1’ to CTRLA.ENABLE
  	while(EIC->SYNCBUSY.reg & EIC_SYNCBUSY_ENABLE);// wait for operation to complete
    
    __enable_irq();

}//GPIO_KeepOnlyDrivePackInterrupts


void GPIO_RestoreAllSystemInterrupts(){
	
	// disable all IRQs while reconfiguring them
	__disable_irq();

    // disable CLCK2 interrupt:
	// temporarily disable EIC peripheral before reconfiguring it because some registers can only be written when the EIC is disabled (CTRLA.ENABLE=0)
	EIC->CTRLA.bit.ENABLE = 0;
	while(EIC->SYNCBUSY.reg & EIC_SYNCBUSY_ENABLE); // wait for operation to complete	
	
  	// UNselect EIC/EXTINT function for PB17 in the multiplexer and use it as standard GPIO
  	PORT->Group[1].PINCFG[17].bit.PMUXEN  = 0; // disable p.multiplexing on PB17
    // disable CLCK2 dpackSignals interrupts
    EIC->INTENCLR.reg = EIC_INTFLAG_EIC1_MASK;// set bit 1 for EIC/EXTINT[1] to disable interrupt Writing a '0' to bit x has no effect. The interrupt will be reenabled we needed in the drivePACK emulation
  	EIC->INTFLAG.reg =  EIC_INTFLAG_EIC1_MASK;// the flag bit EIC/EXTINT[1] interrupt flag is cleared by writing a '1' to it.
		
    NVIC_DisableIRQ(EIC_1_IRQn); // disable EIC_1_IRQn interruption and vector line in the NVIC controller ( ROM pack CLCK2 line interrupt )

	// once configured enable the EIC peripheral ( the encoder uses the EIC )
	EIC->CTRLA.bit.ENABLE = 1; // Once the EIC has been configured, enable the EIC by writing a ‘1’ to CTRLA.ENABLE
	while(EIC->SYNCBUSY.reg & EIC_SYNCBUSY_ENABLE);// wait for operation to complete

	// re-enable Encoder and TimerCounter0 interrupts
	NVIC_EnableIRQ(EIC_7_IRQn); // re-enable EIC_7_IRQn interruption and vector line in the NVIC controller ( for the PA07 encoder line,  the PA08 interrupt is not maskable )
	NVIC_EnableIRQ(TC0_IRQn); // re-enable TC0 interruption and vector line in the NVIC controller ( for the timer )

    __enable_irq();

}//GPIO_RestoreAllSystemInterrupts


void GPIO_Init(){
	uint16_t ui16_aux = 0;
	uint8_t  ui8_aux = 0;


	// disable all IRQs before configuring the system clocks
	__disable_irq();

    // ########## GPI/O lines direction configuration and function selection: ##########
    
	// **** PORT A ( PORT->Group[0] ) GPI/O lines configuration:

	// configure direction of I/O lines: DIR bit = 1 OUTPUT , DIR bit = 0 INPUT
	// configuration of output pins:
	// - '5-3.3V TRANSCEIVERs DIR as OUTPUT'
	// - TEST_PIN as OUTPUT
	// - SERCOM3 PAD0 as Tx OUTPUT'
	// - SERCOM0 PAD0 SD_MOSI OUTPUT
	// - SERCOM0 PAD1 SD_SCK OUTPUT
	// - SD_CS as OUTPUT
	PORT->Group[0].DIRSET.reg = (P0_DIR_CTRL_DATA_MASK|P0_USART_TX_MASK|P0_SPI_SD_MOSI_MASK|P0_SPI_SD_SCK_MASK|P0_SPI_SD_nCS_MASK|P0_ROM_CON_GPIO01|P0_ROM_CON_GPIO02|P0_ROM_CON_GPIO03);

	// configuration of input lines:
	// - UNKNN_CTRL_BIT as INPUT
	// - SERCOM3 PAD1 as Rx INPUT
	// - SERCOM0 PAD2 SD_MISO OUTPUT
	// - USER_IFACE_ENC_A
	// - USER_IFACE_ENC_B
	// - USER_IFACE_ENC_PUSHB
	// - USER_IFACE_PUSHB_A
	// - USER_IFACE_PUSHB_B
	// - USER_IFACE_PUSHB_C
	// - USER_IFACE_PUSHB_D
	PORT->Group[0].DIRCLR.reg = (P0_UNKNWN_CTRL_BIT_MASK|P0_USART_RX_MASK|P0_SPI_SD_MISO_MASK|P0_USER_IFACE_ENC1_A|P0_USER_IFACE_ENC1_B|P0_USER_IFACE_PUSHB_ENC1|P0_USER_IFACE_PUSHB_2|P0_USER_IFACE_PUSHB_4|P0_USER_IFACE_PUSHB_3|P0_USER_IFACE_PUSHB_1);

	// configure  ROM PACK UNKNN_CTRL_BIT I/O lines:
	// ATSMAD51  enable the INEN bits for the cases where that I/O lines are used as input:
	// PA02 -'?' UNKONWN	
	// PA07  -  ENC_A
	// PA08  -  ENC_B
	// PA09  -  ENC_PUSHB
	// PA10  -  PUSHB_A
	// PA11  -  PUSHB_B
	// PA12  -  PUSHB_C
	// PA13  -  PUSHB_D
	// PA20 - SERCOM0 PAD2 SD_MISO 
	PORT->Group[0].PINCFG[2].bit.INEN = 1;	
	PORT->Group[0].PINCFG[7].bit.INEN = 1;
	PORT->Group[0].PINCFG[8].bit.INEN = 1;
	PORT->Group[0].PINCFG[9].bit.INEN = 1;
	PORT->Group[0].PINCFG[10].bit.INEN = 1;
	PORT->Group[0].PINCFG[11].bit.INEN = 1;
	PORT->Group[0].PINCFG[12].bit.INEN = 1;
	PORT->Group[0].PINCFG[13].bit.INEN = 1;	
	PORT->Group[0].PINCFG[20].bit.INEN = 1;
	
	// configure SERCOM0 I/O lines:
	// ATSMAD51            
	//  PA04(SERCOM0 PAD0) 
	//  PA05(SERCOM0 PAD1) 
	//  PA06(SERCOM0 PAD2) 
	// enable multiplexer in SERCOM0 PAD0, PAD1 and PAD2 pins
	PORT->Group[0].PINCFG[4].bit.PMUXEN  = 1; // enable p.multiplexing on PINA04
	PORT->Group[0].PINCFG[5].bit.PMUXEN  = 1; // enable p.multiplexing on PINA05
	PORT->Group[0].PINCFG[6].bit.PMUXEN  = 1; // enable p.multiplexing on PINA06
	// PORT->Group[0].PINCFG[6].bit.PULLEN  = 1; // enable pull in the MISO in
	// PORT->Group[0].OUTSET.reg = P0_SPI_SD_MISO_MASK;// select pull down in MISO pin
	// select SERCOM0 function in the multiplexer
	PORT->Group[0].PMUX[2].bit.PMUXE = 3; // 04 is even, so use PMUXE to set p.multiplexing to function (04/2=2),    function D = SERCOM0/ PAD[0] so set 0x3 ( peripheral D function selected )
	PORT->Group[0].PMUX[2].bit.PMUXO = 3; // 05 is odd, so use PMUXO to set p.multiplexing to function (05/2=2.5=2), function D = SERCOM0/ PAD[1] so set 0x3 ( peripheral D function selected )
	PORT->Group[0].PMUX[3].bit.PMUXE = 3; // 06 is even, so use PMUXE to set p.multiplexing to function (06/2=3),    function D = SERCOM0/ PAD[2] so set 0x3 ( peripheral D function selected )

	// Set I/O lines values:
	PORT->Group[0].OUTSET.reg = P0_DIR_CTRL_DATA_MASK; // configure the TRANSCEIVERS in INPUT direction (A 5.0V to B 3.3V)  0 = OUTPUT (B 3.3V to A 5.0V ) , 1 = INPUT (A 5.0V to B 3.3V)
	PORT->Group[0].OUTSET.reg = P0_SPI_SD_nCS_MASK; // disable SD_CARD nCS=1
	
	// configure SERCOM3 I/O lines:
	//
	// ATSMAD51
	//  PA22        *Tx>(1)
	//  PA23        *Rx<(0)
	// enable multiplexer in SERCOM3 PAD0 and PAD1 pins
	PORT->Group[0].PINCFG[22].bit.PMUXEN  = 1; // enable p.multiplexing on PINA22
	PORT->Group[0].PINCFG[23].bit.PMUXEN  = 1; // enable p.multiplexing on PINA23
	PORT->Group[0].PINCFG[23].bit.PULLEN  = 1; // enable pull in the Rx in
	PORT->Group[0].OUTCLR.reg = P0_USART_RX_MASK;// select pull down in RX pin
	// select SERCOM3 function in the multiplexer
	PORT->Group[0].PMUX[11].bit.PMUXE = 2; // 22 is even, so use PMUXE to set p.multiplexing to function (22/2=11),     function C = SERCOM3/ PAD[0] so set 0x2 ( peripheral C function selected )
	PORT->Group[0].PMUX[11].bit.PMUXO = 2; // 23 is odd, so use PMUXO to set p.multiplexing to function (23/2=11.5=11), function C = SERCOM3/ PAD[1] so set 0x2 ( peripheral C function selected )
		
	// **** PORT B ( PORT->Group[1] ) GPI/O lines configuration:

	// configure direction of I/O lines: DIR bit = 1 OUTPUT , DIR bit = 0 INPUT
	// configuration of output pins:
	// - DATA and CTRL bits initialized as output ( DATA bits direction may change during execution )
	// - LCD RS bit initialized as output
	// - LCD CS bit initialized as output
	// - LCD RST bit initialized as output
	// - LCD SCK bit initialized as output
	PORT->Group[1].DIRSET.reg = P1_CTRL_AND_DATA_MASK | P1_LCD_MASK_LCD_RS | P1_LCD_MASK_LCD_CS | P1_LCD_MASK_LCD_RST | P1_SPI_LCD_SCK_MASK;

	// configure Port B ROM PACK CTRL DATA bits
	// ATSMAD51 ( enable the INEN bits for the cases where that I/O lines are used as input):
	// PB10 - !CS
	// PB11 - R/!W
	// PB12 - !Data0
	// PB13 - !Data1
	// PB14 - !Data2
	// PB15 - !Data3
	// PB16 - CLCK1
	// PB17 - CLCK2
	PORT->Group[1].PINCFG[10].bit.INEN = 1;
	PORT->Group[1].PINCFG[11].bit.INEN = 1;
	PORT->Group[1].PINCFG[12].bit.INEN = 1;
	PORT->Group[1].PINCFG[13].bit.INEN = 1;
	PORT->Group[1].PINCFG[14].bit.INEN = 1;
	PORT->Group[1].PINCFG[15].bit.INEN = 1;
	PORT->Group[1].PINCFG[16].bit.INEN = 1;
	PORT->Group[1].PINCFG[17].bit.INEN = 1;

	// configure Port A SERCOM4 I/O lines:
	//
	// ATSMAD51:
	// PB08(SERCOM4 PAD 0)
	// PB09(SERCOM4 PAD 1)
	// enable multiplexer in SERCOM4 PAD0, PAD1 pins ( PAD2 pin RX is not used )
	PORT->Group[1].PINCFG[8].bit.PMUXEN  = 1; // enable p.multiplexing on PINB08
	PORT->Group[1].PINCFG[9].bit.PMUXEN  = 1; // enable p.multiplexing on PINB09
	// PORT->Group[1].PINCFG[6].bit.PULLEN  = 1; // enable pull in the MISO in
	// PORT->Group[1].OUTSET.reg = P0_SPI_SD_MISO_MASK;// select pull down in MISO pin
	// select SERCOM4 function in the multiplexer
	PORT->Group[1].PMUX[4].bit.PMUXE = 3; // PB08, 08 is even, so use PMUXE to set p.multiplexing to function (08/2=4), function D = SERCOM4/ PAD[0] so set 0x3 ( peripheral D function selected )
	PORT->Group[1].PMUX[4].bit.PMUXO = 3; // PB09, 09 is odd, so use PMUXO to set p.multiplexing to function (08/2=4.5=4), function D = SERCOM4/ PAD[1] so set 0x3 ( peripheral D function selected )
	// PORT->Group[0].PMUX[3].bit.PMUXE = 3; // 06 is even, so use PMUXE to set p.multiplexing to function (06/2=3),    function D = SERCOM0/ PAD[2] so set 0x3 ( peripheral D function selected )

    // ########## Peripherals specific configuration: ##########

	// temporarily disable EIC peripheral before configuring it because some registers can only be written when the EIC is disabled (CTRLA.ENABLE=0)
	EIC->CTRLA.bit.ENABLE = 0;
	while(EIC->SYNCBUSY.reg & EIC_SYNCBUSY_ENABLE); // wait for operation to complete

	// **** EIC: External Interrupt and NMI Controller config: EIC allows external pins to be configured as interrupt lines
	MCLK->APBAMASK.bit.EIC_ = 1; // 1 = The APBA clock for the EIC is enabled.
	
	// GCLK_EIC: #4 is the peripheral channel control register for IEC: PCHCTRLm Mapping Index 4: 4 GCLK_EIC EIC
	GCLK->PCHCTRL[4].bit.GEN = 0;  //0 = Generic Clock Generator 0, 1 = Generic Clock Generator 1, 2 Generic Clock Generator 2 ...
	GCLK->PCHCTRL[4].bit.CHEN = 1; //1 = clock CHANNEL ENABLE
	while ((GCLK->PCHCTRL[4].reg & GCLK_PCHCTRL_CHEN) == 0);// Wait for Sync

	// EIC->CTRLA.bit.CKSEL = 0; // the EIC is clocked by GCLK_EIC.
	// EIC->CTRLA.reg = EIC_CTRLA_SWRST;  // reset the External Interrupt Controller
	// while(EIC->SYNCBUSY.reg & EIC_SYNCBUSY_SWRST); //  wait for reset to complete

	// select EIC/EXTINT function for PA07 in the multiplexer ( used to generate interrupts when moving rotary encoder ENCA signal )
	PORT->Group[0].PINCFG[7].bit.PMUXEN  = 1; // enable p.multiplexing on PINA07
	PORT->Group[0].PMUX[3].bit.PMUXO = 0x0; // PA07, 07 is odd, so use PMUXO to set p.multiplexing to function (07/2=3.5=3), function  A = EIC/EXTINT[7] set 0x0 ( 0x0 A Peripheral function A selected )
	// as specified in the datasheet PINA07 is associated to EIC/EXTINT[>7<] so it is configured in 7/8=0 so CONFIG[0]->, and mod(7,8)=7 so .FILTEN7 and .SENSE7 
    EIC->CONFIG[0].bit.SENSE7 = 0x3; // EIC_CONFIG_SENSE7_RISE_Val 0x0 No detection , 0x1 RISE Rising-edge detection , 0x2 FALL Falling-edge detection, 0x3 BOTH Both-edge detection
    EIC->CONFIG[0].bit.FILTEN7 = 0; // 0 = Filter is disabled for EXTINT[n*8+x] input.	
    EIC->INTENSET.reg = 0x0080;// set bit 7 for EIC/EXTINT[7] to enable interrupt ( Writing a '0' to bit x has no effect. )
    EIC->INTFLAG.reg =  0x0080;// the flag bit EIC/EXTINT[7] interrupt flag is cleared by writing a '1' to it.

	// NVIC_SetPriority(EIC_7_IRQn, 4);
	NVIC_EnableIRQ(EIC_7_IRQn); // enable EIC_7_IRQn interruption and vector line in the NVIC controller

	// select EIC/NMI function for PA08 in the multiplexer ( used to generate interrupts when moving rotary encoder ENCB signal )
	PORT->Group[0].PINCFG[8].bit.PMUXEN  = 1; // enable p.multiplexing on PINA08
	PORT->Group[0].PMUX[4].bit.PMUXE = 0x0; // PA08, 08 is even, so use PMUXE to set p.multiplexing to function (08/2=4), function A = EIC/NMI ... set 0x0 ( 0x0 A Peripheral function A selected )
    // as specified in the datasheet PINA08 is associated to EIC/NMI ( this GPIO line is associated to a NOT MASKABLE INTERRUPT line so it is configured slightly different )
    EIC->NMICTRL.bit.NMISENSE = 0x3;//NMISENSE 0x0 No detection , 0x1 RISE Rising-edge detection , 0x2 FALL Falling-edge detection, 0x3 BOTH Both-edge detection
    EIC->NMICTRL.bit.NMIFILTEN = 0x0;// 0 = NMI filter is disabled, 1=NMI filter is enabled.
    // !AS THIS PIN GENERATES A NOT MASKABLE INTERRUPT THERE IS NO NEED TO ENABLE IT WITH NVIC_EnableIRQ(X). In fact it is not possible to disable it.
    EIC->NMIFLAG.bit.NMI = 0x1;// NMI Non-Maskable Interrupt Flag is cleared by writing a '1' to it. This flag is set when the NMI pin matches the NMI sense configuration, and will generate an interrupt request.

	// select EIC/EXTINT function for PB16 in the multiplexer ( used to generate interrupts on drivePACK CLCK1 signal changes)
	// PORT->Group[1].PINCFG[16].bit.PMUXEN  = 1; // enable p.multiplexing on PB16
	// PORT->Group[1].PMUX[8].bit.PMUXE = 0x0; // PB16, 16 is even, so use PMUXE to set p.multiplexing to function (16/2=8), function A = EIC/EXTINT[0] ... set 0x0 ( 0x0 A Peripheral function A selected )
	// // as specified in the datasheet PINB16 is associated to EIC/EXTINT[>0<] so it is configured in 0/8=2 so CONFIG[0]->, and mod(0,8)=0 so .FILTEN0 and .SENSE0
	// EIC->CONFIG[0].bit.SENSE0 = 0x1; // EIC_CONFIG_SENSE0_RISE_Val 0x0 No detection , 0x1 RISE Rising-edge detection , 0x2 FALL Falling-edge detection, 0x3 BOTH Both-edge detection
	// EIC->CONFIG[0].bit.FILTEN0 = 0; // 0 = Filter is disabled for EXTINT[n*8+x] input.
	// // EIC->INTENSET.reg = 0x0001;// set bit 0 for EIC/EXTINT[0] to enable interrupt Writing a '0' to bit x has no effect.
	// EIC->INTENSET.reg = 0x0001;// set bit 0 on INTENSET for EIC/EXTINT[0] to enable interrupt. Writing a '0' to bit x has no effect. 
	// EIC->INTFLAG.reg =  0x0001;// the flag bit EIC/EXTINT[0] interrupt flag is cleared by writing a '1' to it.

	// NVIC_SetPriority(EIC_0_IRQn, 4);
	// NVIC_DisableIRQ(EIC_0_IRQn); // keep disabled EIC_0_IRQn interruption CLCK1, it will be activated when we need it in the drivePACK emulation
    
	// By default UNselect EIC/EXTINT function for PB17 in the multiplexer and use it as standard GPIO. This pins will be used with EIC (External Interrupt Controller ) when emulating
	// the ROM PACK to generate interrupt on drivePACK CLCK2 signal changes, and will be kept as standard GPIO in all other situations, this is why the IEC is not configured here. Check 
	// the function GPIO_KeepOnlyDrivePackInterrupts() that configures it to operate PB17 to generate interrupts on CLCK2 signal changes. By default this pin will not generate 
	// interrupts.
	// PORT->Group[1].PINCFG[17].bit.PMUXEN  = 1; // enable p.multiplexing on PB17
	// PORT->Group[1].PMUX[8].bit.PMUXO = 0x1; // PB17, 17 is odd, so use PMUXO to set p.multiplexing to function (17/2=8), function A = EIC/EXTINT[1] ... set 0x0 ( 0x0 A Peripheral function A selected )
	// as specified in the datasheet PINB17 is associated to EIC/EXTINT[>1<] so it is configured in 1/8=0 so CONFIG[0]->, and mod(1,8)=1 so .FILTEN1 and .SENSE1
	// EIC->CONFIG[0].bit.SENSE1 = 0x2; // EIC_CONFIG_SENSE0_RISE_Val 0x0 No detection , 0x1 RISE Rising-edge detection , 0x2 FALL Falling-edge detection, 0x3 BOTH Both-edge detection
	// EIC->CONFIG[0].bit.FILTEN1 = 0; // 0 = Filter is disabled for EXTINT[n*8+x] input.
	// EIC->INTENSET.reg = 0x0002;// set bit 1 for EIC/EXTINT[1] to enable interrupt Writing a '0' to bit x has no effect.
	// EIC->INTENCLR.reg = EIC_INTFLAG_EIC1_MASK;// set bit 1 on INTENCLR EIC/EXTINT[1] to disable interrupt Writing a '0' to bit x has no effect. The interrupt will be renabled we needed in the drivePACK emulation
	// EIC->INTFLAG.reg =  EIC_INTFLAG_EIC1_MASK;// the flag bit EIC/EXTINT[1] interrupt flag is cleared by writing a '1' to it.

	// NVIC_DisableIRQ(EIC_1_IRQn); // disable EIC_1_IRQn interruption and vector line in the NVIC controller ( ROM pack CLCK2 line interrupt )
  	 	
    EIC->CTRLA.bit.ENABLE = 1; // Once the EIC has been configured, enable the EIC by writing a ‘1’ to CTRLA.ENABLE
  	while(EIC->SYNCBUSY.reg & EIC_SYNCBUSY_ENABLE);                     // wait for enable to complete
	
	// **** SERCOM0 as SPI0 config:
	
	// Configure APB and CORE clocks for SERCOM0:
	// CLK_SERCOM0_APB (APBA): Peripheral Clock Masking
	MCLK->APBAMASK.bit.SERCOM0_= 1; // 1 = The APBA clock for the SERCOM0 is enabled.
	// GCLK_SERCOM0_CORE: #7 is the peripheral channel control register for SERCOM0: PCHCTRLm Mapping Index 7: 7 GCLK_SERCOM0_CORE SERCOM0 Core
	GCLK->PCHCTRL[7].bit.GEN = 0;  //0 = Generic Clock Generator 0, 1 = Generic Clock Generator 1, 2 Generic Clock Generator 2 ...
	GCLK->PCHCTRL[7].bit.CHEN = 1; //1 = clock CHANNEL ENABLE

	//  Configure the SERCOM0 - SPI registers:
	SERCOM0->SPI.CTRLA.bit.ENABLE = 0;// 0 = Disable before configuring
	SERCOM0->SPI.CTRLA.bit.MODE = 3;// 0x3 = Master mode ( serial clock (SCK) is generated internally by the SERCOM Baud Rate Generator (BRG) )
	SERCOM0->SPI.CTRLA.bit.CPHA = 0;// Trailing Edge: Falling, change
	SERCOM0->SPI.CTRLA.bit.CPOL = 0;// Leading Edge: Rising, sample
	
	SERCOM0->SPI.CTRLA.bit.FORM =   0;// 0x0 = SPI frame
	SERCOM0->SPI.CTRLA.bit.DIPO =   2;// 0x2 = SERCOM PAD[2] is used as data input
	SERCOM0->SPI.CTRLA.bit.DOPO =   0;// 0x0 = PAD[0]:DO  PAD[1]:SCK
	SERCOM0->SPI.CTRLB.bit.CHSIZE = 0;// 0x0 = 8 bits
	SERCOM0->SPI.CTRLA.bit.DORD =   0;// 0 = MSB is transmitted first, 1 = LSB is transmitted first
	SERCOM0->SPI.CTRLB.bit.MSSEN =  0;// 0 = !SS disabled ( a GPIO is used instead for !CS )

	// BAUD register: SYNChronous formula ( 8 bits )
	// BAUD = fref / (2 * (BAUD+1) )
	// fRef = main clock frequency
	// fBAUD = 163872768 / (2 * (BAUD+1) )
	ui8_aux = (uint8_t)50; // = 163872768 / 512
	SERCOM0->SPI.BAUD.bit.BAUD = ui8_aux;

	SERCOM0->SPI.CTRLB.bit.RXEN = 1; // 1= Rx Enabled
	SERCOM0->SPI.CTRLA.bit.ENABLE = 1;// 1 = Once all register and I/O lines have been configured, enable SERCOM SPI

	while (SERCOM0->SPI.SYNCBUSY.bit.ENABLE == 1); // wait for SPI to be enabled

	// **** SERCOM3 as USART config:

	// Configure APB and CORE clocks for SERCOM3:
	// CLK_SERCOM3_APB (APBB): Peripheral Clock Masking
	MCLK->APBBMASK.bit.SERCOM3_= 1; // 1 = The APBB clock for the SERCOM3 is enabled.
	// GCLK_SERCOM3_CORE: #24 is the peripheral channel control register for SERCOM3: PCHCTRLm Mapping Index 24: 24 GCLK_SERCOM3_CORE SERCOM3 Core
	GCLK->PCHCTRL[24].bit.GEN = 0;  //0 = Generic Clock Generator 0, 1 = Generic Clock Generator 1, 2 Generic Clock Generator 2 ...
	GCLK->PCHCTRL[24].bit.CHEN = 1; //1 = clock CHANNEL ENABLE

	// Configure the SERCOM3 - USART registers:
	SERCOM3->USART.CTRLA.bit.ENABLE = 0;// 0 = Disable before configuring
	SERCOM3->USART.CTRLA.bit.MODE = 1;// 1 = Use internal clock
	SERCOM3->USART.CTRLA.bit.CMODE= 0;// 0 = Asynchronous mode clock
	SERCOM3->USART.CTRLA.bit.RXPO = 1;// 0x1 = PAD[1] SERCOM PAD[1] is used for data reception
	SERCOM3->USART.CTRLA.bit.TXPO = 0;// 0x0 = SERCOM PAD[0] SERCOM PAD[1] N/A N/A
	SERCOM3->USART.CTRLB.bit.CHSIZE = 0;// 0x0 = 8 bits
	SERCOM3->USART.CTRLA.bit.DORD = 1;// 0 = MSB is transmitted first, 1 = LSB is transmitted first
	SERCOM3->USART.CTRLA.bit.FORM = 0;// 0 = no parity
	SERCOM3->USART.CTRLB.bit.SBMODE = 0;// 0 = 1 stop bit , 1 = 2 stop bits

	// BAUD register: ASYNChronous formula ( 16 bits )
	// BAUD = 65536 * ( 1 - (16*(fBAUD/fRef) ) )
	// fRef = main clock frequency
	// ui16_aux = (uint16_t)65474;// ( 65326 * ( 1 - (16*(9600/163872768)) )) for 9600 baud at fRef = 163872768 LDR = 5000
	ui16_aux = (uint16_t)64799;// ( 65326 * ( 1 - (16*(115200/163872768)) )) for 115200 baud at fRef = 163872768 LDR = 5000
	SERCOM3->USART.BAUD.bit.BAUD = ui16_aux; // BAUD = 65536 * ( 1 - (16*(fBAUD/fRef) ) )
	SERCOM3->USART.CTRLB.bit.TXEN = 1; // 1= Tx Enabled
	SERCOM3->USART.CTRLB.bit.RXEN = 1; // 1= Rx Enabled
	SERCOM3->USART.CTRLA.bit.ENABLE = 1;// 1 =  Once all registers and I/O lines have been configured, enable SERCOM USART

	while (SERCOM3->USART.SYNCBUSY.bit.ENABLE == 1); // wait for USART to be enabled
	
	// **** SERCOM4 as SPI0 config:
	
	// Configure APB and CORE clocks for SERCOM4:
	// CLK_SERCOM4_APB (APBA): Peripheral Clock Masking
	MCLK->APBDMASK.bit.SERCOM4_= 1; // 1 = The APBD clock for the SERCOM4 is enabled.
	// GCLK_SERCOM4_CORE: #34 is the peripheral channel control register for SERCOM4: PCHCTRLm Mapping Index 7: 7 GCLK_SERCOM4_CORE SERCOM4 Core
	GCLK->PCHCTRL[34].bit.GEN = 0;  //0 = Generic Clock Generator 0, 1 = Generic Clock Generator 1, 2 Generic Clock Generator 2 ...
	GCLK->PCHCTRL[34].bit.CHEN = 1; //1 = clock CHANNEL ENABLE

	// Configure the SERCOM4 - SPI registers:
	SERCOM4->SPI.CTRLA.bit.ENABLE = 0;// 0 = Disable before configuring
	SERCOM4->SPI.CTRLA.bit.MODE = 3;// 0x3 = Master mode ( serial clock (SCK) is generated internally by the SERCOM Baud Rate Generator (BRG) )
	SERCOM4->SPI.CTRLA.bit.CPHA = 0;// Trailing Edge: Falling, change
	SERCOM4->SPI.CTRLA.bit.CPOL = 0;// Leading Edge: Rising, sample

	SERCOM4->SPI.CTRLA.bit.FORM =   0;// 0x0 = SPI frame
	SERCOM4->SPI.CTRLA.bit.DIPO =   2;// 0x2 = SERCOM PAD[2] is used as data input
	SERCOM4->SPI.CTRLA.bit.DOPO =   0;// 0x0 = PAD[0]:DO  PAD[1]:SCK
	SERCOM4->SPI.CTRLB.bit.CHSIZE = 0;// 0x0 = 8 bits
	SERCOM4->SPI.CTRLA.bit.DORD =   0;// 0 = MSB is transmitted first, 1 = LSB is transmitted first
	SERCOM5->SPI.CTRLB.bit.MSSEN =  0;// 0 = !SS disabled ( a GPIO is used instead for !CS )

	// BAUD register: SYNChronous formula ( 8 bits )
	// BAUD = fref / (2 * (BAUD+1) )
	// fRef = main clock frequency
	// fBAUD = 163872768 / (2 * (BAUD+1) )
	ui8_aux = (uint8_t)1; //0 // = 163872768 / 512
	SERCOM4->SPI.BAUD.bit.BAUD = ui8_aux;

	SERCOM4->SPI.CTRLB.bit.RXEN = 0; // 0= Rx Disabled
	SERCOM4->SPI.CTRLA.bit.ENABLE = 1;// 1 = Once all register and I/O lines have been configured, enable SERCOM SPI

	while (SERCOM4->SPI.SYNCBUSY.bit.ENABLE == 1); // wait for SPI to be enabled	
	
	// **** TC0 config:
	
	// CLK_xxxxx_APB (APBA): Peripheral Clock Masking
	MCLK->APBAMASK.bit.TC0_= 1; // 1 = The APBA clock for the TC0 is enabled.
	// GCLK_TC0_CORE: #9 is the peripheral channel control register for TC0 and TC1: PCHCTRLm Mapping Index 9: 9 GCLK_TC0, GCLK_TC1 TC0, TC1
	GCLK->PCHCTRL[9].bit.GEN = 0;  //0 = Generic Clock Generator 0, 1 = Generic Clock Generator 1, 2 Generic Clock Generator 2 ...
	GCLK->PCHCTRL[9].bit.CHEN = 1; //1 = clock CHANNEL ENABLE

	TC0->COUNT16.CTRLA.bit.ENABLE = 0;// 0 = Disable before configuring. Some registers can only be written when TC is disabled
	while (TC0->COUNT16.SYNCBUSY.reg);

	TC0->COUNT16.CTRLA.bit.SWRST = 1; // Writing a '1' to this bit resets all registers in the TC ( also interrupt bits), except DBGCTRL, to their initial state, and the TC will be disabled
	while (TC0->COUNT16.SYNCBUSY.reg);
	
	TC0->COUNT16.CTRLA.bit.MODE = 0x0; // 0x0 = COUNT16 Count in 16bit mode , 0x1 = COUNT8 Counter in 8-bit mode
	TC0->COUNT16.WAVE.bit.WAVEGEN = 0x1; // 0x0 = NFRQ:Operation: Normal frequency TOP VALUE is PER1 / Max , 0x1 = MFRQ: Match Frequency TOP VALUE is CC0 , 0x2: NPWM Single-slope PWM TOP VALUE is PER1 / Max , 0x3: MPWM Single-slope PWM TOP VALUE is CC0
	
	TC0->COUNT16.CTRLA.bit.PRESCALER = 0x1; // select Prescaler factor: 0x0 DIV1, 0x1 DIV2, 0x3 DIV8, 0x4 DIV16, 0x5 DIV64, 0x6 DIV256, 0x7 DIV1024
	TC0->COUNT16.CTRLBCLR.bit.ONESHOT = 0; // 0x0 continuously counting and restarting, 0x1 stops counting when it reaches ZERO or TOP. Writing a '1' to CTRLBCLR.bit.ONESHOT bit will disable one-shot operation.
	while (TC0->COUNT16.SYNCBUSY.reg);
	
	TC0->COUNT16.CTRLBCLR.bit.DIR = 0; // 0x0 counter increases, 0x1 counter decreases. Writing a '1' to CTRLBCLR.bit.DIR bit will clear the bit and make the counter count up.
	while (TC0->COUNT16.SYNCBUSY.reg);

	TC0->COUNT16.CC[0].reg = 1000;// Channel x Compare/Capture Value ( 16 bits mode )
	while (TC0->COUNT16.SYNCBUSY.reg);
		
	TC0->COUNT16.INTENSET.bit.OVF = 1; // enable the OVERFLOW interrupt in the peripheral register: when it is counting up and TOP is reached, the counter will be set to zero at the next tick (overflow) and the Overflow Interrupt Flag in the Interrupt Flag Status and Clear register (INTFLAG.OVF) will be set.
	while (TC0->COUNT16.SYNCBUSY.reg);
			
	TC0->COUNT16.CTRLA.bit.ENABLE = 1; // 1 = Once configured, enable TC0
	while (TC0->COUNT16.SYNCBUSY.reg);

	// NVIC_SetPriority(TC0_IRQn, 2);
	NVIC_EnableIRQ(TC0_IRQn); // enable TC0 interruption and vector line in the NVIC controller

	// once all peripheral have been configured, enable interrupts	
    __enable_irq();

}//GPIO_Init


void GPIO_SetDataLinesOutput(uint8_t ui8_data_value){
	uint16_t ui16_aux = 0;
	uint32_t ui32_aux = 0;


	// set to '0' the VALUE of I/O line  used to set '5-3.3V TRANSCEIVERs DIR' direction in order to configure
	// them in OUTPUT direction (B 3.3V to A 5.0V)  0 = OUTPUT (B 3.3V to A 5.0V ) , 1 = INPUT (A 5.0V to B 3.3V)
	PORT->Group[0].OUTCLR.reg = P0_DIR_DATA_BIT_MASK;

	// ########## PORT B

	// configure DIRECTION of I/O pins used for 'DATA'  signals: DIR bit = 1 OUTPUT , DIR bit = 0 INPUT
	PORT->Group[1].DIRSET.reg = P1_DATA_MASK;

	// configure Port B DATA bits
	// ATSMAD51  AdaFruitMetro4 labels ( disable the INEN bits for the cases where I/O lines are used as output):
	// PB10 - !CS
	// PB11 - R/!W
	// PB12 - !Data0
	// PB13 - !Data1
	// PB14 - !Data2
	// PB15 - !Data3
	// PB16 - CLCK1
	// PB17 - CLCK2
	PORT->Group[1].PINCFG[12].bit.INEN = 0;
	PORT->Group[1].PINCFG[13].bit.INEN = 0;
	PORT->Group[1].PINCFG[14].bit.INEN = 0;
	PORT->Group[1].PINCFG[15].bit.INEN = 0;

	// set the received value in the 'DATA' signals:
	// place received ui8_data_value in the right register position: P1_DATA_MASK = 0x0000F000
	ui32_aux = PORT->Group[1].OUT.reg;
	ui32_aux = ui32_aux & (~P1_DATA_MASK); // set to '0' the data bits in the OUT port
	ui32_aux = ui32_aux | ((ui8_data_value&0x0F)<<12); // 'or' the received data values in the output register state
	PORT->Group[1].OUT.reg = ui32_aux;
	
}//GPIO_SetDataLinesOutput


/*********************************************************************************************
* @brief
*********************************************************************************************/
void GPIO_SetCtrlLinesOutput(uint8_t ui8_ctrl_value){
	uint16_t ui16_aux = 0;
	uint32_t ui32_aux = 0;


    // set to '0' the VALUE of I/O line  used to set '5-3.3V TRANSCEIVERs DIR' direction in order to configure
    // them in OUTPUT direction (A 5.0V to B 3.3V)  0 = OUTPUT (B 3.3V to A 5.0V ) , 1 = INPUT (A 5.0V to B 3.3V)
    PORT->Group[0].OUTCLR.reg = P0_DIR_CTRL_BIT_MASK;

	// ########## PORT A

	// configure direction of PORTA CTRL DATA bits DIR bit = 1 OUTPUT , DIR bit = 0 INPUT
	PORT->Group[0].DIRSET.reg = P0_UNKNWN_CTRL_BIT_MASK|P0_ROM_CON_GPIO01|P0_ROM_CON_GPIO02|P0_ROM_CON_GPIO03;

	// configure Port A input bits INEN bit to disable sampling of INPUT pin:
	// PA02 - '?'
	// PA14 - GPIO1
	// PA15 - GPIO2
	// PA16 - GPIO3
	PORT->Group[0].PINCFG[2].bit.INEN = 0;
	PORT->Group[0].PINCFG[14].bit.INEN = 0;
	PORT->Group[0].PINCFG[15].bit.INEN = 0;
	PORT->Group[0].PINCFG[16].bit.INEN = 0;

	// set the state of the P0_UNKNWN_CTRL_BIT_MASK, P0_ROM_CON_GPIO01, P0_ROM_CON_GPIO02, P0_ROM_CON_GPIO03 pins to the same
	// state of the corresponding bits received in ui8_ctrl_value in the OUT register position:
	// VAR_CTRL_nUNKNOWN_MASK VAR_CTRL_GPIO01 VAR_CTRL_GPIO02 VAR_CTRL_GPIO03
	ui32_aux = PORT->Group[0].OUT.reg;
	ui32_aux = ui32_aux & (~(P0_UNKNWN_CTRL_BIT_MASK)); // set to '0' the '?' bit in the OUT port
	ui32_aux = ui32_aux  | ((ui8_ctrl_value&(VAR_CTRL_nUNKNOWN_MASK))>>2);// 'or' the received '?' value in the output register state
	ui32_aux = ui32_aux & (~(P0_ROM_CON_GPIO01|P0_ROM_CON_GPIO02|P0_ROM_CON_GPIO03)); // set to '0' the GPIO1, GPIO2 and GPIO3 bits in the OUT port
	ui32_aux = ui32_aux  | ((ui8_ctrl_value&(VAR_CTRL_GPIO01|VAR_CTRL_GPIO02|VAR_CTRL_GPIO03))<<9);// 'or' the received GPIO1, GPIO2 and GPIO3 values in the output register state
	PORT->Group[0].OUT.reg = ui32_aux;

	// ########## PORT B
	
	// configure direction of PORTB CTRL DATA bits DIR bit = 1 OUTPUT , DIR bit = 0 INPUT
	PORT->Group[1].DIRSET.reg = P1_CTRL_MASK;

	// configure Port B input bits INEN bits to disable sampling of INPUT pins:
	// PB10 - !CS
	// PB11 - R/!W
	// PB16 - CLCK1
	// PB17 - CLCK2
	PORT->Group[1].PINCFG[10].bit.INEN = 0;
	PORT->Group[1].PINCFG[11].bit.INEN = 0;
	PORT->Group[1].PINCFG[16].bit.INEN = 0;
	PORT->Group[1].PINCFG[17].bit.INEN = 0;
			
	// set the state of the P1_nCLCK2_BIT, P1_nCLCK1_BIT, P1_nCS_BIT, P1_RnW_BIT_MASK pins to the same state of the
	// corresponding bits received in ui8_ctrl_value in the OUT register position:
	ui32_aux = PORT->Group[1].OUT.reg;
	ui32_aux = ui32_aux & (~(P1_nCLCK2_BIT_MASK|P1_nCLCK1_BIT_MASK)); // set to '0' the CLCK1 and CLCK2 CTRL bits in the OUT port
	ui32_aux = ui32_aux | ((ui8_ctrl_value&(VAR_CTRL_nCLCK2_MASK|VAR_CTRL_nCLCK1_MASK))<<14);// 'or' the received CLCK1 and CLCK2 CTRL values in the output register state
	ui32_aux = ui32_aux & (~(P1_RnW_BIT_MASK|P1_nCS_BIT_MASK)); // set to '0' the RnW and nCS CTRL bits in the OUT port
	ui32_aux = ui32_aux | ((ui8_ctrl_value&(VAR_CTRL_RnW_MASK|VAR_CTRL_nCS_MASK))<<10);// 'or' the received RnW and nCS CTRL values in the output register state
	PORT->Group[1].OUT.reg = ui32_aux;    
   
}//GPIO_SetCtrlLinesOutput


/*********************************************************************************************
* @brief
*********************************************************************************************/
uint8_t GPIO_GetDataLinesInput(){
	uint8_t ui8_ret_val = 0;
	uint32_t ui32_aux = 0;


	// set to '1' the VALUE of I/O line  used to set '5-3.3V TRANSCEIVERs DIR' direction in order to
	// configure them in INPUT direction (A 5.0V to B 3.3V)  0 = OUTPUT (B 3.3V to A 5.0V ) , 1 = INPUT (A 5.0V to B 3.3V)
	PORT->Group[0].OUTSET.reg = P0_DIR_DATA_BIT_MASK;

    ui8_ret_val = 0;
	
	// ########## PORT B

	// configure DIRECTION of I/O pins used for 'DATA'  signals: DIR bit = 1 OUTPUT , DIR bit = 0 INPUT
	PORT->Group[1].DIRCLR.reg = P1_DATA_MASK;

	// configure Port B DATA bits
	// Enable INEN bits to enable sampling
	// PB12 - !Data0
	// PB13 - !Data1
	// PB14 - !Data2
	// PB15 - !Data3
	PORT->Group[1].PINCFG[12].bit.INEN = 1;
	PORT->Group[1].PINCFG[13].bit.INEN = 1;
	PORT->Group[1].PINCFG[14].bit.INEN = 1;
	PORT->Group[1].PINCFG[15].bit.INEN = 1;

	// get the received value in the 'DATA' signals:
	ui32_aux=PORT->Group[1].IN.reg;
	ui8_ret_val = (uint8_t)((ui32_aux & P1_DATA_MASK)>>12);
	
	return 	ui8_ret_val;

}//GPIO_GetDataLinesInput


/*********************************************************************************************
* @brief
*********************************************************************************************/
uint8_t GPIO_GetCtrlLinesInput(){
	uint8_t ui8_ret_val = 0;
	uint32_t ui32_aux = 0;


	// set to '1' the VALUE of I/O line  used to set '5-3.3V TRANSCEIVERs DIR' direction in order to configure
	// them in INPUT direction (A 5.0V to B 3.3V)  0 = OUTPUT (B 3.3V to A 5.0V ) , 1 = INPUT (A 5.0V to B 3.3V)
	PORT->Group[0].OUTSET.reg = P0_DIR_CTRL_BIT_MASK;

	ui8_ret_val = 0;

	// ########## PORT A

	// configure direction of PORTA CTRL DATA bits DIR bit = 1 OUTPUT , DIR bit = 0 INPUT
	PORT->Group[0].DIRCLR.reg = P0_UNKNWN_CTRL_BIT_MASK|P0_ROM_CON_GPIO01|P0_ROM_CON_GPIO02|P0_ROM_CON_GPIO03;

	// configure Port A input bits INEN bit to enable sampling of INPUT pin
	// PA02 - '?'
	// PA14 - GPIO1
	// PA15 - GPIO2
	// PA16 - GPIO3
	PORT->Group[0].PINCFG[2].bit.INEN = 1;
	PORT->Group[0].PINCFG[14].bit.INEN = 1;
	PORT->Group[0].PINCFG[15].bit.INEN = 1;
	PORT->Group[0].PINCFG[16].bit.INEN = 1;
	
	// get the state of the PORTA P0_UNKNWN_CTRL_BIT and store it in the corresponding position of the result variable
	ui32_aux=PORT->Group[0].IN.reg;
	ui8_ret_val = ui8_ret_val | (uint8_t)((ui32_aux & P0_UNKNWN_CTRL_BIT_MASK)<<2);
	ui8_ret_val = ui8_ret_val | (uint8_t)((ui32_aux & (P0_ROM_CON_GPIO01|P0_ROM_CON_GPIO02|P0_ROM_CON_GPIO03) )>>9);

	// ########## PORT B
	
	// configure direction of PORTB CTRL DATA bits DIR bit = 1 OUTPUT , DIR bit = 0 INPUT
	PORT->Group[1].DIRCLR.reg = PORT->Group[1].DIRCLR.reg | P1_CTRL_MASK;

	// configure Port B input bits INEN bits to enable sampling of INPUT pins
	// PB10 - !CS
	// PB11 - R/!W
	// PB16 - CLCK1
	// PB17 - CLCK2
	PORT->Group[1].PINCFG[10].bit.INEN = 1;
	PORT->Group[1].PINCFG[11].bit.INEN = 1;
	PORT->Group[1].PINCFG[16].bit.INEN = 1;
	PORT->Group[1].PINCFG[17].bit.INEN = 1;

	// get the state of PORTB nCLCK1 and nCLCK2 CTRL bits and store then in the corresponding position of the result variable
	ui32_aux = PORT->Group[1].IN.reg;
	ui8_ret_val = ui8_ret_val | (uint8_t)((ui32_aux & (P1_nCLCK2_BIT_MASK|P1_nCLCK1_BIT_MASK))>>14);
	ui8_ret_val = ui8_ret_val | (uint8_t)((ui32_aux & (P1_RnW_BIT_MASK|P1_nCS_BIT_MASK))>>10);
	
	return 	ui8_ret_val;
	
}//GPIO_GetCtrlLinesInput