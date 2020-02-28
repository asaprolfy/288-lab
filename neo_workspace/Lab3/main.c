

/**
 * main.c
 * author: rolf
 */
#include "button.h"
#include "Timer.h"
#include "lcd.h"
#include "cyBot_uart.h"  // Functions for communicating between CyBot and Putty (via UART)
                         // PuTTy: Baud=115200, 8 data bits, No Flow Control, No Parity,  COM1


int main(void) {
    button_init();
    timer_init();
    lcd_init();

    int button_num = 0;

    while(1) {
        button_num = button_getButton();
        //lcd_printf("%d", button_num);
        if(button_num == 3){
            GPIO_PORTE_DATA_R = (GPIO_PORTE_DATA_R & 0b11111110);
        }
        else {
            GPIO_PORTE_DATA_R |= 0b00000001;
        }
    }
}
