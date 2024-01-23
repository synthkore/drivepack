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
#include "user_iface.h"

t_encoder encoder[USER_IFACE_MAX_ENCODERS]; // with the encoders data
t_push_button button[USER_IFACE_MAX_PUSH_BUTTONS];//with the state of the push buttons	



void EIC_7_Handler( void ){

	if ( (EIC->INTFLAG.reg & 0x0080)!=0 ){
		
		// Encoder 1 A line is connected to PA07 with EIC7 enabled
		
		// update the encoder A B lines read sequence: each time an encoder interrupts occurs, sample both encoder 
		// outputs ( A and B) and store that samples values in the lowest 2 bits of .ui8_last_sequence
		encoder[USER_IFACE_ENC_1].ui8_last_sequence = encoder[USER_IFACE_ENC_1].ui8_last_sequence << 2;
		encoder[USER_IFACE_ENC_1].ui8_last_sequence = encoder[USER_IFACE_ENC_1].ui8_last_sequence & 0xFC;
		if ((PORT->Group[0].IN.reg & P0_USER_IFACE_ENC1_A)!=0) encoder[USER_IFACE_ENC_1].ui8_last_sequence = encoder[USER_IFACE_ENC_1].ui8_last_sequence | 0x02;
		if ((PORT->Group[0].IN.reg & P0_USER_IFACE_ENC1_B)!=0) encoder[USER_IFACE_ENC_1].ui8_last_sequence = encoder[USER_IFACE_ENC_1].ui8_last_sequence | 0x01;

		// The flag bit x is cleared by writing a '1' to it.
		EIC->INTFLAG.reg = EIC->INTFLAG.reg | 0x0080;
		
	}else{
		// spurious interrupt?, clear all interrupt flags, clear all EIC interrupt flags
        EIC->INTFLAG.reg = EIC->INTFLAG.reg; 
	}//if

}//EIC_7_Handler



void NMI_Handler( void ){

    // Encoder 1 B line is connected to PA08 with NMI enabled

	// update the encoder A B lines read sequence: each time an encoder interrupts occurs, sample both encoder 
	// outputs ( A and B) and store that samples values in the lowest 2 bits of .ui8_last_sequence
	encoder[USER_IFACE_ENC_1].ui8_last_sequence = encoder[USER_IFACE_ENC_1].ui8_last_sequence << 2;
	encoder[USER_IFACE_ENC_1].ui8_last_sequence = encoder[USER_IFACE_ENC_1].ui8_last_sequence & 0xFC;
	if ((PORT->Group[0].IN.reg & P0_USER_IFACE_ENC1_A)!=0) encoder[USER_IFACE_ENC_1].ui8_last_sequence = encoder[USER_IFACE_ENC_1].ui8_last_sequence | 0x02;
	if ((PORT->Group[0].IN.reg & P0_USER_IFACE_ENC1_B)!=0) encoder[USER_IFACE_ENC_1].ui8_last_sequence = encoder[USER_IFACE_ENC_1].ui8_last_sequence | 0x01;
	
	EIC->NMIFLAG.bit.NMI = 0x1;// NMI Non-Maskable Interrupt Flag is cleared by writing a '1' to it. This flag is set when the NMI pin matches the NMI sense configuration, and will generate an interrupt request.
	
}//NMI_Handler



void USER_IFACE_Init(){
	uint8_t return_value=FALSE;
	uint8_t i;


	// initialize to 0 all the encoders structures.
	for (i=0;i<USER_IFACE_MAX_ENCODERS;i++){
		encoder[i].ui8_last_sequence=0;
	}//for

	// initialize to 0 all the pushbutton variables
	for (i=0;i<USER_IFACE_MAX_PUSH_BUTTONS;i++){
		button[i].tticks_timer = 0;
		button[i].ui16_debounce_counter=0;
	}//for


	return return_value;

}// USER_IFACE_init



uint8_t USER_IFACE_process_pushbutton(uint8_t ui8_pushbutton_number, uint8_t ui8_pushbutton_state){
	uint8_t ret_val=USER_IFACE_NOT_PRESSED;


        
	if ( (ui8_pushbutton_state==0) && (TIMING_CheckTicksDelayReached(button[ui8_pushbutton_number].tticks_timer)) ){
		
		// the button is now pressed ( == 0 ) and the configured time since the last 
		// time the push button was pressed has elapsed so check the debounce counter
		if (button[ui8_pushbutton_number].ui16_debounce_counter>0){
		
			// debounce filter is yet >0, so we decrease the debounce counter until it 
			// reaches 0. The push button is considered to be finally pressed when its GPIO
			// is read as pressed during at least USER_IFACE_DEBOUNCE_CICLES cycles.
			button[ui8_pushbutton_number].ui16_debounce_counter--;
			ret_val=USER_IFACE_NOT_PRESSED;
		
		}else{			
			
			// the push button is considered to be finally pressed when its GPIO is read
			// as pressed during at least USER_IFACE_DEBOUNCE_CICLES cycles. Restart all
			// the timers and counters used to filter the push button detection.
			switch (ui8_pushbutton_number){
				case USER_IFACE_PUSHBT_1:
				case USER_IFACE_PUSHBT_4:
				case USER_IFACE_PUSHBT_ENC:
					button[ui8_pushbutton_number].ui16_debounce_counter = USER_IFACE_DEBOUNCE_CICLES_SLOW;
					button[ui8_pushbutton_number].tticks_timer =  TIMING_SetTicksDelay(USER_IFACE_PUSHBT_READ_WAIT_TIME_SLOW);
					break;

				case USER_IFACE_PUSHBT_2:
				case USER_IFACE_PUSHBT_3:
				default:	
					button[ui8_pushbutton_number].ui16_debounce_counter = USER_IFACE_DEBOUNCE_CICLES_FAST;
					button[ui8_pushbutton_number].tticks_timer =  TIMING_SetTicksDelay(USER_IFACE_PUSHBT_READ_WAIT_TIME_FAST);
					break;
			}//switch
			ret_val=USER_IFACE_PRESSED;
		}//if
		
	//}else{
    //
	//	// the push button is not pressed so initialize the debounce counter filter to the max value
	// 	button[ui8_pushbutton_number].ui16_debounce_counter = USER_IFACE_DEBOUNCE_CICLES;
	// 	ret_val=USER_IFACE_NOT_PRESSED;
	//	
	}//if
	
	return ret_val;
	
}//USER_IFACE_process_pushbutton



