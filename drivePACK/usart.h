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

uint8_t USART_send_char(uint8_t ui8_to_send);
uint8_t USART_send_string(uint8_t * ui8_to_string, uint16_t ush_MAX_LEN );
uint8_t USART_receive_char(uint8_t * pui8_received);
int16_t USART_1KXmodem_read(void * p_read_data, uint32_t ui32_n_bytes_to_read, uint32_t * pui32_n_read_bytes);
int16_t USART_1KXmodem_rx_open();
int16_t USART_1KXmodem_rx_close();
int16_t USART_1KXmodem_tx_open();
int16_t USART_1KXmodem_tx_close();
static int16_t USART_1KXmodem_calcrc(uint8_t *ptr, int count);
static int16_t USART_1KXmodem_rx_packet_next();
static int16_t USART_1KXmodem_tx_current_packet();
static int16_t USART_1KXmodem_rx_packet();
static int16_t USART_1KXmodem_tx_packet();

#endif /* USART_H_ */