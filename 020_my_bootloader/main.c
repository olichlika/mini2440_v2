#include "uart.h"
#include "nandflash.h"
#include "lcd.h"
#include "setup.h"

void delay(int tt);

/* char g_char1 = 'A';
char g_char2 = 'a';
int g_A = 0 */;

static struct tag *params;

void setup_start_tag(void){
	params = (struct tag *)0x30000100;
 
	params->hdr.tag = ATAG_CORE;
	params->hdr.size = tag_size (tag_core);
 
	params->u.core.flags = 0;
	params->u.core.pagesize = 0;
	params->u.core.rootdev = 0;
 
	params = tag_next (params);
}

void setup_memory_tags(void){
	params->hdr.tag = ATAG_MEM;
	params->hdr.size = tag_size (tag_mem32);
	
	params->u.mem.start = 0x30000000;
	params->u.mem.size  = 64*1024*1024;
	
	params = tag_next (params);
}

int strlen_(char *str){
	int i = 0;
	while (str[i])
	{
		i++;
	}
	return i;
}
 
void strcpy(char *dest, char *src){
	while ((*dest++ = *src++) != '\0');
}
 
void setup_commandline_tag(char *cmdline){
	int len = strlen_(cmdline) + 1;
	
	params->hdr.tag  = ATAG_CMDLINE;
	params->hdr.size = (sizeof (struct tag_header) + len + 3) >> 2;
 
	strcpy (params->u.cmdline.cmdline, cmdline);
 
	params = tag_next (params);
}

void setup_end_tag(void){
	params->hdr.tag = ATAG_NONE;
	params->hdr.size = 0;
}



int main(){
	void (*theKernel)(int zero, int arch, unsigned int params);
	
	volatile unsigned char *p = (volatile unsigned char *)0x30008000;
	
	SendString("Copy kernel from nand\n\r");
	Nand_Copy_to_SDRAM(0x60000, 0x30008000, 0x300000);
	
	printf("0x%x\n\r", *p);
	printf("0x%x\n\r", *(p+1));
	printf("0x%x\n\r", *(p+2));
	printf("0x%x\n\r", *(p+3));
	
	printf("0x%x\n\r", *(p+32));
	printf("0x%x\n\r", *(p+33));
	printf("0x%x\n\r", *(p+34));
	printf("0x%x\n\r", *(p+35));
	
	printf("0x%x\n\r", *(p+36));
	printf("0x%x\n\r", *(p+37));
	printf("0x%x\n\r", *(p+38));
	printf("0x%x\n\r", *(p+39));

	printf("0x%x\n\r", *(p+0x1edbb0));
	printf("0x%x\n\r", *(p+0x1edbb1));
	printf("0x%x\n\r", *(p+0x1edbb2));
	printf("0x%x\n\r", *(p+0x1edbb3));	
	
	// printf("0x%x\n\r", *p++);
	// printf("0x%x\n\r", *p++);
	// printf("0x%x\n\r", *p++);
	// printf("0x%x\n\r", *p++);
	
	
	SendString("Set boot params\n\r");
	setup_start_tag();
	setup_memory_tags();
	setup_commandline_tag("noinitrd root=/dev/mtdblock3 init=/linuxrc console=ttySAC0");
	setup_end_tag();
	
	SendString("Boot kernel\n\r");
	theKernel = (void (*)(int, int, unsigned int))0x30008000;
	theKernel(0, 1999, 0x30000100); 
	
	SendString("Error!\n\r");
	
	return 0;
/* 	char c;
	
	Init_EINT();
	Init_TFT_LCD();
	TFT_Font_Test();
	//Init_Nand();
	
	//SendString("This is Mini2440!\n\r");
	
	// SendString("g_A = ");
	// PrintHex(g_A);
	// SendString("\n\r");
	
	// my_printf_test2();

	while(1){
		//打印菜单, 供我们选择测试内容
		printf("[s] Scan nand flash\n\r");
		printf("[e] Erase nand flash\n\r");
		printf("[w] Write nand flash\n\r");
		printf("[r] Read nand flash\n\r");
		printf("[l] Draw LCD\n\r");
		printf("[c] Clean LCD\n\r");
		printf("[q] quit\n\r");
		printf("Enter selection: ");
		
		c = ReciveChar();
		printf("%c\n\r", c);

		switch (c){
			case 'q':
			case 'Q':
				return;
				break;
				
			case 's':
			case 'S':
				Nand_Read_ID();
				break;

			case 'e':
			case 'E':
				Nand_Erase_Test();
				break;

			case 'w':
			case 'W':
				Nand_Write_Test();
				break;

			case 'r':
			case 'R':
				Nand_Read_Test();
				break;
				
			case 'l':
			case 'L':
				TFT_String_Test();
				break;

			case 'c':
			case 'C':
				Lcd_ClearScr(BG_COLOR);
				break;
				
			default:
				break;
		}
	} */
	
	//return 0;
}

void delay(int tt){
	int a,b;
	for(a=0;a<=tt;a++)
	for(b=0;b<=100;b++);
}