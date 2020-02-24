/*
 * Blink on board LED
 
 * Kasper Nyhus Kaae
 * s195087
 */ 

#define F_CPU 16000000L // Specify oscillator frequency
#include <avr/io.h>
#include <util/delay.h>


void init(){
	DDRB |= (1<<7); // set Arduino pin PWM pin 13 (PB7) as output
}

int main(void)
{
	init();
	
	while(1)
	{
		PORTB |= (1<<7); // set PB7 output HIGH
		_delay_ms(500);
		PORTB &=~ (1<<7); // set PB/ output LOW
		_delay_ms(500);
	}
}