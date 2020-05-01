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

// lab9 checkpoint 1
//
void sweep1();


int main() {
	
	// initializers
	//
    timer_init();
    lcd_init();
    uart_init();
    adc_init();
    ping_init();
    servo_init();

}

void sweep1() {

    int deg = 0;
    double ir_dist, ping_dist;

    lcd_printf("Degrees\nIR Distance (cm)\nPing Distance (cm)");
}
