/*
    
    Kasper Nyhus Kaae
        s195087

---------------------------------
Interrupt module for
ARDUINO MEGA2560
---------------------------------

*/

#include <avr/io.h>
#include "interrupt.h"


void itr4_Init() {
  // initialize external interrupt INT4
  EICRB |= (1<<ISC41);  // 
  EICRB &=~ (1<<ISC40); // NOTE: trigger on FALLING edge
  EIMSK |= (1<<INT4);   // enable INT4
}