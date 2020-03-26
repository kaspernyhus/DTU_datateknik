/*
    
    Kasper Nyhus Kaae
        s195087

---------------------------------
Timer module for
ARDUINO MEGA2560
---------------------------------

*/


#include <avr/io.h>
#include "timer.h"


void timer3_Init(unsigned int compareValue) {
  TCCR3B |= (1<<CS31) | (1<<CS30); // pre-scaling
  TCCR3B |= (1<<WGM32); // CTC mode, TOP value = OCR3A
  OCR3A = compareValue; // f.eks. for 100ms = compareValue: 6249
  TIMSK3 |= (1<<OCIE3A); // interrupt when TCNT3 = OCR3A value
}