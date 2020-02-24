/*
    ****** interrupt TEST ******
NOTE: pushbutton connected to PWN pin 2

Blinks on board LED when interrupt occurs
 
 * Kasper Nyhus Kaae
 * s195087
*/ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>


volatile char intr_flag = 0; // interrupt flag

// external interrupt 4 (INT4) setup
void init_interrupt() {
  EICRB |= (1<<ISC41);  // 1 
  EICRB &=~ (1<<ISC40); // 0      NOTE: trigger on FALLING edge
  EIMSK |= (1<<INT4);   // enable INT4
}

void blink_led() {
  PORTB &=~ (1<<7);
  for (int i=0; i<2; i++) {
    PORTB |= (1<<7);
    _delay_ms(100);
    PORTB &=~ (1<<7);
    _delay_ms(100);
  }
}

void init() {
  // set pwm pin 13 as OUTPUT (PB7) (Onboard LED)
  DDRB |= (1<<7);

  // set pwm pin 2 (PE4) as input pullup
  DDRE &=~ (1<<4); // 0
  PORTE |= (1<<4); // 1
  
  // initialize interrupt
  init_interrupt();
  sei(); // global interrupt enable

  _delay_ms(500);
  blink_led();
}


int main(void)
{
  init();
  
	while (1)
    {
      if (intr_flag == 1) {
        intr_flag = 0;
        _delay_ms(80); //debounce interrupt
        blink_led();
      }
    }
}

ISR(INT4_vect) { // Interupt Service Routine (INT4)
  intr_flag = 1;
}