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

#include "sdmmc.h"

uint8_t CRCTable[256];// Precalculated table with all CRC bytes values 

SDMMC_unit_t SDMMC_units[SDMMC_MAX_UNITS];



 void SDMMC_Init(){
	uint8_t ui8_aux;
	
	SDMMC_GenerateCRCTable();
	
	for (ui8_aux=0;ui8_aux<SDMMC_MAX_UNITS;ui8_aux++){
		SDMMC_units[ui8_aux].st_type = INVALID;	
		SDMMC_units[ui8_aux].sector_length = SDMMC_SECTOR_SIZE;
	}

}// SDMMC_Init



void SDMMC_GenerateCRCTable(){
	int i,  j;
	uint8_t CRCPoly = 0x89;  // the value of our CRC-7 polynomial: G(x) = X^7 + X^3 + 1

	// generate a table value for all 256 possible byte values
	for (i = 0; i < 256; ++i) {
		CRCTable[i] = (i & 0x80) ? i ^ CRCPoly : i;
		for (j = 1; j < 8; ++j) {
			CRCTable[i] <<= 1;
			if (CRCTable[i] & 0x80)
			CRCTable[i] ^= CRCPoly;
		}
	}
		
}//SDMMC_GenerateCRCTable



uint8_t SDMMC_CRCAdd(uint8_t CRC,  uint8_t message_byte){
	return CRCTable[(CRC << 1) ^ message_byte];
	
}//SDMMC_CRCAdd



uint8_t SDMMC_GetCRC(uint8_t message[],  int length){
	int i;
	uint8_t CRC = 0;


	for (i = 0; i < length; ++i){
		CRC = SDMMC_CRCAdd(CRC,  message[i]);
	}

	CRC = (CRC << 1 | 1);

	return CRC;
	
}//SDMMC_GetCRC



 uint8_t SDMMC_send_byte(uint8_t unit_idx,  uint8_t ui8_to_send){


	switch (unit_idx){
		
		case SDMMC_IDX_0:
			// "...When writing a character to the Data register (DATA),  the character will be transferred to the Shift
			// register. Once the content of TxDATA has been transferred to the Shift register,  the Data Register Empty
			// flag in the Interrupt Flag Status and Clear register (INTFLAG.DRE) will be set ..."
			while (SERCOM0->SPI.INTFLAG.bit.DRE!=1){};
			//if ( SERCOM0->SPI.INTFLAG.bit.DRE == 1 ) {
				// SERCOM0->USART.INTENSET.bit.TXC == 1;
				SERCOM0->SPI.DATA.bit.DATA = ui8_to_send;
			//}//if
		
			// "...Each time one character is shifted out from the master,  another character will be shifted in from the
			// slave simultaneously. If the receiver is enabled (CTRLA.RXEN=1),  the contents of the Shift register will be
			// transferred to the two-level receive buffer. The transfer takes place in the same clock cycle as the last
			// data bit is shifted in. And the Receive Complete Interrupt flag in the Interrupt Flag Status and Clear
			// register (INTFLAG.RXC) will be set. The received data can be retrieved by reading DATA..."	
			while (SERCOM0->SPI.INTFLAG.bit.RXC != 1 ){};
			ui8_to_send = SERCOM0->SPI.DATA.bit.DATA;
			break;
			
		default:
			break;		
			
	}//switch (unit_idx)
	
	return ui8_to_send; // return the value read during transmission 
	
}// SDMMC_send_byte



void SDMMC_select_card(uint8_t unit_idx){
   
	// select the specified unit_idx card
	switch(unit_idx){
		
		case SDMMC_IDX_0:
			CLR_SPI_nCS_0; // Select the SD Card ( nCS=0 SD Card enabled ).
			break;
			
	}//switch	
	
	SDMMC_send_byte(unit_idx,  0xFF); // Dummy clocks
	
	

}//SDMMC_select_card



void SDMMC_unselect_card(uint8_t unit_idx){
	
	// unselect the specified unit_idx card
	switch(unit_idx){
		
		case SDMMC_IDX_0:
			SET_SPI_nCS_0; // Unselect the specified SD Card ( nCS=1 SD Card disabled ).
			break;
			
	}//switch
		
	SDMMC_send_byte(unit_idx, 0xFF); // Dummy clocks

}//SDMMC_select_card



void SDMMC_wake_up(uint8_t unit_idx){
	uint8_t i;

    // 80 dummy (10x8 bits) clocks to wake up SD / MMC card    
	TIMMING_BlockingDelay(TIMMING_TICKS_200_MS);
    for (i=0;i<10;i++){ // 10x8 bits
	    SDMMC_send_byte(unit_idx, 0xFF); // to wake-up MMC SD card's SPI 80 Clocks are sent
    };//for
		
}//SDMMC_wake_up



uint8_t SDMMC_send_command_R1(uint8_t unit_idx, uint8_t *p_bytes_tx, uint8_t *p_bytes_rx){
	uint8_t ui8_retries=0;
	uint8_t ui8_aux;	
	uint8_t	ui8_ret_val=0xff;


	for (ui8_aux=0;ui8_aux<4;ui8_aux++){p_bytes_rx[ui8_aux]=0;}
 
	if ( (p_bytes_tx==0) || (p_bytes_rx==0) || (unit_idx>=SDMMC_MAX_UNITS) ) {
		
		// check that pointers are not null
		p_bytes_rx[0] = 0xff;
		ui8_ret_val = 0xff;
	
	}else{

        // SEND COMMAND:

		// SPI SD Command structure:
		// byte  0:             | byte 1-4:        | byte 5:
		// bits  7   6  5-0     | 31-0             | 7-1      0
		//      '0' '1' Command | Command_Argument | CRC      1
	
		// Send command bytes
		SDMMC_send_byte(unit_idx, p_bytes_tx[0]);// Command
		SDMMC_send_byte(unit_idx, p_bytes_tx[1]);
		SDMMC_send_byte(unit_idx, p_bytes_tx[2]);
		SDMMC_send_byte(unit_idx, p_bytes_tx[3]);
		SDMMC_send_byte(unit_idx, p_bytes_tx[4]);
		SDMMC_send_byte(unit_idx, p_bytes_tx[5]);// CRC
	
		// RECEIVE R1 RESPONSE:
	
		ui8_aux=0xFF;

		// there should be a delay of 1 to 8 bytes,  but I set 32 bytes retries instead of 8 bytes ( that is 4x more )
		// to be sure that there is time between the command is sent and the answer received from the card. The R1
		// answer is always 1 byte long and starts with '0'.
		ui8_retries = 128;
		while ( (ui8_retries>0) && (ui8_aux==0xFF) ){
			ui8_aux=SDMMC_send_byte(unit_idx, 0xFF);
			ui8_retries--;
		};//while
	
		if ( (ui8_retries==0) || (ui8_aux==0x05) ){
			
			// no valid response or illegal command has been received
			p_bytes_rx[0] = 0xff;
			ui8_ret_val = ui8_aux;// return value is R1
			
		}else{

			p_bytes_rx[0] = ui8_aux;
			ui8_ret_val = ui8_aux;
				
		}//if
		
	}//if ( (p_bytes_tx==0) || (p_bytes_rx==0) )
	
	return ui8_ret_val;
	
}/// SDMMC_send_command_R1



