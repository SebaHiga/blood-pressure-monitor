#include "display.h"
#include "main.h"


extern handler_t handler;

void Task_display(void){
    delay(handler.display.delay);

    if(handler.sp.status == idle){
        LCD_InstantPressure(Convert2mmHg(handler.adc.lowpass));
    }
    else if(handler.sp.status == measuring){
        static int i = 0;

        switch(i){
            case 0:{
                LCD_printf(row1, "Please wait");
            }break;
            case 1:{
                LCD_printf(row1, "Please wait.");
            }break;
            case 2:{
                LCD_printf(row1, "Please wait..");
            }break;
            case 3:{
                LCD_printf(row1, "Please wait...");
            }break;

        }

        i++;
        if(i > 3) {i = 0;}
    }
}