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

// please keep any and all logic and function definitions out of this file
//
int main() {
	
	// initializers
	//
    timer_init();
    lcd_init();
    uart_init();
    adc_init();

    // constantly display and send through uart:
    //
    //     adc raw value
    //     refined dist value
    //
    test_dist_logic();

}
