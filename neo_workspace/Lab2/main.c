#include "lcd.h"
#include "open_interface.h"
#include "movement.h"

/**
 * main.c
 *
 * Simple 'Hello, world program'.
 * This program prints "Hello, world" to the LCD screen
 * @author  rolf
 * @date    Jan 31, 2020
 */

void movesquare(oi_t *sensor_data){
    move_forward_cm(sensor_data, 50);
    turn_right_degrees(sensor_data, 90);
    move_forward_cm(sensor_data, 50);
    turn_right_degrees(sensor_data, 90);
    move_forward_cm(sensor_data, 50);
    turn_right_degrees(sensor_data, 90);
    move_forward_cm(sensor_data, 50);
    turn_right_degrees(sensor_data, 90);
}

int main(void)
{
    // Initialize the LCD. This also clears the screen
    lcd_init();

    lcd_printf("Initium");
	
	// Clear the LCD screen and print "Hello, world" on the LCD
    // lcd_puts("Hello, world");
    //lcd_rotatingBanner("Microcontrollers are awesome!");
	
    // initialize open interface sensors
    oi_t *sensor_data = oi_alloc();
    oi_init(sensor_data);


    //move_forward_cm(sensor_data, 100);
    //move_backward_cm(sensor_data, 100);

    //turn_left_degrees(sensor_data, 90);
    //turn_right_degrees(sensor_data, 90);

    //movesquare(sensor_data);

    move_two_meters(sensor_data);

    lcd_printf("Terminus");

    oi_free(sensor_data);
	return 0;
}
