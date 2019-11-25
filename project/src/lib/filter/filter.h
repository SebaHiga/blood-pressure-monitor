#ifndef __FILTER_H__
#define __FILTER_H__

#include "chip.h"

#define orderFilterLP 2

uint16_t iirFilterLP (uint16_t data);
int iirFilterHP (uint16_t data);

#endif
