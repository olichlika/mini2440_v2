#include "uart.h"

void delay(int tt);

char g_char1 = 'A';
char g_char2 = 'a';
int g_A = 0;

int main(){

	Init_UART0();
	SendString("This is Mini2440!\n\r");
	
	SendString("g_A = ");
	PrintHex(g_A);
	SendString("\n\r");
	
	while(1){
		SendChar(g_char1);
		g_char1++;
		SendChar(g_char2);
		g_char2++;
		delay(1000);
	}
	return 0;
}

void delay(int tt){
	int a,b;
	for(a=0;a<=tt;a++)
	for(b=0;b<=100;b++);
}