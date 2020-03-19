/*
    
    Kasper Nyhus Kaae
        s195087

---------------------------------
UART module for
ARDUINO MEGA2560
---------------------------------

*/

void UART0_Init(unsigned int baud);
void UART0_sendChar(char data);
char UART0_receiveChar();
void UART0_puts(char *string);
void UART0_gets(char *cPtr);