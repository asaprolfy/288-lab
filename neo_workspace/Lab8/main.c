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

// please keep any and all logic and function definitions out of this file
//
int main() {
	
	// initializers
	//
    timer_init();
    lcd_init();
    uart_init();
    adc_init();
    ping_init();

    // constantly display on lcd:
    //
    //     distance (cm)
    //     pulsewidth
    //     overflow
    //
    float dist = pingdist();
    int pw = get_pw();
    int of = get_of();

    for (;;) {
        lcd_printf("dist: %4.2f\npw: %d\nof: %d", dist, pw, of);
        timer_waitMillis(250);
        dist = pingdist();
        pw = get_pw();
        of = get_of();
    }

}
