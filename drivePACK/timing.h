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
// *****************************************************************
#ifndef TIMING_H_
#define TIMING_H_

#include "sam.h"
#include "gpio.h"
#include "sys_defs.h"


typedef uint64_t TIMING_TICKS;


// As TC register configuration is ( check gpio.c to confirm !!!! ):
//    GCLK->PCHCTRL[9].bit.GEN = 0; //0 = Generic Clock Generator 0 = 163872768
//    TC0->COUNT16.CTRLA.bit.MODE = 0x0; // 0x0 = COUNT16 Count in 16bit mode
//    TC0->COUNT16.WAVE.bit.WAVEGEN = 0x1; 0x1 = MFRQ: Match Frequency TOP VALUE is CC0
//    TC0->COUNT16.CTRLBCLR.bit.DIR = 0; // 0x0 counter increases, 0x1 counter decreases.
//    TC0->COUNT16.CC[0].reg = 1000;// Channel x Compare/Capture Value ( 16 bits mode )
//    TC0->COUNT16.CTRLA.bit.PRESCALER = 0x1;// select Prescaler factor: 0x0 DIV1, 0x1 DIV2,
// Then counter is increased each:
//    163872768 / 2 / 1000 = 81936 interrupts per second
//    1 / 81936 = 12,20uS
#define TIMMING_TICKS_1_MS      82
#define TIMMING_TICKS_2_MS      164
#define TIMMING_TICKS_3_MS      246
#define TIMMING_TICKS_4_MS      328
#define TIMMING_TICKS_5_MS      410
#define TIMMING_TICKS_10_MS     819
#define TIMMING_TICKS_20_MS     1639
#define TIMMING_TICKS_30_MS     2458
#define TIMMING_TICKS_40_MS     3277
#define TIMMING_TICKS_50_MS     4097
#define TIMMING_TICKS_100_MS    8194
#define TIMMING_TICKS_200_MS    16387
#define TIMMING_TICKS_300_MS    24581
#define TIMMING_TICKS_400_MS    32775
#define TIMMING_TICKS_500_MS    40968
#define TIMMING_TICKS_1_S       81936
#define TIMMING_TICKS_2_S       163873
#define TIMMING_TICKS_3_S       245809
#define TIMMING_TICKS_4_S       327746
#define TIMMING_TICKS_5_S       409682


TIMING_TICKS tc_ticks_since_start;

void TC0_Handler ( void );

// 04-05-2021  Jordi Bartolome
// Disables the TC0 interrupt used as reference for the timers.
// Receives:
// Returns:
//  By reference:
//  By value:
void TIMING_Disable_TC0();

// 04-05-2021  Jordi Bartolome
// Enables the TC0 interrupt used as reference for the timers.
// Receives:
// Returns:
//  By reference:
//  By value:
void TIMING_Restore_TC0();

// 02-08-2020  Jordi Bartolome
// Initializes the TIMING module variables
// Receives:
// Returns:
//  By reference:
//  By value:
void TIMING_Init();

// 02-08-2020  Jordi Bartolome
// Returns the ticks that have been counted by the corresponding system timer since the
// CPU has been started
// Receives:
// Returns:
//  By reference:
//  By value:
//      The he ticks that have been counted since the CPU has been started
TIMING_TICKS TIMING_GetTicksSinceStart();

// 02-08-2020  Jordi Bartolome
// Receives a number of ticks with corresponding to a desired delay, and returns the value
// that the internal system ticks counter should have when that number of ticks has elapsed.
// Receives:
//   desired_delay: the number of ticks of delay ( the number of ticks to add to current system ticks ).
// Returns:
//  By reference:
//  By value:
//     The value that the internal system ticks counter will have when the specified desired_delay
//  ticks have elapsed.
TIMING_TICKS TIMING_SetTicksDelay(uint32_t desired_delay);

// 02-08-2020  Jordi Bartolome
// Receives a number of ticks including the desired delay and checks if this number of ticks has been
// reached or not by the the internal system clock.
// Receives:
//   desired_delay: the number of ticks to check if have been reached or not by the internal system clock.
// Returns:
//  By reference:
//  By value:
//     TRUE if the internal system clock has reached the value received in ticks_to_reach parameter.
//     FALSE if the internal system clock has NOT reached yet the value received in ticks_to_reach parameter,
uint8_t TIMING_CheckTicksDelayReached(TIMING_TICKS ticks_to_reach);

// 04-04-2022  Jordi Bartolome
// Implements an active pause with a stupid loop waiting for the specified time specified 
// in ui32_desired_delay.
// Receives:
//   ui32_desired_delay:
// Returns:
// Note: AVOID USING THIS FUNCTION, USE IT ONLY WHEN IT IS STRICTLY NECESSARY !!!! Use
// other non blocking timing control functions to implement pause when possible
void TIMMING_BlockingDelay(uint32_t ui32_desired_delay);

// 02-08-2020  Jordi Bartolome
// Implements an active pause with a stupid loop. It may be useful when the OS has not
// been started because the vTaskDelay(x) can only be called when the OS is started.
// Receives:
// Returns:
// Note: AVOID USING THIS FUNCTION, USE IT ONLY WHEN IT IS STRICTLY NECESSARY !!!! Use
// other non blocking timing control functions to implement pause when possible
void TIMING_ActiveDelay(uint16_t cycles);

#endif /* TIMING_H_ */