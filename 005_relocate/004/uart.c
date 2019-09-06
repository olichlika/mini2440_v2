#include "uart.h"
#include "s3c2440.h"

void Init_UART0(){
	//初始化GPIO
	GPHCON &= ~((3 << 6) | (3 << 4));
	GPHCON |= (2 << 6) | (2 << 4);
	GPHUP &= ~((1 << 3) | (1 << 2));//上拉

	//配置
	UCON0 |= (1 << 2) | (1 << 0);
	UBRDIV0 = 26; //波特率115200
	ULCON0 |= (3 << 0);//8数据位 1停止位
}


void SendChar(char c){
	while(!(UTRSTAT0 & (1 << 2)));//发送移位寄存器不为空就等待
	UTXH0 = c;
}

char ReciveChar(){
	while (!(UTRSTAT0 & (1<<0)));
	return URXH0;	
}

void SendString(char *str){
	// if(!str)return;
	while(*str != '\0'){
		SendChar(*str);
		str++;
	}
}