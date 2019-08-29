#include "uart.h"

void delay(int tt);

int main(){
	char c;

	Init_UART1();
	while(1){
		// SendChar('C');
		// delay(10000);
		c = ReciveChar();
		SendChar(c);
	}
	return 0;
}

void delay(int tt){
	int a,b;
	for(a=0;a<=tt;a++)
	for(b=0;b<=100;b++);
}