#include <inc/tm4c123gh6pm.h>
#include "Timer.h"
#include "lcd.h"
#include "adc.h"
#include "uart.h"
#include "math.h"
#include "stdlib.h"

void basic_raw_loop();

void manual_raw();

void test_polynomial_logic();

float calc_dist_polynom(int x);

void shift_buff(int arr[], int newval);

int main() {
    timer_init();
    lcd_init();
    uart_init();
    adc_init();



    //lcd_printf("initium\npress return to continue");
    //while(uart_receiveByte() != '\r') {}

    //manual_raw();

    test_polynomial_logic();

    //lcd_printf("termino");

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

// y = -9E-06x5 + 0.0024x4 - 0.2432x3 + 12.399x2 - 328.82x + 4571.2
// ^^ equation from excel line of best fit, 5th order polynomial
// x is the quantized adc reading
//
void test_polynomial_logic() {

    int rolling_buff[100];
    int rolling_avg = 0;
    int rolling_sum = 0;
    int tmp = 0;
    int i = 0;
    float dist = 0;
    char str[20];

    for(i = 0; i < 100; i++) {
        tmp = adc_read_raw();
        rolling_buff[i] = tmp;
        rolling_sum += tmp;
        timer_waitMillis(20);
    }
    rolling_avg = rolling_sum / 100;
    dist = calc_dist_polynom(rolling_avg);
    lcd_printf("%d    %f", rolling_avg, dist);

    for (;; timer_waitMillis(20)) {
        tmp = adc_read_raw();
        rolling_sum = rolling_sum + tmp - rolling_buff[0];
        rolling_avg = rolling_sum / 100;
        shift_buff(rolling_buff, tmp);
        dist = calc_dist_polynom(rolling_avg);

        lcd_printf("%d    %f", rolling_avg, dist);
        sprintf(str, "%d    %f", rolling_avg, dist);
    }
}

void shift_buff(int arr[], int newval) {

    int i = 0;

    for(i = 0; i < 99; i++) {
        arr[i] = arr[i + 1];
    }
    arr[99] = newval;
}

float calc_dist_polynom(int x){

    float result = 770675 * pow(x, -1.47);
    return result;
}