uint8_t SDMMC_send_command_R2(uint8_t unit_idx, uint8_t *p_bytes_tx, uint8_t *p_bytes_rx){
	uint8_t  ui8_retries=0;
	uint8_t  ui8_aux;
	uint8_t ui8_ret_val=0xff;


	for (ui8_aux=0;ui8_aux<4;ui8_aux++){p_bytes_rx[ui8_aux]=0;}
	
	if ( (p_bytes_tx==0) || (p_bytes_rx==0) || (unit_idx>=SDMMC_MAX_UNITS) ) {
		
		// check that pointers are not null
		p_bytes_rx[0] = 0xff;
		ui8_ret_val = 0xff;
		
	}else{

		// SEND COMMAND:

		// SPI SD Command structure:
		// byte  0:             | byte 1-4:        | byte 5:
		// bits  7   6  5-0     | 31-0             | 7-1      0
		//      '0' '1' Command | Command_Argument | CRC      1
		
		// Send command bytes
		SDMMC_send_byte(unit_idx, p_bytes_tx[0]);// Command
		SDMMC_send_byte(unit_idx, p_bytes_tx[1]);
		SDMMC_send_byte(unit_idx, p_bytes_tx[2]);
		SDMMC_send_byte(unit_idx, p_bytes_tx[3]);
		SDMMC_send_byte(unit_idx, p_bytes_tx[4]);
		SDMMC_send_byte(unit_idx, p_bytes_tx[5]);// CRC
		
		// RECEIVE R2 RESPONSE:
		
		ui8_aux=0xFF;

		// there should be a delay of 1 to 8 bytes,  but I set 32 bytes retries instead of 8 bytes ( that is 4x more )
		// to be sure that there is time between the command is sent and the answer received from the card. The R1
		// answer is always 1 byte long and starts with '0'.
		ui8_retries = 32;
		while ( (ui8_retries>0) && (ui8_aux==0xFF) ){
			// first byte of the R2 response is identical to the R1 response
			ui8_aux=SDMMC_send_byte(unit_idx, 0xFF);
			ui8_retries--;
		};//while
		
		if ( (ui8_retries==0) || (ui8_aux==0x05) ){
			
			// no valid response or illegal command has been received
			p_bytes_rx[0] = 0xff;
			ui8_ret_val = ui8_aux;// return value is first byte of R2 that it is identical to R1
			
		}else{

			ui8_ret_val = ui8_aux;

			p_bytes_rx[0] = ui8_aux;
			// get the second byte of the R2 response
			ui8_aux = SDMMC_send_byte(unit_idx, 0xFF);
			p_bytes_rx[1] = ui8_aux;
			
		}//if		

	}//if ( (p_bytes_tx==0) || (p_bytes_rx==0) )
	
	return ui8_ret_val;
	
}/// SDMMC_send_command_R2



uint8_t SDMMC_send_command_R3(uint8_t unit_idx, uint8_t *p_bytes_tx, uint8_t *p_bytes_rx){
	uint8_t ui8_retries=0;
	uint8_t ui8_aux;
	uint8_t	ui8_ret_val=0xff;

	
	for (ui8_aux=0;ui8_aux<4;ui8_aux++){p_bytes_rx[ui8_aux]=0;}
	
	if ( (p_bytes_tx==0) || (p_bytes_rx==0) || (unit_idx>=SDMMC_MAX_UNITS) ) {
		
		// check that pointers are not null
		p_bytes_rx[0] = 0xff;
		ui8_ret_val = 0xff;
		
	}else{

		// SEND COMMAND:

		// SPI SD Command structure:
		// byte  0:             | byte 1-4:        | byte 5:
		// bits  7   6  5-0     | 31-0             | 7-1      0
		//      '0' '1' Command | Command_Argument | CRC      1
		
		// Send command bytes
		SDMMC_send_byte(unit_idx, p_bytes_tx[0]);// Command
		SDMMC_send_byte(unit_idx, p_bytes_tx[1]);
		SDMMC_send_byte(unit_idx, p_bytes_tx[2]);
		SDMMC_send_byte(unit_idx, p_bytes_tx[3]);
		SDMMC_send_byte(unit_idx, p_bytes_tx[4]);
		SDMMC_send_byte(unit_idx, p_bytes_tx[5]);// CRC
		
		// RECEIVE R1 RESPONSE:
		
		ui8_aux=0xFF;

		// there should be a delay of 1 to 8 bytes,  but I set 32 bytes retries instead of 8 bytes ( that is 4x more )
		// to be sure that there is time between the command is sent and the answer received from the card. The R1
		// answer is always 1 byte long and starts with '0'.
		ui8_retries = 32;
		while ( (ui8_retries>0) && (ui8_aux==0xFF) ){
			ui8_aux=SDMMC_send_byte(unit_idx, 0xFF);
			ui8_retries--;
		};//while
		
		// RECEIVE R3 RESPONSE:
		
		if ( (ui8_retries==0) || (ui8_aux==0x05) ){
			
			// no valid response or illegal command has been received
			p_bytes_rx[0] = 0xff;
			ui8_ret_val = ui8_aux;// return value is R1
			
		}else{
			
			ui8_ret_val = ui8_aux;// return value is R1

			// get and store other R3 bytes
			ui8_aux = SDMMC_send_byte(unit_idx, 0xFF);
			p_bytes_rx[0] = ui8_aux;
			ui8_aux = SDMMC_send_byte(unit_idx, 0xFF);
			p_bytes_rx[1] = ui8_aux;
			ui8_aux = SDMMC_send_byte(unit_idx, 0xFF);
			p_bytes_rx[2] = ui8_aux;
			ui8_aux = SDMMC_send_byte(unit_idx, 0xFF);
			p_bytes_rx[3] = ui8_aux;
						
		}//if
		
	}//if ( (p_bytes_tx==0) || (p_bytes_rx==0) )
	
	return ui8_ret_val;
	
}/// SDMMC_send_command_R3



uint8_t SDMMC_send_command_R7(uint8_t unit_idx, uint8_t *p_bytes_tx, uint8_t *p_bytes_rx){
	uint8_t ui8_retries=0;
	uint8_t ui8_aux;
	uint8_t	ui8_ret_val=0xff;


	for (ui8_aux=0;ui8_aux<4;ui8_aux++){p_bytes_rx[ui8_aux]=0;}
	
	if ( (p_bytes_tx==0) || (p_bytes_rx==0) || (unit_idx>=SDMMC_MAX_UNITS) ) {
		
		// check that pointers are not null
		p_bytes_rx[0] = 0xff;
		ui8_ret_val = 0xff;
		
	}else{

		// SEND COMMAND:

		// SPI SD Command structure:
		// byte  0:             | byte 1-4:        | byte 5:
		// bits  7   6  5-0     | 31-0             | 7-1      0
		//      '0' '1' Command | Command_Argument | CRC      1
		
		// Send command bytes
		SDMMC_send_byte(unit_idx, p_bytes_tx[0]);// Command
		SDMMC_send_byte(unit_idx, p_bytes_tx[1]);
		SDMMC_send_byte(unit_idx, p_bytes_tx[2]);
		SDMMC_send_byte(unit_idx, p_bytes_tx[3]);
		SDMMC_send_byte(unit_idx, p_bytes_tx[4]);
		SDMMC_send_byte(unit_idx, p_bytes_tx[5]);// CRC
		
		// RECEIVE R1 RESPONSE:
		
		ui8_aux=0xFF;

		// there should be a delay of 1 to 8 bytes,  but I set 32 bytes retries instead of 8 bytes ( that is 4x more )
		// to be sure that there is time between the command is sent and the answer received from the card. The R1
		// answer is always 1 byte long and starts with '0'.
		ui8_retries = 32;
		while ( (ui8_retries>0) && (ui8_aux==0xFF) ){
			ui8_aux=SDMMC_send_byte(unit_idx, 0xFF);
			ui8_retries--;
		};//while
		
		// RECEIVE R7 RESPONSE:
		
		if ( (ui8_retries==0) || (ui8_aux==0x05) ){
			
			// no valid response or illegal command has been received
			p_bytes_rx[0] = 0xff;
			ui8_ret_val = ui8_aux;// return value is R1
				
		}else{
			
			ui8_ret_val = ui8_aux;// return value is R1

			// get and store other R7 bytes
			ui8_aux = SDMMC_send_byte(unit_idx, 0xFF);
			p_bytes_rx[0] = ui8_aux;
			ui8_aux = SDMMC_send_byte(unit_idx, 0xFF);
			p_bytes_rx[1] = ui8_aux;
			ui8_aux = SDMMC_send_byte(unit_idx, 0xFF);
			p_bytes_rx[2] = ui8_aux;
			ui8_aux = SDMMC_send_byte(unit_idx, 0xFF);
			p_bytes_rx[3] = ui8_aux;
			
		}//if
		
	}//if ( (p_bytes_tx==0) || (p_bytes_rx==0) )
	
	return ui8_ret_val;
	
}/// SDMMC_send_command_R7



