#include <stdio.h>
#include <inttypes.h>
#include <string.h>

#include "CarControl.h"
#include "drivers/mss_uart/mss_uart.h"
#include "xbee.h"



int main() {
	initialize_controller_reciever();

	setLeftSideForward();
	setRightSideForward();

	setLeftSideSpeed(64);
	setRightSideSpeed(64);

	setLeftSideReverse();
	setRightSideReverse();

	setLeftSideSpeed(128);
	setRightSideSpeed(128);

	setLeftSideSpeed(0);
	setRightSideSpeed(0);

	while(1){

	}

}
