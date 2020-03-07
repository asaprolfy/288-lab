/*
*
*   adc.h
*
*   Uses adc0, ss0 to sample readings from ir sensor and
*   converts them to cm distances
*
*
*
*   @author rolf
*
*/

#ifndef ADC_H_
#define ADC_H_

#include <inc/tm4c123gh6pm.h>
#include "Timer.h"
#include "lcd.h"
#include "uart.h"
#include "math.h"

// init all required registers
// uses:
//      adc0
//      ss0
//
void adc_init();

// get raw adc value
//
int adc_read_raw();

// read raw adc ir value and display
//
void basic_raw_loop();

// function for taking manual readings
// of measured distance and adc reading
// then sent to computer through uart
// formatted as .csv
//
void manual_raw();

// tester function that uses a rolling average (100 samples)
// and the binomial equation from plotting the data from the
// manual readings to determine actual distance in cm
//
void test_dist_logic();

// y = 770675x^(-1.47)
//
float calc_dist(int x);

// remove arr[0], shift all vals down
// insert newval in arr[99]
//
void shift_buff(int arr[], int newval);

#endif /* ADC_H_ */
