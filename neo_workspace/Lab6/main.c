#include <inc/tm4c123gh6pm.h>
#include "Timer.h"
#include "lcd.h"
#include "adc.h"
#include "uart.h"
#include "math.h"
#include "stdlib.h"

void basic_raw_loop();

void manual_raw();

  int main() {
    timer_init();
    lcd_init();
    uart_init();
    adc_init();



    lcd_printf("initium\npress return to continue");
    while(uart_receiveByte() != '\r') {}

    manual_raw();

    lcd_printf("termino");

}

void basic_raw_loop() {

    int adc_val;
    char snd[20];
    int rep = 0;

    for (;;) {
        adc_val = adc_read_raw();
        sprintf(snd, "%d     %d\r\n", rep++, adc_val);
        lcd_printf(snd);
        uart_prints(snd);

        timer_waitMillis(100);
    }
}

void manual_raw() {

    //char input = 0;
    for (;;) {

        char str[20];
        int i = 0;
        int readings_sum = 0;
        int final_avg = 0;
        char input_dist[3];

        lcd_printf("input the measured distance in cm");
        for(i = 0; i < 3; i++) {
            input_dist[i] = uart_receiveByte();
        }

        for(i = 0; i < 100; i++) {
            readings_sum += adc_read_raw();
        }
        final_avg = readings_sum / 100;

        sprintf(str, "%s, %d\r\n", input_dist, final_avg);
        uart_prints(str);

        //lcd_printf("press return to continue\nor q to quit");
        //while(uart_receiveByte() != '\r');
    }
}
