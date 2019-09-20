#include "uart.h"
#include "nandflash.h"
#include "lcd.h"

void delay(int tt);

char g_char1 = 'A';
char g_char2 = 'a';
int g_A = 0;

int main(){
	char c;
	
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
	}
	
	return 0;
}

void delay(int tt){
	int a,b;
	for(a=0;a<=tt;a++)
	for(b=0;b<=100;b++);
}