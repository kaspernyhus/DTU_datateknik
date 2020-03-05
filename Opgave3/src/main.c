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

  all_data = (MSB << 8) | LSB ; // combine the two bytes in to one variable

  /* Extraxt the 12bits of temperature information */
  temp_data = 0b111111111111 & (all_data >> 3); // 111111111111 & all_data (MINUS de tre første bits)

  return temp_data;
}


void print_bin(int c, int bits, int row) {
  char c_buffer;
  int XY = 15 - ((16 - bits) / 2); // prints in the center of the screen

  for (int i=0; i<bits; i++) {
    c_buffer = '0' + !!(c & (1 << i));
    sendCharXY(c_buffer, row, XY-i);
  }
}


void print_dec(int dec, int x, int y) {
  char str[20];
  sprintf(str, "%d", dec);
  sendStrXY(str, x, y);
}


void show_temp(int data) {
  print_bin(data, 12, 4);
  
  data = data * 0.25;
  print_dec(data, 5,7);
}


int main(void) {
  init();
  sendStrXY("MAX6675 test",0,2);
  sendStrXY("Temperatur:",2,2);
  
  int temp;
  
  while (1) {
    temp = getRawData();
    show_temp(temp);
    _delay_ms(1000);
  }
}