uint8_t USER_IFACE_sample_encoders(int16_t * i16_encoders_var_value ){
	uint8_t ret_val=0;
	uint8_t i;

	
	// read the pins and process each one of the configured encoders
	for (i=0;i<USER_IFACE_MAX_ENCODERS;i++){

		if (encoder[i].ui8_last_sequence==0x87){
			
			// A ____________         ______________
			//             1 |_0___0_| 1
			// B ________         __________________
			//           |_0___0_| 1   1
			//
			// last_sequence:  10  00  01  11
			// 1000  0111 = 0x87			
			i16_encoders_var_value[i]--;
			encoder[i].ui8_last_sequence = 0x00;
			
		}else if (encoder[i].ui8_last_sequence==0x4B){
			
			// A ________         __________________
			//           |_0___0_| 1   1
			// B ____________         ______________
			//             1 |_0___0_| 1
			//
			// last_sequence:  01  00  10  11
			// 0100  1011 = 0x4B		
			i16_encoders_var_value[i]++;
			encoder[i].ui8_last_sequence = 0x00;
			
		}else if ((encoder[i].ui8_last_sequence&0x7)==0x07) {
			
			// two lowest bits are 1, so the encoder lines are IDLE but
			// no valid sequence has been identified, so clear the variables
			encoder[i].ui8_last_sequence = 0x00;
			
		}//if

	}//for
	
	return ret_val;

}//USER_IFACE_sample_encoders



void USER_IFACE_sample_pushbuttons(uint8_t * buttons_state){
	uint8_t ui8_aux;
	uint32_t ui32_aux;


    ui32_aux = PORT->Group[0].IN.reg;
		
	ui8_aux =  (uint8_t) ((ui32_aux & P0_USER_IFACE_PUSHB_ENC1) != 0);
	buttons_state[USER_IFACE_PUSHBT_ENC]=USER_IFACE_process_pushbutton(USER_IFACE_PUSHBT_ENC,ui8_aux);
	
	ui8_aux =  (uint8_t) ((ui32_aux & P0_USER_IFACE_PUSHB_2) != 0);
	buttons_state[USER_IFACE_PUSHBT_2]=USER_IFACE_process_pushbutton(USER_IFACE_PUSHBT_2,ui8_aux);
	
	ui8_aux =  (uint8_t) ((ui32_aux & P0_USER_IFACE_PUSHB_4) != 0);
	buttons_state[USER_IFACE_PUSHBT_4]=USER_IFACE_process_pushbutton(USER_IFACE_PUSHBT_4,ui8_aux);

	ui8_aux =  (uint8_t) ((ui32_aux & P0_USER_IFACE_PUSHB_3) != 0);
	buttons_state[USER_IFACE_PUSHBT_3]=USER_IFACE_process_pushbutton(USER_IFACE_PUSHBT_3,ui8_aux);

	ui8_aux =  (uint8_t) ((ui32_aux & P0_USER_IFACE_PUSHB_1) != 0);
	buttons_state[USER_IFACE_PUSHBT_1]=USER_IFACE_process_pushbutton(USER_IFACE_PUSHBT_1,ui8_aux);

}//USER_IFACE_sample_pushbuttons



void USER_IFACE_pusbuttons_clear( uint8_t * buttons_state){
	uint8_t ch_aux;

		    
	// clear all the pushbuttons
	for (ch_aux=0;ch_aux<USER_IFACE_MAX_PUSH_BUTTONS;ch_aux++){
		buttons_state[ch_aux]=USER_IFACE_NOT_PRESSED;
	}//for

}//USER_IFACE_pusbuttons_clear



void USER_IFACE_encoders_clear(int16_t * encoders_var_value){
	uint8_t ch_aux;


	// clear the encoders var and abs values
	for (ch_aux=0;ch_aux<USER_IFACE_MAX_ENCODERS;ch_aux++){
		encoders_var_value[ch_aux]=0;
	}//for

}//USER_IFACE_encoders_clear