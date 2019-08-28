#include "uart.h"

#define GPHCON (*(volatile unsigned int *)0x56000070)
#define GPHUP (*(volatile unsigned int *)0x56000078)

#define UCON1 (*(volatile unsigned int *)0x50004004)
#define UBRDIV1 (*(volatile unsigned int *)0x50004028)
#define ULCON1 (*(volatile unsigned int *)0x50004000)
#define UTRSTAT1 (*(volatile unsigned int *)0x50004010)
#define UTXH1 (*(volatile unsigned char *)0x50004020)

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