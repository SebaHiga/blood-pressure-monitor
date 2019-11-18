/*
 * cmd.c
 *
 *  Created on: Nov 17, 2019
 *      Author: seba
 */

#include "cmd.h"

extern handler_t handler;


static const char *commands_str[] = {
    "adc",
    "logger"
};

cmd_callback commands_fp[] = {
    CMD_adc,
    CMD_logger
};

void CMD_parse(const char* str){
    int argc = 0;
    char argv[CMD_MAX_ARGS][CMD_STRLEN_ARGS]; //hardcoded max of 5 arguments check header

    char tmp[255];
    int len = strlen(str);

    strcpy(tmp, str);

    log_printf(__func__, debug0, "Received command: %s", tmp);

    for (uint8_t i = 0; i < len; i++)   //replace al spaces with null
    {
        if(tmp[i] == ' '){
            tmp[i] = '\0';
        }
    }

    for (uint8_t i = 0; i < len; i++)   //save arguments into argv
    {
        if(tmp[i] == '\0'){
            strcpy(argv[argc], tmp + i + 1);
            argc++;
            i += strlen(tmp + i);

            log_printf(__func__, debug0, "Parsed argument %d: %s", argc, argv[argc - 1]);
        }
    }

    int amount_commands = VECT_SIZE(commands_str);
    
    for (int i = 0; i < amount_commands; i++)
    {
        if(EQUAL_STRINGS(tmp, commands_str[i])){
            commands_fp[i](argc, argv);
        }
    }
}

/*
    adc start -> starts converting
    adc stop  -> stops converting
    adc delay 1000 -> sets adc's delay to 1000ms
*/
void CMD_adc(int argc, char argv[CMD_MAX_ARGS][CMD_STRLEN_ARGS]){
    log_printf(__func__, debug0, "In command adc");

    if(argc == 0) return;

    if(EQUAL_STRINGS(argv[0], "start")){
        Chip_ADC_SetStartMode(LPC_ADC,ADC_START_NOW,ADC_TRIGGERMODE_RISING);
        handler.adc_start = 1;
        Chip_GPIO_SetPinOutHigh(LPC_GPIO,LED_ROJO_PORT,LED_ROJO_PIN);
    }
    else if(EQUAL_STRINGS(argv[0], "stop")){
        handler.adc_start = 0;
        Chip_GPIO_SetPinOutLow(LPC_GPIO,LED_ROJO_PORT,LED_ROJO_PIN);
    }
    else if(EQUAL_STRINGS(argv[0], "delay")){
        if(argc > 1){
            handler.adc_delay = atoi(argv[1]);
        }
    }
}

void CMD_logger(int argc, char argv[CMD_MAX_ARGS][CMD_STRLEN_ARGS]){
    log_printf(__func__, debug0, "In command logger");

    if(argc == 0) return;

    if(EQUAL_STRINGS(argv[0], "level")){
        if(argc < 1) return;
        log_setLevel(atoi(argv[1]));
    }
}