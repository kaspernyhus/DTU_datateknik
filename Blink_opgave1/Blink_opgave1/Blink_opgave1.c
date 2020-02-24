/*
 * Blink_opgave1.c
 *
 * Created: 08/02/2020 12.33.16
 * Author : kaspernyhus
 */ 

#define F_CPU 16000000L // Specify oscillator frequency
#include <avr/io.h>
#include <util/delay.h>


void pinModes(uint8_t pin, uint8_t mode);


void init(){
	DDRG = 0b00100000; // set Arduino pin PWM pin 4 (PG5) as output
}

int main(void)
{
	init();
	
	while(1)
	{
		PORTG = 0b00100000; // set PG5 output HIGH
		_delay_ms(500);
		PORTG = 0b00000000; // set PG5 output LOW
		_delay_ms(500);
	}
}

void pinModes(uint8_t pin, uint8_t mode) {
	uint8_t ddreg;
	uint8_t bit_num;
	
	switch (pin) {
		case 4: //PG5
			ddreg = DDRG; 
			bit_num = 0b00100000;
			break;
		case 5: //PE3
			ddreg = DDRE; 
			bit_num = 0b00001000;
			break;
		default:
			ddreg = 0;
	};
	
	if (ddreg == 0) {
		return;
		};
	
	if (mode == 1) {
		ddreg |= bit_num;
	};
	
}