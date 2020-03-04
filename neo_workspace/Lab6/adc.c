#include "adc.h"
//ADC
void adc_init()
{
    //GPIO
    SYSCTL_RCGC2_R |= 0x00000010;// 1) activate clock for Port B
    GPIO_PORTB_DIR_R &= ~0x04; // 2) make PB4 input
    GPIO_PORTB_AFSEL_R |= 0x04; // 3) enable alternate function on PB2
    GPIO_PORTB_DEN_R &= ~0x04; // 4) disable digital I/O on PB2
    GPIO_PORTB_AMSEL_R |= 0x04;

    ADC0_ACTSS_R &= ~0x0008; // 9) disable sample sequencer 3
    ADC0_EMUX_R &= ~0xF000; // 10) seq3 is software trigger
    ADC0_SSMUX3_R &= ~0x000F; // 11) clear SS3 field
    ADC0_SSMUX3_R += 0; // set channel Ain10 (PB4)
    ADC0_SSCTL3_R = 0x0006; // 12) no TS0 D0, yes IE0 END0
    ADC0_ACTSS_R |= 0x0008; // 13) enable sample sequencer 3
}

int adc_read(){
    ADC0_PSSI_R = 0x1;

    while ((ADC0_RIS_R & 0x1) == 0){}

    ADC0_ISC_R = 0x01;

    return ADC0_SSFIFO0_R;
}
