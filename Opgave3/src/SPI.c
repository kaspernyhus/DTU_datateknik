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

#include "SPI.h"
#include <avr/io.h>


void SPI_init_master() {
  DDRB |= (1<<0)|(1<<1)|(1<<2);  // OUTPUTs - PB0: SS / PB1: SCK / PB2: MOSI
  SPCR |= (1<<SPE) | (1<<MSTR) | (1<<CPOL); // SPI setup - enable / set as MASTER / Clk polarity - "clock starts HIGH" note: CPHA sets Clk phase
  SPCR &=~(1<<CPHA);
  SPCR |= (1<<SPR1); // SPI Clock Rate Select. (SPR0 = 0, SPR1 = 1 => f_osc / 64) => 16MHz/64 = 250kHz
  SPCR &=~ (1<<SPR0);
}


void SPI_init_slave() {
  DDRB |= (1<<3); // OUTPUT - PB3: MISO
  SPCR |= (1<<SPE) | (1<<CPOL); // SPI enable / Clk polarity - "clock starts HIGH" note: CPHA sets Clk phase
  PORTB |= (1<<PB0); // set SS HIGH
}


void SPI_MasterTransmit(char cData) {
  //PORTB &=~ (1<<0); // set SS LOW

  SPDR = cData; // save to Data Register

  while(!(SPSR & (1<<SPIF))); // Wait for transmission complete

  //PORTB |= (1<<0); // set SS HIGH
}

char SPI_SlaveReceive(void) {
  while(!(SPSR & (1<<SPIF))); // Wait for reception complete
  
  return SPDR; // Return Data Register
}