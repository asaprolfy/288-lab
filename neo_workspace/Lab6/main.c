
#include <inc/tm4c123gh6pm.h>
#include "Timer.h"
#include "lcd.h"
#include "adc.h"
#include "uart.h"
#include "math.h"

void basic_raw_loop();

void take_manual_raw();

int main() {
    timer_init();
    lcd_init();
    uart_init();
    adc_init();

    int adc_val;
    char snd[20];
    int rep = 0;

    lcd_printf("initium");




}

void basic_raw_loop() {
    for (;;) {
        adc_val = adc_read_raw();
        sprintf(snd, "%d     %d\r\n", rep++, adc_val);
        lcd_printf(snd);
        uart_prints(snd);

        timer_waitMillis(100);
    }
}

void take_manual_raw() {

}
