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

#include <cr_section_macros.h>

typedef struct{
	int start;
	int delay;
    int pressure;
	int debug;
}adc_handler_t;

typedef struct{
	adc_handler_t adc;
}handler_t;

#define delay(ticks)			\
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

/*ADC*/
#define ADC0_PORT		(0)
#define ADC0_PIN		(23)

#endif
