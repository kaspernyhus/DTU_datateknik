/*
 * 
 
 * Kasper Nyhus Kaae
 * s195087
 */ 
#define BAUD 19200
#define MYUBRR F_CPU/8/BAUD-1 //full dublex

#include <avr/io.h>
#include <util/delay.h>
#include "I2C.h"
#include "ssd1306.h"
#include "UART.h"
#include "interrupt.h"
#include <avr/interrupt.h>


volatile char data;
volatile char Reset_flag = 0; // interrupt reset button
volatile char RX_flag = 0;
volatile char RX_recieved = 0;
char init_time[50] = {0}; //initialize the buffer!
char time[50];
volatile char start_time = 0;

  
void OLED_Init() {
	I2C_Init();
	InitializeDisplay();
	clear_display();
}


void itr_button_Init() {
  itr4_Init();

	DDRE &=~ (1<<4); // set PE4 as INPUT PULLUP
	PORTE |= (1<<4); 
}


void update_time(int *hh , int *mm , int *ss) {
  if (*ss < 59)
    (*ss)++;
  else if (*mm < 59) {
    (*ss) = 0;
    (*mm)++;
  }
  else if (*hh < 23) {
    (*ss) = 0;
    (*mm) = 0;
    (*hh)++;
  }
  else {
    (*ss) = 0;
    (*mm) = 0;
    (*hh) = 0;
  }
}


int main(void) {
  UART0_Init(MYUBRR);
  UART0_enableReceive_Itr();
  OLED_Init();
  itr_button_Init();
  sei(); // global interrupt enable
  
  UART0_puts("Indtast den aktuelle tid (hh:mm:ss): \r");

  int hours;
  int minutes;
  int seconds;

  while (1) {
    if (Reset_flag == 1) {
      Reset_flag = 0;
      sscanf(init_time, "%d:%d:%d" , &hours, &minutes, &seconds);
      UART0_puts("\nUr resat til: \r");
      sprintf(time, "%02i:%02i:%02i\r", hours, minutes, seconds);
      UART0_puts(time);
    }
    
    if (RX_recieved == 1) {
      RX_recieved = 0;
      UART0_sendChar(data);
    }
    
    if (RX_flag == 1) {
      sscanf(init_time, "%d:%d:%d" , &hours, &minutes, &seconds);
      UART0_puts("\nUr sat til: \r");
      sprintf(time, "%02i:%02i:%02i\r", hours, minutes, seconds);
      UART0_puts(time);

      start_time = 1;
    }

    if (start_time == 1) {
      update_time(&hours, &minutes, &seconds);
      sprintf(time, "\nAktuelt klokkeslæt er %02i:%02i:%02i\r", hours, minutes, seconds);
      UART0_puts(time);
      _delay_ms(1000);
    }
  }
}


ISR(INT4_vect) { // Reset button ISR
  Reset_flag = 1;
}

ISR(USART0_RX_vect) { // RX complete ISR
  start_time = 0;
  RX_flag = 0;
  RX_recieved = 1;
  static int i = 0;
  
  data = UDR0;
  init_time[i] = UDR0;
  i++;

  if (i == 9) {
    RX_flag = 1;
    i = 0;
  }
}