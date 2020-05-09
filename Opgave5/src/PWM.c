/*
    
    Kasper Nyhus Kaae
        s195087

---------------------------------
PWM module for
ARDUINO MEGA2560
---------------------------------

*/


#include "PWM.h"

void PWM_timer4_ph_correct_Init() {
	TCCR4A |= (1<<COM4A1);  //  - DS: p160
	TCCR4A |= (1<<WGM40);		// PWM, ph correct 8-bit. TOP: 0x00FF - DS table: p148
	TCCR4B |= (1<<CS41); //prescaler 8 - DS: 161
	
	TCNT4 = 0; //force counter reset

	DDRH |= (1<<PH3); // configure OC4A pin OUTPUT (PH3)
}