int8_t SDMMC_card_status(uint8_t unit_idx){
	int8_t  ret_value = -2;
	uint8_t tx_byte[6];
	uint8_t rx_byte[4];
	uint8_t r1_byte=0x00;


	if (unit_idx>=SDMMC_MAX_UNITS){
		
		ret_value=-1;
		
	}else{		
		
		// CMD13 SEND STATUS Asks the selected card to send its status register
		SDMMC_select_card(unit_idx);
		tx_byte[0]=0x4D;// CMD13 = 0100  1101 
		tx_byte[1]=0x00;
		tx_byte[2]=0x00;
		tx_byte[3]=0x00;
		tx_byte[4]=0x00;
		tx_byte[5]=0xFF;
		r1_byte=SDMMC_send_command_R2(unit_idx, tx_byte, rx_byte);
		// 2nd R2 byte:
	    // 0 0 : Card is locked
		// 1 1 : wp erase skip / lock/unlock cmd failed
		// 2 2 : error
		// 3 3 : CC error
		// 4 4 : card ecc failed
		// 5 5 : wp violiation
		// 6 6 : erase param
		// 7 7 : out of range / csd overwrite     
		// 1st R2 byte is identical to R1 byte:
		// 8 0 : in idle state                     
		// 9 1 : erase reset
		// A 2 : illegal command
		// B 3 : com crc error
		// C 4 : erase sequence error
		// D 5 : address error
		// E 6 : parameter error
	    // F 7 : '0'
		if ((rx_byte[0]!=0)||(rx_byte[1]!=0)){
			ret_value=-1;
		}else{
			ret_value=1;
		}//if
		
	}//if (unit_idx>
     		
	return ret_value;
	
}//SDMMC_card_status



