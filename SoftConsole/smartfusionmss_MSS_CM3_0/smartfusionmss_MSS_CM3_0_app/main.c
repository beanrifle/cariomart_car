#include <stdio.h>
#include <inttypes.h>

#include "CarControl.h"

int main() {
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

	while(1) {

	}
}
