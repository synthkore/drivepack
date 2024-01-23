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

/*********************************************************************************************
* @brief TC0 timer interrupt handler
*********************************************************************************************/
void TC0_Handler ( void );

/*********************************************************************************************
* @brief Disables the TC0 interrupt used as reference for the timers.
* @note 04-05-2021  Jordi Bartolome
*********************************************************************************************/
void TIMING_Disable_TC0();

/*********************************************************************************************
* @brief Enables the TC0 interrupt used as reference for the timers.
* @note 04-05-2021  Jordi Bartolome
*********************************************************************************************/
void TIMING_Restore_TC0();

/*********************************************************************************************
* @brief Initializes the TIMING module variables
* @note 02-08-2020 Jordi Bartolome
*********************************************************************************************/
void TIMING_Init();

/*********************************************************************************************
* @brief Returns the ticks that have been counted by the corresponding system timer since the
* CPU has been started
* @return the he ticks that have been counted since the CPU has been started
* @note 02-08-2020 Jordi Bartolome
*********************************************************************************************/
TIMING_TICKS TIMING_GetTicksSinceStart();

/*********************************************************************************************
* @brief Receives a number of ticks with corresponding to a desired delay, and returns the value
* that the internal system ticks counter should have when that number of ticks has elapsed.
* @param[in] desired_delay the number of ticks of delay ( the number of ticks to add to current
* system ticks ).
* @return The value that the internal system ticks counter will have when the specified desired_delay
* ticks have elapsed.
* @note 02-08-2020 Jordi Bartolome
*********************************************************************************************/
TIMING_TICKS TIMING_SetTicksDelay(uint32_t desired_delay);

/*********************************************************************************************
* @brief Receives a number of ticks including the desired delay and checks if this number of ticks has 
* been reached or not by the the internal system clock.
* @param[in] desired_delay the number of ticks to check if have been reached or not by the internal 
* system clock.
* @return  TRUE if the internal system clock has reached the value received in ticks_to_reach parameter.
* FALSE if the internal system clock has NOT reached yet the value received in ticks_to_reach parameter,
* @note 02-08-2020  Jordi Bartolome
*********************************************************************************************/
uint8_t TIMING_CheckTicksDelayReached(TIMING_TICKS ticks_to_reach);

/*********************************************************************************************
* @brief Implements an active pause with a stupid loop waiting for the specified time specified 
* in ui32_desired_delay.
* @param[in] ui32_desired_delay the number of ticks that the CPU will keep blocked.
* Returns:
* @note AVOID USING THIS FUNCTION, USE IT ONLY WHEN IT IS STRICTLY NECESSARY !!!! Use
* other non blocking timing control functions to implement pause when possible
* 04-04-2022 Jordi Bartolome
*********************************************************************************************/
void TIMMING_BlockingDelay(uint32_t ui32_desired_delay);

/*********************************************************************************************
* @brief Implements an active pause with a stupid loop. It may be useful when the OS has not
* been started because the vTaskDelay(x) can only be called when the OS is started.
* @param[in] the number of ++ then -- loops to execute in the loop
* @note AVOID USING THIS FUNCTION, USE IT ONLY WHEN IT IS STRICTLY NECESSARY !!!! Use
* other non blocking timing control functions to implement pause when possible
* @note 02-08-2020  Jordi Bartolome
*********************************************************************************************/
void TIMING_ActiveDelay(uint16_t cycles);

#endif /* TIMING_H_ */