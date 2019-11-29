#ifndef __DISPLAY_H__
#define __DISPLAY_H__

#include "tasks.h"
#include "utils.h"

#define DISPLAY_DELAY 500;

typedef struct{
    int delay;
}display_handler_t;

void Task_display(void);

#endif