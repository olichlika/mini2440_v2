#ifndef _UART_H
#define _UART_H

void Init_UART0();
void SendChar(char c);
char ReciveChar();
void SendString(char *str);
void PrintHex(unsigned int val);

#endif