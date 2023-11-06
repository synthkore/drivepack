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
#include "usart.h"

uint8_t  ui8_usart_1kXmodem_data_buf[USART_1KXMODEM_BUF_SIZE];
uint8_t  ui8_usart_1kXmodem_packet_n;// the number of packet currently sent ( in tx mode ) or received ( in rx mode )
uint16_t u16_usart_1kX_pack_read_bytes;// index of the last byte read from the 1KXmodem buffer received packet
uint16_t u16_usart_1kX_pack_written_bytes;// index of the last byte written on the 1KXmodem buffer before sending the packet
uint8_t  ui8_usart_1kX_retries;

enum {
	PACKET_HEADER_RX = 0,
	PACKET_DATA_RX,
	PACKET_CRC_RX,
	PACKET_RECEIVED,
} e_data_io_state;


// Synthkore  27-09-2020 Tolaemon
// Function: uint8_t USART_send_char(uint8_t char_to_send)
// Sends the received char through the USART
// Receives:
//    ui8_to_send:the code of the char to send through the USART
// Returns:
// By value:
//   TRUE: if the character has been sent ( the USART is ready )
//   FALSE: if was not possible to send the character
uint8_t USART_send_char(uint8_t ui8_to_send){
	uint8_t ret_val = TRUE;
  
 
 	while (SERCOM3->USART.INTFLAG.bit.DRE != 1 ){}

	// SERCOM3->USART.INTENSET.bit.TXC == 1;
	SERCOM3->USART.DATA.bit.DATA = ui8_to_send;
	ui8_to_send = 0;
 	 
	return ret_val;
 
}//USART_send_char

// Synthkore  27-09-2020 Tolaemon
// Function: uint8_t USART_send_string(uint8_t * ui8_to_string, uint16_t ush_MAX_LEN )
// Sends the received string through the USART. It sends all the characters until
// it reaches the end of string symbol '\0' or stops when it has reached the
// number of characters set in ush_MAX_LEN
// Receives:
//   ui8_to_string: the string with the array of characters to send
//   ush_MAX_LEN: the maximum allowed length for the string to send
// Returns:
// By reference:
// By value:
//    TRUE: if the string has been sent
//    FALSE: if there has been any kind of error while transmitting the string, or
//  the maximum number of allowed characters has been reached.
uint8_t USART_send_string(uint8_t * ui8_to_string, uint16_t ush_MAX_LEN ){
	uint16_t ui16_ch_counter;
	uint8_t ret_val = TRUE;

	
	ui16_ch_counter = 0;
	while ( (ui16_ch_counter<ush_MAX_LEN)&&(ui8_to_string[ui16_ch_counter]!='\0') ){
		
		if (SERCOM3->USART.INTFLAG.bit.DRE == 1) {
			SERCOM3->USART.DATA.bit.DATA = ui8_to_string[ui16_ch_counter];
			ui16_ch_counter++;
		}//if					
	
	}//while
	
	if (ui16_ch_counter>=ush_MAX_LEN) ret_val = FALSE;
	
	return ret_val;
	
}//USART_send_string


// Synthkore  27-09-2020 Tolaemon
// Function: uint8_t USART_receive_char(uint8_t * ui8_received)
// Checks if a character has been received and takes it if it is present
// Receives:
// Returns:
// By reference:
//    pui8_received: the received character if a character was present in the USART
// By value:
//    TRUE: if a character has been read ( the USART is ready )
//    FALSE: if it has not been possible to read a character
uint8_t USART_receive_char(uint8_t * pui8_received){
	uint8_t ret_val = FALSE;


	if (SERCOM3->USART.INTFLAG.bit.RXC == 1 ){
		(*pui8_received) = SERCOM3->USART.DATA.bit.DATA;
		ret_val = TRUE;
	}else{
		(*pui8_received) = 0x00;
	}//if
	
	return ret_val;
	
}//USART_receive_char


