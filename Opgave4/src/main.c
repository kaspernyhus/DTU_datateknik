/*
 * Kasper Nyhus Kaae
 * s195087
 */ 

#define BAUD 19200
#define MYUBRR F_CPU/8/BAUD-1 //full dublex

#include <avr/io.h>
#include "UART.h"
#include "interrupt.h"
#include <avr/interrupt.h>
#include "timer.h"


volatile char c_buffer;
volatile int index = 0;
volatile char Reset_flag = 0; // interrupt reset button
volatile char RX_recieved = 0;
volatile char timer_flag = 0;
volatile int counter = 0;
char init_time[50] = {0}; //initialize the buffer
char time[50]; //buffer for time string
char start_timer = 0;


void itr_button_Init() {
  itr4_Init();
	DDRE &=~ (1<<4); // set PE4 as INPUT PULLUP
	PORTE |= (1<<4); 
}


void update_time(int *h , int *m , int *s) {
  if (*s < 59)
    (*s)++;
  else if (*m < 59) {
    (*s) = 0;
    (*m)++;
  }
  else if (*h < 23) {
    (*s) = 0;
    (*m) = 0;
    (*h)++;
  }
  else {
    (*s) = 0;
    (*m) = 0;
    (*h) = 0;
  }
}


int main(void) {
  UART0_Init(MYUBRR);
  UART0_enableReceive_Itr();
  itr_button_Init();
  timer3_Init(249); // = 1ms, prescaling = 64
  sei(); // global interrupt enable
  
  UART0_puts("Indtast den aktuelle tid (hh:mm:ss): ");

  int hours;
  int minutes;
  int seconds;

  while (1) {
    if (Reset_flag == 1) {
      Reset_flag = 0;
      sscanf(init_time, "%d:%d:%d" , &hours, &minutes, &seconds);
      UART0_puts("\nUr resat til: ");
      sprintf(time, "%02i:%02i:%02i", hours, minutes, seconds);
      UART0_puts(time);
      start_timer = 1;
    }
    
    if (RX_recieved == 1) {
      start_timer = 0;
      RX_recieved = 0;
      UART0_sendChar(c_buffer);
    }

    if (start_timer == 1) {
      if (timer_flag) {
        timer_flag = 0;
        update_time(&hours, &minutes, &seconds);
        sprintf(time, "\nAktuelt klokkeslæt er %02i:%02i:%02i", hours, minutes, seconds);
        UART0_puts(time);
      }
    }
  }
}


ISR(INT4_vect) { // Reset button ISR
  Reset_flag = 1;
}

ISR(USART0_RX_vect) { // RX complete ISR
  RX_recieved = 1;
  
  if (index == 9) {
    Reset_flag = 1;
    index = 0;
  }

  c_buffer = UDR0;
  init_time[index] = UDR0;
  index++;
}

ISR(TIMER3_COMPA_vect) { // timer match interrupt
  if (counter == 999) {
    counter = 0;
    timer_flag = 1;
  }
  else {
    counter++;
  }
}