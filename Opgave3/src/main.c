/*
----------------------------
 Datateknik & programmering 
// 		Opgave 3
----------------------------

Oscar Emil Vinholt s182500
Mehrdad Dadkhah s195077
Kasper Nyhus Kaae s195087

*/ 

#define F_CPU 16000000UL

#include <avr/io.h>
#include <stdio.h>
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
  char MSB;
  char LSB;
  int temp_data;

  PORTB &=~ (1<<0); // set SS LOW
  
  SPI_MasterTransmit(0x00);
  MSB = SPDR;
  
  SPI_MasterTransmit(0x00);
  LSB = SPDR;

  PORTB |= (1<<0); // set SS HIGH

  temp_data = (MSB << 8 | LSB)>>3 ; // combine the two bytes in to one variable and cut first 3 bits
  return temp_data;
}


int main(void) {
  init();
	sendStrXY("MAX6675 test",0,2);
	sendStrXY("Temperatur:",2,2);
	
	int temp;
	int reel_temp = 0;
	int deci = 0;
	char str[20];
	
	while (1) {
		temp = getRawData();
		reel_temp = trunc(temp*0.25);
		deci = (temp*0.25 - reel_temp) * 100;

		sprintf(str, "%d.%0.2d", reel_temp, deci);
		sendStrXY(str, 3,5);
		
		_delay_ms(1000);
  }
}
