#ifndef _LCD_CONTROLLER_H
#define _LCD_CONTROLLER_H

#include "lcd.h"

typedef struct lcd_controller {
	void (*init)(p_lcd_params plcdparams);
	void (*enable)(void);
	void (*disable)(void);
}lcd_controller, *p_lcd_controller;

#endif /* _LCD_CONTROLLER_H */

