#ifndef __LOGGER_H__
#define __LOGGER_H__

#include <uart.h>

typedef enum {
    mute = 0,
    error,
    info,
    debug0,
    debug1
}log_level_t;

void log_setLevel(int);
void log_printf(const char*, int, const char*, ...);
int log_getLevel(void);

#endif
