#ifndef __SIGNAL_PROCESS_H__
#define __SIGNAL_PROCESS_H__

#include "tasks.h"

struct sp_status_t{
    enum sig_process_status_t{
        idle = 0,
        measuring,
        end
    } Type;
};

typedef struct{
    enum sig_process_status_t status;
    int offset;
    int sys;
    int debug;
} sp_handler_t;

float Convert2mmHg (int value);
void Task_SignalProcess(void);

#endif
