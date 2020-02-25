#include <avr/io.h>

#define OUTPUT 0x1
#define NOT_VALLID_PIN 0

void setPinMode(char Reg, int pin, char mode);