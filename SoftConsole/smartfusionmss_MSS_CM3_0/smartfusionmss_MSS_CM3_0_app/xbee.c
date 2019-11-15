#include "xbee.h"

void initialize_controller_reciever(void) {
	MSS_UART_init(&g_mss_uart1, MSS_UART_57600_BAUD, MSS_UART_DATA_8_BITS | MSS_UART_NO_PARITY | MSS_UART_ONE_STOP_BIT);
	MSS_UART_enable_irq( &g_mss_uart1, MSS_UART_RBF_IRQ );
    MSS_UART_set_rx_handler( &g_mss_uart1, controller_data_rx_handler, MSS_UART_FIFO_SINGLE_BYTE);
}

void controller_data_rx_handler( mss_uart_instance_t * this_uart ) {
	uint8_t rx_buff[20];
	MSS_UART_get_rx( this_uart, rx_buff, sizeof(rx_buff) );

}
