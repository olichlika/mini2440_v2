#include "uart.h"
#include "string_utils.h"
#include "s3c2440.h"

#define PAGE_SIZE 512

#define TACLS		0	// 1-clk(0ns) 
#define TWRPH0		6	// 3-clk(25ns)
#define TWRPH1		0	// 1-clk(10ns)  //TACLS+TWRPH0+TWRPH1>=50ns

void Init_Nand(){
	// NFCONF &= ~(3 << 12); //TACLS >= 0ns
	// NFCONF &= ~(7 << 8); //TWRPH0 >= 25ns
	// NFCONF |= (2 << 8);
	// NFCONF &= ~(7 << 4); //TWRPH1 >= 10ns
	
	NFCONF = (TACLS<<12)|(TWRPH0<<8)|(TWRPH1<<4)|(0<<0);
	
	//配置使能NAND flash控制器，暂时关闭片选，开ECC
	//NFCONT |= (1 << 4) | (1 << 1) | (1 << 0);
	NFCONT = (0<<13)|(0<<12)|(0<<10)|(0<<9)|(0<<8)|(0<<6)|(0<<5)|(1<<4)|(1<<1)|(1<<0);	
}

//片选使能
void Nand_Select(){
	NFCONT &= ~(1 << 1); 
}

//片选失效
void Nand_Deselect(){
	NFCONT |= (1 << 1); 
}

//发命令
void Nand_CMD(unsigned char cmd){
	volatile int i;
	
	NFCMD = cmd;
	for(i=0; i<10; i++);
}

//发地址
void Nand_Addr_Byte(unsigned char addr){
	volatile int i;
	
	NFADDR = addr;
	for(i=0; i<10; i++);	
}

unsigned char Nand_Data(){
	return NFDATA;
}
void Nand_WriteData(unsigned char data){
	NFDATA = data;
}

//读ID
void Nand_Read_ID(){
	unsigned char buf[4];
	
	Nand_Select();
	Nand_CMD(0x90);
	Nand_Addr_Byte(0x00);
	buf[0] = Nand_Data();
	buf[1] = Nand_Data();
	buf[2] = Nand_Data();
	buf[3] = Nand_Data();
	Nand_Deselect();
	
	printf("Byte 1 = 0x%x\n\r", buf[0]);
	printf("Byte 2 = 0x%x\n\r", buf[1]);
	printf("Byte 3 = 0x%x\n\r", buf[2]);
	printf("Byte 4 = 0x%x\n\r", buf[3]);
	printf("=================\n\r");
}

void Nand_Wait_Ready(){
	while(!(NFSTAT & 0x1));
}

//读flash
//参数；读取地址 读取储存数组 读取长度
void Nand_Read(unsigned int addr, unsigned char * buf, unsigned int len){
	int i = 0;
	int page, col;
	
	Nand_Select();
	
	while(i < len){//循环读
		page = addr / PAGE_SIZE;//第几页
		col = addr % PAGE_SIZE;//第几列
		
		if(col >= 0 && col <= 255){
			Nand_CMD(0x00);//上半区读命令
		}else if(col >= 256 && col <= 511){
			Nand_CMD(0x01);//下半区读命令
			col = col - 256;	
		}
		Nand_Addr_Byte(col & 0xff);
		Nand_Addr_Byte(page & 0xff);
		Nand_Addr_Byte((page>>8) & 0xff);
		Nand_Addr_Byte((page>>16) & 0xff);
		
		Nand_Wait_Ready();
		
		//连续读
		for(;(col < 256) && (i < len); addr++){
			buf[i++] = Nand_Data();
		}
		if(i == len)
			break;
	}
	Nand_Deselect();
}

void Nand_Read_Test(){
	int i, j;
	int len = 160;
	unsigned char s[len];
	unsigned int addr;
	volatile char * p;
	
	printf("Enter the address to read: ");
	addr = get_uint();
	
	Nand_Read(addr, s, len);
	
	printf("Data : \n\r");
	for(i=0; i<10; i++){
		for(j=0; j<16; j++){
			printf("%02x ", *(s + 16*i + j));
		}
		printf("    ");
		
		//输出ascii字符
		for (j=0; j<16; j++){
			/* 后打印字符 */
			if (*(s + 16*i + j) < 0x20 || *(s + 16*i + j) > 0x7e)  /* 不可视字符 */
				SendChar('.');
			else
				SendChar(*(s + 16*i + j));
		}
		
		printf("\n\r");
	}
	printf("=================\n\r");	
}

