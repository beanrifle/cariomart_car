#include <stdlib.h>
#include "CarControl.h"
#include "CarController_hw_platform.h"
#include "drivers/mss_timer/mss_timer.h"
#include "hal/hal.h"
#include "utility.h"

void CarControl_init(void) {
	setLeftSideForward();
	setRightSideForward();
	max_speed = 150;
	srand(3654);
}

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

void CarControl_boost(void) {
	max_speed = 200;
	MSS_TIM1_init(MSS_TIMER_ONE_SHOT_MODE);
	MSS_TIM1_load_immediate(200000000);
	MSS_TIM1_enable_irq();
	MSS_TIM1_start();
}

void CarControl_spinout(void) {
	psr_t interrupt_status = HAL_disable_interrupts();

	int i = 0;
	while (i < 4) {
		if (rand()%2 == 0) setLeftSideForward();
		else setLeftSideReverse();
		setLeftSideSpeed(rand()%256);

		if (rand()%2 == 0) setRightSideForward();
		else setRightSideReverse();
		setRightSideSpeed(rand()%256);

		delay(250);
		++i;
	}

	setLeftSideForward();
	setRightSideForward();
	setLeftSideSpeed(0);
	setRightSideSpeed(0);

	delay(1000);

	HAL_restore_interrupts(interrupt_status);
}

void Timer1_IRQHandler() {
	max_speed = 150;
	MSS_TIM1_clear_irq();
}
