#include "xbee.h"
#include <stdio.h>

void initialize_controller_reciever(void) {
	MSS_UART_init(&g_mss_uart1, MSS_UART_9600_BAUD, MSS_UART_DATA_8_BITS | MSS_UART_NO_PARITY | MSS_UART_ONE_STOP_BIT);
	MSS_UART_set_rx_handler( &g_mss_uart1, uart1_rx_handler, MSS_UART_FIFO_EIGHT_BYTES);
}

void uart1_rx_handler( mss_uart_instance_t * this_uart ) {
	uint8_t rx_buff[8];
	char recieved_data[50];
	int rx_size = MSS_UART_get_rx( this_uart, rx_buff, sizeof(rx_buff) );
	char current_char = rx_buff[0];
	int i = 0, chr_index = 0;
	while (current_char != 10) {
		if (chr_index >= rx_size || chr_index == 8) {
			rx_size = MSS_UART_get_rx( this_uart, rx_buff, sizeof(rx_buff) );
			chr_index = 0;
			continue;
		}
		recieved_data[i] = current_char;
		i++;
		chr_index++;
		current_char = rx_buff[chr_index];
	}
	recieved_data[i] = 10;
	recieved_data[i+1] = 0;

	puts(recieved_data);

}
