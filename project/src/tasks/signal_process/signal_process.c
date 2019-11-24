#include "signal_process.h"
#include "main.h"

extern handler_t handler;

void Task_SignalProcess(void){
    switch (handler.sp.status){
        case idle:{
            if(handler.sp.debug){
            }
        }break;

        case measuring:{

        }break;

        case end:{

        }break;

        default:{

        }
    }
}

float Convert2mmHg (int value){
  return ((value* 0.0065975421819632896) + 12.737942826257036);
}