// Synthkore  19-02-2022  Tolaemon
// Description:
//   Returns the requested number of bytes from the currently open 1KXModem connection. If the requested
// bytes are not available in the current packet then requests following packet to the sender.
// Receives:
//   ui32_n_bytes_to_read: the number of bytes to read from the currently open 1KXModem connection
// Returns:
//  By reference:
//   p_read_data: buffer where the function stores data read from current 1KXModem connection
//   pui32_n_read_bytes: number of bytes that the function could finally read.
//  By value:
//	 USART_1KX_EOT: End Of Transmission char code received from sender
//	 USART_1KX_PACKET_OK: current packet has been completely and successfully received
//	 USART_1KX_TOUT_ERR: timeout receiving the packet content ( no bytes received during the configured timeout period ).
//	 USART_1KX_HEADER_ERR: error on received header ( wrong packet number )
//	 USART_1KX_CRC_ERR: received wrong packet CRC
int16_t USART_1KXmodem_read(void * p_read_data, uint32_t ui32_n_bytes_to_read, uint32_t * pui32_n_read_bytes){
	int16_t i16_ret_val = 0;
	uint32_t  ui32_n_read_bytes = 0;
	uint8_t * pui8_read_data = (uint8_t*)p_read_data;


    ui32_n_read_bytes = 0;
	i16_ret_val = 0;

    // keep copying the requested bytes of the last read 1KXmodem packet to the destination buffer. If 
	// the end of the last received packet has been reached, then request the following packet to keep
	// reading from following packet
	while ( (i16_ret_val==0) && (ui32_n_read_bytes<ui32_n_bytes_to_read) ){
         
		 pui8_read_data[ui32_n_read_bytes]=ui8_usart_1kXmodem_data_buf[u16_usart_1kX_pack_read_bytes];
		 ui32_n_read_bytes++;
		 u16_usart_1kX_pack_read_bytes++;
	 
		 // if the last data byte of the packet has been reached then request sender
		 // to send the next packet
		 if (u16_usart_1kX_pack_read_bytes>=USART_1KXMODEM_BUF_SIZE){
			 i16_ret_val=USART_1KXmodem_rx_packet_next();
			 u16_usart_1kX_pack_read_bytes = 0;
		 }//if
		 
	 }//while

     (*pui32_n_read_bytes)=ui32_n_read_bytes;

	return i16_ret_val;
	
}//USART_1KXmodem_read


int16_t USART_1KXmodem_write(void * p_data_to_write, uint32_t ui32_n_bytes_to_write, uint32_t * pui32_n_written_bytes){
	int16_t   i16_ret_val = 0;
	uint32_t  ui32_n_written_bytes = 0;
	uint8_t * pui8_write_data = (uint8_t*)p_data_to_write;


	ui32_n_written_bytes = 0;
	i16_ret_val = 0;

	// keep writing the received bytes in the 1kXModem buffer
	while ( (i16_ret_val==0) && (ui32_n_written_bytes<ui32_n_bytes_to_write) ){
		
		ui8_usart_1kXmodem_data_buf[u16_usart_1kX_pack_written_bytes] = pui8_write_data[ui32_n_written_bytes];
		ui32_n_written_bytes++;
		u16_usart_1kX_pack_written_bytes++;
		
		// if the last data byte of the packet has been reached then send it
		// and then start filling a new packet
		if (u16_usart_1kX_pack_written_bytes>=USART_1KXMODEM_BUF_SIZE){
			i16_ret_val=USART_1KXmodem_tx_current_packet();
			u16_usart_1kX_pack_written_bytes = 0;
		}//if
		
	}//while

	(*pui32_n_written_bytes)=ui32_n_written_bytes;

	return i16_ret_val;
	
}//USART_1KXmodem_write


