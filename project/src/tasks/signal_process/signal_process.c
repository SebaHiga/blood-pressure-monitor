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

                if(systolic > 0 && diastolic > 0){
                    LCD_printf(row1, "SYS: %d", systolic);
                    LCD_printf(row2, "DIA: %d", diastolic);

                    _log(info, "Systolic pressure:\t%d", systolic);
                    _log(info, "Diastolic pressure:\t%d", diastolic);
                }
                else{
                    LCD_printf(row1, "ERROR");

                    _log_smpl(error, "Error analizing diastolic/systolic pressures, please read tech manual");
                }
            }
        }break;

        case end:{
            if(pressure > TOP_PRESSURE){
                _log_smpl(info, "Starting measuring pressure");
            	handler.sp.status = measuring;
                
                pulse_record.index = 0;
                memset(pulse_record.height, 0, HEIGHT_LEN_VECT);
            }
        }break;

        default:{
            _log_smpl(error, "Error in state");
        }
    }
}

void analyzeRecords(void){
    int sys, dia;
    int map;

    // removeWeird();
    smoothPulse();

    map = findMAP();

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
    static int len = 0;
    static pulse_state_t state = rising;

    static sp_handler_t *sp;
    static adc_handler_t *adc;
    static pulse_parameters_t *pulse_param;

    sp = &handler.sp;
    adc = &handler.adc;
    pulse_param = &handler.sp.pulse_param;

    if(val >= MAX_VAL) state = rising;

    switch(state){
        case rising:{
            //comienza a crecer
            if(val > sp->pulse_param.upper){
                max = 0;
                min = MAX_VAL;
                state = middle;
                len = 0;
            }
        }break;

        case middle:{
            len++;
            LED_ON;

            //busco el maximo
            if(val > max){
                max = val;
            }
            if(val < sp->pulse_param.middle){
                state = falling;
            }
        }break;

        case falling:{
            len++;

            //busco el minimo
            if(val < min){
                min = val;
            }

            if(val > pulse_param->fall){
                int height = max - min;

                LED_OFF;
                if(height < pulse_param->max_height 
                    && len > pulse_param->min_lenght){

                    //print to lcd
                    LCD_InstantPressure((int) Convert2mmHg(adc->lowpass));

                    //save into vect
                    if (pulse_record.index < HEIGHT_LEN_VECT){
                        _log(info, "Pulse %d\tHeight %d", pulse_record.index, height, len);
                        _log(debug4, "Max %d\tMin %d\tLenght %d", max, min);

                        pulse_record.height[pulse_record.index] = height;
                        pulse_record.pressure[pulse_record.index] = adc->lowpass;
                        pulse_record.index++;
                    }   
                    state = rising;
                }
            }

            if(len > 1500) state = rising;
        }break;
    }

}

float Convert2mmHg (int value){
  return ((value * 0.101681638333) - 7.11367814086);
}

#define TOLERANCE    0.2
#define HEIGHT_TOLERANCE(val1, val2, tol) (float)val1 > (float)val2*(float)(1+tol) || (float)val1 < (float)val2*(float)(1-tol)


void removeWeird(void){

    for(int i = 0; i < pulse_record.index; i++){                  
        if(HEIGHT_TOLERANCE(pulse_record.height[i], pulse_record.height[i+1], TOLERANCE)){
             removePulse(i + 1);
        }
    }

}

void removePulse(int index){
    int amount = pulse_record.index - index - 1;

    _log(debug5, "Removing pulse %d", index);


    pulse_record.index--;

    memcpy(pulse_record.height + index, pulse_record.height + index + 1, amount);
    memcpy(pulse_record.pressure + index, pulse_record.pressure + index + 1, amount);

    for(int i = 0; i < pulse_record.index; i++){
        _log(debug7, "Pulse %d\t Height %d", i, pulse_record.height[i]);
    }
}

void smoothPulse(void){
    for(int i = 0; i < 40; i++){
        smoothenFilter(pulse_record.height[0]);
    }

    for(int i = 0; i < pulse_record.index; i++){
        pulse_record.height[i] = smoothenFilter(pulse_record.height[i]);
        _log(debug1, "Smooth pressure height: %d", pulse_record.height[i]);
    }
}

uint16_t smoothenFilter (uint16_t data)
{
  uint16_t i;
  static uint16_t x[orderFilterLP] = {0};
  static uint16_t y[orderFilterLP] = {0};
  
  const float b [] = {
       0.434739348285,  0.869478696569,  0.434739348285};
       
  const float a [] = {
       1.            ,  0.519303409225,  0.219653983914};
  
  uint16_t filtered;

  filtered =  b[0] * (float)data + b[1] * x[0] + b[2] * x[1]//
                                 - a[1] * y[0] - a[2] * y[1];//

  if(filtered > 32767) filtered = 450;  

  for(i = orderFilterLP-1; i!=0; i--)
  {
    x[i] = x[i-1];
  }
  x[0]=(float)data;

  for(i = orderFilterLP-1; i!=0; i--)
  {
    y[i] = y[i-1];
  }
  y[0]=filtered;

  return filtered;
}