#include "s3c2440.h"

void Init_SDRAM(){
	BWSCON = 0x22000000;

	BANKCON6 = 0x00018001;
	BANKCON7 = 0x00018001;

	REFRESH = 0x008404f5;

	BANKSIZE = 0x000000b1;

	MRSRB6 = 0x00000020;
	MRSRB7 = 0x00000020;
}

int SDRAM_Test(){
	volatile unsigned char *p = (volatile unsigned char *)0x30000000;
	int i;

	//写
	for(i=0; i<1000; i++)
		p[i] = 0x55;

	//读
	for(i=0; i<1000; i++)
		if(p[i] != 0x55)
			return -1;

	return 0;
}