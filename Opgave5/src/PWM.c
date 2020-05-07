/*
    
    Kasper Nyhus Kaae
        s195087

---------------------------------
PWM module for
ARDUINO MEGA2560
---------------------------------

*/


#include "PWM.h"

void PWM_timer1_ph_correct_Init() {
	TCCR1A |= (1<<COM1A1)|(1<<WGM10); // Clear OCnA on compare match (set output to low level) - DS: p155
	TCCR1B |= (1<<CS11); //prescaler 8
	//OCR2A = 102; //40% duty cycle
	TCNT1 = 0; //force counter reset

	DDRB |= (1<<5); // configure OC1A pin OUTPUT
}