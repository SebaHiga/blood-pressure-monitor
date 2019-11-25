#ifndef __SIGNAL_PROCESS_H__
#define __SIGNAL_PROCESS_H__

#include "tasks.h"

#define   MAX_PRESSURE   25341

#define   UPPER_VAL         200
#define   MIDDLE_VAL        0
#define   FALL_VAL          50         
#define   MAX_VAL           4000
#define   MIN_VAL           -4000
#define   MAX_HEIGHT        3000
#define   MIN_LENGHT        4

enum sig_process_status_t{
    idle = 0,
    measuring,
    end
};

typedef enum{
    rising = 0,
    middle,
    falling
}pulse_state_t;

typedef struct{
    uint16_t upper;
    uint16_t middle;
    uint16_t fall;
    uint16_t max_height;
    uint16_t min_lenght;
}pulse_parameters_t;

typedef struct{
    enum sig_process_status_t status;
    pulse_parameters_t pulse_param;
    int offset;
    int sys;
    int debug;
} sp_handler_t;

float Convert2mmHg (int value);
void processPulse(int value);
void Task_SignalProcess(void);

#endif
