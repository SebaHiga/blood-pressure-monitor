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
    "logger",
    "pulse",
    "lcd"
};

cmd_callback commands_fp[] = {
    CMD_adc,
    CMD_logger,
    CMD_pulse,
    CMD_lcd
};

void CMD_parse(const char* str){
    int argc = 0;
    char argv[CMD_MAX_ARGS][CMD_STRLEN_ARGS]; //hardcoded max of 5 arguments check header

    char tmp[255];
    int len = strlen(str);

    strcpy(tmp, str);

    _log(debug0, "Received command: %s", tmp);

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

            _log(debug1, "Parsed argument %d: %s", argc, argv[argc - 1]);
        }
    }

    int amount_commands = N_ELEMENTS(commands_str);
    
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
    _log_smpl(debug0, "In command adc");

    if(argc == 0) return;

    if(EQUAL_STRINGS(argv[0], "start")){
        handler.adc.start = 1;
        Chip_GPIO_SetPinOutHigh(LPC_GPIO,LED_ROJO_PORT,LED_ROJO_PIN);
    }
    else if(EQUAL_STRINGS(argv[0], "stop")){
        handler.adc.start = 0;
        Chip_GPIO_SetPinOutLow(LPC_GPIO,LED_ROJO_PORT,LED_ROJO_PIN);
    }
    else if(EQUAL_STRINGS(argv[0], "delay")){
        if(argc < 1){
            _log_smpl(error, "No value for delay\n");
            return;
        }

        //error prone
        handler.adc.delay = atoi(argv[1]);
    }
    else if(EQUAL_STRINGS(argv[0], "debug")){
        if(argc < 1){
            _log_smpl(error, "Please use on/off as arguments");
            return;
        }

        if(EQUAL_STRINGS(argv[1], "on")){
            handler.adc.debug = 1;
        }
        else{
            handler.adc.debug = 0;
        }
    }

    else if(EQUAL_STRINGS(argv[0], "pressure")){
        if(argc < 1){
            _log_smpl(error, "No value for pressure\n");
            return;
        }

        //error prone
        handler.adc.pressure = atoi(argv[1]);
    }
    else{
        _log(error, "Error in argument %s\n", argv[0]);
    }
}

/*
    logger level 1 -> sets logger's level to 1 (error)
*/
void CMD_logger(int argc, char argv[CMD_MAX_ARGS][CMD_STRLEN_ARGS]){
    _log_smpl(debug0, "In command logger");

    if(argc == 0) return;

    if(EQUAL_STRINGS(argv[0], "level")){
        if(argc < 1) return;
        log_setLevel(atoi(argv[1]));
    }
}


/*
    pulse upper 100 -> sets upper to 100 in uint16_t values
    other possible parameters
        -middle
        -fall
        -height
        -lenght
*/
void CMD_pulse(int argc, char argv[CMD_MAX_ARGS][CMD_STRLEN_ARGS]){
    _log_smpl(debug0, "In command pulse");

    if(argc == 0) return;

    if(EQUAL_STRINGS(argv[0], "upper")){
        if(argc < 1) return;
        handler.sp.pulse_param.upper = atoi(argv[1]);
    }
    else if(EQUAL_STRINGS(argv[0], "middle")){
        if(argc < 1) return;
        handler.sp.pulse_param.middle = atoi(argv[1]);
    }
    else if(EQUAL_STRINGS(argv[0], "fall")){
        if(argc < 1) return;
        handler.sp.pulse_param.fall = atoi(argv[1]);
    }
    else if(EQUAL_STRINGS(argv[0], "height")){
        if(argc < 1) return;
        handler.sp.pulse_param.max_height = atoi(argv[1]);
    }
    else if(EQUAL_STRINGS(argv[0], "lenght")){
        if(argc < 1) return;
        handler.sp.pulse_param.min_lenght = atoi(argv[1]);
    }
    else if(EQUAL_STRINGS(argv[0], "map2dia")){
        if(argc < 1) return;
        handler.sp.pulse_param.map2dia = atoi(argv[1]);
    }
    else if(EQUAL_STRINGS(argv[0], "map2sys")){
        if(argc < 1) return;
        handler.sp.pulse_param.map2sys = atoi(argv[1]);
    }

    if(argc > 1){
        _log(debug1, "Setting %s to %d", argv[0], atoi(argv[1]));
    }
}

/*

    lcd print 1 hola mundo -> en la fila uno escribe "hola mundo"

*/
void CMD_lcd(int argc, char argv[CMD_MAX_ARGS][CMD_STRLEN_ARGS]){
    _log_smpl(debug0, "In command lcd");

    if(argc == 0) return;

    if(EQUAL_STRINGS(argv[0], "print")){
        if(argc < 3) return;

        int row;

        row = atoi(argv[1]) - 1;

        LCD_printf(row, "%s", argv[2]);

        _log(debug1, "Printing in lcd row%d: %s", row, argv[2]);
    }
}