/* uart.h
 * Author: Michael
 * Copyright 2013 University of Colorado at Boulder ECEE Dept
 * Provides a UART interface for the MSP430G2553.
 * 
 */

#ifndef UART_H
#define UART_H

#define UART_BAUD_9600 0
#define UART_BAUD_19200 1
#define UART_BAUD_38400 2
#define UART_BAUD_56000 3
#define UART_BAUD_115200 4

#include <msp430g2553.h>
//TODO Maybe use <msp430.h>

// init_uart: Initialize everything necessary for the UART functionality you are implementing.  Be sure not to affect pins other than the TX and RX pins (output values, directions, or select registers).  You must support a baud rate of 9600 (UART_BAUD_9600) and 115200 (UART_BAUD_115200).  The other baud rates are optional.
void init_uart(char baud){
	//TODO Adrian: I have no clue which of this stuff is right, I'm just putting potentially relevant stuff in here.
	// INIT for 9600 baud
	WDTCTL = WDTPW + WDTHOLD;                 // Stop Watchdog Timer
	P1DIR = 0xFF;                             // All P1.x outputs
	P1OUT = 0;                                // All P1.x reset
	P2DIR = 0xFF;                             // All P2.x outputs
	P2OUT = 0;                                // All P2.x reset
	P1SEL = BIT1 + BIT2 ;                     // P1.1 = RXD, P1.2=TXD
	P1SEL2= BIT1 + BIT2 ;                     // P1.1 = RXD, P1.2=TXD
	P3DIR = 0xFF;                             // All P3.x outputs
	P3OUT = 0;                                // All P3.x reset  
	UCA0CTL1 |= UCSSEL_1;                     // CLK = ACLK
	UCA0BR0 = 0x03;                           // 32kHz/9600 = 3.41
	UCA0BR1 = 0x00;                           //
	UCA0MCTL = UCBRS1 + UCBRS0;               // Modulation UCBRSx = 3
	UCA0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
	IE2 |= UCA0RXIE;                          // Enable USCI_A0 RX interrupt
	__bis_SR_register(LPM3_bits + GIE);       // Enter LPM3, interrupts enabled

	// INIT for 115200 baud
	WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
	if (CALBC1_1MHZ==0xFF)                                        // If calibration constant erased
	{                                                                                     
		while(1);                               // do not load, trap CPU!!  
	}
	DCOCTL = 0;                               // Select lowest DCOx and MODx settings
	BCSCTL1 = CALBC1_1MHZ;                    // Set DCO
	DCOCTL = CALDCO_1MHZ;
	P1SEL = BIT1 + BIT2 ;                     // P1.1 = RXD, P1.2=TXD
	P1SEL2 = BIT1 + BIT2;                      
	UCA0CTL1 |= UCSSEL_2;                     // SMCLK
	UCA0BR0 = 8;                              // 1MHz 115200
	UCA0BR1 = 0;                              // 1MHz 115200
	UCA0MCTL = UCBRS2 + UCBRS0;               // Modulation UCBRSx = 5
	UCA0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
	IE2 |= UCA0RXIE;                          // Enable USCI_A0 RX interrupt
	__bis_SR_register(LPM0_bits + GIE);       // Enter LPM0, interrupts enabled

}

// uninit_uart: Uninitialize the uart driver.
void uninit_uart(void);

// putch: Send an unsigned char via UART.  This function should transmit characters correctly regardless of how many times this function is called in rapid succession.
void putch(unsigned char c);

// put_str: Send each element of a null-terminated array of unsigned chars via UART.  Do not send the final null-termination character.
void put_str(unsigned char* c);

// uart_rx: Return the most recent character received via UART.
// The block parameter determines the behavior of uart_rx if no character has been received.  The functionality is defined as follows:
//   If a character has been received, return the most recently received character.
//   If no character has been received and block is set to zero, return -1.
//   If no character has been received and block is set to one, wait for a character to be received and then return that character.
// Thus, if the microcontroller receives 'a' one time, and this function is called twice with block = 0, the first call should return 'a' and the second should return -1.  If the microcontroller receives 'a' one time, and this function is called twice with block = 1, the first call should return 'a' and the second should wait indefinitely until a character is received, and then return that character.
int uart_rx(char block);

#endif
