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

// lab9 checkpoint 1
//
void sweep();

int main() {
	
	// initializers
	//
    timer_init();
    lcd_init();
    uart_init();
    adc_init();
    ping_init();
    servo_init();

    sweep();
}

void sweep() {

    int deg = 0;
    double ir_dist, ping_dist;

    lcd_printf("Degrees\nIR\nSonar");
    uart_prints("Degrees\tIR Distance (cm)\tSonar Distance (cm)");

    for(deg = 0; deg < 181; deg += 2) {

        servo_move(deg);
        timer_waitMillis(350);

        ir_dist = irdist();
        ping_dist = pingdist();

        lcd_printf("%d\n%f\n%f", deg, ir_dist, ping_dist);

        char str[30];
        sprintf(str, "%d\t%f\t%f", deg, ir_dist, ping_dist);
        uart_prints(str);
    }
}
