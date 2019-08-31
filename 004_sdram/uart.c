#include "uart.h"
#include "s3c2440.h"

void Init_UART1(){
	//初始化GPIO
	GPHCON &= ~((3 << 10) | (3 << 8));
	GPHCON |= (2 << 10) | (2 << 8);
	GPHUP &= ~((1 << 5) | (1 << 4));//上拉

	//配置
	UCON1 |= (1 << 2) | (1 << 0);
	UBRDIV1 = 26; //波特率115200
	ULCON1 |= (3 << 0);//8数据位 1停止位
}


void SendChar(char c){
	while(!(UTRSTAT1 & (1 << 2)));//发送移位寄存器不为空就等待
	UTXH1 = c;
}

char ReciveChar(){
	while (!(UTRSTAT1 & (1<<0)));
	return URXH1;	
}

void SendString(char *str){
	// if(!str)return;
	while(*str != '\0'){
		SendChar(*str);
		str++;
	}
}