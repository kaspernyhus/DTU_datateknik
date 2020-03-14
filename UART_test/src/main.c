/*
 * 
 
 * Kasper Nyhus Kaae
 * s195087
 */ 

#include <avr/io.h>
#include "I2C.h"
#include "ssd1306.h"


void init_OLED() {
	I2C_Init();
	InitializeDisplay();
	clear_display();
}


int main(void) {
  init_OLED();
  
  while (1) {

  }
}