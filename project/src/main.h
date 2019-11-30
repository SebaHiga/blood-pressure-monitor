#ifndef __MAIN_H__
#define __MAIN_H__


#include "chip.h"
#include "string.h"
#include "board.h"
#include "uart.h"
#include "logger.h"
#include <stdio.h>
#include <stdarg.h>
#include "main.h"
#include "cmd.h"
#include "utils.h"
#include "filter.h"
#include "lcd.h"

#include <cr_section_macros.h>

#include "tasks.h"

typedef struct{
	adc_handler_t adc;
	sp_handler_t sp;
	display_handler_t display;
}handler_t;

#define DELAY(ticks)			\
{								\
	static int _count = 0;		\
	if(_count){					\
		_count--;				\
		return;					\
	}							\
	else {						\
		_count = ticks;			\
	}							\
}								\

#define TIC_1MS 		(1000)
#define TIC_LED_MS		(250)
#define LED_ROJO_PORT	(0)
#define LED_ROJO_PIN	(22)
#define UART1_PORT		(0)
#define UART1_TXD_PIN	(15)
#define UART1_RXD_PIN	(16)
#define UART1_BAUDRATE	(57600)
#define MSG_LEN			(11)

#define LED_VERDE_PORT	(2)
#define LED_VERDE_PIN	(12)


void InitHardware(void);

void TareaLeeSerie(void);

#endif
