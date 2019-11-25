#ifndef __LCD_H__
#define __LCD_H__

#include "chip.h"
#include "string.h"
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

/*PINES LCD*/
#define LCD_E_PORT	            (0)
#define LCD_E_PIN	            (4)
#define LCD_D4_PORT	            (0)
#define LCD_D4_PIN	            (5)
#define LCD_D5_PORT	            (0)
#define LCD_D5_PIN	            (10)
#define LCD_D6_PORT	            (2)
#define LCD_D6_PIN	            (4)
#define LCD_D7_PORT	            (2)
#define LCD_D7_PIN	            (5)
#define LCD_RST_PORT	        (2)
#define LCD_RST_PIN	            (6)

#define	LCD_CLEAR   			0x01

#define	RENGLON_1   			0
#define	RENGLON_2   			1

void LCD_init (void);
void LCD_WaitLong(uint16_t ciclos);
void LCD_WaitShort(uint8_t ciclos);
void LCD_latch(void);
void LCD_WComando4(uint8_t comando);
void LCD_WComando8(uint8_t comando);
void LCD_WDato(uint8_t comando);
void LCD_WString (uint8_t *ptr);
void LCD_printf(int row, const char *format, ...);

void Display_lcd( char *msg , char r , char p );
void LCD_InstantPressure (int value);

#endif
