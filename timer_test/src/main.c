/*
----------------------------
 Datateknik & programmering 
 	
----------------------------
*/ 

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "I2C.h"
#include "ssd1306.h"
#include "timer.h"

volatile char count = 0;


void init() {
  DDRB |= (1<<PB7); //on-board LED
}



int main(void)
{
	init();
  timer3_Init(6249);
	sei(); // global interrupt enable

	while (1)
    {
		
    }
}


ISR(TIMER3_COMPA_vect) {
  static char toggle = 0;
  if (toggle == 0) {
    PORTB |= (1<<PB7);
    toggle = 1;
  }
  else {
    PORTB &=~ (1<<PB7);
    toggle = 0;
  }
}
