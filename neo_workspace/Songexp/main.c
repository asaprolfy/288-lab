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
#include "stdlib.h"
#include "song.h"
#include "open_interface.h"

// please keep any and all logic and function definitions out of this file
//
int main() {
	

    //timer_init();
    //lcd_init();

    // initialize open interface sensors
    oi_t *sensor_data = oi_alloc();
    oi_init(sensor_data);

    load_the_wake();

    while(1) {
        play_the_wake();
    }

    oi_free(sensor_data);

}
