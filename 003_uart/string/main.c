#include "uart.h"

void delay(int tt);

int main(){
	char c;

	Init_UART1();
	while(1){
		SendString("This is Mini2440!\n\r");
		//SendChar('A');
		delay(10000);
	}
	return 0;
}

void delay(int tt){
	int a,b;
	for(a=0;a<=tt;a++)
	for(b=0;b<=100;b++);
}