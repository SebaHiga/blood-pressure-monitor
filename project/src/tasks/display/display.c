#include "display.h"
#include "main.h"


extern handler_t handler;

void Task_display(void){

    display_handler_t *display;

    display = &handler.display;

    DELAY(display->delay);

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

        LCD_InstantPressure(Convert2mmHg(handler.adc.lowpass));
    }
    else if(handler.sp.status == end){
        static int i = 20; //10 seconds

        if(i){
            i--;
        }
        else{
            LCD_printf(row1, "Ready");
            handler.sp.status = idle;
        }
    }
}