// Synthkore  06-07-2021  Tolaemon
// Procedure that manages the start of 1KXmodem file transmission in the sender side. It also
// receives first transmission packet.
// packet.
// Receives:
// Returns:
//  By reference:
//	By value:
//	 USART_1KX_EOT: End Of Transmission char code received from sender
//	 USART_1KX_PACKET_OK: current packet has been completely and successfully received
//	 USART_1KX_TOUT_ERR: timeout receiving the packet content ( no bytes received during the configured timeout period ).
//	 USART_1KX_HEADER_ERR: error on received header ( wrong packet number )
//	 USART_1KX_CRC_ERR: received wrong packet CRC
// Note:
//  1K Communications sequence is ( C='C', ACK = 6, NACK=21, STX=2, EOT=4 ):
///          Figure 1.  XMODEM-1k Blocks
//    SENDER                             RECEIVER
//                                   "sx -k foo.bar"
//    "foo.bar open x.x minutes"
//       <--------------------------------- C
//    STX 01 FE Data[1024] CRC CRC --------->
//       <-------------------------------- ACK
//    STX 02 FD Data[1024] CRC CRC --------->
//       <-------------------------------- ACK
//    STX 03 FC Data[1000] CPMEOF[24] CRC CRC
//       <-------------------------------- ACK
//    EOT ---------------------------------->
//       <-------------------------------- ACK
int16_t USART_1KXmodem_rx_open(){
	int16_t i16_ret_val = 0;


	// initialize transmission variables
	ui8_usart_1kXmodem_packet_n = 1;
	
	// send DATA_IO_CHAR_C char code to request sender to start file transmission
	USART_send_char(USART_1KXMODEM_CHAR_C);
	
	// wait to receive the answer from sender with first 1KXmodem data packet
	i16_ret_val=USART_1KXmodem_rx_packet();

	return i16_ret_val;

}//USART_1KXmodem_rx_open


// Synthkore  27-12-2022  Tolaemon
// Description:
//   Executes the final steps to close a previously open 1KXModem RX connection.
// Receives:
// Returns:
//  By reference:
//  By value:
int16_t USART_1KXmodem_rx_close(){
	int16_t i16_ret_val = 0;
	
	// NOTHING TO DO: because system is receiver and transmission is always closed by transmitter 
	
	return i16_ret_val;
	
}//USART_1KXmodem_rx_close


// Synthkore  06-07-2021  Tolaemon
// Procedure that manages the start of 1KXmodem file transmission in the receiver side
// Receives:
// Returns:
//  By reference:
//	By value:
//	 USART_1KX_EOT: End Of Transmission char code received from sender
//	 USART_1KX_PACKET_OK: current packet has been completely and successfully received
//	 USART_1KX_TOUT_ERR: timeout receiving the packet content ( no bytes received during the configured timeout period ).
//	 USART_1KX_HEADER_ERR: error on received header ( wrong packet number )
//	 USART_1KX_CRC_ERR: received wrong packet CRC
// Note:
//  1K Communications sequence is ( C='C', ACK = 6, NACK=21, STX=2, EOT=4 ):
///          Figure 1.  XMODEM-1k Blocks
//    SENDER                             RECEIVER
//                                   "sx -k foo.bar"
//    "foo.bar open x.x minutes"
//       <--------------------------------- C
//    STX 01 FE Data[1024] CRC CRC --------->
//       <-------------------------------- ACK
//    STX 02 FD Data[1024] CRC CRC --------->
//       <-------------------------------- ACK
//    STX 03 FC Data[1000] CPMEOF[24] CRC CRC
//       <-------------------------------- ACK
//    EOT ---------------------------------->
//       <-------------------------------- ACK
int16_t USART_1KXmodem_tx_open(){
	int16_t i16_ret_val = 0;
	TIMING_TICKS timer_wait;
	uint8_t ui8_received_char = 0;


	// initialize transmission variables
	ui8_usart_1kXmodem_packet_n = 1;

    // wait for the start transmission USART_1KXMODEM_CHAR_C byte from the receiver. Receiver 
	// should keep sending USART_1KXMODEM_CHAR_C byte indicating that he is ready to receive data.
    timer_wait = TIMING_SetTicksDelay(TIMMING_TICKS_500_MS);
    ui8_received_char = 0;
	while ( (ui8_received_char!=USART_1KXMODEM_CHAR_C) && (!TIMING_CheckTicksDelayReached(timer_wait)) ){
		
		USART_receive_char(&ui8_received_char);
		
	}//while

    // check timeout condition error code
    if ((i16_ret_val>=0) && (ui8_received_char!=USART_1KXMODEM_CHAR_C)){
	    i16_ret_val=USART_1KX_TOUT_ERR;
	}else{
		i16_ret_val=USART_1KX_CHAR_C_OK;
    }//if

	return i16_ret_val;

}//USART_1KXmodem_tx_open

