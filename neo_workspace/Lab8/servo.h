/*
*
*   servo.h
*
*   uses timer1b and pwm output on pb5
*   to move the servo to a desired angle
*
*
*
*   @author rolf
*           alec
*           adam
*
*/

#ifndef SERVO_H_
#define SERVO_H_

#include <inc/tm4c123gh6pm.h>
#include <stdint.h>
#include <stdbool.h>
#include "Timer.h"
#include "lcd.h"

// number of clock cycles in 20ms
// this is the period of t1b
//
#define PERIOD 320000

// initialize pb5 as output, and t1b as
// pwm, countdown, set interval load and
// match registers
//
void servo_init();

// determine the correct match value necessary
// to move the servo to the provided degree value
//
// NOTE: does nothing and returns -1 if value
//       less then 0 or more than 180 is provided
//
int servo_move(float degree);

#endif /* SERVO_H_ */
