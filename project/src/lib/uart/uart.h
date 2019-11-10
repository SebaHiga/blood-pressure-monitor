#ifndef __UART_H__
#define __UART_H__

#include "chip.h"
#include "string.h"
#include <stdio.h>
#include <stdarg.h>

/*UART0*/
#define UART0_PORT		(0)
#define UART0_TXD_PIN	(2)
#define UART0_RXD_PIN	(3)
#define UART0_BAUDRATE	(115200)

void UART_init(void);
void UART_send_string(char*);
void UART_printf(const char*, ...);

#endif