// Synthkore  27-12-2022  Tolaemon
// Description:
//   Executes the final steps to close a previously open 1KXModem TX connection.
// Receives:
// Returns:
//  By reference:
//  By value:
int16_t USART_1KXmodem_tx_close(){
	int16_t   i16_ret_val = 0;


    // before closing tx check if there are pending bytes to send in the packet buffer. If
	// there are pending bytes to send then pad the packet data buffer with USART_1KXMODEM_CHAR_CPM_EOF 
	// and then  send the packet
    if (u16_usart_1kX_pack_written_bytes !=0){
		
		// pad packet data buffer with USART_1KXMODEM_CHAR_CPM_EOF
		while (u16_usart_1kX_pack_written_bytes<USART_1KXMODEM_BUF_SIZE){
		   	ui8_usart_1kXmodem_data_buf[u16_usart_1kX_pack_written_bytes] = USART_1KXMODEM_CHAR_CPM_EOF;
		   	u16_usart_1kX_pack_written_bytes++;		
		}//while
		
		// transmit last data buffer packet
		i16_ret_val=USART_1KXmodem_tx_current_packet();
		u16_usart_1kX_pack_written_bytes = 0;
		
	}//if

    // send the END OF TRANSMISSION byte code USART_1KXMODEM_CHAR_EOT
   USART_send_char(USART_1KXMODEM_CHAR_EOT);

	return i16_ret_val;
	
}//USART_1KXmodem_tx_close


// Synthkore  13-11-2022  Tolaemon
// Procedure that calculates and returns the CRC of the first 'count' bytes in the buffer
// pointed by ptr.
// Receives:
//	*ptr pointer to the buffer with the bytes we want to calculate the CRC
//  count the number of bytes whose CRC we want to calculate
// Returns:
//	By value:
//   the calculated CRC value
int16_t USART_1KXmodem_calcrc(uint8_t *ptr, int count){
	int16_t  crc;
	uint8_t i;


	crc = 0;
	while (--count >= 0){
		
		crc = crc ^ (int) *ptr++ << 8;
		i = 8;
		do{

			if (crc & 0x8000)
			crc = crc << 1 ^ 0x1021;
			else
			crc = crc << 1;
		} while(--i);
	}
	return (crc);
	
}//USART_1KXmodem_calcrc


// Synthkore  06-07-2021  Tolaemon
// That acknowledges previous packet sent by the sender and receives next packet
// Receives:
// Returns:
//  By reference:
//	By value:
//	 USART_1KX_EOT: End Of Transmission char code received from sender
//	 USART_1KX_PACKET_OK: current packet has been completely and successfully received
//	 USART_1KX_TOUT_ERR: timeout receiving the packet content ( no bytes received during the configured timeout period ).
//	 USART_1KX_HEADER_ERR: error on received header ( wrong packet number )
//	 USART_1KX_CRC_ERR: received wrong packet CRC
// Note:
//  1K Communications sequence is ( C='C', ACK = 6, NACK=21, STX=2, EOT=4 ):
//          Figure 1.  XMODEM-1k Blocks
//    SENDER                             RECEIVER
//                                   "sx -k foo.bar"
//    "foo.bar open x.x minutes"
//       <--------------------------------- C
//    STX 01 FE Data[1024] CRC CRC --------->
//       <-------------------------------- ACK
//    STX 02 FD Data[1024] CRC CRC --------->
//       <-------------------------------- ACK
//    STX 03 FC Data[1000] CPMEOF[24] CRC CRC
//       <-------------------------------- ACK
//    EOT ---------------------------------->
//       <-------------------------------- ACK
static int16_t USART_1KXmodem_rx_packet_next(){
	int16_t i16_ret_val = 0;


	// IMPORTANT!!: bytes are read from the USART by polling ( not by interrupts ) and for that reason
	// if the application does not poll the USART while transmitter is sending bytes then these bytes 
	// are lost. So if the transmitter sends the packet while the application is doing other things, 
	// all sent data is lost ( except the first 2 bytes, because the USART can only store 2 bytes ). 
	// As the application must execute some actions after receiving a packet ( specially after
	// receiving the first packet in USART_1KXmodem_rx_establish ),  and as the transmitter starts 
	// sending the packet just after receiving the ACK, the application sends the ACK just before
	// receiving the following packet and not after receiving a packet. Sending the ACK just after 
	// receiving the packet would cause the transmitter start sending following packet bytes while 
	// the application is still processing the previous packet ( specially after receiving first 
	// packet) and this data would be lost.
		 
	// send DATA_IO_CHAR_ACK byte code to to confirm to sender previous data packet
	// reception and to request to sender next packet transmission
	USART_send_char(USART_1KXMODEM_CHAR_ACK);
	
	// wait to receive the next 1KXModem data packet from sender
	i16_ret_val=USART_1KXmodem_rx_packet();

	return i16_ret_val;

}//USART_1KXmodem_rx_packet_next


