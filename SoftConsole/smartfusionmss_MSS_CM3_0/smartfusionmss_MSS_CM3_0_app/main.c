#include <stdio.h>
#include <inttypes.h>
#include <string.h>

#include "drivers/mss_uart/mss_uart.h"
#include "drivers/mss_gpio/mss_gpio.h"

#include "CarControl.h"
#include "xbee.h"



int main() {
	; // state of forward/reverse buttons
	int race=0;
	initialize_controller_reciever();

	setLeftSideForward();
	setRightSideForward();

	setLeftSideSpeed(0);
	setRightSideSpeed(0);

	/*setLeftSideSpeed(64);
	setRightSideSpeed(64);

	setLeftSideReverse();
	setRightSideReverse();

	setLeftSideSpeed(128);
	setRightSideSpeed(128);

	setLeftSideSpeed(0);
	setRightSideSpeed(0);
	*/

	while(1){

	}

}
