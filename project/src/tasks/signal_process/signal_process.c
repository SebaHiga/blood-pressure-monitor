#include "signal_process.h"
#include "main.h"

extern handler_t handler;



void Task_SignalProcess(void){
    switch (handler.sp.status){
        case idle:{
            if(handler.sp.debug){
            }
            if(handler.adc.lowpass > 1000){
            	handler.sp.status = measuring;
            }
        }break;

        case measuring:{
            processPulse(handler.adc.highpass);
        }break;

        case end:{

        }break;

        default:{

        }
    }
}

int max, min;

void processPulse(int val){
    static pulse_state_t state = rising;
    static int len = 0;

    if(val >= MAX_VAL) state = rising;

    switch(state){
        case rising:{
            //comienza a crecer
            if(val > handler.sp.pulse_param.upper){
                _log_smpl(debug3, "Going to middle state");
                max = 0;
                min = MAX_VAL;
                state = middle;
                len = 0;
            }
        }break;

        case middle:{
            len++;

            //busco el maximo
            if(val > max){
                max = val;
            }

            if(val < handler.sp.pulse_param.middle){
                _log(debug1, "Middle value reached, maximum peak is %d", max);
                state = falling;
            }
        }break;

        case falling:{
            len++;

            //busco el minimo
            if(val < min){
                min = val;
            }

            if(val > handler.sp.pulse_param.fall){
                _log(debug1, "Fall value reached, minimum peak is %d", min);

                int height = max - min;

                if(height < handler.sp.pulse_param.max_height 
                    && len < handler.sp.pulse_param.min_lenght){

                    _log(info, "Pulse height is: %d", height);
                }

                state = rising;
            }
        }break;
    }

}

float Convert2mmHg (int value){
  return ((value* 0.0065975421819632896) + 12.737942826257036);
}