SDMMC_type_t SDMMC_card_initialize (uint8_t unit_idx){
	SDMMC_type_t ret_value = INVALID;
	uint8_t tx_byte[6];
	uint8_t rx_byte[4];
	uint8_t r1_byte;
	uint8_t retries;


    if (unit_idx<SDMMC_MAX_UNITS){

		// SD Initialization algorithm according to: "Figure 7-2 SPI Mode Initialization Flow" page 211 
		// of document "Physical Layer Simplified Specification Version 6.00" from SD Card Association.
		// MMC Initialization consists in CMD0 until R1=0x01 followed by CMD1 until R1=0x00
    
		// Physical Layer Simplified Specification Version 6.00:
		// "...
		// The (SPI) interface is selected during the first reset command after power up (CMD0) 
		// and cannot be changed once the part is powered on
		// ...
		// The SD Card is powered up in the SD mode. It will enter SPI mode if the CS signal is
		// asserted (negative) during the reception of the reset commadn (CMD0).
		// ..."

		retries=10;
		r1_byte=0;
		while ( (retries>0)&&(r1_byte!=0x01) ){

			SDMMC_unselect_card(unit_idx);
			// Send 74 dummy clocks to "wake up" the card
			SDMMC_wake_up(unit_idx);
		
			// CMD0 GO_IDLE_STATE=0 Reset command. Resets the SD Card
			SDMMC_select_card(unit_idx);
			tx_byte[0]=0x40;// CMD0 = 01 000000 = 0x40
			tx_byte[1]=0x00;
			tx_byte[2]=0x00;
			tx_byte[3]=0x00;
			tx_byte[4]=0x00;
			tx_byte[5]=0x95;// 0x95 = Valid CRC7 for CMD0(0) + Stop 
			r1_byte=SDMMC_send_command_R1(unit_idx, tx_byte, rx_byte);
			SDMMC_unselect_card(unit_idx);
		
			retries--;		
		
		    TIMMING_BlockingDelay(TIMMING_TICKS_100_MS);//TIMMING_TICKS_1_S); // Initialization timeout
			
		};// while
    
		// Physical Layer Simplified Specification Version 6.00:
		// "...
		// For the Thin (2.1mm) SD MEmroy Card - CMD1 (SEND_OP_COND) is also valid - this means that in
		// SPI mode,  CMD1 and ACMD41 have the same behaviors,  but the usage of ACMD41 is preferable since
		// it allows easy distinction between SD Memory Card and a Multimedia Card.
		// ...
		// For the thin (1.4mm) Standard Size SD Memory Card,  CMD1 (SENDO_OP_COND) is an illegal command during the
		// initialization that is done after power on. After Power On,  once the card has accepted validACMD41,  it
		// will be able to also accept CMD1 even if used after re-initializing (CMD0) the card.
		// ..."
	
		// card_answer r1 to CMD0 should be 0x01 
		if (r1_byte!=0x01){
		
			// no IDLE command received
			SDMMC_units[unit_idx].st_type = INVALID;
			ret_value = INVALID;

		}else{
		
			// IDLE command received
		
			// CMD8 SEND_IF_COND is used to verify SD Memory Card Interface operation Condition. Sends SD Memory
			// Card interface condition that includes Host Supply Voltage (VHS) information and the argument 
			// format of CMD8 is the same as defined in SD mode and the response is defined in Section 7.3.2.6
			// With this command:
			// - The card checks the operation condition of the the HOST bu analyzing the argument of CMD8.
			// - The host checks the validity of the card by analyizing the response of SD card to CMD8.
			// Responses:
			// - If the card responds illegal command,  the card is legacy and dos not support CMD8.
			// - If the card echoes back the supply voltage and check pattern it means that the card supports
			// the specified mode. If check pattern is not matched,  CMD8 communication is not valid.
			// - If VCA in the respons is ste to 0,  the card cannot operate in the supplied voltage.
			// Receipt of CMD8 makes the cards realiza that the host supports the Physical Layer Version  2.00 or 
			// later and the card can enable new functions.
			
			// Activates the card’s initialization process.  asks the accessed card whether card can operate in supplied
			// voltage range. Reserved bits shall be set to '0'.

			// CMD8 SEND_IF_COND Verify SD Memory Card InterFace Operation CONDition
			SDMMC_select_card(unit_idx);
			tx_byte[0]=0x48;// CMD8 = 01 001000  = 0x48
			tx_byte[1]=0x00;
			tx_byte[2]=0x00;
			tx_byte[3]=0x01;// 0x1 Voltage supplied: 0001b = 2.7-3.6V
			tx_byte[4]=0xAA;// echo pattern is OK (byte_3=0xAA)
			tx_byte[5]=0x87;// 0x87 = Valid CRC for CMD8(0) + Stop 
			r1_byte=SDMMC_send_command_R7(unit_idx, tx_byte, rx_byte);
			SDMMC_unselect_card(unit_idx);		
			// R7 bits:
			// 0:7 Echo-back of check pattern
			// 8:11 Accepted voltage
			// 12:32 Reserved bits
		
			if (r1_byte==0x01){	
		
				// CMD8 is NOT illegal command so it is a SD Ver2 card ( Standard Capacity,  or High Capacity,  or Extended Capacity )
						
				// check R7 bytes to see if card accepts 2.7-3.6V (byte_2==0x01) and if echo pattern is OK (byte_3=0xAA)
				if ( (rx_byte[2]!=0x01) || (rx_byte[3]!=0xAA) ){				
			
					// no valid voltage and/or not valid echo pattern received
					SDMMC_units[unit_idx].st_type = INVALID;
					ret_value = INVALID;
			
				}else{
				
					// valid voltage and echo pattern received				
					retries=32;
					r1_byte=0xFF;
					while ( (retries>0)&&(r1_byte!=0x00) ){

						// CMD55 APP_CMD Defines to the card that the next command is an ACMD application specific command rather 
						// than a standard command. CMD55 must be sent prior to ACMD41 ( CMD55 preceeds all ACMDxx commands ).
						SDMMC_select_card(unit_idx);
						tx_byte[0]=0x77;//CMD55 = 01 110111 = 0x77
						tx_byte[1]=0x00;
						tx_byte[2]=0x00;
						tx_byte[3]=0x00;
						tx_byte[4]=0x00;
						tx_byte[5]=0xFF;
						r1_byte=SDMMC_send_command_R1(unit_idx, tx_byte, rx_byte);
						SDMMC_unselect_card(unit_idx);				
						
						// ACMD41 41 SD_SEND_OP_COND To send ACMDxx it must be preceeded by a CMD55 command. ACMD41 commands sends host 
						// capacity support information and activates the card's initialization process. Reserved bits shall be set to 
						// '0'. It is used to start initialization and to check if the card has completed initialization. It is mandatory 
						// to issue CMD8 prior to the first ACMD41. Receiving of the CMD8 expands the CMD58 and ACMD41 function:
						// - HCS ( High Capacity Support ) in the argument of ACMD41
						SDMMC_select_card(unit_idx);
						tx_byte[0]=0x69; // ACMD41 = 01 101001
						tx_byte[1]=0x40;
						tx_byte[2]=0x00;
						tx_byte[3]=0x00;
						tx_byte[4]=0x00;
						tx_byte[5]=0xFF;
						r1_byte=SDMMC_send_command_R1(unit_idx,  tx_byte, rx_byte);
						SDMMC_unselect_card(unit_idx);
				
						//"...
						// The "in idle state" bit in the R1 response of ACMD41 is used by the card to inform the host if initialization of 
						// ACMD41 is completed. Setting this bit to "1" indicates that the card is still initializing. Setting this bit to 
						// "0" indicates completion of initialization. The repeateadly issues ACMD41 until this bit is set to "0".
						//..."
				
						retries--;
				
					}//while
								
					// CMD58 READ_OCR Reads the OCR register of a card. CCS bit is assigned to OCR[30]					 
					SDMMC_select_card(unit_idx);
					tx_byte[0]=0x7A;// CMD58 = 01 111010 = 0x7A
					tx_byte[1]=0x00;
					tx_byte[2]=0x00;
					tx_byte[3]=0x00;
					tx_byte[4]=0x00;
					tx_byte[5]=0xFF;
					r1_byte=SDMMC_send_command_R3(unit_idx, tx_byte, rx_byte);
					SDMMC_unselect_card(unit_idx);				

					// OCR register bit   OCR Fields definition
					// 0-14                reserved
					// 15                  2.7 to 2.8V accepted
					// 16                  2.8 to 2.9V accepted
					// 17                  2.9 to 3.0V accepted
					// 18                  3.0 to 3.1V accepted
					// 19                  3.1 to 3.2V accepted
					// 20                  3.2 to 3.3V accepted
					// 21                  3.3 to 3.4V accepted
					// 22                  3.4 to 3.5V accepted
					// 23                  3.5 to 3.6V accepted
					// 24                  Switching to 1.8V Accepted
					// 25-28               reserved
					// 29                  UHS-II Card Status
					// 30                  Card Capacity Status (CCS): the value of this bit is only valid when the card power up bit is set
					// 31				   Card power up status bit (busy): this bit is set to LOW if the card has not finished the power up routine
				
					// check the CCS bit of byte 0
					if ( (rx_byte[0]&0x40)!=0 ){
						// The card is SDv2 or later High Capacity or Extended Capacity SD Memory Card
						SDMMC_units[unit_idx].st_type = SDv2_HC_XC;
						SDMMC_units[unit_idx].sector_length = SDMMC_SECTOR_SIZE;
						ret_value = SDv2_HC_XC;
					}else{
						// The card is SDv2 or later Standard Capacity SD Memory Card
						SDMMC_units[unit_idx].st_type = SDv2;
						SDMMC_units[unit_idx].sector_length = SDMMC_SECTOR_SIZE;
						ret_value = SDv2;
					}//if ( (rx_byte[0]&0x40)!=0

				}// if ( (byte_2==0x01)&&(byte_3==0xAA) )
				
			}else{
			
				// CMD8 is illegal command so it is not SD Ver2 Standard Capacity neither High Capacity neither Extendeded Capacity		
				// it is SDv1 or MMCv3						

				retries=32;
				r1_byte=0xFF;
				while ( (retries>0)&&(r1_byte!=0x00)&&(r1_byte!=0x05) ){

					// CMD55 APP_CMD Defines to the card that the next command is an ACMD application specific command rather
					// than a standard command. CMD55 must be sent prior to ACMD41 ( CMD55 preceeds all ACMDxx commands ).
					// CMD55 = 01 110111 = 0x77
					// Params:
					//  0x00 0x00 0x00 0x00
					SDMMC_select_card(unit_idx);
					tx_byte[0]=0x77;//CMD55 = 01 110111 = 0x77
					tx_byte[1]=0x00;
					tx_byte[2]=0x00;
					tx_byte[3]=0x00;
					tx_byte[4]=0x00;
					tx_byte[5]=0xFF;
					r1_byte=SDMMC_send_command_R1(unit_idx, tx_byte, rx_byte);
					SDMMC_unselect_card(unit_idx);
					
					// ACMD41 41 SD_SEND_OP_COND To send ACMDxx it must be preceeded by a CMD55 command. ACMD41 commands sends host
					// capacity support information and activates the card's initialization process. Reserved bits shall be set to
					// '0'. It is used to start initialization and to check if the card has completed initialization. It is mandatory
					// to issue CMD8 prior to the first ACMD41. Receiving of the CMD8 expands the CMD58 and ACMD41 function:
					// - HCS ( High Capacity Support ) in the argument of ACMD41
					// CMD55 = 01 110111 = 0x77
					// Params:
					//  0x00 0x00 0x00 0x00
					SDMMC_select_card(unit_idx);
					tx_byte[0]=0x69;// ACMD41 = 01 101001
					tx_byte[1]=0x40;
					tx_byte[2]=0x00;
					tx_byte[3]=0x00;
					tx_byte[4]=0x00;
					tx_byte[5]=0xFF;
					r1_byte=SDMMC_send_command_R1(unit_idx, tx_byte, rx_byte);
					SDMMC_unselect_card(unit_idx);
					
					retries--;
			
				}//while ( (retries>0)&&(R1_byte_0!=0x0) ){
			
				//"...
				// The "in idle state" bit in the R1 response of ACMD41 is used by the card to inform the host if initialization of
				// ACMD41 is completed. Setting this bit to "1" indicates that the card is still initializing. Setting this bit to
				// "0" indicates completion of initialization. The repeateadly issues ACMD41 until this bit is set to "0".
				//..."
					
				if (r1_byte==0x00){
										
					// It is an  SDv1 card
					SDMMC_units[unit_idx].st_type = SDv1;
					SDMMC_units[unit_idx].sector_length = SDMMC_SECTOR_SIZE;
					ret_value = SDv1;
				
				}else{
					
					// It may be an MMC card, but first execute MMC initialization sequence to confirm it
					retries=20;
					r1_byte=0xFF;
					while ( (retries>0)&&(r1_byte!=0x01) ){

						SDMMC_unselect_card(unit_idx);
						// Send 74 dummy clocks to "wake up" the card
						SDMMC_wake_up(unit_idx);
			
						// CMD0 GO_IDLE_STATE=0 Reset command. Resets the SD Card
						SDMMC_select_card(unit_idx);
						tx_byte[0]=0x40;// CMD0 = 01 000000 = 0x40
						tx_byte[1]=0x00;
						tx_byte[2]=0x00;
						tx_byte[3]=0x00;
						tx_byte[4]=0x00;
						tx_byte[5]=0x95;// 0x95 = Valid CRC7 for CMD0(0) + Stop
						r1_byte=SDMMC_send_command_R1(unit_idx, tx_byte, rx_byte);
						SDMMC_unselect_card(unit_idx);
			
						retries--;
			
						TIMMING_BlockingDelay(TIMMING_TICKS_500_MS);// Initialization timeout
			
					};// while

					retries=20;
					r1_byte=0xFF;
					while ( (retries>0)&&(r1_byte!=0x00) ){
	
						// CMD1 SEND_OP_COND Activates the card’s initialization process.
						SDMMC_select_card(unit_idx);
						tx_byte[0]=0x41;
						tx_byte[1]=0x00;
						tx_byte[2]=0x00;
						tx_byte[3]=0x00;
						tx_byte[4]=0x00;
						tx_byte[5]=0xFF;
						r1_byte=SDMMC_send_command_R1(unit_idx, tx_byte, rx_byte);
						SDMMC_unselect_card(unit_idx);

						retries--;

					}//while
					
					if ( (retries>0) && (r1_byte==0x00) ){
						// It is an  MMC card
						SDMMC_units[unit_idx].st_type = MMCv3;
						SDMMC_units[unit_idx].sector_length = SDMMC_SECTOR_SIZE;
						ret_value = MMCv3; // MMCv3
					}
					
				}//if		
			
			}//if (byte_0==0x01)
		
		}//if (byte_0==0x01) 
	
	}//if (unit_idx<SDMMC_MAX_UNITS)

	return ret_value;

}//SDMMC_disk_initialize


