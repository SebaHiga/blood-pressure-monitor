#include "display.h"
#include "main.h"


extern handler_t handler;

void Task_display(void){

    display_handler_t *display;
    sp_handler_t *sp;
    adc_handler_t *adc;
    static int i; //10 seconds

    display = &handler.display;
    sp = &handler.sp;
    adc = &handler.adc;

    DELAY(display->delay);

    if(sp->status == idle){
        LCD_InstantPressure(Convert2mmHg(adc->lowpass));
    }
    else if(sp->status == measuring){
        static int j = 0;

        switch(j){
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

        j++;
        if(j > 3) {j = 0;}

        i = 20;

        LCD_InstantPressure(Convert2mmHg(adc->lowpass));
    }
    else if(sp->status == end){

        if(i){
            i--;
        }
        else{
            LCD_printf(row1, "Ready");
            LCD_InstantPressure(Convert2mmHg(adc->lowpass));
            sp->status = idle;
        }
    }
    else if(sp->status == wtf){
        static int i = 20;
        static bool flag = false;

        if(flag){
            LCD_printf(row1, "ERROR");
            LCD_printf(row2, " ");
            flag = false;
        }
        else{
            LCD_printf(row1, " ");
            flag = true;
        }

        if(i){
            i--;
        }
        else{
            LCD_printf(row1, "Ready");
            LCD_InstantPressure(Convert2mmHg(adc->lowpass));
            sp->status = idle;
        }
    }
}