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
#ifndef USER_IFACE_H_
#define USER_IFACE_H_

#include "sam.h"
#include "gpio.h"
#include "sys_defs.h"
#include "timing.h"


#define USER_IFACE_ENC_1			      0
#define USER_IFACE_MAX_ENCODERS           1// the number of encoders on the user interface

#define USER_IFACE_MAX_PUSH_BUTTONS       5// the number of push buttons on the user interface
#define USER_IFACE_PUSHBT_ENC             0// PUSB ENCODER pushbutton
#define USER_IFACE_PUSHBT_1               1// PORTA-13: PA13 - PUSHB_D in schematic
#define USER_IFACE_PUSHBT_2               2// PORTA-10: PA10 - PUSHB_A in schematic
#define USER_IFACE_PUSHBT_3               3// PORTA-12: PA12 - PUSHB_C in schematic
#define USER_IFACE_PUSHBT_4               4// PORTA-11: PA11 - PUSHB_B in schematic

#define USER_IFACE_PRESSED		0
#define USER_IFACE_NOT_PRESSED	1

#define USER_IFACE_DEBOUNCE_CICLES_FAST        10// number of cycles that the pushbuttons will remain disabled after having been pushed
#define USER_IFACE_DEBOUNCE_CICLES_SLOW        20// number of cycles that the pushbuttons will remain disabled after having been pushed

#define USER_IFACE_PUSHBT_READ_WAIT_TIME_FAST  TIMMING_TICKS_100_MS+TIMMING_TICKS_50_MS// time to wait to continue sampling the push buttons once the push button has been pressed
#define USER_IFACE_PUSHBT_READ_WAIT_TIME_SLOW  TIMMING_TICKS_500_MS// time to wait to continue sampling the push buttons once the push button has been pressed

#define USER_IFACE_PUSHBT_UP              USER_IFACE_PUSHBT_2
#define USER_IFACE_PUSHBT_DOWN            USER_IFACE_PUSHBT_3
#define USER_IFACE_PUSHBT_RIGHT           USER_IFACE_PUSHBT_4
#define USER_IFACE_PUSHBT_LEFT            USER_IFACE_PUSHBT_1



// When turning to RIGHT direction it generates the sequence: 10,00,01,11, 10,00,01,11,  10,00,01,11, ... When turning to LEFT direction it
// generates the inverse sequence: 11,01,00,10, 11,01,00,10, 11,01,00,01 ... so we will be able to find on which direction is turning by
// checking the sequence it generates. A filter will should be applied to remove noise from the inputs and avoid errors when detecting direction.

// with all the information of an encoder
// on each encoder line A and line B rise or fall interrupt, all .ui8_last_sequence
// bits are shifted 2 positions to the left and the value of both lines is sampled
// and stored in the lowest 2 bits of the .ui8_last_sequence field of the:
// encoder struct
//
//  Counter ClockWise = 0x87:
//    A ____________         ______________
//                1 |_0___0_| 1
//    B ________         __________________
//              |_0___0_| 1   1
//
// last_sequence:  10  00  01  11
//    1000  0111 = 0x87
//
//  Counter ClockWise = 0x4B:
//    A ________         __________________
//              |_0___0_| 1   1
//    B ____________         ______________
//                1 |_0___0_| 1
//
// last_sequence:  01  00  10  11
//    0100  1011 = 0x4B
typedef struct{
	uint8_t ui8_last_sequence; // byte with the encoder state. Each two bits codify a state, less significant two bits are the most recent state, more significant two bits are the less recent state
}t_encoder;

// with all the information of a button
typedef struct{
	TIMING_TICKS tticks_timer; // to control the time to wait before continue sampling that push buttons once that push button has been pressed
	uint16_t ui16_debounce_counter; // counter used to avoid successive pushing ( debounce ) when pressing the button. It disables the pushbutton until it reaches 0 again
}t_push_button;


void EIC_7_Handler( void ); // Declared in C:\Program Files (x86)\Atmel\Studio\7.0\Packs\atmel\SAMD51_DFP\1.0.70\include\samd51g19a.h
void NMI_Handler( void ); // Declared in C:\Program Files (x86)\Atmel\Studio\7.0\Packs\atmel\SAMD51_DFP\1.0.70\include\samd51g19a.h

/*********************************************************************************************
* @brief creates and initializes all necessary resources for the user interface ( knobs and push buttons ... )
* @return 
* 	 FALSE: if could not initialize the input user interface
*   TRUE: if could initialize the input user interface
*********************************************************************************************/
void USER_IFACE_Init();

/*********************************************************************************************
* @brief applies the debouncing filter control and returns the state of the pushbutton
* @param[in] ui8_pushbutton_number with the number of pushbutton that has to be updated
* @param[in] ui8_pushbutton_state with the last read value of the pushbutton input port
* @return 0 or 1 indicating the state of the pushbutton.
* @note  Pay attention to the fact that the pushbuttons use inverse logic. This is
* because they use microcontroller's internal pull up resistors of the GPIOs, so by
* default, if the user is not pressing any key, the read value through these pins is "1".
* When the user presses a button then the read value is "0". So when a "0" is read a
* it means that the push button is pressed.
*********************************************************************************************/
uint8_t USER_IFACE_process_pushbutton(uint8_t ui8_pushbutton_number, uint8_t ui8_pushbutton_state);

/*********************************************************************************************
* @brief reads, processes all the GPIOs, and returns the state of all the encoders
* @param[out] i16_encoders_var_value array with the variation of value of the configured encoders
* @return
*    1: if the encoders state could be sampled
*    0: if the encoders state could NOT be sampled
*********************************************************************************************/
uint8_t USER_IFACE_sample_encoders(int16_t * i16_encoders_var_value );

/*********************************************************************************************
* @brief reads and processes all the GPIOs of the pushbuttons
* @param[out] ui8_buttons_state array with the state of each pushbuttons
*********************************************************************************************/
void USER_IFACE_sample_pushbuttons(uint8_t * ui8_buttons_state);

/*********************************************************************************************
* @brief  clears the buffer used to store the state of the push buttons
* @param[out] ui8_buttons_state array with the state of the push buttons that is going to be cleared
*********************************************************************************************/
void USER_IFACE_pusbuttons_clear(uint8_t * ui8_buttons_state);

/*********************************************************************************************
* @brief clears the buffer used to store the state of the encoders
* @param[in] i16_encoders_var_value: array with the current encoder values
* @param[out] encoders_var_value: array with the variation of value of the encoders with all values cleared
*********************************************************************************************/
void USER_IFACE_encoders_clear(int16_t * i16_encoders_var_value);

#endif // USER_IFACE_H_
