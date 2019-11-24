#ifndef __SIGNAL_PROCESS_H__
#define __SIGNAL_PROCESS_H__

struct sp_status_t{
    enum sig_process_status_t{
        offset = 0,
        idle,
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

float Convert2mmHg (uint16_t value);
void Task_SignalProcess(void);

#endif