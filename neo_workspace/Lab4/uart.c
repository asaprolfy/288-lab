#include "uart.h"

void uart_init(){

    // enable clock to gpio port b and uart1
    //
    SYSCTL_RCGCUART_R |= SYSCTL_RCGCUART_R1;
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R1;


    // enable alternate function on pb0 and pb1
    //
    GPIO_PORTB_AFSEL_R |= 0x03;

    // enable pctl for pb0 and pb1
    // NOTE: the pin selects in this register
    //       are 4 bits wide, it is not a
    //       mistake that we are initalizing
    //       with a hex
    //
    GPIO_PORTB_PCTL_R |= 0x11;

    // digital enable pb0, pb1
    //
    GPIO_PORTB_DEN_R |= 0x03;

    // set pb0 to 0 as input
    // set pb1 to 1 as output
    //
    GPIO_PORTB_DIR_R &= 0xFFFFFFFE;
    GPIO_PORTB_DIR_R |= 0x02;

    // baudrate divisors
    //      baud:    9600        115200
    //      iBRD:    104         8
    //      fBRD:    11          44
    //

    UART1_CTL_R &= 0xFFFFFFFE;

    uint16_t ibrd = 8;
    uint16_t fbrd = 44;

    // set baud
    //
    UART1_IBRD_R = ibrd;
    UART1_FBRD_R = fbrd;


    // set frame:
    // 8 data bits
    // 1 stop bit
    // no parity
    // no fifo
    //
    UART1_LCRH_R = UART_LCRH_WLEN_8;

    // set clock
    //
    UART1_CC_R = UART_CC_CS_SYSCLK;

    //UART1_CTL_R = 0x03;
    UART1_CTL_R = (UART_CTL_RXE | UART_CTL_TXE | UART_CTL_UARTEN);
}

void uart_sendByte(char data) {
    // wait for room in frame
    //
    while(UART1_FR_R & 0x20) {}
    // send data
    //
    UART1_DR_R = data;
}

int uart_receiveByte(){

    // return byte
    //
    char data = 0;

    // wait for data
    //
    while(UART1_FR_R & UART_FR_RXFE) {}

    // mask error bits and grab only 8 data bits
    //
    data = (char)(UART1_DR_R & 0xFF);

    return data;
}

void display_from_uart(){
    char data[20];
    char input = 0;
    int i = 0;

    lcd_printf("waiting");

    while(1){
        input = uart_receiveByte();
        if(input != '\r'){
            data[i] = input;
            i++;
            lcd_printf("%c %d", input, i);
        }
        if(input == '\r' || i > 19) {
            lcd_printf("%s", data);
        }
    }
}

void echo_to_putty(void){
    char input = 0;
    int i = 0;
    lcd_printf("waiting");

    while(1){
        input = uart_receiveByte();
        if(input != '\r'){
            i++;
            lcd_printf("%c  %d", input, i);
            uart_sendByte(input);
        }
        else {
            uart_sendByte('\r');
            uart_sendByte('\n');
        }
    }
}


void uart_prints(char str[]){
    int len = strlen(str);
    int i = 0;
    for(i = 0; i < len; i++){
        uart_sendByte(str[i]);
    }
}

void display_and_echo(){
    int lcd_i = 0;
    char input = 0;
    char lcd[20];

    while(1) {
        input = uart_receiveByte();
        if(input != '\r' && lcd_i < 20){
            lcd_printf("%c   %d", input, lcd_i);
            lcd[lcd_i++] = input;
            uart_sendByte(input);
        }
        else if(input == '\r' | lcd_i == 20){
            lcd_i = 21;
            lcd_printf("%s", lcd);
            uart_sendByte('\r');
        }
        else {
            lcd_i = 0;
            lcd = {};
            lcd_printf("");
            uart_sendByte('\r');
        }
    }
}
