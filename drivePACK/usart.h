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
#ifndef USART_H_
#define USART_H_

#include "sam.h"
#include "gpio.h"
#include "sys_defs.h"
#include "timing.h"

#define _USART_SEND_STRING(string) USART_send_string(string,sizeof(string))

// 1KXmodem char codes
#define USART_1KXMODEM_BUF_SIZE              1024
#define USART_1KXMODEM_CHAR_C                 'C'
#define USART_1KXMODEM_CHAR_STX                02
#define USART_1KXMODEM_CHAR_ACK                06
#define USART_1KXMODEM_CHAR_NACK               21
#define USART_1KXMODEM_CHAR_EOT                04
#define USART_1KXMODEM_CHAR_CPM_EOF            26

// 1KXmodem error codes
#define USART_1KX_EOT                 3
#define USART_1KX_CHAR_C_OK           2
#define USART_1KX_PACKET_OK           1
#define USART_1KX_IN_PROGRESS         0
#define USART_1KX_TOUT_ERR           -1
#define USART_1KX_HEADER_ERR         -2
#define USART_1KX_CRC_ERR            -3
#define USART_1KX_NOT_INITIALIZED    -4

#define USART_1KXMODEM_RX_RETRIES              30 // number of retries to start file RX before considering error

// Synthkore  27-09-2020 Tolaemon
// Function: uint8_t USART_send_char(uint8_t char_to_send)
// Sends the received char through the USART
// Receives:
//    ui8_to_send:the code of the char to send through the USART
// Returns:
// By value:
//   TRUE: if the character has been sent ( the USART is ready )
//   FALSE: if was not possible to send the character
uint8_t USART_send_char(uint8_t ui8_to_send);

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
uint8_t USART_send_string(uint8_t * ui8_to_string, uint16_t ush_MAX_LEN );

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
uint8_t USART_receive_char(uint8_t * pui8_received);

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
int16_t USART_1KXmodem_read(void * p_read_data, uint32_t ui32_n_bytes_to_read, uint32_t * pui32_n_read_bytes);

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
int16_t USART_1KXmodem_rx_open();

// Synthkore  27-12-2022  Tolaemon
// Description:
//   Executes the final steps to close a previously open 1KXModem RX connection.
// Receives:
// Returns:
//  By reference:
//  By value:
int16_t USART_1KXmodem_rx_close();

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
int16_t USART_1KXmodem_tx_open();

// Synthkore  27-12-2022  Tolaemon
// Description:
//   Executes the final steps to close a previously open 1KXModem TX connection.
// Receives:
// Returns:
//  By reference:
//  By value:
int16_t USART_1KXmodem_tx_close();

// Synthkore  13-11-2022  Tolaemon
// Procedure that calculates and returns the CRC of the first 'count' bytes in the buffer
// pointed by ptr.
// Receives:
//	*ptr pointer to the buffer with the bytes we want to calculate the CRC
//  count the number of bytes whose CRC we want to calculate
// Returns:
//	By value:
//   the calculated CRC value
static int16_t USART_1KXmodem_calcrc(uint8_t *ptr, int count);

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
static int16_t USART_1KXmodem_rx_packet_next();

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
static int16_t USART_1KXmodem_tx_current_packet();

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
static int16_t USART_1KXmodem_rx_packet();

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
static int16_t USART_1KXmodem_tx_packet();

#endif /* USART_H_ */