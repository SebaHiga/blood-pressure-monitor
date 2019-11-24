#include "adc.h"
#include "main.h"

extern handler_t handler;

void Task_ADC(void)
{
	delay(handler.adc.delay);
	_log(debug5, "ADC flag: %d", handler.adc.start);

	if(handler.adc.start){
        handler.adc.lowpass = ADC_read(ADC_CH0);
        handler.adc.highpass = ADC_read(ADC_CH1);

        //low pass filtered
		if(handler.adc.debug){
			UART_printf("%d, %d, %d\r\n", handler.adc.highpass, handler.adc.lowpass, handler.adc.pressure);
		}
	}
}

uint16_t ADC_read(enum CHIP_ADC_CHANNEL adc){
	uint16_t val = 0;
	uint16_t samples[ADC_VECT_TOTAL];
	uint16_t accumulate = 0;

    uint8_t i = 0;

    switch(adc){
    case ADC_CH0:
	Chip_ADC_EnableChannel(LPC_ADC, adc, ENABLE);
	Chip_ADC_EnableChannel(LPC_ADC, ADC_CH1, DISABLE);
	break;

    case ADC_CH1:
    	Chip_ADC_EnableChannel(LPC_ADC, adc, ENABLE);
    	Chip_ADC_EnableChannel(LPC_ADC, ADC_CH0, DISABLE);
    	break;
    }

    Chip_ADC_SetStartMode(LPC_ADC, ADC_START_NOW, ADC_TRIGGERMODE_RISING);

    //read till buffer is full
    while(i < ADC_VECT_TOTAL){
        if(Chip_ADC_ReadStatus(LPC_ADC, adc, ADC_DR_DONE_STAT)){
            Chip_ADC_ReadValue(LPC_ADC, adc, &val);

            samples[i] = val;

            i++;
            Chip_ADC_SetStartMode(LPC_ADC, ADC_START_NOW, ADC_TRIGGERMODE_RISING);
        }
    }

    //order buffer from MAX to min
    bubbleSort(samples, ADC_VECT_TOTAL);

    //accumulate values in the middle
    for (uint8_t i = ADC_VECT_NOTAKE; i < (ADC_VECT_TOTAL - ADC_VECT_NOTAKE); i++){
        accumulate += samples[i];
    }
    
    //mean value of accumulated
    val = accumulate / (ADC_VECT_TOTAL - ADC_VECT_NOTAKE*2);
    
    switch(adc){
        case ADC_CH0:{
            val = iirFilterLP(val);
            if(handler.sp.offset){
                handler.adc.new_val = 1;
                return val - handler.sp.offset;
            }
            else{
                handler.adc.new_val = 1;
                return val;	
            }
        }break;

        case ADC_CH1:{
            val = iirFilterHP(val);
            handler.adc.new_val = 1;
            return val;
        }break;
        
        default:
            return 0;
    }

    return 0;
}
