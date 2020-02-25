#include <avr/io.h>
#include "setPinMode.h"


void setPinMode(char Reg, int Pin, char Mode) {
    int DDR_reg;
    int PORT_reg;

    switch (Reg) {
        case 'A':
            DDR_reg = DDRA;
            PORT_reg = PORTA;
            break;
        case 'B':
            DDR_reg = DDRB;
            PORT_reg = PORTB;
            break;
        case 'C':
            DDR_reg = DDRC;
            PORT_reg = PORTC;
            break;
        case 'D':
            DDR_reg = DDRD;
            PORT_reg = PORTD;
            break;
        case 'E':
            DDR_reg = DDRE;
            PORT_reg = PORTE;
            break;
        
        default:
            return;
    }   
    
    switch (Mode) {
        case OUTPUT:
            DDR_reg |= (1<<Pin); //1
            break;
        
        case 'I': // INPUT
            DDR_reg &=~ (1<<Pin); //0
            PORT_reg &=~ (1<<Pin); //0
            break;
        
        case 'P': // INPUT-PULLUP
            DDR_reg &=~ (1<<Pin); // 0
            PORT_reg |= (1<<Pin); // 1
            break;
    }
}