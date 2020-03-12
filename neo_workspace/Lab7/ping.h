/*
*
*   ping.h
*
*   Uses timer3b to measure input edge-time from
*   the sonar (ping) sensor and convert that elapsed
*   time value into a distance
*
*   speed of sound = 343m/s
*
*
*
*   @author rolf
*
*/

#ifndef PING_H_
#define PING_H_

#define BIT3 0x08

#include <inc/tm4c123gh6pm.h>
#include <stdint.h>
#include <stdbool.h>
#include "Timer.h"
#include "driverlib/interrupt.h"

// current state of timer
//
volatile enum {RISE, FALL, DONE, IDLE} state;

// time at rising edge
//
volatile uint32_t rise_e;

// time at falling edge
//
volatile uint32_t fall_e;

// time between rising and falling edge
//
volatile uint32_t pulse_wid;

// initialize timer3b on pb3
//
// config t3b to be 16 bit, input capture, edge time, count up,
// both edges trigger interrupt, 8 bits prescaler, set priority for both interrupts
//
void ping_init();

// weirdly complex function to send command for sonar pulse
//
// turn afsel off, set pb3 output, set pb3 high, wait a bit, set pb3 low,
// set back to input turn afsel back on, enable t3b
//
void send_pulse();

// interrupt handler for both rise and fall interrupt
//
// uses state variable for logic structure
//
void t3b_handler();

// use send_pulse and interrupt handler to determine distance (cm)
//
// if there is an overflow it incrememnts the number of overflows
//
float pingdist();

// returns number of overflows
//
int get_of();

// return last pulsewidth
//
int get_pw();

#endif