uint8_t SDMMC_WaitFEDataToken(uint8_t unit_idx){
	int32_t retries = SDMMC_MAX_RETRIES;
	uint8_t valor=0x00;

    // keep waiting to receive the Data Toke sent by the card 
	retries = SDMMC_MAX_RETRIES;
	while ( (retries>0) && (valor!=0xFE) ){
 	    valor=SDMMC_send_byte(unit_idx, 0xFF);

		retries--;
	}//while
		
	return valor;

}// SDMMC_WaitFEDataToken 



uint8_t SDMMC_WaitDataResponse(uint8_t unit_idx){
	int32_t retries = SDMMC_MAX_RETRIES;
	uint8_t ui8_read_value=0x00;

	// As Data Response Token bytes have following format:
    //   7 6 5 4 3 2 1  0
    //   x x x 0 Status	1
	// keep waiting to receive a byte with 1st bit to '1' and 5th 
	// bit to '0' ( mask 0x11 over the answer should be 0x01 )
	retries = SDMMC_MAX_RETRIES;
	while ( (retries>0) && ( (ui8_read_value&0x11)!=0x01) ){
		ui8_read_value=SDMMC_send_byte(unit_idx, 0xFF);

		retries--;
	}/*while*/
		
	return ui8_read_value;

}//SDMMC_WaitDataResponse



uint8_t SDMMC_WaitR1bResponseNotBusy(uint8_t unit_idx){
	int32_t retries = SDMMC_MAX_RETRIES;
	uint8_t ui8_read_value=0x00;
	
	
	retries = SDMMC_MAX_RETRIES;
	do{
		ui8_read_value=SDMMC_send_byte(unit_idx,0xFF);
		retries--;
	}while ( (ui8_read_value==0x00)&&(retries>0) );

	return ui8_read_value;

}//SDMMC_WaitR1bResponseNotBusy



int8_t SDMMC_card_write(uint8_t unit_idx, uint8_t *buff, uint64_t sector, uint16_t count){
	int8_t  ret_value = 1;	
	

	if (count==1){
	
		// use the command to write a single block (  )
		ret_value = SDMMC_card_write_single_block (unit_idx, buff, sector);

	}else{
	
		// use the command to write multiple blocks (  )
		ret_value = SDMMC_card_write_multiple_blocks(unit_idx, buff, sector,count);
			
	}//if	
	
	return ret_value;	

}//SDMMC_card_write



int8_t SDMMC_card_read (uint8_t unit_idx, uint8_t *buff, uint64_t sector, uint16_t count){
	int8_t  ret_value = -1;	


	if (count==1){
		
		// use the command to read a single block ( CMD17 READ_SINGLE_BLOCK )
		ret_value = SDMMC_card_read_single_block (unit_idx, buff, sector);
		
	}else{
		
		// use the command to read multiple blocks ( CMD18 READ_MULTIPLE_BLOCK )
		ret_value = SDMMC_card_read_multiple_blocks(unit_idx, buff, sector, count);
		
	}//if
	
	return ret_value;	
	
}//SDMMC_card_read



