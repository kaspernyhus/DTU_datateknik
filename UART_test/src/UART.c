/*
    
    Kasper Nyhus Kaae
        s195087

---------------------------------
SPI module for
ARDUINO MEGA2560
---------------------------------

MISO  =>  pin 50 (PB3)
MOSI  =>  pin 51 (PB2)
SCK   =>  pin 52 (PB1)
SS    =>  pin 53 (PB0)

*/

#include <avr/io.h>
#include "UART.h"


void UART0_Init(unsigned int baud) {
  UCSR0A = (1<<U2X0); //Enable dublex. OBS: ONLY use with async operation
  UCSR0B |= (1<<RXEN0)|(1<<TXEN0); //Enable receive and transmit
  //UCSR0C &=~ (1<<UMSEL00) | (1<<UMSEL01); //Async UART
  //UCSR0C &=~ (1<<UPM00) | (1<<UPM01) | (1<<USBS0); //No parity. 1 stop bit
  UCSR0C |= (1<<UCSZ00)|(1<<UCSZ01); //8-bit data frame

  //Baud rate setting
  UBRR0H = (unsigned char)(baud>>8);
  UBRR0L = (unsigned char)(baud);
}

void UART0_send(char data) {
  while ( !( UCSR0A & (1<<UDRE0)) ); //Wait for empty transmit buffer
  UDR0 = data; //Load data into UART Data Register
}
 
char UART0_receive() {
  while ( !(UCSR0A & (1<<RXC0)) ); //Wait until char is received
  return UDR0; //return data from UART Data Register
}

void UART0_puts(char *StringPtr) {
  while(*StringPtr) {
    UART0_send(*StringPtr);
    StringPtr++;
  }
}

void UART0_gets(unsigned char *cPtr) {
  char c;
  while((c=UART0_receive()) != 0x0D) { // while not "Enter"
    *cPtr = c;
    UART0_send(c);
    cPtr++;
  }
  *cPtr = 0; // terminate string
}
