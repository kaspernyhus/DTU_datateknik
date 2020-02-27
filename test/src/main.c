/*
 * 
 * Kasper Nyhus Kaae
 * s195087
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include "setPinMode.h"




int main(void) {
    
  //DDRB |= (1<<7);
  setPinMode('B', 7, OUTPUT);

	while (1)
    {
		for (int i=0; i<2; i++) {
            PORTB |= (1<<7);
            _delay_ms(100);
            PORTB &=~ (1<<7);
            _delay_ms(100);
        }
      _delay_ms(1000);
    }
}
