#ifndef XBEE_H
#define XBEE_H

#include <inttypes.h>
#include "drivers/mss_uart/mss_uart.h"

int turn_direction;
uint8_t gas_button;
char recieved_data[20];

// Initialize uart interface and function handler for controller data reception
void initialize_controller_reciever(void);

// Function handler for recieving of controller data
void uart1_rx_handler( mss_uart_instance_t * this_uart );

#endif
