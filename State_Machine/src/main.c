/*
----------------------------
 		State Machine TEST
----------------------------
*/ 

#define F_CPU 16000000UL
#define BAUD 19200
#define MYUBRR F_CPU/8/BAUD-1 //full dublex

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <util/delay.h>
#include "UART.h"
#include "timer.h"


volatile char init_flag = 1;
volatile char data=0;
volatile char rx_flag=0;
int MIN_value;
int MAX_value;
volatile char buffer[8] = {'\0'};
volatile int _index = 0;
volatile char PWMvalues[8];


//Menu message
char message[]= "Indtast:\r a to set PWM MIN value (0) \r b to set PWM MAX value (1023)\r\0";

//States
typedef enum {
    init,
		listen,
    MIN,
		MIN_val,
    MAX,
		MAX_val
} state;

//Events
typedef enum {
    NILEVENT,
    a,
    b,
    enter,
} event;

//a pointer to functions
typedef void (*action)();

//function which test on the event at shift states
void stateEval(event e);

//the next state and actions definition
typedef struct {
    state nextState;
    action actionToDo;
} stateElement;

//the actions definitions as functions
void noAction(void);
void init_MIN_MAX(void);
void updateMIN(void);
void updateMAX(void);
void echoUART(void);
void resetBuffer(void);
void setLEDbrightness(int PWMvalue);

/*
CS				noinput							"a"						"b"						"Enter"
init			init,init_MIN_MAX		MIN,noAction	MAX,noAction	listen,noAction
listen		listen,noAction			MIN,noAction	MAX,noAction	listen,noAction
MIN				MIN_val,noAction		MIN,noAction	MAX,noAction	listen,noAction
MIN_val		MIN_val,noAction		MIN,noAction	MAX,noAction	listen,updateMIN
MAX				MAX_val,noAction		MIN,noAction	MAX,noAction	listen,noAction
MAX_val		MAX_val,noAction		MIN,noAction	MAX,noAction	listen,updateMAX
*/

stateElement stateMatrix[6][4] = {
	{ 	{init,init_MIN_MAX}, 	{MIN,noAction}, 	{MAX,noAction}, 	{listen,noAction} 	},
	{ 	{listen,noAction}, 		{MIN,noAction}, 	{MAX,noAction}, 	{listen,noAction} 	},
	{ 	{MIN_val,noAction},		{MIN,noAction}, 	{MAX,noAction}, 	{listen,noAction} 	},
	{ 	{MIN_val,echoUART},		{MIN,noAction}, 	{MAX,noAction}, 	{listen,updateMAX} 	},
	{ 	{MAX_val,noAction},		{MIN,noAction}, 	{MAX,noAction}, 	{listen,noAction} 	},
	{ 	{MAX_val,echoUART},		{MIN,noAction}, 	{MAX,noAction}, 	{listen,updateMAX} 	}
};

//initial event
event 	eventOccured= NILEVENT;
void  stateEval(event e);
//initial state
state currentState=init;

//state action control based on events
void stateEval(event e)
{
    stateElement stateEvaluation = stateMatrix[currentState][e];     //a table look-up to fetch the relevant next state and the related function pointer
    currentState= stateEvaluation.nextState;
    (*stateEvaluation.actionToDo)();                               //the execution
}


void PWM_ph_correct_Init() {
	TCCR2A |= (1<<COM2A1)|(1<<WGM20);
	TCCR2B |= (1<<CS22); //prescaler 8
	OCR2A = 102; //40% duty cycle
	TCNT2 = 0; //force counter reset
	DDRB |= (1<<4); // OUTPUT
}



void initialize() {
	UART0_Init(MYUBRR);
	UART0_enableReceive_Itr();
	
	PWM_ph_correct_Init();

	UART0_puts("UART initialized\n");
}


int main(void) {
  initialize();
	sei();
	
	stateEval((event)NILEVENT); //initialize message to the user
	while (1) {
		
		/*for initial message*/
		if (init_flag == 1) {
			 UART0_puts(message);
			 init_flag=0;
		}
		
		if(rx_flag==1) {
			rx_flag = 0;
			
			switch(data) {
				case 'a':
				eventOccured = a;
				UART0_puts("MIN value for PWM\r");
				break;
				case 'b' :
				eventOccured = b;
				UART0_puts("MAX value for PWM\r");
				break;
				case '\r':
				eventOccured = enter;
				UART0_puts("\nValue saved!\r");
				break;

				default:
				eventOccured=NILEVENT;
			}

			stateEval((event)eventOccured);
		}
	}
}


ISR(USART0_RX_vect) {
	data=UDR0;
	rx_flag=1;
}



/*these functions is implementing the actions*/

void noAction(void) {

}

void init_MIN_MAX(void) {
	MIN_value = 0;
	MAX_value = 1023;
}

void updateMIN(void) {
	sscanf(buffer, "%d", &MIN_value);

	UART0_puts("PWM value sat til: ");
	sprintf(PWMvalues, "%i\n", MIN_value);
  UART0_puts(PWMvalues);

	_index = 0;
	resetBuffer();
	
	setLEDbrightness(MIN_value);
}

void updateMAX(void) {
	sscanf(buffer, "%d", &MAX_value);
	
	UART0_puts("PWM value sat til: ");
	sprintf(PWMvalues, "%i\n", MAX_value);
  UART0_puts(PWMvalues);
	
	_index = 0;
	resetBuffer();

	setLEDbrightness(MAX_value);
}

void echoUART(void) {
	UART0_sendChar(data);

	buffer[_index] = data;
	_index++;

}

void resetBuffer(void) {
	for (int i=0; i<8; i++) {
		buffer[i] = '\0';
	}
}

void setLEDbrightness(int PWMvalue) {
	OCR2A = PWMvalue;
}