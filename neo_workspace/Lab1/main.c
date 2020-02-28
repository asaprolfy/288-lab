#include "lcd.h"

/**
 * main.c
 *
 * Simple 'Hello, world program'.
 * This program prints "Hello, world" to the LCD screen
 * @author 
 * @date 
 */
int main(void)
{
    //Initialize the LCD. This also clears the screen
    lcd_init();
	
	// Clear the LCD screen and print "Hello, world" on the LCD
//  lcd_puts("Hello, world");
    lcd_rotatingBanner("Microcontrollers gfjkdhgjkdfgdfg!");
	
	return 0;
}