int8_t SDMMC_card_read_single_block (uint8_t unit_idx, uint8_t *buff, uint64_t sector){
	int8_t  ret_value = 1;
	uint8_t tx_byte[6];
	uint8_t rx_byte[4];
	uint8_t r1_byte=0x00;
	uint8_t ui8_aux=0x00;
	
	uint32_t address;
	uint8_t address_byte_0;
	uint8_t address_byte_1;
	uint8_t address_byte_2;
	uint8_t address_byte_3;

	uint16_t ui16_bytes_count;
    uint16_t ui16_max_bytes_block;
	
	
	if (unit_idx>=SDMMC_MAX_UNITS){
	
		ret_value=-1;
	
	}else{
		
		// IMPORTANT: "...SDSC Card (CCS=0) uses byte unit address and SDHC and 
		// SDXC Cards (CCS=1) use block unit address (512 bytes unit)..."
		// So the access is done by bytes in SDSC and MMC Card mode and by blocks
		// in SDXC and SDHC mode
		switch (SDMMC_units[unit_idx].st_type){
		   
			case SDv2_HC_XC:				
				
				//  SD HIGH CAPACITY,  SD EXTENDED CAPACITY work with "sectors" addresses
				address = sector;
				address_byte_0=(uint8_t)(address&(0x000000FF));
				address_byte_1=(uint8_t)((address&(0x0000FF00))>>8);
				address_byte_2=(uint8_t)((address&(0x00FF0000))>>16);
				address_byte_3=(uint8_t)((address&(0xFF000000))>>24);	
				
				// In SD HC and SD XC the block length is by default 512, so CMD16 SET_BLOCKLEN is not needed
				break;
				
			default: 
				
				//  SDv1, SDv2 and MMCv3 modes work with "byte" addresses
				address=(sector*SDMMC_SECTOR_SIZE);
				address_byte_0=0x00;
				address_byte_1=(uint8_t)((address&(0x0000FF00))>>8);
				address_byte_2=(uint8_t)((address&(0x00FF0000))>>16);
				address_byte_3=(uint8_t)((address&(0xFF000000))>>24);

				// CMD16 SET_BLOCKLEN Selects a block length (in bytes) for all following block commands 
				// (read & write) and it is only used in SDv1, SDv2 and MMCv3 modes ( SDv2_HC_XC always
				// uses 512 sector block size so SET_BLOCKLEN is not needed )
				SDMMC_select_card(unit_idx);
				tx_byte[0]=0x50;// CMD16 = 01 010000  = 0x50
				tx_byte[1]=0x00;
				tx_byte[2]=0x00;
				tx_byte[3]=(uint8_t)((SDMMC_units[unit_idx].sector_length&0xFF00)>>8);// high 8 bits of the length
				tx_byte[4]=(uint8_t)SDMMC_units[unit_idx].sector_length&0x00FF;// low 8 bits of the length
				tx_byte[5]=0xFF;
				r1_byte=SDMMC_send_command_R1(unit_idx, tx_byte, rx_byte);
				SDMMC_unselect_card(unit_idx);
				break;
	   
		}//switch
		
		// CMD17 READ_SINGLE_BLOCK Reads a block of the size selected by the SET_BLOCKLEN command. In
		SDMMC_select_card(unit_idx);	
		tx_byte[0]=0x51;// CMD17 = 01 010001 = 0x51
		tx_byte[1]=address_byte_3;
		tx_byte[2]=address_byte_2;
		tx_byte[3]=address_byte_1;
		tx_byte[4]=address_byte_0;
		tx_byte[5]=0xFF;
		r1_byte=SDMMC_send_command_R1(unit_idx, tx_byte, rx_byte);
		if (r1_byte!=0x00){
		
			// if R1 value is NOT 0x00 it means that READ_SINGLE_BLOCK  not been accepted by the card		
			ret_value=-1;
		
		}else{
		
			// if R1 value is 0x00 it means that READ_SINGLE_BLOCK operation has been accepted by the card
		
    		// waits the data Start Block token
			SDMMC_WaitFEDataToken(unit_idx);
			// start receiving data bytes
		
			ui16_bytes_count=0;
			ui16_max_bytes_block=SDMMC_units[unit_idx].sector_length;
			while (ui16_bytes_count<ui16_max_bytes_block){
				ui8_aux=SDMMC_send_byte(unit_idx, 0xFF);
				buff[ui16_bytes_count]= ui8_aux;
				ui16_bytes_count++;
			};//while

			// two last CRC bytes are received
			ui8_aux=SDMMC_send_byte(unit_idx, 0xFF);
			ui8_aux=SDMMC_send_byte(unit_idx, 0xFF);

			ret_value=1;	

		};//if
	
		SDMMC_unselect_card(unit_idx);

	}//if ( (unit_idx<SDMMC_MAX_UNITS) && (SDMMC_units[unit_idx]!=INVALID) ){

    return ret_value;
	
}//SDMMC_card_read_single_block



int8_t SDMMC_card_read_multiple_blocks(uint8_t unit_idx, uint8_t *buff, uint64_t sector, uint16_t count){
	int8_t  ret_value = 1;
	uint8_t tx_byte[6];
	uint8_t rx_byte[4];
	uint8_t r1_byte=0x00;
	uint8_t ui8_aux=0x00;
	
	uint32_t address;
	uint8_t address_byte_0;
	uint8_t address_byte_1;
	uint8_t address_byte_2;
	uint8_t address_byte_3;

	uint16_t ui16_bytes_count;
	uint16_t ui16_max_bytes_block;
	uint16_t ui16_blocks_count;
	uint16_t ui16_total_byte_count;
	

	if (unit_idx>=SDMMC_MAX_UNITS){
		
		ret_value=-1;
		
	}else{
		
		// IMPORTANT: "...SDSC Card (CCS=0) uses byte unit address and SDHC and
		// SDXC Cards (CCS=1) use block unit address (512 bytes unit)..."
		// So the access is done by bytes in SDSC and MMC Card mode and by blocks
		// in SDXC and SDHC mode
		switch (SDMMC_units[unit_idx].st_type){
			
			case SDv2_HC_XC:
			
				//  SD HIGH CAPACITY,  SD EXTENDED CAPACITY work with "sectors" addresses
				address = sector;
				address_byte_0=(uint8_t)(address&(0x000000FF));
				address_byte_1=(uint8_t)((address&(0x0000FF00))>>8);
				address_byte_2=(uint8_t)((address&(0x00FF0000))>>16);
				address_byte_3=(uint8_t)((address&(0xFF000000))>>24);
			
				// In SD HC and SD XC the block length is by default 512, so CMD16 SET_BLOCKLEN is not needed
				break;
			
			default:
			
				//  SDv1, SDv2 and MMCv3 modes work with "byte" addresses
				address=(sector*SDMMC_SECTOR_SIZE);
				address_byte_0=0x00;
				address_byte_1=(uint8_t)((address&(0x0000FF00))>>8);
				address_byte_2=(uint8_t)((address&(0x00FF0000))>>16);
				address_byte_3=(uint8_t)((address&(0xFF000000))>>24);

				// CMD16 SET_BLOCKLEN Selects a block length (in bytes) for all following block commands
				// (read & write) and it is only used in SDv1, SDv2 and MMCv3 modes ( SDv2_HC_XC always
				// uses 512 sector block size so SET_BLOCKLEN is not needed )
				SDMMC_select_card(unit_idx);
				tx_byte[0]=0x50;// CMD16 = 01 010000  = 0x50
				tx_byte[1]=0x00;
				tx_byte[2]=0x00;
				tx_byte[3]=(uint8_t)((SDMMC_units[unit_idx].sector_length&0xFF00)>>8);// high 8 bits of the length
				tx_byte[4]=(uint8_t)SDMMC_units[unit_idx].sector_length&0x00FF;// low 8 bits of the length
				tx_byte[5]=0xFF;
				r1_byte=SDMMC_send_command_R1(unit_idx, tx_byte, rx_byte);
				SDMMC_unselect_card(unit_idx);
				break;
			
		}//switch
		
		// CMD18 READ_MULTIPLE_BLOCK Continuously transfers data blocks from card to host
		// until interrupted by a STOP_TRANSMISSION command. Block length is specified the 
		// same as READ_SINGLE_BLOCK. 		
		SDMMC_select_card(unit_idx);
		tx_byte[0]=0x52;// CMD18 = 01 010010 = 0x52
		tx_byte[1]=address_byte_3;
		tx_byte[2]=address_byte_2;
		tx_byte[3]=address_byte_1;
		tx_byte[4]=address_byte_0;
		tx_byte[5]=0xFF;
		r1_byte=SDMMC_send_command_R1(unit_idx, tx_byte, rx_byte);
		
		if (r1_byte!=0x00){
			
			// if R1 value is NOT 0x00 it means that READ_MULTIPLE_BLOCK  not been accepted by the card
			ret_value=-1;
			
		}else{
			
			// if R1 value is 0x00 it means that READ_MULTIPLE_BLOCK operation has been accepted by the card
								
			// start receiving blocks
			ui16_blocks_count =0;		
			ui16_total_byte_count = 0;	
			while ( (ret_value>=0) && (ui16_blocks_count<count) ){

				if (SDMMC_WaitFEDataToken(unit_idx) != 0xFE){
					ret_value = -1;
				}else{

					// start receiving the data bytes of the currently processed block
					ui16_bytes_count=0;
					ui16_max_bytes_block=SDMMC_units[unit_idx].sector_length;
					while (ui16_bytes_count<ui16_max_bytes_block){
						ui8_aux=SDMMC_send_byte(unit_idx, 0xFF);
						buff[ui16_total_byte_count]= ui8_aux;
						ui16_bytes_count++;
						ui16_total_byte_count++;					
					};//while

					// two last CRC bytes are received
					ui8_aux=SDMMC_send_byte(unit_idx, 0xFF);
					ui8_aux=SDMMC_send_byte(unit_idx, 0xFF);

					ui16_blocks_count++;
					
				}//if (SDMMC_WaitFEDataToken(unit_idx) != 0xFE)
					
			}// while

			// CMD12 STOP_TRANSMISSION Forces the card to stop transmission during a multiple block read operation.						
			SDMMC_select_card(unit_idx);
			tx_byte[0]=0x4C;// CMD12 0100 1100
			tx_byte[1]=0x00;
			tx_byte[2]=0x00;
			tx_byte[3]=0x00;
			tx_byte[4]=0x00;
			tx_byte[5]=0xFF;
			r1_byte=SDMMC_send_command_R1(unit_idx, tx_byte, rx_byte);

			ret_value=1;

		};//if
		
		SDMMC_unselect_card(unit_idx);

	}//if ( (unit_idx<SDMMC_MAX_UNITS) && (SDMMC_units[unit_idx]!=INVALID) ){

	return ret_value;
		
}//SDMMC_card_read_multiple_blocks