// Synthkore  11-12-2021  Tolaemon
// Sends the 1Kxmodem buffer current content to the receiver and waits for the acknowledge
// Receives:
// Returns:
//  By reference:
//	By value:
//	 USART_1KX_EOT: End Of Transmission char code received from sender
//	 USART_1KX_PACKET_OK: current packet has been completely and successfully received
//	 USART_1KX_TOUT_ERR: timeout receiving the packet content ( no bytes received during the configured timeout period ).
//	 USART_1KX_HEADER_ERR: error on received header ( wrong packet number )
//	 USART_1KX_CRC_ERR: received wrong packet CRC
// Note:
//  1K Communications sequence is ( C='C', ACK = 6, NACK=21, STX=2, EOT=4 ):
//          Figure 1.  XMODEM-1k Blocks
//    SENDER                             RECEIVER
//                                   "sx -k foo.bar"
//    "foo.bar open x.x minutes"
//       <--------------------------------- C
//    STX 01 FE Data[1024] CRC CRC --------->
//       <-------------------------------- ACK
//    STX 02 FD Data[1024] CRC CRC --------->
//       <-------------------------------- ACK
//    STX 03 FC Data[1000] CPMEOF[24] CRC CRC
//       <-------------------------------- ACK
//    EOT ---------------------------------->
//       <-------------------------------- ACK
static int16_t USART_1KXmodem_tx_current_packet(){
	int16_t i16_ret_val = 0;
	TIMING_TICKS timer_wait;
	uint8_t ui8_received_char = 0;


	// send tan 1kXmodem packet with the current content of the 1KXmodem buffer
	i16_ret_val=USART_1KXmodem_tx_packet();

    // wait for the acknowledge from the receiver
    timer_wait = TIMING_SetTicksDelay(TIMMING_TICKS_5_S);
    ui8_received_char = 0;
	while ( (i16_ret_val>=0) && (ui8_received_char!=USART_1KXMODEM_CHAR_ACK) && (!TIMING_CheckTicksDelayReached(timer_wait)) ){
	
		USART_receive_char(&ui8_received_char);
		
	}//while

    // check timeout condition error code
    if ((i16_ret_val>=0) &&  (ui8_received_char!=USART_1KXMODEM_CHAR_ACK)){
		i16_ret_val=USART_1KX_TOUT_ERR;
	}//if

	return i16_ret_val;

}//USART_1KXmodem_tx_current_packet


