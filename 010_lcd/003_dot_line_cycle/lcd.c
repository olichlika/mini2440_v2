#include "s3c2440.h"
//#include "lcd.h"

#define MVAL		(13)
#define MVAL_USED 	(0)		//0=each frame   1=rate by MVAL
#define INVVDEN		(1)		//0=normal       1=inverted
#define BSWP		(0)		//Byte swap control
#define HWSWP		(1)		//Half word swap control

#define M5D(n) ((n) & 0x1fffff)	// To get lower 21bits

//NEC35
#define LCD_XSIZE 	(240)	
#define LCD_YSIZE 	(320)

#define SCR_XSIZE 	(240)
#define SCR_YSIZE 	(320)

//NEC35
#define HOZVAL	(LCD_XSIZE-1)
#define LINEVAL	(LCD_YSIZE-1)

#define VBPD					(1)
#define VFPD					(5)
#define VSPW					(1)
#define HBPD					(36)
#define HFPD					(19)
#define HSPW					(5)

#define CLKVAL				(4)

#define BLACK 0x0000//黑色    0,   0,   0
#define NAVY 0x000F//深蓝色  0,   0, 127
#define DGREEN 0x03E0//深绿色  0,  127,  0
#define DCYAN 0x03EF//深青色  0,  127, 127       
#define MAROON 0x7800//深红色  127,   0,   0      
#define PURPLE 0x780F//紫色    127,   0, 127      
#define OLIVE 0x7BE0//橄榄绿  127, 127,   0      
#define LGRAY 0xC618//灰白色  192, 192, 192      
#define DGRAY 0x7BEF//深灰色  127, 127, 127      
#define BLUE 0x001F//蓝色    0,   0, 255        
#define GREEN 0x07E0//绿色    0, 255,   0        
#define CYAN 0x07FF//青色    0, 255, 255        
#define RED 0xF800//红色    255,   0,   0      
#define MAGENTA 0xF81F//品红    255,   0, 255      
#define YELLOW 0xFFE0//黄色    255, 255, 0        
#define WHITE 0xFFFF//白色    255, 255, 255 

volatile static unsigned short LCD_BUFFER[SCR_YSIZE][SCR_XSIZE];

void Lcd_Port_Init(void){
	GPCUP = 0xffffffff; // Disable Pull-up register
    GPCCON = 0xaaaa02a8; //Initialize VD[7:0],VM,VFRAME,VLINE,VCLK

    GPDUP = 0xffffffff; // Disable Pull-up register
    GPDCON = 0xaaaaaaaa; //Initialize VD[15:8]
}

static void LCD_Init(void){
	LCDCON1 = (CLKVAL << 8) | (1 << 7) | (3 << 5) | (12 << 1) | 0;
	LCDCON2 = (VBPD << 24) | (LINEVAL << 14) | (VFPD << 6) | (VSPW);
	LCDCON3 = (HBPD << 19) | (HOZVAL << 8) | (HFPD);
	LCDCON4 = (MVAL << 8) | (HSPW);
	LCDCON5 = (1 << 11) | (1 << 10) | (1 << 9) | (1 << 8) | (0 << 7) | (0 << 6)
             | (1 << 3)  |(BSWP << 1) | (HWSWP);
	LCDSADDR1 = (((unsigned int)LCD_BUFFER >> 22) << 21) | M5D((unsigned int)LCD_BUFFER >> 1);
	LCDSADDR2 = M5D( ((unsigned int)LCD_BUFFER + (SCR_XSIZE * LCD_YSIZE * 2)) >> 1);
	LCDSADDR3 = (((SCR_XSIZE-LCD_XSIZE) / 1) << 11)|(LCD_XSIZE / 1);
	LCDINTMSK |= (3); // MASK LCD Sub Interrupt
    TCONSEL &= ~((1 << 4) | 1); // Disable LCC3600, LPC3600
	TPAL = 0; // Disable Temp Palette
}

void Lcd_EnvidOnOff(int onoff){
    if(onoff == 1)
		LCDCON1 |= 1; // ENVID=ON
    else
		LCDCON1 = LCDCON1 & 0x3fffe; // ENVID Off
}

void Lcd_ClearScr(unsigned short c){
	unsigned int x,y ;
    for(y = 0; y < SCR_YSIZE; y++){
    	for(x = 0; x < SCR_XSIZE; x++){
			LCD_BUFFER[y][x] = c;
    	}
    }
}

void TFT_LCD_Test(){
	Lcd_Port_Init();
	LCD_Init();
	Lcd_EnvidOnOff(1);
	Lcd_ClearScr(0xFFFF);//clear screen白色
}

void Init_TFT_LCD(){
	Lcd_Port_Init();
	LCD_Init();
	Lcd_EnvidOnOff(1);
	Lcd_ClearScr(0xFFFF);//clear screen白色
}

void TFT_Draw_Pixel(int x, int y, unsigned short color){
	LCD_BUFFER[y][x] = color;
}

//***************以下是测试函数*****************
void TFT_Dot_Test(){
	TFT_Draw_Pixel(10, 10, RED);
	TFT_Draw_Pixel(20, 20, GREEN);
	TFT_Draw_Pixel(30, 30, BLUE);
}