int8_t SDMMC_card_write_single_block (uint8_t unit_idx, uint8_t *buff, uint64_t sector){
	int8_t  ret_value = 1;
	uint8_t tx_byte[6];
	uint8_t rx_byte[4];
	uint8_t r1_byte=0x00;
	uint8_t ui8_aux=0x00;
	
	uint32_t address;
	uint8_t address_byte_0;
	uint8_t address_byte_1;
	uint8_t address_byte_2;
	uint8_t address_byte_3;

	uint16_t ui16_bytes_count;
	uint16_t ui16_max_bytes_block;
	
	
	if (unit_idx>=SDMMC_MAX_UNITS){
		
		ret_value=-1;
		
	}else{
		
		// IMPORTANT: "...SDSC Card (CCS=0) uses byte unit address and SDHC and
		// SDXC Cards (CCS=1) use block unit address (512 bytes unit)..."
		// So the access is done by bytes in SDSC and MMC Card mode and by blocks
		// in SDXC and SDHC mode
		switch (SDMMC_units[unit_idx].st_type){
			
			case SDv2_HC_XC:
			    // "... Un case if SDHC and SDXC Cards, block length is fixed to 512 bytes
				// regardless of the block length set by CMD 16..."
				
				//  SD HIGH CAPACITY,  SD EXTENDED CAPACITY work with "sectors" addresses
				address = sector;
				address_byte_0=(uint8_t)(address&(0x000000FF));
				address_byte_1=(uint8_t)((address&(0x0000FF00))>>8);
				address_byte_2=(uint8_t)((address&(0x00FF0000))>>16);
				address_byte_3=(uint8_t)((address&(0xFF000000))>>24);
			
				// In SD HC and SD XC the block length is by default 512, so CMD16 SET_BLOCKLEN is not needed
				break;
			
			default:
			
				//  SDv1, SDv2 and MMCv3 modes work with "byte" addresses
				address=(sector*SDMMC_SECTOR_SIZE);
				address_byte_0=0x00;
				address_byte_1=(uint8_t)((address&(0x0000FF00))>>8);
				address_byte_2=(uint8_t)((address&(0x00FF0000))>>16);
				address_byte_3=(uint8_t)((address&(0xFF000000))>>24);

				// CMD16 SET_BLOCKLEN Selects a block length (in bytes) for all following block commands
				// (read & write) and it is only used in SDv1, SDv2 and MMCv3 modes ( SDv2_HC_XC always
				// uses 512 sector block size so SET_BLOCKLEN is not needed )
				SDMMC_select_card(unit_idx);
				tx_byte[0]=0x50;// CMD16 = 01 010000  = 0x50
				tx_byte[1]=0x00;
				tx_byte[2]=0x00;
				tx_byte[3]=(uint8_t)((SDMMC_units[unit_idx].sector_length&0xFF00)>>8);// high 8 bits of the length
				tx_byte[4]=(uint8_t)SDMMC_units[unit_idx].sector_length&0x00FF;// low 8 bits of the length
				tx_byte[5]=0xFF;
				r1_byte=SDMMC_send_command_R1(unit_idx, tx_byte, rx_byte);
				SDMMC_unselect_card(unit_idx);
				break;
			
		}//switch
		
		// CMD24 WRITE_SINGLE_BLOCK Writes a block of the size selected by the SET_BLOCKLEN command.
		SDMMC_select_card(unit_idx);
		tx_byte[0]=0x58;// CMD24 = 01 011000 = 0x58
		tx_byte[1]=address_byte_3;
		tx_byte[2]=address_byte_2;
		tx_byte[3]=address_byte_1;
		tx_byte[4]=address_byte_0;
		tx_byte[5]=0xFF;
		r1_byte=SDMMC_send_command_R1(unit_idx, tx_byte, rx_byte);
		if (r1_byte!=0x00){
			
			// if R1 value is NOT 0x00 it means that WRITE_SINGLE_BLOCK  not been accepted by the card
			ret_value=-1;
			
		}else{
			
			// if R1 value is 0x00 it means that WRITE_SINGLE_BLOCK operation has been accepted by the card
			
			// send the Start Block before the data block
			SDMMC_send_byte(unit_idx,0xFE);
			
			// send the data block 			
			ui16_bytes_count=0;
			ui16_max_bytes_block=SDMMC_units[unit_idx].sector_length;
			while (ui16_bytes_count<ui16_max_bytes_block){
				ui8_aux=SDMMC_send_byte(unit_idx,buff[ui16_bytes_count]);
				ui16_bytes_count++;
			};//while
						
			// wait the Data Response "... After a data block has been received, the card will
			// respond with a data-response toke ...".
			ui8_aux=SDMMC_WaitDataResponse(unit_idx);
			if ((ui8_aux & 0x1F) != 0x05){	// If not accepted, return with error
				ret_value=-1;
			}
			
			// wait for the not busy signal token response ( waits for the card to save 
			// all sent data ) "... As long as tge card is busy programming, a continuous
			// stream of busy tokens will be sent to the host ( effectively holding the 
			// DataOut line to '0' ) ...".
			ui8_aux = SDMMC_WaitR1bResponseNotBusy(unit_idx);
			if (ui8_aux==0x00){
				ret_value=-1;
			}
			
		}//if
		
		SDMMC_unselect_card(unit_idx);

	}//if ( (unit_idx<SDMMC_MAX_UNITS) && (SDMMC_units[unit_idx]!=INVALID) ){

	return ret_value;
		
}//SDMMC_card_write_single_block



