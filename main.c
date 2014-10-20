/* uart.h
 * Author: Michael
 * Copyright 2013 University of Colorado at Boulder ECEE Dept
 * Provides a UART interface for the MSP430G2553.
 *
*/

#include <msp430g2553.h>
#include "uart.h"

int main(void)
{
    char flag = 0;
  WDTCTL = WDTPW + WDTHOLD; // Stop watchdog timer
  unsigned char c = 'a';
  unsigned char arr[128] = "\nIn the year of '39, assembled here the volunteers, in the days when the lands were few.  Here the ship sailed out into the ...";

  P2DIR = BIT1;
  P2OUT = 0x00;
  P1SEL = 0xA8;
  P1SEL2 = 0xA8;

  init_uart(UART_BAUD_9600);

  putch(c);
  __delay_cycles(20000);

  put_str(arr);

  while(c != 'q')
  {
    c = uart_rx(1);
    putch(c);
  }

  uninit_uart();

  flag = 1;
  if (P1SEL != 0xA8) flag = 0;
  if (P1SEL2 != 0xA8) flag = 0;
  if (IE2 & UCA0TXIE) flag = 0;
  if (IE2 & UCA0RXIE) flag = 0;
  if ((IFG2 & UCA0TXIFG)==0) flag = 0;
  if (IFG2 & UCA0RXIFG) flag = 0;

  while(flag)
  {
    P2OUT = (P2IN&BIT0)<<1; // If button is not pressed, light LED.
    __delay_cycles(100000);
  }
  
  while(1);

}
