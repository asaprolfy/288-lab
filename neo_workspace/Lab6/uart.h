/*
*
*   uart.h
*
*   Used to set up the RS232 connector and WIFI module
*   uses UART1 at 115200
*
*
*   @author rolf
*
*/

#ifndef UART_H_
#define UART_H_

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "lcd.h"
#include <inc/tm4c123gh6pm.h>
#include "driverlib/interrupt.h"

volatile int *rx_trigger_ptr;
volatile char *rx_char_ptr;

//
extern volatile char command_byte; // byte value for special character used as a command

//
extern volatile int command_flag; // flag to tell the main program a special command was received


// Initialize the UART to communciate between CyBot and PuTTy
//
void uart_init(void);

// initialize uart with interrupts
//
void uart_interrupt_init(volatile int *rx_trigger_ptr, volatile char *rx_char_ptr);

// interrupt handler for receive interrupts
//
void uart1_handler(void);

// Send a byte over the UART from CyBot and PuTTy (Buad Rate 115200, No Parity, No Flow Control)
//
void uart_sendByte(char data);

// Cybot WAITs to recive a byte from PuTTy (Buad Rate 115200, No Parity, No Flow Control).
// In other words, this is a blocking fucntion.
//
int uart_receiveByte(void);

// take bytes from uart and display them on screen, when return is pressed
// the entire buffer of received characters are displayed
//
void display_from_uart(void);

// receive bytes from putty through uart and send them back
//
void echo_to_putty(void);

// Clean CyBot to PuTTy UART initialization, before running your UART GPIO init code
//void cyBot_uart_init_clean(void);

// transmits unformatted string on uart
//
void uart_prints(char str[]);

// combination of display_from_uart and echo_to_putty
// clears display after 20 chars or '\r'
//
void display_and_echo();

// displays the last character that was sent to it and how much
// chars have been sent so far -- mostly for debug
//
void raw_display_rx();

// takes char arr and turns all items into ' '
//
void clear_char_array(char arr[]);

// using interrupts, display input chars from putty and echo back
//
void interrupt_display_and_echo();

#endif /* UART_H_ */
