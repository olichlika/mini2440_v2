#include "uart.h"

void delay(int tt);

char g_char = 'C';

int main(){

	Init_UART1();
	SendString("This is Mini2440!\n\r");
	while(1){
		SendChar(g_char);
		g_char++;
		delay(1000);
	}
	return 0;
}

void delay(int tt){
	int a,b;
	for(a=0;a<=tt;a++)
	for(b=0;b<=100;b++);
}