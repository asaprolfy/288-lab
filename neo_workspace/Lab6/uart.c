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

void uart_interrupt_init(volatile int *rx_trigger_ptrr, volatile char *rx_char_ptrr) {

        // global variables used for interrupts
        //
        rx_trigger_ptr = rx_trigger_ptrr;
        rx_char_ptr = rx_char_ptrr;

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

        //
        //------------------------- I n t e r r u p t s ---------------------------
        //

        // clear RX interrupt flag
        // (cleared with 1)
        //
        UART1_ICR_R |= UART_ICR_RXIC;

        // enable RX raw interrupts in interrupts
        // mask register
        //
        UART1_IM_R |= UART_IM_RXIM;

        // NVIC setup: set priority of UART1 interrupt
        // (1 in bits 21-23
        //
        NVIC_PRI1_R = (NVIC_PRI1_R & 0xFF0FFFFF) | 0x00200000;

        // NVIC setup: enable interrupt for UART1
        // (IRQ #6, set bit 6)
        //
        NVIC_EN0_R |= 0x40;

        // tell CPU to use ISR handler for UART1 (see interrupt.h file)
        // from system header file: #define INT_UART1 22
        //
        IntRegister(INT_UART1, uart1_handler);

        // globally allow CPU to service interrupts (see interrupt.h file)
        //
        IntMasterEnable();

        // re-enable uart: tx, rx
        //
        UART1_CTL_R = (UART_CTL_RXE | UART_CTL_TXE | UART_CTL_UARTEN);
}

void uart1_handler(){

    char byte_received;

    // check if handler called due to RX event
    //
    if (UART1_MIS_R & UART_MIS_RXMIS) {

        // byte was received in the UART data register
        // clear the RX trigger flag (clear by writing 1 to ICR)
        //
        UART1_ICR_R |= 0x10;

        // read the byte received from UART1_DR_R and echo it back to PuTTY
        // ignore the error bits in UART1_DR_R
        //
        byte_received = UART1_DR_R & UART_DR_DATA_M;

        //uart_sendByte(byte_received); // try to keep this logic outside the interrupt

        // set rx trigger to 1
        // set global rx char to the byte received
        //
        *rx_trigger_ptr = 1;
        *rx_char_ptr = byte_received;

    }
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
    char data[20];
    char input = 0;
    int i = 0;

    lcd_printf("waiting");

    while (1){
        input = uart_receiveByte();
        if (input != '\r' && i < 19){
            data[i] = input;
            lcd_printf("%c %d", input, i);
            i++;
            uart_sendByte(input);
        }
        else if (i == 20) {
            clear_char_array(data);
            lcd_printf(data);
            i = 0;
        }
        else if (input == '\r' || i == 19){
            lcd_printf("%s", data);
            uart_sendByte('\r');
            uart_sendByte('\n');
            i = 20;
        }

    }
}

void clear_char_array(char arr[]){
    int len = strlen(arr);
    int i = 0;
    for(i = 0; i < len; i++){
        arr[i] = ' ';
    }
}

void raw_display_rx(){
    int i = 0;
    char inp = 0;
    while(1){
        inp = uart_receiveByte();
        lcd_printf("%c    %d", inp, i);
        i++;
    }
}

void interrupt_display_and_echo() {

    int i = 0;
    char inp = 0;
    char data[20];

    while(1) {
        if (*rx_trigger_ptr != 0){
            inp = *rx_char_ptr;
            if (inp != '\r' && i < 20){
                uart_sendByte(inp);
                data[i] = inp;
                lcd_printf("%c   %d", inp, i);
                i++;
            }
            else if (i > 20){
                uart_sendByte('\r');
                uart_sendByte('\n');
                clear_char_array(data);
                i = 0;
                lcd_printf(data);
            }
            else if (inp == '\r' || i == 20) {
                uart_sendByte('\r');
                uart_sendByte('\n');
                lcd_printf(data);
                i = 21;
            }
            inp = 0;
            *rx_trigger_ptr = 0;
            *rx_char_ptr = 0;
        }
    }
}

