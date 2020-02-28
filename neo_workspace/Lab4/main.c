

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
void send_results(cyBOT_Scan_t scans[]);

void main(void){
    timer_init();
    lcd_init();
    uart_init();
    cyBOT_init_Scan();

    //display_from_uart();
    //echo_to_putty();


    cyBOT_Scan_t scans[37];
    int i = 0;
    int a = 0;
    char input = 0;

    while(1) {
        input = uart_receiveByte();
        if(input == 'm'){
            while(a <= 180){
                cyBOT_Scan_t *ptr = &scans[i];
                cyBOT_Scan(a, ptr);
                a += 5;
                i++;
            }
            send_results(scans);
        }
    }

}

void send_results(cyBOT_Scan_t scans[]){
    uart_prints("Angle     PING distance     IR raw value\r\n");
    int i = 0;
    int a = 0;
    char str[40];
    for(i = 0; i < 37; i++){
        sprintf(str, "%d     %f     %d\r\n", a, scans[i].sound_dist, scans[i].IR_raw_val);
        uart_prints(str);
        a += 5;
    }
}