//复位
void Nand_Reset(){
	Nand_Select();
	Nand_CMD(0xff);
	Nand_Wait_Ready();
	Nand_Deselect();
}

//擦除flash 块为单位
//参数；擦除起始地址 擦除长度
int Nand_Erase(unsigned int addr, unsigned int len){
	unsigned int page;
	unsigned char status;
	
	if(addr & (0x3FFF)){
		printf("Nand_Erase addr is not block align\n\r");
		return -1;
	}
	if(len & (0x3FFF)){
		printf("Nand_Erase len is not block align\n\r");
		return -1;
	}
	if(len == 0){
		printf("Nand_Erase len is 0\n\r");
		return -1;		
	}

	Nand_Select();
	while(1){
		
		page = addr / PAGE_SIZE;//擦除地址所在的块
		
		//printf("page = %d\n\r", page);
		
		Nand_CMD(0x60);

		Nand_Addr_Byte(page & 0xff);
		Nand_Addr_Byte((page>>8) & 0xff);
		Nand_Addr_Byte((page>>16) & 0xff);
		
		Nand_CMD(0xD0);

		Nand_Wait_Ready();
		
		//读取状态位
		Nand_CMD(0x70);
		status = Nand_Data();
		printf("status = 0x%x\n\r", status);
		
		len = len - (16 * 1024); //减去1个块的长度
		if(len == 0)
			break;
		addr = addr + (16 * 1024);//指向下一个block
	}
	Nand_Deselect();
	return 0;
}

void Nand_Erase_Test(){
	unsigned int addr, len;
	int r;
	
	printf("Enter the address to erase: ");
	addr = get_uint();
	
	// printf("Enter the len: ");
	// len = get_uint();	
	len = 16 * 1024;//1 block
	
	printf("erasing ...\n\r");
	
	r = Nand_Erase(addr, len);
	if(r == 0)
		printf("erase success!\n\r");
	else
		printf("erase failed!\n\r");
	printf("=================\n\r");	
}

//写flash
//参数；写入地址 写入数组 长度
int Nand_Write(unsigned int addr, unsigned char * buf, unsigned int len){
	int i = 0;
	int page, col;
	
	printf("addr = 0x%x; buf = %s; len = %d\n\r", addr, buf, len);
	
	col = addr % PAGE_SIZE;
	if(col > 255){
		printf("Starting address is greater than 255\n\r");
		return -1;
	}
	
	
	Nand_Select();
	while(1){
		page = addr / PAGE_SIZE;//第几页
		
		col = addr % PAGE_SIZE;//第几列
		
		printf("page = %d; col = %d\n\r", page, col);
		
		Nand_CMD(0x80);
		
		//发出地址
		Nand_Addr_Byte(col & 0xff);
		Nand_Addr_Byte(page & 0xff);
		Nand_Addr_Byte((page>>8) & 0xff);
		Nand_Addr_Byte((page>>16) & 0xff);
		
		//写数据 连续写
		for(;(col < PAGE_SIZE) && (i < len); addr++){
			//buf[i++] = Nand_Data();
			Nand_WriteData(buf[i++]);
		}
		Nand_CMD(0x10);
		Nand_Wait_Ready();
		if(i == len)
			break;		
	}
	Nand_Deselect();
	
	return 0;
}

void Nand_Write_Test(){
	unsigned int addr;
	unsigned char str[100];
	int r;
	
	//获得地址
	printf("Enter the address of sector to write: ");
	addr = get_uint();
	
	printf("Enter the string to write: ");
	gets(str);

	printf("writing ...\n\r");	
	
	r = Nand_Write(addr, str, strlen(str)+1);
	if(r == 0)
		printf("write success!\n\r");
	else
		printf("write failed!\n\r");
	printf("=================\n\r");
}

//Nand -> SDRAM 复制函数
void Nand_Copy_to_SDRAM(unsigned int source_addr, unsigned char * dst_addr, int len){
	unsigned char buf[64];
	int i;
	int index = 0;
	
	while(1){
		Nand_Read(source_addr, buf, 64);
		
		for(i=0; i<64; i++){
			*dst_addr = buf[i];
			dst_addr++;
			source_addr++;
			index++;
		}
		
		if(index > len)
			break;
	}
} 