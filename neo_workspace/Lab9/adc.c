#include "adc.h"

// init adc0 and ss0 for IR sensor
//
void adc_init() {

    // enable adc clock
    //
    SYSCTL_RCGCADC_R |= 0x01;

    // enable clock to gpio portb
    //
    SYSCTL_RCGCGPIO_R |= 0x02;

    // enable afsel on PB4 (ain0)
    //
    GPIO_PORTB_AFSEL_R |= 0x10;

    // set pb6 to an input
    //
    GPIO_PORTB_DIR_R &= ~0x40;

    // enable pb4
    //
    GPIO_PORTB_DEN_R &= ~0x10;

    // enable analog function of pb4
    //
    GPIO_PORTB_AMSEL_R |= 0x10;

    // setting this to 0 still allows pb4 to be used
    //
    GPIO_PORTB_ADCCTL_R &= 0xF;

    //
    //
    timer_waitMillis(300);

    // disable ss1
    //
    ADC0_ACTSS_R &= ~0x01;

    // set ss0 to use the processor as the trigger
    // ss0 is then triggered with the ADCPSSI register
    //
    ADC0_EMUX_R &= ~0xF;

    // select ain10 (pb4) to be sampled
    //
    ADC0_SSMUX0_R |= 0xA;

    // set 1st sample to the only sample and cause interrupt
    //
    ADC0_SSCTL0_R = 0x6;

    // enable ss0
    //
    ADC0_ACTSS_R |= 0x1;

    // set adc sample averaging control to use 32x
    // hardware oversampling
    //
    ADC0_SAC_R = ADC_SAC_AVG_32X;
}

// get raw adc value from adc0 ss0 fifo
//
int adc_read_raw(){
    // start sampling
    //
    ADC0_PSSI_R = 0x1;

    // wait for adc0 interrupt to trigger
    // (ss0 has finished sampling)
    //
    while ((ADC0_RIS_R & 0x1) == 0){}

    // clear adc0 interrupt
    //
    ADC0_ISC_R = 0x01;

    // return data in adc0 ss0 fifo
    //
    return ADC0_SSFIFO0_R;
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
// NOTE:    the measured distance input MUST be 3 digits long
//          i.e. for 35 cm enter "030"
//          the program will only advance when 3 digits have
//          been entered
//
void manual_raw() {

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
    }
}

// y = 770675x^(-1.47)
//
//      NOTE: THIS FORMULA IS CALLIBRATED FOR BOT 06
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

// same as above but returns value instead of displaying it, doesn't loop
//
float irdist() {

    int rolling_avg = 0;
    int rolling_sum = 0;
    int tmp = 0;
    int i = 0;
    float dist = 0;

    for(i = 0; i < 100; i++) {
        tmp = adc_read_raw();
        rolling_sum += tmp;
        timer_waitMillis(20);
    }
    rolling_avg = rolling_sum / 100;
    dist = calc_dist(rolling_avg);
    return dist;
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

void set_coefficient(uint32_t c) {
    coefficient = c;
}
void set_power(float p) {
    power = p;
}
