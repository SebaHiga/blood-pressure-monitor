#ifndef __ADC_H__
#define __ADC_H__

#include "chip.h"
#include "logger.h"

typedef struct{
	int start;
	int delay;
    int pressure;
	int debug;
	char debug_mode[16];
	int new_val;

	int highpass;
	uint16_t lowpass;
}adc_handler_t;


/*ADC*/
#define ADC0_PORT		(0)
#define ADC0_PIN		(23)
#define ADC1_PORT		(0)
#define ADC1_PIN		(24)

#define	ADC_VECT_TOTAL	12
#define ADC_VECT_NOTAKE	3
#define	ADC_FREQ_500Hz	2

void Task_ADC(void);
int ADC_read(enum CHIP_ADC_CHANNEL adc);

#endif
