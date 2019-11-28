#include "display.h"
#include "main.h"


extern handler_t handler;

void Task_display(void){
    delay(handler.display.delay);

    LCD_InstantPressure(Convert2mmHg(handler.adc.lowpass));
}