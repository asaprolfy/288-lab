/*
*
*   calibrate.h
*
*   Uses sonar values to calibrate IR
*
*   Uses Gauss-Elimination to determine the
*   quadratic regression from a set of sonar
*   readings taken at specific positions using
*   the open_interface movement to move specific
*   distances
*
*
*
*   @author rolf
*
*/

#ifndef PING_H_
#define PING_H_

#include <inc/tm4c123gh6pm.h>
#include <stdio.h>
#include <math.h>
#include "open_interface.h"
#include "ping.h"
#include "adc.h"

typedef struct Power_results {
    int coef;
    float powr;
} Power_result;

typedef struct Linear_results {
    int a;
    int b;
} Linear_result;

Power_result power_regression();

Linear_result linear_regression();

void calibrate_ir_only_openint();

#endif
