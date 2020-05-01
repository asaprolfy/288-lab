/*
*
*   servo.c
*
*   uses timer1b and pwm output on pb5
*   to move the servo to a desired angle
*
*
*
*   @author rolf
*           alec
*           adam
*
*/
#include "servo.h"

volatile signed pulsewidth;

void servo_init(){

    // enable clock to pb5
    // bit 1
    //
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R1;

    // turn on alternate function for pb5
    //
    GPIO_PORTB_AFSEL_R |= 0x20;

    // mux t1ccp1 to pb5
    // see table 23-5 in datasheet
    //
    GPIO_PORTB_PCTL_R |= 0x700000;

    // set pb5 output
    //
    GPIO_PORTB_DIR_R |= 0x20;

    // enable pb5 to be digital
    //
    GPIO_PORTB_DEN_R |= 0x20;

    // enable clock to timer1
    //
    SYSCTL_RCGCTIMER_R |= SYSCTL_RCGCTIMER_R1;

    // disable timer 1b to configure
    // bit8
    //
    TIMER1_CTL_R &= ~(0x100);

    // set t1 to 16 bit mode
    // although it's really 24 bit
    //
    TIMER1_CFG_R |= 0x04;

    // periodic and PWM enable
    // bit1, 3
    //
    TIMER1_TBMR_R |= 0x0A;

    // clear tbcm bit (required for pwm mode)
    // set count direction to down
    TIMER1_TBMR_R &= ~(0x14);


    // set to non-inverted PWM mode
    // bit14
    //
    TIMER1_CTL_R &= ~(0x4000);

    // set lower 16 bits of interval load register
    // period is 320000 (base 10)
    // AND-ing the period and 0xFFFF preserves the lower 16 bits
    //
    TIMER1_TBILR_R |= (PERIOD & 0xFFFF);

    // set upper 8 bits of interval load register
    // bitshifting allows us to remove the lower 16 bits
    //
    TIMER1_TBPR_R |= (PERIOD >> 16);

    // we initialize the match value to be 304000
    // this will cause the servo to move to 0 degrees
    // before anything else
    //

    pulsewidth = 16000;

    // set lower 16 bits of match value
    //
    TIMER1_TBMATCHR_R = ((PERIOD - pulsewidth) & 0xFFFF);

    // set upper 8 bits of match value
    //
    TIMER1_TBPMR_R = ((PERIOD - pulsewidth) >> 16);

    // enable timer 1b
    //
    TIMER1_CTL_R |= 0x0100;

    // wait to allow servo to move to 0 degrees
    //
    timer_waitMillis(1000);
}

int servo_move(float degree) {
    if (degree < 0) {
        return -1;
    }
    if (degree > 180) {
        return -2;
    }

    pulsewidth = 16000 + 88.88 * degree;

    // lower 16 bits of match
    //
    TIMER1_TBMATCHR_R = (PERIOD - pulsewidth) & 0xFFFF;

    // upper 8 bits of match
    //
    TIMER1_TBPMR_R = (PERIOD - pulsewidth) >> 16;

    // wait to allow servo to move
    //
    timer_waitMillis(500);

    return pulsewidth;
}
