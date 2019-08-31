#ifndef _UART_H
#define _UART_H

void Init_UART1();
void SendChar(char c);
char ReciveChar();
void SendString(char *str);

#endif