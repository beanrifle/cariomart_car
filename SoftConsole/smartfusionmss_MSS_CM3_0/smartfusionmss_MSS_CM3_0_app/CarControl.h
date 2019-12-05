#ifndef CARCONTROL_H
#define CARCONTROL_H

#include <inttypes.h>

int max_speed;

void CarControl_init(void);

// Set the left side of the vehicle to turn forward
void setLeftSideForward(void);

// Set the left side of the vehicle to turn reverse
void setLeftSideReverse(void);

// Set the right side of the vehicle to turn forward
void setRightSideForward(void);

// Set the right side of the vehicle to turn reverse
void setRightSideReverse(void);

// Set the speed of the left side of the vehicle. Speed should
// be a uint32 between 0 and 255 (inclusive);
void setLeftSideSpeed(uint32_t speed);

// Set the speed of the right side of the vehicle. Speed should
// be a uint32 between 0 and 255 (inclusive);
void setRightSideSpeed(uint32_t speed);

// Boost the speed of the car
void CarControl_boost(void);

// Cause the car to spinout
void CarControl_spinout(void);

void Timer1_IRQHandler();

#endif
