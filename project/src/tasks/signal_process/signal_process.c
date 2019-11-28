#include "signal_process.h"
#include "main.h"

extern handler_t handler;

static pulse_record_t pulse_record;

void Task_SignalProcess(void){
    int pressure = Convert2mmHg(handler.adc.lowpass);
    
    switch (handler.sp.status){
        case idle:{

            if(pressure > TOP_PRESSURE){
                _log_smpl(info, "Starting measuring pressure");

            	handler.sp.status = measuring;

                pulse_record.index = 0;
                memset(pulse_record.height, 0, HEIGHT_LEN_VECT);
            }
        }break;

        case measuring:{
            processPulse(handler.adc.highpass);
            if(pressure < BOTTOM_PRESSURE){
                _log_smpl(info, "Finished measuring pressure");
                handler.sp.status = end;

                analyzeRecords();

                int systolic = Convert2mmHg(handler.sp.sys);
                int diastolic = Convert2mmHg(handler.sp.dia);

                LCD_printf(row1, "SYS: %d", systolic);
                LCD_printf(row2, "DIA: %d", diastolic);

                _log(info, "Systolic pressure:\t%d", systolic);
                _log(info, "Diastolic pressure:\t%d", diastolic);
            }
        }break;

        case end:{
            if(pressure > TOP_PRESSURE){
                _log_smpl(info, "Starting measuring pressure");
            	handler.sp.status = measuring;
                
                pulse_record.index = 0;
                memset(pulse_record.height, 0, HEIGHT_LEN_VECT);

                LCD_printf(row1, "Pressure");
                LCD_printf(row2, "");
            }
        }break;

        default:{
            _log_smpl(error, "Error in state");
        }
    }
}

void analyzeRecords(void){
    int sys, dia;
    int map = findMAP();

    _log(info, "Height of MAP is: %d", pulse_record.height[map]);

    sys = findSYS(map);
    dia = findDIA(map);

    if(sys > 0){
        handler.sp.sys = pulse_record.pressure[sys];
    }

    if(dia < pulse_record.index){
        handler.sp.dia = pulse_record.pressure[dia];
    }

}

int findMAP(void){
    int max;
    int index;

    max = 0;
    for (int i = 0; i < pulse_record.index + 1; i++){
        if(max < pulse_record.height[i]){
            max = pulse_record.height[i];
            index = i;
        }
    }
    
    return index;
}

/*
    it return 0 when not found
*/
int findSYS(int map){
    int map_height = pulse_record.height[map];
    float map2sys_ratio = handler.sp.pulse_param.map2sys;
    int index;

    for(index = map; index > 0; index--){
        if(map_height * map2sys_ratio > pulse_record.height[index]){
            _log(info, "Found SYS height in %d", pulse_record.height[index]);
            break;
        }
    }

    return index;
}

int findDIA(int map){
    int map_height = pulse_record.height[map];
    float map2dia_ratio = handler.sp.pulse_param.map2dia;
    int index;

    for(index = map; index > pulse_record.index + 1; index++){
        if(map_height * map2dia_ratio < pulse_record.height[index]){
            _log(info, "Found DIA height in %d", pulse_record.height[index]);
            break;
        }
    }

    return index;
}


void processPulse(int val){
    static int max, min;
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
                    && len > handler.sp.pulse_param.min_lenght){

                    //print to lcd
                    LCD_InstantPressure((int) Convert2mmHg(handler.adc.lowpass));

                    //save into vect
                    if (pulse_record.index < HEIGHT_LEN_VECT){
                        _log(info, "Pulse %d height is: %d", pulse_record.index, height);

                        pulse_record.height[pulse_record.index] = height;
                        pulse_record.pressure[pulse_record.index] = handler.adc.lowpass;
                        pulse_record.index++;
                    }   
                }

                state = rising;
            }
        }break;
    }

}

float Convert2mmHg (int value){
  return ((value * 0.101681638333) - 7.11367814086);
}




