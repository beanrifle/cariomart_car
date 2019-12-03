#include "xbee.h"
#include "CarControl.h"
#include <stdio.h>

int button_state=0;

void initialize_controller_reciever(void) {
	MSS_UART_init(&g_mss_uart1, MSS_UART_9600_BAUD, MSS_UART_DATA_8_BITS | MSS_UART_NO_PARITY | MSS_UART_ONE_STOP_BIT);
	MSS_UART_set_rx_handler( &g_mss_uart1, uart1_rx_handler, MSS_UART_FIFO_SINGLE_BYTE);
}

void uart1_rx_handler( mss_uart_instance_t * this_uart ) {
	char rx_buff[1];
	char recieved_data[50];
	int button_state=0;
	int accel=0;
	int rx_size = MSS_UART_get_rx( this_uart, (uint8_t*)rx_buff, sizeof(rx_buff) );
	if (*rx_buff == '\r' || *rx_buff == '\n') rx_size = MSS_UART_get_rx( this_uart, (uint8_t*)rx_buff, sizeof(rx_buff) );
	if (*rx_buff == '\r' || *rx_buff == '\n') rx_size = MSS_UART_get_rx( this_uart, (uint8_t*)rx_buff, sizeof(rx_buff) );

	int i = 0;
	while (*rx_buff != '\n' && *rx_buff != '\r' && i < 50) {
		if (rx_size > 0 ) recieved_data[i++] = *rx_buff;
		rx_size = MSS_UART_get_rx( this_uart, (uint8_t*)rx_buff, sizeof(rx_buff) );
	}
	recieved_data[i] = '\0';

	// Process the packet string recieved data, i is the length of the recieved data

	if(recieved_data[0]=='0'){
		//Packet structure  = source,button_state,accel_data
		button_state = int(recieved_data[2]);
		accel = ;
		if(button_state == 1){
			setLeftSideForward();
			setRightSideForward();
			setLeftSideSpeed(100 - (335-accel)/2);
			setRightSideSpeed(100 + (335-accel)/2);
		}
		else if(button_state==2){
			setLeftSideReverse();
			setRightSideReverse();
			setLeftSideSpeed(100 - (335-accel)/2);
			setRightSideSpeed(100 + (335-accel)/2);
		}
		else{
			setLeftSideForward();
			setRightSideForward();
			setLeftSideSpeed(0);
			setRightSideSpeed(0);
		}
	}

}
