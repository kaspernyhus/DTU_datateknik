/*
----------------------------
 Datateknik & programmering 
		 		Opgave 5
----------------------------
*/ 

#define F_CPU 16000000UL
#define BAUD 19200
#define MYUBRR F_CPU/8/BAUD-1 //full dublex


#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <util/delay.h>
#include "I2C.h"
#include "ssd1306.h"
#include "UART.h"
#include "timer.h"
#include "adc.h"
#include "PWM.h"
#include "statedef.h"


volatile unsigned int sample = 0;
volatile char sample_ready = 0;
volatile char rx_flag = 0;
volatile int timer_counter = 0;
volatile char timer_flag = 0;

unsigned int MIN_value = 0;
unsigned int MAX_value = 255;
volatile char init_flag = 1;
char val_buffer[8] = {'\0'};
int _index = 0;
char PWMvalues[8];
volatile char data = 0;


//------------------------------------------------------------------------
//													STATE MACHINE ACTIONs
//------------------------------------------------------------------------

// ACTION funcitons
void noAction(void) {

}

void init_MIN_MAX(void) {
	MIN_value = 0;
	MAX_value = 255;
}

void updateMIN(void) {
	sscanf(val_buffer, "%d", &MIN_value); // convert reveiced string to int

	UART0_puts("PWM MIN value sat til: ");
	sprintf(PWMvalues, "%i\n", MIN_value);
  UART0_puts(PWMvalues);

	_index = 0;
	resetBuffer();
}

void updateMAX(void) {
	sscanf(val_buffer, "%d", &MAX_value); // convert reveiced string to int
	
	UART0_puts("PWM MAX value sat til: ");
	sprintf(PWMvalues, "%i\n", MAX_value);
  UART0_puts(PWMvalues);
	
	_index = 0;
	resetBuffer();
}

void saveChar(void) {
	UART0_sendChar(data); // echo UART char received

	val_buffer[_index] = data; // save received char in val_buffer
	_index++;
}

void resetBuffer(void) {
	for (int i=0; i<8; i++) {
		val_buffer[i] = '\0';
	}
}

//------------------------------------ STATE MACHINE END ------------------------------------



void init() {
	I2C_Init();
	InitializeDisplay();
	clear_display();
	sendStrXY("Opgave 5",0,4);
	sendStrXY("PWM values",2,3);                                                                                                              

	UART0_Init(MYUBRR);
	UART0_puts("PWM Control\n");
	UART0_enableReceive_Itr();

	timer1_Normal_OVitr_Init();
	adc1_125khz_timer1_OV_Init();
	PWM_timer4_ph_correct_Init();

	DDRB |= (1<<7); //On-Board LED
}


int map_ADC_PWM(unsigned int sample, unsigned int min, unsigned int max) { // map ADC sample range (0-1023) to PWM range (0-255)
	sample = sample/4; // 1024/4 = 256, ADC range: 0-255
	return sample * (max - min) / 255 + min;
}


int main(void) {
  init();
	sei();
	char oled_buffer[20];
	char oled_buffer2[20];
	stateEval((event)NILEVENT); // initial state
	
	while (1) {
		if (init_flag == 1) { // Menu message on load only
			 UART0_puts(message);
			 init_flag=0;
		}
		
		if(rx_flag==1) { // if UART0 itr received
			rx_flag = 0;
			
			switch(data) {
				case 'a':
				eventOccured = a;
				UART0_puts("MIN value for PWM: ");
				break;
				case 'b' :
				eventOccured = b;
				UART0_puts("MAX value for PWM: ");
				break;
				case 0x0A: // Enter pressed
				eventOccured = enter;
				UART0_puts("\nValue saved!\r");
				break;

				default:
				eventOccured=NILEVENT;
			}

			stateEval((event)eventOccured); // Update current state based on what event occured
		}
		
		
		if (sample_ready) {
			sample_ready = 0;
			sample = map_ADC_PWM(sample, MIN_value, MAX_value); // map the ADC signal to PWM range incl setting limits
			OCR4A = sample; // update PWM
		}
		
		if (timer_flag) { // update OLED display approx 30ms
			timer_flag = 0;
			
			//PORTB ^= (1<<7); //blink on-board LED to check lower refresh rate
			
			sprintf(oled_buffer, "%.3d", sample); //8 bit value
			sprintf(oled_buffer2, "%.3d%%", (sample*100/255)); // % of full value
			sendStrXY(oled_buffer, 4,6);
			sendStrXY(oled_buffer2, 5,6);
		}
  }
}



//------------------------------------------------------------------------
//											INTERRUPT SERVICE ROUTINES
//------------------------------------------------------------------------

ISR(USART0_RX_vect) { // UART0 receive
	data=UDR0;
	rx_flag=1;
}


ISR(ADC_vect) { // Interrupt on sample ready
	sample_ready = 1;
	sample = (ADCL+ (ADCH<<8));
}


ISR(TIMER1_OVF_vect) { // Timer 1 overflow
	if (timer_counter == 29) { // 30ms
		timer_flag = 1;
		timer_counter = 0;
	}
	else {
		timer_counter++;
	}
	TCNT1 = 63536;
}