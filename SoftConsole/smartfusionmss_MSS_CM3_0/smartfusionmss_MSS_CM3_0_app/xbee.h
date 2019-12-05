#ifndef XBEE_H
#define XBEE_H

#include <inttypes.h>
#include "drivers/mss_uart/mss_uart.h"

#define ACCEL_CENTER 335

int game_status;

// Initialize uart interface and function handler for controller data reception
void XBEE_init(void);

// Send string over the xbee channel
void XBEE_send(char* string);

// Function handler for recieving of controller data
void uart1_rx_handler( mss_uart_instance_t * this_uart );

#endif
