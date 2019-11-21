#ifndef __LOGGER_H__
#define __LOGGER_H__

#include <uart.h>

typedef enum {
    mute = 0,
    error,
    info,
    debug0,
    debug1,
    debug2,
    debug3,
    debug4,
    debug5,
    debug6,
    debug7,
    debug8,
    debug9
}log_level_t;

void log_setLevel(int);
void log_printf(const char*, int, const char*, ...);
int log_getLevel(void);

#endif
