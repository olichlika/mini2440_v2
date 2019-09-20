#include "uart.h"
#include "s3c2440.h"

//中断处理函数
void handle_irq_c(){
	int bit;
	
	bit = INTOFFSET;//读当前发生中断的值
	
	if(bit == 5){ //eint8 ~23
		if(EINTPEND & (1 << 8)){
			SendString("EINT8!\n\r");
			EINTPEND |= (1 << 8);//清中断
		}else if(EINTPEND & (1 << 11)){
			SendString("EINT11!\n\r");
			EINTPEND |= (1 << 11);//清中断
		}else if(EINTPEND & (1 << 13)){
			SendString("EINT13!\n\r");
			EINTPEND |= (1 << 13);//清中断
		}
	}
	
	//清中断 写1清除
	SRCPND |= (1 << bit);
	INTPND |= (1 << bit);
}

//外部中断初始化
//GPG0 -> EINT8
//GPG3 -> EINT11
//GPG5 -> EINT13
void Init_EINT(){
	//初始化寄存器
	GPGCON &= ~((3 << 0) | (3 << 6) | (3 << 10));
	GPGCON |= ((2 << 0) | (2 << 6) | (2 << 10));
	
	//设置为下降沿触发
	EXTINT1 &= ~((7 << 0) | (7 << 12) | (7 << 20));
	EXTINT1 |= ((3 << 0) | (3 << 12) | (3 << 20));
	
	//使能中断
	EINTMASK &= ~((1 << 8) | (1 << 11) | (1 << 13));
	
	INTMSK &= ~(1 << 5);
}