#include "ping.h"

int ovrflws;

// initialize timer3b on pb3
//
// config t3b to be 16 bit, input capture, edge time, count up,
// both edges trigger interrupt, 8 bits prescaler, set priority for both interrupts
//
void ping_init(){

    // reset number of overflows
    //
    ovrflws = 0;

    // send clock to GPIO
    //
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R1;

    // select alternate func for ping sensor
    // 0x08
    //
    GPIO_PORTB_AFSEL_R |= BIT3;

    // configure port mux control for pin 3
    // bits 12-14
    //
    GPIO_PORTB_PCTL_R |= 0x7000;

    // digital enable 0x08
    //
    GPIO_PORTB_DEN_R |= BIT3;

    // make 0x08 input
    //
    GPIO_PORTB_DIR_R &= ~BIT3;

    // send clock to TIMER3
    //
    SYSCTL_RCGCTIMER_R |= SYSCTL_RCGCTIMER_R3;

    // disable timer3b
    // bit 8
    //
    TIMER3_CTL_R &= ~BIT8;

    // set 16 bit timer config
    // bit 2
    //
    TIMER3_CFG_R |= BIT2;

    // input capture mode, edge time count up (0b0111)
    // bits 0-2, 4
    //
    TIMER3_TBMR_R |= 0x17;

    // both edges trigger interrupt
    //
    //
    TIMER3_CTL_R |= 0x0c00;

    // set t3b prescale to 8 bits (24 bit timer now)
    // this basically extends the range of the timer
    // (this value from lab7 manual)
    //
    TIMER3_TBPR_R = 0xff;

    // more setting prescaler (from lab7 manual)
    //
    TIMER3_TBILR_R = 0xffff;

    // clear timer3b interrupt capture
    //
    TIMER3_ICR_R |= 0x400;

    // enable timer3 interrupt
    //
    TIMER3_IMR_R |= 0x400;

    // re-enable timer3b
    //
    TIMER3_CTL_R |= 0x100;

    // set interrupt priority and enable interrupt for timer3b
    //
    NVIC_PRI9_R |= 0x20;
    NVIC_EN1_R |= 0x10;

    // enable interrupt register
    //
    IntRegister(INT_TIMER3B, t3b_handler);
    IntMasterEnable();
}

// weirdly complex function to send command for sonar pulse
//
// turn afsel off, set pb3 output, set pb3 high, wait a bit, set pb3 low,
// set back to input turn afsel back on, enable t3b
//
void send_pulse(){

    // disable timer b
    //
    TIMER3_CTL_R &= ~0x300;

    // turn alternate function off
    //
    GPIO_PORTB_AFSEL_R &= ~BIT3;

    // set PB3 as output
    //
    GPIO_PORTB_DIR_R |= BIT3;

    // set PB3 to high
    //
    GPIO_PORTB_DATA_R |= BIT3;

    // wait 10 microseconds for output to be read
    //
    timer_waitMicros(10);

    // set PB3 to low
    //
    GPIO_PORTB_DATA_R &= ~BIT3;

    // set PB3 as input
    //
    GPIO_PORTB_DIR_R &= ~BIT3;

    // turn alternate function back on
    //
    GPIO_PORTB_AFSEL_R |= BIT3;

    // enable timer b
    //
    TIMER3_CTL_R |= 0x100;
}

// interrupt handler for both rise and fall interrupt
//
// uses state variable for logic structure
//
void t3b_handler(){

    // check if unmasked capture b event interrupt has occured
    // 0x400
    // bit10
    //
    if (TIMER3_MIS_R & TIMER_MIS_CBEMIS){

        if (state == RISE){

            //clear capture interrupt flag
            // 0x400
            // bit10
            //
            TIMER3_ICR_R |= TIMER_ICR_CBECINT;
            // captures time of rising edge event
            //
            // TBPS_R is the prescaler (extra 8 bits)
            // TBR_R is the timer reading
            //
            rise_e = ((int) TIMER3_TBPS_R << 16) | TIMER3_TBR_R;
            // now capturing falling edge
            //
            state = FALL;
        }
        else if (state == FALL){

            // clear capture interrupt flag
            //
            TIMER3_ICR_R |= TIMER_ICR_CBECINT;
            // capture time of falling edge
            //
            fall_e = ((int) TIMER3_TBPS_R << 16) | TIMER3_TBR_R;
            // change state
            //
            state = DONE;
            // disable t3b
            //
            TIMER3_CTL_R &= ~TIMER_CTL_TBEN;

        }
    }
}

// use send_pulse and interrupt handler to determine distance (cm)
//
// if there is an overflow it incrememnts the number of overflows
//
float pingdist(){

    state = RISE;
    send_pulse();

    while(state != DONE) {}

    state = IDLE;

    int overflow = (fall_e < rise_e);

    if(overflow == 1) {
        ovrflws++;
    }

    pulse_wid = fall_e + (overflow << 24) - rise_e;


    // NOTE: the sonar system measures from the sensor to the object, this
    //       is not terribly useful because the bumper sticks out 4cm in front
    //       of the sensor; that's why I made this function subtract 4 from the quotient
    //
    //
    //
    return pulse_wid / 903.2 - 4;
}

// returns number of overflows
//
int get_of(){
    return ovrflws;
}

int get_pw(){
    return pulse_wid;
}
