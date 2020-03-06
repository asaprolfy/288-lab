#include <inc/tm4c123gh6pm.h>
#include "Timer.h"
#include "lcd.h"
#include "adc.h"
#include "uart.h"
#include "math.h"
#include "stdlib.h"

void basic_raw_loop();

void manual_raw();

void test_dist_logic();

float calc_dist(int x);

void shift_buff(int arr[], int newval);

int main() {
	
	// initializers
	//
    timer_init();
    lcd_init();
    uart_init();
    adc_init();

    //manual_raw();

    test_dist_logic(););

}

// takes raw adc reading and displays it
//
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

// this is the function I made to take manual readings of
// the quantized raw adc value and the actual measured distance
// 
// to use this function, fire it up and connect via uart,
// place the robot at the desired distance and input the
// distance in cm 
//
// the bot will then send back formatted csv data to putty
// you can save this output and open it in excel to better
// analyze it
//
// NOTE:	the measured distance input MUST be 3 digits long
//			i.e. for 35 cm enter "030"
//			the program will only advance when 3 digits have 
//			been entered
//
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

// y = 770675x^(-1.47)
//
//		NOTE: THIS FORMULA IS CALLIBRATED FOR BOT 06
//
// ^^ equation from excel line of best fit, exponential
// x is the quantized adc reading
//
// this function is a demonstrator for the function to determine 
// the distance from the detected object to the ~bumper~ of the cybot
//
// it uses a rolling average of the last 100 values to 
// smooth out the inherent variability in the readings
//
// this function calculates the distance (a float)
// and prints it on the lcd and sends it to uart formatted
// in csv as to permit easy data management in excel
//
void test_dist_logic() {

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
    dist = calc_dist(rolling_avg);
    lcd_printf("%d    %f", rolling_avg, dist);

    for (;; timer_waitMillis(20)) {
        tmp = adc_read_raw();
        rolling_sum = rolling_sum + tmp - rolling_buff[0];
        rolling_avg = rolling_sum / 100;
        shift_buff(rolling_buff, tmp);
        dist = calc_dist(rolling_avg);


        sprintf(str, "%d,  %f\r\n", rolling_avg, dist);
        lcd_printf(str);
        uart_prints(str);
    }
}

// remove value at pos 0, shift all other values down 1
// e.g. arr[12] = arr[13]
// insert newval in position 99 (last value)
//
void shift_buff(int arr[], int newval) {

    int i = 0;

    for(i = 0; i < 99; i++) {
        arr[i] = arr[i + 1];
    }
    arr[99] = newval;
}

// y = 770675x^(-1.47)
//
float calc_dist(int x){

    float result = 770675 * pow(x, -1.47);
    return result;
}

