
#include <inc/tm4c123gh6pm.h>
#include "Timer.h"
#include "lcd.h"
#include "adc.h"

int main()
{

    timer_init();
    lcd_init();
    adc_init();



    return 0;

}
