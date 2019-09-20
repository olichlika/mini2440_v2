#include "s3c2440.h"


void delay(int tt);

int Init_LED(){
	GPBCON = 0x00015400;
	GPBDAT = 0x00000000;
	
	GPBDAT |= (1<<5) | (1<<6) | (1<<7) | (1<<8);
	while(1){
		GPBDAT &= ~(1<<6);
		delay(10000);
		GPBDAT |= (1<<6);
		delay(10000);
	}
}

void delay(int tt){
	int a,b;
	for(a=0;a<=tt;a++)
	for(b=0;b<=100;b++);
}