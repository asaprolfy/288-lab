/*
*
*   288 lab, spring 2020
*
*   main.c
*
*
*
*
*
*   @author rolf
*
*/

#include <inc/tm4c123gh6pm.h>
#include "Timer.h"
#include "lcd.h"
#include "adc.h"
#include "uart.h"
#include "stdlib.h"
#include "ping.h"
#include "servo.h"

// please keep any and all logic and function definitions out of this file
//
int main() {
	
	// initializers
	//
    timer_init();
    lcd_init();
    //uart_init();
    //adc_init();
    //ping_init();
    servo_init();

    servo_move(30);
    timer_waitMillis(2000);
    servo_move(60);
    timer_waitMillis(2000);
    servo_move(90);
    timer_waitMillis(2000);
    servo_move(120);
    timer_waitMillis(2000);
    servo_move(150);
    timer_waitMillis(2000);
    servo_move(180);
    timer_waitMillis(2000);
}
