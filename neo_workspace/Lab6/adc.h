/*
 * adc.h
 *
 *  Created on: Feb 28, 2020
 *      Author: alecrm4
 */


#ifndef ADC_H_
#define ADC_H_
#include <inc/tm4c123gh6pm.h>
#include "Timer.h"
#include "lcd.h"

void adc_init();

int adc_read_raw();

#endif /* ADC_H_ */
