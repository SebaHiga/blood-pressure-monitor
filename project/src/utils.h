#ifndef __UTILS_H__
#define __UTILS_H__

#include "logger.h"
#include "chip.h"


#define LED_ON  Chip_GPIO_SetPinOutHigh(LPC_GPIO,LED_ROJO_PORT,LED_ROJO_PIN)
#define LED_OFF Chip_GPIO_SetPinOutLow(LPC_GPIO,LED_ROJO_PORT,LED_ROJO_PIN);


void bubbleSort(uint16_t * samples, uint8_t total);


#endif