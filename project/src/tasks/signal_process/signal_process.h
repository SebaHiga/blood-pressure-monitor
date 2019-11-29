#ifndef __SIGNAL_PROCESS_H__
#define __SIGNAL_PROCESS_H__

#include "tasks.h"

#define HEIGHT_LEN_VECT 150

#define MAX_PRESSURE    25341

#define TOP_PRESSURE    220
#define BOTTOM_PRESSURE 90

#define UPPER_VAL       600
#define MIDDLE_VAL      0
#define FALL_VAL        550
#define MAX_VAL         4000
#define MIN_VAL         -4000
#define MAX_HEIGHT      3000
#define MIN_LENGHT      400

#define MAP_2_SYS_RATIO 0.55
#define MAP_2_DIA_RATIO 0.80

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

    float map2dia;
    float map2sys;
}pulse_parameters_t;

typedef struct{
    int index;
    uint16_t height[HEIGHT_LEN_VECT];
    uint16_t pressure[HEIGHT_LEN_VECT];
}pulse_record_t;

typedef struct{
    enum sig_process_status_t status;
    pulse_parameters_t pulse_param;
    int offset;
    int sys, dia;
    int debug;
} sp_handler_t;

float Convert2mmHg (int value);
void processPulse(int value);
void analyzeRecords(void);

int findMAP(void);
int findSYS(int);
int findDIA(int);

void Task_SignalProcess(void);

#endif
