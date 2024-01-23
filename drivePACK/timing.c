// *****************************************************************
// ****                       drivePACK                         ****
// ****                  www.tolaemon.com/dpack                 ****
// ****                       Source code                       ****
// ****                       01/04/2020                        ****
// ****                     Jordi Bartolome                     ****
// ****                                                         ****
// **** IMPORTANT:                                              ****
// **** Using this code or any part of it means accepting all   ****
// **** conditions exposed in: http://www.tolaemon.com/dpack    ****
// **** This code is part of the bareSAM platform. All info at: ****
// **** http://www.tolaemon.com/baresam                         ****
// *****************************************************************
#include "timing.h"



// 02-08-2020  Jordi Bartolome
// Function: void TC0_Handler ( void )
// TC0 interrupt handler. It is called when Interrupt line 85 is active: TCC0 - Timer Counter Control 0 corresponds 
// to Interrupt line 85  which holds all these interruption causes: CNT A, DFS A, ERR A, FAULTA A, FAULTB A, FAULT0 A, 
// FAULT1 A, OVF, TRG, UFS A 
// Receives:
// Returns:
//  By reference:
//  By value:
void TC0_Handler ( void ){
	
	// many interruption causes may trigger the interrupt TC0, so try to find
	// out which of the interrupt causes has triggered the interrupt.
	if (TC0->COUNT16.INTFLAG.bit.OVF>=1){
		tc_ticks_since_start ++;
		// Writing a '1' to one of these bits will clear the corresponding Match or Capture Channel x interrupt flag. In capture
		// operation, this flag is automatically cleared when CCx register is read. So write 1 to clear the interrupt flag in 
		// the Interrupt Flag  Status and Clear register (INTFLAG) until next interrupt condition occurs,
		TC0->COUNT16.INTFLAG.bit.OVF = 1;
	}
	
}//TC0_Handler



void TIMING_Disable_TC0(){
	
	// disables the TC0 interrupt
	TC0->COUNT16.CTRLA.bit.ENABLE = 0;
	while (TC0->COUNT16.SYNCBUSY.reg);
	
}//TIMING_Disable_TC0



void TIMING_Restore_TC0(){
	
	// enables the TC0 interrupt
	TC0->COUNT16.CTRLA.bit.ENABLE = 1; 
	while (TC0->COUNT16.SYNCBUSY.reg);
	
}//TIMING_Restore_TC0



void TIMING_Init(){
	
	tc_ticks_since_start = 0;
	
}//TIMING_Init



TIMING_TICKS TIMING_GetTicksSinceStart(){
	
	return tc_ticks_since_start;
	
}//TIMING_GetTicksSinceStart



TIMING_TICKS TIMING_SetTicksDelay(uint32_t desired_delay){
	
	return (TIMING_TICKS)(tc_ticks_since_start + desired_delay);
	
}//TIMING_SetTicksDelay



uint8_t TIMING_CheckTicksDelayReached(TIMING_TICKS ticks_to_reach){
	uint8_t ui8_ret_val = FALSE;
	
	if (ticks_to_reach>=tc_ticks_since_start){
		ui8_ret_val = FALSE;
	}else{
		ui8_ret_val = TRUE;		
	}
	
	return ui8_ret_val;
	
}//TIMING_CheckTicksDelayReached



void TIMMING_BlockingDelay(uint32_t ui32_desired_delay){
    TIMING_TICKS tticks_timer;
        
    tticks_timer = TIMING_SetTicksDelay(ui32_desired_delay);
    while (TIMING_CheckTicksDelayReached(tticks_timer)==FALSE){1;}
		
}//TIMMING_BlockingDelay



void TIMING_ActiveDelay(uint16_t cycles){
	unsigned int ui_aux=0;
	unsigned int ui_aux2=0;
		
	for (ui_aux=0;ui_aux<cycles;ui_aux++){
		ui_aux2++;
		ui_aux2--;
	}//for

}//TIMING_ActiveDelay