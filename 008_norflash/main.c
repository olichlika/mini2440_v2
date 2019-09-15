#include "uart.h"
#include "norflash.h"

void delay(int tt);

char g_char1 = 'A';
char g_char2 = 'a';
int g_A = 0;

int main(){
	char c;
	
	Init_EINT();
	SendString("This is Mini2440!\n\r");
	
	SendString("g_A = ");
	PrintHex(g_A);
	SendString("\n\r");
	
	my_printf_test2();

	while(1){
		//打印菜单, 供我们选择测试内容
		printf("[s] Scan nor flash\n\r");
		printf("[e] Erase nor flash\n\r");
		printf("[w] Write nor flash\n\r");
		printf("[r] Read nor flash\n\r");
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
				do_scan_nor_flash();
				break;

			case 'e':
			case 'E':
				do_erase_nor_flash();
				break;

			case 'w':
			case 'W':
				do_write_nor_flash();
				break;

			case 'r':
			case 'R':
				do_read_nor_flash();
				break;
			default:
				break;
		}
	}
	
	return 0;
}

void delay(int tt){
	int a,b;
	for(a=0;a<=tt;a++)
	for(b=0;b<=100;b++);
}