#include "uart.h"
#include "string_utils.h"

#define NOR_FLASH_BASE  0x00000000


void nor_write_word(unsigned int base, unsigned int offset, char val){
	volatile char *p = (volatile char *)(base + (offset << 1));
	*p = val;
}
void nor_cmd(unsigned int offset, char cmd){
	nor_write_word(NOR_FLASH_BASE, offset, cmd);
}

void nor_cmd_16bit(unsigned int offset, unsigned int cmd){
	//nor_write_word(NOR_FLASH_BASE, offset, cmd);
	volatile unsigned short *p = (volatile unsigned short *)(NOR_FLASH_BASE + (offset << 1));
	*p = cmd;
}

unsigned short nor_read_word(unsigned int base, unsigned int offset){
	volatile unsigned short *p = (volatile unsigned short *)(base + (offset << 1));
	return *p;
}

unsigned short nor_dat(unsigned int offset){
	return nor_read_word(NOR_FLASH_BASE, offset);
}

void wait_ready(unsigned int addr){
	unsigned int val;
	unsigned int pre;

	pre = nor_dat(addr>>1);
	val = nor_dat(addr>>1);
	while ((val & (1<<6)) != (pre & (1<<6)))
	{
		pre = val;
		val = nor_dat(addr>>1);		
	}
}

void do_scan_nor_flash(){
	unsigned short m_id, d_id;
 	char str[4];
	unsigned int size;
	unsigned short sector_num, sector_size;
	unsigned short block_num, block_size;
	int i, j;
	unsigned int sector_addr; 
	
	//进入cfi模式
	nor_cmd(0x5555, 0xaa);  
	nor_cmd(0x2aaa, 0x55);
	nor_cmd(0x5555, 0x98);
	
	//打印string
	str[0] = nor_dat(0x10);
	str[1] = nor_dat(0x11);
	str[2] = nor_dat(0x12);
	str[3] = '\0';
	printf("str = %s\n\r", str);
	
	//打印容量
	size = 1 << (nor_dat(0x27));
	printf("nor size = 0x%x, %dM\n\r", size, size/(1024*1024));
	
	//读sectors数量
	sector_num = ((nor_dat(0x2e) << 8) | nor_dat(0x2d)) + 1;
	printf("sector_num = %d\n\r", sector_num);
	//读sectors大小
	sector_size = ((nor_dat(0x30) << 8) | nor_dat(0x2f)) * 256 / 1024;
	printf("sector_size = %dKByte/sector\n\r", sector_size);
	
	//读block数量
	block_num = ((nor_dat(0x32) << 8) | nor_dat(0x31)) + 1;
	printf("block_num = %d\n\r", block_num);
	//读block大小
	block_size = ((nor_dat(0x34) << 8) | nor_dat(0x33)) * 256 / 1024;
	printf("block_size = %dKByte/block\n\r", block_size);
	
	
	nor_cmd(0x0, 0xf0);//退出
	
	//打印厂家ID、设备ID
	nor_cmd(0x5555, 0xaa);
	nor_cmd(0x2aaa, 0x55);
	nor_cmd(0x5555, 0x90);
	m_id = nor_dat(0x00);
	d_id = nor_dat(0x01);
	nor_cmd(0x0, 0xf0);//退出
	
	printf("Manufacturer ID = %x\n\r", m_id);
	printf("Device ID = %x\n\r", d_id);
	
	//打印所有sectors的起始地址
	printf("Sectors : \n\r");
	sector_addr = 0;
	for(i=0; i<64; i++){
		for(j=0; j<8; j++){
			//printf("%x ", *p);
			//p++;//指向下一个数据
			printf("%08x ", sector_addr);
			sector_addr = sector_addr + 0x1000;
		}
		printf("\n\r");
	}
	
/* 	printf("0 = %x\n\r", *(volatile char *)0);
	printf("1 = %x\n\r", *(volatile char *)1);
	printf("2 = %x\n\r", *(volatile char *)2);
	printf("3 = %x\n\r", *(volatile char *)3);
	printf("4 = %x\n\r", *(volatile char *)4);
	printf("5 = %x\n\r", *(volatile char *)5);
	printf("6 = %x\n\r", *(volatile char *)6);
	printf("7 = %x\n\r", *(volatile char *)7); */
	printf("=================\n\r");
}

//扇区擦除
void do_erase_nor_flash(){ 
	unsigned int addr;
	printf("Enter the sector address to erase: ");
	addr = get_uint();
	
	printf("erasing ...\n\r");
	
	nor_cmd(0x5555, 0xaa);  
	nor_cmd(0x2aaa, 0x55);
	nor_cmd(0x5555, 0x80);
	nor_cmd(0x5555, 0xaa); 
	nor_cmd(0x2aaa, 0x55);
	nor_cmd(addr >> 1, 0x30);//扇区擦除 右移1位 抵消nor_cmd里面的左移
	
	wait_ready(addr);//等待擦除结束
	printf("erase success!\n\r");
	printf("=================\n\r");
}

//word写入(16bit)
void do_write_nor_flash(){
	unsigned int addr;
	int i, j;
	unsigned char str[100];
	unsigned int val;
	
	//获得地址
	printf("Enter the address of sector to write: ");
	addr = get_uint();
	
	printf("Enter the string to write: ");
	gets(str);

	printf("writing ...\n\r");
	
	i = 0;
	j = 1;
	while(str[i] && str[j]){
		val = str[i] + (str[j] << 8);//拼接两个字节写入
		
		nor_cmd(0x5555, 0xaa);  
		nor_cmd(0x2aaa, 0x55);
		nor_cmd(0x5555, 0xa0);
		nor_cmd_16bit(addr>>1, val);
		
		wait_ready(addr);
		
		i += 2;
		j += 2;
		addr += 2;
	}	
	
	val = str[i];//写最后一个字节

	nor_cmd(0x5555, 0xaa);  
	nor_cmd(0x2aaa, 0x55);
	nor_cmd(0x5555, 0xa0);
	
	nor_cmd_16bit(addr>>1, val);

	wait_ready(addr);

	printf("write success!\n\r");
	printf("=================\n\r");
}

//从指定地址读4096字节数据
void do_read_nor_flash(){
	unsigned int addr;
	volatile char * p;
	int i, j;
	unsigned char str[16];
	
	printf("Enter the address to read: ");
	addr = get_uint();
	
	p = (volatile char *)addr;
	printf("Data : \n\r");
	
	for(i=0; i<256; i++){
		for(j=0; j<16; j++){
			printf("%02x ", *p);
			str[j] = *p;
			p++;//指向下一个数据
		}
		printf("    ");
		
		//输出ascii字符
		for (j=0; j<16; j++){
			/* 后打印字符 */
			if (str[j] < 0x20 || str[j] > 0x7e)  /* 不可视字符 */
				SendChar('.');
			else
				SendChar(str[j]);
		}
		
		printf("\n\r");
	}
	printf("=================\n\r");
}

void delay2(int tt){
	int a,b;
	for(a=0;a<=tt;a++)
	for(b=0;b<=100;b++);
}