// Synthkore  06-07-2021  Tolaemon
// Procedure that manages communications to receive next 1K XModem packet
// Receives:
// Returns:
//  By reference:
//	By value:
//	 USART_1KX_EOT: End Of Transmission char code received from sender
//	 USART_1KX_PACKET_OK: current packet has been completely and successfully received
//	 USART_1KX_TOUT_ERR: timeout receiving the packet content ( no bytes received during the configured timeout period ).
//	 USART_1KX_HEADER_ERR: error on received header ( wrong packet number )
//	 USART_1KX_CRC_ERR: received wrong packet CRC
// Note:
//  1K Communications sequence is ( C='C', ACK = 6, NACK=21, STX=2, EOT=4 ):
//          Figure 1.  XMODEM-1k Blocks
//    SENDER                             RECEIVER
//                                   "sx -k foo.bar"
//    "foo.bar open x.x minutes"
//       <--------------------------------- C
//    STX 01 FE Data[1024] CRC CRC --------->
//       <-------------------------------- ACK
//    STX 02 FD Data[1024] CRC CRC --------->
//       <-------------------------------- ACK
//    STX 03 FC Data[1000] CPMEOF[24] CRC CRC
//       <-------------------------------- ACK
//    EOT ---------------------------------->
//       <-------------------------------- ACK
static int16_t USART_1KXmodem_rx_packet(){
	uint32_t ui32_data_bytes = 0;
	uint32_t ui32_packet_bytes = 0;
	uint8_t ui8_received_char;
	TIMING_TICKS timer_wait;
	uint8_t ui8_crc1 = 0;
	uint8_t ui8_crc2 = 0;
	uint8_t ui8_aux = 0;
	uint8_t ui8_aux2 = 0;
	int16_t i16_aux = 0;
	int16_t i16_ret_val = 0;
	
	
	// initializations
	timer_wait = TIMING_SetTicksDelay(TIMMING_TICKS_500_MS);// initialize the timer used to detect communications timeout
	e_data_io_state = PACKET_HEADER_RX;// start in read HEADER state to process next packet header
	ui32_packet_bytes = 0;
	ui32_data_bytes = 0;
	i16_ret_val = USART_1KX_IN_PROGRESS;
	
	while (i16_ret_val == USART_1KX_IN_PROGRESS){
			
		ui8_aux = USART_receive_char(&ui8_received_char);
		if (ui8_aux==TRUE){

			// if a character has been received then update the timer used to detect communications timeout
			timer_wait = TIMING_SetTicksDelay(TIMMING_TICKS_500_MS);
			
			// process received byte
			switch (e_data_io_state){
				
				case PACKET_HEADER_RX:
				
					// receive and process next header byte
					switch (ui32_packet_bytes){
					
						case 0:
						
							// header byte 0 should be STX, error otherwise
							if (ui8_received_char==USART_1KXMODEM_CHAR_STX){
								// nothing to to do, continue reading following bytes 
							}else if (ui8_received_char==USART_1KXMODEM_CHAR_EOT){
								// by any reason sender has send the DATA_IO_EOT ( End Of Transmission) byte to end
								// file transmission send last DATA_IO_CHAR_ACK byte code to acknowledge the end of
								// transmission requested by sender
								USART_send_char(USART_1KXMODEM_CHAR_ACK);
								i16_ret_val = USART_1KX_EOT;// return END OF TRANSMISSION
							}else{
								// invalid char received 
								i16_ret_val = USART_1KX_HEADER_ERR;// return HEADER RX ERROR
							}//if
							break;
					
						case 1:
						
							// header byte 1 should contain the current packet number
							if (ui8_received_char!=ui8_usart_1kXmodem_packet_n){
								// invalid char received
								i16_ret_val = USART_1KX_HEADER_ERR;// return HEADER RX ERROR
							}//if
							break;
					
						case 2:
						
							// header byte 2 should contain the inverted current packet number
							ui8_aux = (uint8_t)(~ui8_usart_1kXmodem_packet_n);
							if (ui8_received_char != ui8_aux){
								// invalid char received
								i16_ret_val = USART_1KX_HEADER_ERR;// return HEADER RX ERROR
							}else{
								// start receiving data bytes
								ui32_data_bytes = 0;
								e_data_io_state = PACKET_DATA_RX;
							}//if
							break;
					
					}//switch
					break;//PACKET_HEADER_RX:

				case PACKET_DATA_RX:
				
					// receive and store next data byte			
					if (ui32_data_bytes<USART_1KXMODEM_BUF_SIZE){
					
						ui8_usart_1kXmodem_data_buf[ui32_data_bytes]=ui8_received_char;
						ui32_data_bytes++;
					
						if (ui32_data_bytes>=USART_1KXMODEM_BUF_SIZE){
							// all data bytes have been received so jump to receive the CRC
							e_data_io_state = PACKET_CRC_RX;
						}
					
					}//if
					break;//case PACKET_DATA_RX:

				case PACKET_CRC_RX:
				
					// receive CRC-16 bytes
					if (ui32_packet_bytes == 1027){
					
						// take first CRC-16 byte
						ui8_crc1 = ui8_received_char;
					
					}else if (ui32_packet_bytes == 1028){
					
						// take second CRC-16 byte
						ui8_crc2 = ui8_received_char;
						// calculate data CRC and compare result with received CRC bytes
						i16_aux = USART_1KXmodem_calcrc(ui8_usart_1kXmodem_data_buf, USART_1KXMODEM_BUF_SIZE);
						ui8_aux=(uint8_t)((i16_aux&0xFF00)>>8);
						ui8_aux2=(uint8_t)(i16_aux&0x00FF);
						if ( (ui8_crc1==ui8_aux) && (ui8_crc2==ui8_aux2) ){
						
							// packet has been successfully received
							ui8_usart_1kXmodem_packet_n++;
							e_data_io_state = PACKET_RECEIVED;
							i16_ret_val= USART_1KX_PACKET_OK;
						
						}else{
							
							// packet CRC is wrong
							i16_ret_val = USART_1KX_CRC_ERR;// CRC ERROR
						
						}//if
					
					}else{
					
						// something has gone wrong because CRC bytes should be packet bytes #1027 and #108
						i16_ret_val = USART_1KX_CRC_ERR;// CRC ERROR
					
					}//if
					break;//case PACKET_CRC_RX:
				
			}//switch

			// increase the counter of current packet received bytes
			ui32_packet_bytes++;
			
		}//if (ui8_aux==TRUE)

		// check receive char timeout condition
		if (TIMING_CheckTicksDelayReached(timer_wait)){
			// no answer received
			i16_ret_val = USART_1KX_TOUT_ERR;// USART_receive_char timeout error
		}//if

	}//while

	return i16_ret_val;

}//USART_1KXmodem_rx_packet


