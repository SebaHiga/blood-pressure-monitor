#ifndef __FILTER_H__
#define __FILTER_H__

#include "chip.h"

#define orderFilterLP 2

int iirFilterLP (int data);
int iirFilterHP (int data);

#endif
