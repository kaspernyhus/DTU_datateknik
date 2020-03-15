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


void OLED_Init() {
	I2C_Init();
	InitializeDisplay();
	clear_display();
}


int main(void) {
  OLED_Init();
  UART0_Init(MYUBRR);
  sendStrXY("UART TEST",1,3);
  
  char string[16];
  //char c;

  while (1) {
    UART0_gets(string);
    sendStrXY(string,3,0);
    
    _delay_ms(5000);
    
  }
}