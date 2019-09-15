#include "uart.h"
#include "s3c2440.h"

void PrintException(unsigned int cpsr, unsigned int spsr, char *str){
	SendString("cpsr = ");
	PrintHex(cpsr);
	SendString(" ");
	SendString("spsr = ");
	PrintHex(spsr);
	SendString(" ");	
	SendString(str);
	SendString("\n\r");
}
