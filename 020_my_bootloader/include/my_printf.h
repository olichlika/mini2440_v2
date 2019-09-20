
#ifndef _MY_PRINTF_H
#define _MY_PRINTF_H

#include "uart.h"
#define  __out_putchar  SendChar


#define  MAX_NUMBER_BYTES  64

extern int my_printf_test(void);
extern int my_printf_test2(void);
int printf(const char *fmt, ...);

#endif /* _MY_PRINTF_H */

