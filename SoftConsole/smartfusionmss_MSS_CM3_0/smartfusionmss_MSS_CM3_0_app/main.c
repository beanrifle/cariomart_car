#include <stdio.h>
#include <inttypes.h>
#include <string.h>

#include "drivers/mss_uart/mss_uart.h"
#include "drivers/mss_gpio/mss_gpio.h"

#include "CarControl.h"
#include "xbee.h"

int main() {
	XBEE_init();
	CarControl_init();

	while(1);
}
