#ifndef __LOGGER_H__
#define __LOGGER_H__

#include <uart.h>

#define _log(level, format, ...) log_printf(__func__, level, format,__VA_ARGS__)
#define _log_smpl(level, format) log_printf(__func__, level, format)


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
