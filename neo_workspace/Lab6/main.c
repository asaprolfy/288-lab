
#include <inc/tm4c123gh6pm.h>
#include "Timer.h"
#include "lcd.h"
#include "adc.h"
#include "uart.h"
#include "math.h"

int main() {
    timer_init();
    lcd_init();
    uart_init();
    adc_init();

    int adc_val;
    float dist;

    for(;;){
        adc_val = adc_read();

        dist = pow(adc_val, -1.855) * 2E+07;
        lcd_printf("%f", dist);
        timer_waitMillis(100);
    }

}
