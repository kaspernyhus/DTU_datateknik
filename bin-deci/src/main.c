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


void print_bin(int c, int bits, int row) {
char c_buffer;
int XY = 15 - ((16 - bits) / 2); // prints in the center of the screen

for (int i=0; i<bits; i++) {
c_buffer = '0' + !!(c & (1 << i));
sendCharXY(c_buffer, row, XY-i);
}
}


void print_dec(int dec, int x, int y) {
  char str[20];
  sprintf(str, "%d", dec);
  sendStrXY(str, x, y);
}
    


int main(void) {
  init_OLED();
  
  int bin = 0b011000101101;

  print_bin(bin, 12, 3);
  print_dec(bin, 3, 5);

  while (1) {

  }
}