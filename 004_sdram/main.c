#include "uart.h"
#include "sdram.h"

int main(){

	Init_UART1();
	Init_SDRAM();

	SendString("System init success\n\r");


	if(SDRAM_Test() == 0)
		SendString("SDRAM test success!\n\r");
	else
		SendString("SDRAM test fail!\n\r");

	return 0;
}