int8_t SDMMC_card_write_multiple_blocks(uint8_t unit_idx, uint8_t *buff, uint64_t sector, uint16_t count){
	int8_t  ret_value = 1;
	uint8_t tx_byte[6];
	uint8_t rx_byte[4];
	uint8_t r1_byte=0x00;
	uint8_t ui8_aux=0x00;
	
	uint32_t address;
	uint8_t address_byte_0;
	uint8_t address_byte_1;
	uint8_t address_byte_2;
	uint8_t address_byte_3;

	uint16_t ui16_bytes_count;
	uint16_t ui16_max_bytes_block;
	uint16_t ui16_blocks_count;
	uint16_t ui16_total_byte_count;
	

	if (unit_idx>=SDMMC_MAX_UNITS){
		
		ret_value=-1;
		
	}else{
		
		// IMPORTANT: "...SDSC Card (CCS=0) uses byte unit address and SDHC and
		// SDXC Cards (CCS=1) use block unit address (512 bytes unit)..."
		// So the access is done by bytes in SDSC and MMC Card mode and by blocks
		// in SDXC and SDHC mode
		switch (SDMMC_units[unit_idx].st_type){
			
			case SDv2_HC_XC:
			
				//  SD HIGH CAPACITY,  SD EXTENDED CAPACITY work with "sectors" addresses
				address = sector;
				address_byte_0=(uint8_t)(address&(0x000000FF));
				address_byte_1=(uint8_t)((address&(0x0000FF00))>>8);
				address_byte_2=(uint8_t)((address&(0x00FF0000))>>16);
				address_byte_3=(uint8_t)((address&(0xFF000000))>>24);
			
				// In SD HC and SD XC the block length is by default 512, so CMD16 SET_BLOCKLEN is not needed
				break;
			
			default:
			
				//  SDv1, SDv2 and MMCv3 modes work with "byte" addresses
				address=(sector*SDMMC_SECTOR_SIZE);
				address_byte_0=0x00;
				address_byte_1=(uint8_t)((address&(0x0000FF00))>>8);
				address_byte_2=(uint8_t)((address&(0x00FF0000))>>16);
				address_byte_3=(uint8_t)((address&(0xFF000000))>>24);

				// CMD16 SET_BLOCKLEN Selects a block length (in bytes) for all following block commands
				// (read & write) and it is only used in SDv1, SDv2 and MMCv3 modes ( SDv2_HC_XC always
				// uses 512 sector block size so SET_BLOCKLEN is not needed )
				SDMMC_select_card(unit_idx);
				tx_byte[0]=0x50;// CMD16 = 01 010000  = 0x50
				tx_byte[1]=0x00;
				tx_byte[2]=0x00;
				tx_byte[3]=(uint8_t)((SDMMC_units[unit_idx].sector_length&0xFF00)>>8);// high 8 bits of the length
				tx_byte[4]=(uint8_t)SDMMC_units[unit_idx].sector_length&0x00FF;// low 8 bits of the length
				tx_byte[5]=0xFF;
				r1_byte=SDMMC_send_command_R1(unit_idx, tx_byte, rx_byte);
				SDMMC_unselect_card(unit_idx);
				break;
			
		}//switch
	
		// if the card is an SD type ( SDv1,SDv2 or SDv2_HC_XC ) then it accepts "specific commands" (ACMD) so  send the ACMD23 SET_WR_BLK_ERASE_COUNT
		// command that pre-erases the sectors before writting to increase the speed of multiple blocks write operation. If card is 
		// an MMC it does not support "specific commands" (ACMD) so do not send the ACMD23
		if ( (SDMMC_units[unit_idx].st_type == SDv1) || (SDMMC_units[unit_idx].st_type == SDv2) || (SDMMC_units[unit_idx].st_type == SDv2_HC_XC) ){

					// CMD55 APP_CMD Defines to the card that the next command is an ACMD application specific command rather 
					// than a standard command. CMD55 must be sent prior to ACMD41 ( CMD55 preceeds all ACMDxx commands ).
					SDMMC_select_card(unit_idx);
					tx_byte[0]=0x77;//CMD55 = 01 110111 = 0x77
					tx_byte[1]=0x00;
					tx_byte[2]=0x00;
					tx_byte[3]=0x00;
					tx_byte[4]=0x00;
					tx_byte[5]=0xFF;
					r1_byte=SDMMC_send_command_R1(unit_idx, tx_byte, rx_byte);
					SDMMC_unselect_card(unit_idx);				
						
					// ACMD23 SET_WR_BLK_ERASE_COUNT To send ACMDxx it must be preceeded by a CMD55 command. ACMD23 command sets
					// the number of wirte blocks to be pre-erased before writing ( to be used for faster Multiple Block WR
					// commad, '1' = default one wr block ). Artument: [31:23] stuff bits , [22:0] number of blocks.
						
					// Reserved bits shall be set to 
					SDMMC_select_card(unit_idx);
					tx_byte[0]=0x57; // ACMD23 = 01 010111 = 0x57
					
					tx_byte[1]=0x40;
					tx_byte[2]=0x00;
					tx_byte[3]=(uint8_t)((count&&0xFF00)>>8);
					tx_byte[4]=(uint8_t)(count&&0x00FF);
					tx_byte[5]=0xFF;
					r1_byte=SDMMC_send_command_R1(unit_idx,  tx_byte, rx_byte);
					SDMMC_unselect_card(unit_idx);		
					
		}//if SDMMC_units[unit_idx].st_type == SDv1 ...
		
		// CMD25 WRITE_MULTIPLE_BLOCK Continuously transfers data blocks from host to card
		// and interrupts transmission with a STOP_TRANSMISSION command. Block length is 
		// specified the same as READ_SINGLE_BLOCK.
		SDMMC_select_card(unit_idx);
		tx_byte[0]=0x59;// CMD25 = 01 011001 = 0x59
		tx_byte[1]=address_byte_3;
		tx_byte[2]=address_byte_2;
		tx_byte[3]=address_byte_1;
		tx_byte[4]=address_byte_0;
		tx_byte[5]=0xFF;
		r1_byte=SDMMC_send_command_R1(unit_idx, tx_byte, rx_byte);
		
		if (r1_byte!=0x00){
			
			// if R1 value is NOT 0x00 it means that WRITE_MULTIPLE_BLOCK  not been accepted by the card
			ret_value=-1;
			
		}else{
			
			// if R1 value is 0x00 it means that WRITE_MULTIPLE_BLOCK operation has been accepted by the card
			
			// start sending blocks
			ui16_blocks_count =0;
			ui16_total_byte_count = 0;
			while ( (ret_value>=0) && (ui16_blocks_count<count) ){

				// send the Start Block before the data block
				SDMMC_send_byte(unit_idx,0xFC);
			
				// send the data block
				ui16_bytes_count=0;
				ui16_max_bytes_block=SDMMC_units[unit_idx].sector_length;
				while (ui16_bytes_count<ui16_max_bytes_block){
					ui8_aux=SDMMC_send_byte(unit_idx,buff[ui16_total_byte_count]);
					ui16_bytes_count++;
					ui16_total_byte_count++;
				}//while

				// send dummy CRC
				SDMMC_send_byte(unit_idx,0xFF);
				SDMMC_send_byte(unit_idx,0xFF);

				ui16_blocks_count++;

				// wait the Data Response "... After a data block has been received, the card will
				// respond with a data-response toke ...".
				ui8_aux=SDMMC_WaitDataResponse(unit_idx);
				if ((ui8_aux & 0x1F) != 0x05){	// If not accepted, return with error
					ret_value=-1;
				}
			
				// wait for the not busy signal token response ( waits for the card to save
				// all sent data ) "... As long as tge card is busy programming, a continuous
				// stream of busy tokens will be sent to the host ( effectively holding the
				// DataOut line to '0' ) ...".
				ui8_aux = SDMMC_WaitR1bResponseNotBusy(unit_idx);
				if (ui8_aux==0x00){
					ret_value=-1;
				}
				
			}// while

			// CMD12 STOP_TRANSMISSION Forces the card to stop transmission during a multiple block read operation.
			SDMMC_select_card(unit_idx);
			tx_byte[0]=0x4C;// CMD12 0100 1100
			tx_byte[1]=0x00;
			tx_byte[2]=0x00;
			tx_byte[3]=0x00;
			tx_byte[4]=0x00;
			tx_byte[5]=0xFF;
			r1_byte=SDMMC_send_command_R1(unit_idx, tx_byte, rx_byte);

			// wait for the not busy signal token response ( waits for the card to save
			// all sent data ) "... As long as tge card is busy programming, a continuous
			// stream of busy tokens will be sent to the host ( effectively holding the
			// DataOut line to '0' ) ...".
			ui8_aux = SDMMC_WaitR1bResponseNotBusy(unit_idx);
			if (ui8_aux==0x00){
				ret_value=-1;
			}

		}//if
		
		SDMMC_unselect_card(unit_idx);

	}//if ( (unit_idx<SDMMC_MAX_UNITS) && (SDMMC_units[unit_idx]!=INVALID) ){

	return ret_value;
		
}//SDMMC_card_write_multiple_blocks