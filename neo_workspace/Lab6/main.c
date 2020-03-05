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

    char str[20];
    char input = 0;
    int i = 0;
    int specific_dist;
    int readings_sum = 0;
    int final_avg;

    for (; input != 'q';) {
        lcd_printf("input measured distance in cm");
        char input_dist[4];
        for (i = 0; input != '\r' && i < 4; i++) {
            input = uart_receiveByte();
            input_dist[i] = input;
        }
        specific_dist = atoi(input_dist);
        lcd_printf("Input dist:  %d", specific_dist);

        for(i = 0; i < 100; i++) {
            readings_sum += adc_read_raw();
        }
        final_avg = readings_sum / 100;

        sprintf(str, "%d, %d\r\n", specific_dist, final_avg);
        uart_prints(str);

        lcd_printf("press return to continue\nor q to quit");
        while(uart_receiveByte() != '\r');
    }
}
