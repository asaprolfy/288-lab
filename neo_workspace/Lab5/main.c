

/**
 * main.c
 * author: rolf
 */
#include <uart.h>  // Functions for communicating between CyBot and Putty (via UART)
//#include "button.h"
#include "Timer.h"
#include "lcd.h"
#include "uart.h"
#include "scan.h"
                         // PuTTy: Baud=115200, 8 data bits, No Flow Control, No Parity,  COM1

volatile int rx_trigger = 0;
volatile char rx_char = 0;

void main(void){
    timer_init();
    lcd_init();
    cyBOT_init_Scan();
    uart_interrupt_init(&rx_trigger, &rx_char);
    //uart_init();


    //scan_labfour();

    //display_from_uart();
    //echo_to_putty();
    //display_and_echo();
    //raw_display_rx();

    interrupt_display_and_echo();

}