// Synthkore  06-07-2021  Tolaemon
// Procedure that manages communications to send current 1K XModem packet
// Receives:
// Returns:
//  By reference:
//	By value:
//	 USART_1KX_EOT: End Of Transmission char code received from sender
//	 USART_1KX_PACKET_OK: current packet has been completely and successfully received
//	 USART_1KX_TOUT_ERR: timeout receiving the packet content ( no bytes received during the configured timeout period ).
//	 USART_1KX_HEADER_ERR: error on received header ( wrong packet number )
//	 USART_1KX_CRC_ERR: received wrong packet CRC
// Note:
//  1K Communications sequence is ( C='C', ACK = 6, NACK=21, STX=2, EOT=4 ):
//          Figure 1.  XMODEM-1k Blocks
//    SENDER                             RECEIVER
//                                   "sx -k foo.bar"
//    "foo.bar open x.x minutes"
//       <--------------------------------- C
//    STX 01 FE Data[1024] CRC CRC --------->
//       <-------------------------------- ACK
//    STX 02 FD Data[1024] CRC CRC --------->
//       <-------------------------------- ACK
//    STX 03 FC Data[1000] CPMEOF[24] CRC CRC
//       <-------------------------------- ACK
//    EOT ---------------------------------->
//       <-------------------------------- ACK
static int16_t USART_1KXmodem_tx_packet(){
	uint8_t ui8_aux = 0;
	int16_t i16_aux = 0;
	int16_t i16_ret_val = 0;

	
	// Send HEADER
	USART_send_char(USART_1KXMODEM_CHAR_STX); // Header: STX 
	ui8_aux = (uint8_t)ui8_usart_1kXmodem_packet_n;
	USART_send_char(ui8_aux); // Header: packet number
	USART_send_char(~ui8_aux);// Header: !(packet number)
	// Send BODY
	for (i16_aux=0;i16_aux<USART_1KXMODEM_BUF_SIZE;i16_aux++){
		ui8_aux = ui8_usart_1kXmodem_data_buf[i16_aux];
		USART_send_char(ui8_aux);
	}//for
	// Send CRC
	i16_aux = USART_1KXmodem_calcrc(ui8_usart_1kXmodem_data_buf, USART_1KXMODEM_BUF_SIZE);
	ui8_aux = (uint8_t)((i16_aux&0xFF00)>>8);
	USART_send_char(ui8_aux);// highest CRC-16 byte
	ui8_aux = (uint8_t)(i16_aux&0x00FF);
	USART_send_char(ui8_aux);// lowest CRC-16 byte

    ui8_usart_1kXmodem_packet_n++;

	return i16_ret_val;

}//USART_1KXmodem_tx_packet
