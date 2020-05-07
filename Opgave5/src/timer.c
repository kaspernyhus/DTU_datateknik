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

void timer1_Normal_OVitr_Init() {
  //TCCR1B |= (1<<CS11); // pre-scaling /8
  // normal mode
  TIMSK1 |= (1<<TOIE1); // overflow interrupt enable 
}


void timer3_CTC_Init(unsigned int compareValue) {
  TCCR3B |= (1<<CS31) | (1<<CS30); // pre-scaling
  TCCR3B |= (1<<WGM32); // CTC mode, TOP value = OCR3A
  OCR3A = compareValue; // f.eks. for 100ms = compareValue: 6249
  TIMSK3 |= (1<<OCIE3A); // interrupt when TCNT3 = OCR3A value
}