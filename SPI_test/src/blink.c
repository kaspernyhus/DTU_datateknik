/*
 * Kasper Nyhus Kaae
 * s195087
 */

#include "blink.h"
#include <avr/io.h>
#include <util/delay.h>


void blink_led() {
  DDRB |= (1<<7); // set pwm pin 13 as OUTPUT (PB7) (Onboard LED)
  
  PORTB &=~ (1<<7); // turn OFF
  
  for (int i=0; i<2; i++) {
    PORTB |= (1<<7);
    _delay_ms(100);
    PORTB &=~ (1<<7);
    _delay_ms(100);
  }
}