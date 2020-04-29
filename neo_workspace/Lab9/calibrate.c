#include "calibrate.h"
#define DATAPTS 10
#define POLYNOMDEGREE 2

// https://math.stackexchange.com/questions/1430852/calculate-power-regression
//
Power_result power_regression(int dst[20], float msr[20]) {
    int i = 0;

    float ln_dist[20];
    float ln_msr[20];

    for(i = 0; i < 20; i++){
        ln_dist[i] = log(dst[i]);
        ln_msr[i] = log(msr[i]);
    }


}

// NOTE: bot must begin touching the wall (for now)
//
void calibrate_ir_only_openint(){

}

// https://studentboxoffice.in/jntuh/notes/computer-programming-lab/write-a-c-program-to-implement-the-linear-regression-algorithm/123
//
Linear_result linear_regression(float x[20], float y[20]) {

}
