#include "CarControl.h"
#include "CarController_hw_platform.h"

void setLeftSideForward(void) {
	*((uint32_t*)HBRIDGECONTROLLER_0) = 0x0;
}

void setLeftSideReverse(void) {
	*((uint32_t*)HBRIDGECONTROLLER_0) = 0x1;
}

void setRightSideForward(void) {
	*((uint32_t*)(HBRIDGECONTROLLER_0 + 4)) = 0x0;
}

void setRightSideReverse(void) {
	*((uint32_t*)(HBRIDGECONTROLLER_0 + 4)) = 0x1;
}

void setLeftSideSpeed(uint32_t speed) {
	*((uint32_t*)(HBRIDGECONTROLLER_0 + 8)) = speed;
}

void setRightSideSpeed(uint32_t speed) {
	*((uint32_t*)(HBRIDGECONTROLLER_0 + 12)) = speed;
}
