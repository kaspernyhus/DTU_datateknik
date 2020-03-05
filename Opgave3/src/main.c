/*
 *
 * Kasper Nyhus Kaae
 * s195087
 * 
 * Opgave 3
 * 
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include "I2C.h"
#include "ssd1306.h"
#include "SPI.h"


void init() {
	I2C_Init();
	InitializeDisplay();
	clear_display();

  SPI_init_master();
}

int getRawData() {
  PORTB &=~ (1<<0); // set SS LOW
  char MSB;
  char LSB;
  int all_data;
  int temp_data;

  SPI_MasterTransmit(0x00);

  MSB = SPDR;
  
  SPI_MasterTransmit(0x00);

  LSB = SPDR;

  PORTB |= (1<<0); // set SS HIGH

  all_data = (MSB << 8) | LSB ;

  temp_data = 0b111111111111 & (all_data >> 3); // 111111111111 & all_data (MINUS de tre første bits)

  return temp_data;
}



void show_temp(int data) {
  int decimal = binaryToDecimal(data);
  sendStrXY(decimal, 4, 5);
}


int main(void) {
  init();
  int temp;
  while (1) {
    temp = getRawData();
    show_temp(temp);
    _delay_ms(1000);
  }
}
