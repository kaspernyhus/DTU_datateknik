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


volatile char c;
volatile char RX_flag = 0;


void OLED_Init() {
	I2C_Init();
	InitializeDisplay();
	clear_display();
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
  OLED_Init();
  UART0_Init(MYUBRR);
  
  UART0_puts("Indtast den aktuelle tid (hh:mm:ss): \r");
  
  char init_time[50];
  UART0_gets(init_time);

  int hours;
  int minutes;
  int seconds;

  sscanf(init_time, "%d:%d:%d" , &hours, &minutes, &seconds);

  char time[50];
  sprintf(time, "\nInit time: %02i:%02i:%02i\r", hours, minutes, seconds);
  UART0_puts(time);

  
  //sei(); Global Interrupt Enable

  while (1) {
    update_time(&hours, &minutes, &seconds);
    sprintf(time, "\nAktuelt klokkeslæt er %02i:%02i:%02i\r", hours, minutes, seconds);
    UART0_puts(time);
    _delay_ms(1000);
  }
}


ISR(USART0_RX_vect) {
  RX_flag = 1;
  c=UDR0;
}