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
#include "lcd.h"
#include <inc/tm4c123gh6pm.h>


// Initialize the UART to communciate between CyBot and PuTTy
void uart_init(void);

// Send a byte over the UART from CyBot and PuTTy (Buad Rate 115200, No Parity, No Flow Control)
void uart_sendByte(char data);

// Cybot WAITs to recive a byte from PuTTy (Buad Rate 115200, No Parity, No Flow Control).
// In other words, this is a blocking fucntion.
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

// send raw string to uart
// no formatting
//
void uart_prints(char str[]);


// combination of display_from_uart() and
// echo_to_putty()
//
void display_and_echo();

#endif /* UART_H_ */
