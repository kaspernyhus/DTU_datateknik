/*
 
 * Kasper Nyhus Kaae
 * s195087
 
 */

#include <avr/io.h>
#include <util/delay.h>
#include "I2C.h"
#include "ssd1306.h"
#include "blink.h"
#include "SPI.h"


void init_OLED() {
	I2C_Init();
	InitializeDisplay();
	clear_display();
}


int main(void) {
  // Master
  // SPI_init_master();

  // Slave
  SPI_init_slave();
  char c = '-';
  init_OLED();
  sendCharXY(c,3,5);
  blink_led();
  

  while (1) {
    // Master
    // SPI_MasterTransmit('H');
    // blink_led();
    // _delay_ms(2000);
    
    // SPI_MasterTransmit('E');
    // blink_led();
    // _delay_ms(2000);
    
    // SPI_MasterTransmit('J');
    // blink_led();
    // _delay_ms(2000);
    

    // Slave
    c = SPI_SlaveReceive();
    sendCharXY(c,3,5);

  }
} 