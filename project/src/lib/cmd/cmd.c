/*
 * cmd.c
 *
 *  Created on: Nov 17, 2019
 *      Author: seba
 */

#include "cmd.h"

static const char *commands_str[] = {
    "start",
    "stop"
};

cmd_callback commands_fp[] = {
    CMD_start,
    CMD_stop
};

void CMD_parse(const char* str){
    int argc = 0;
    char argv[CMD_MAX_ARGS][CMD_STRLEN_ARGS]; //hardcoded max of 5 arguments check header

    char tmp[255];
    int len = strlen(str);

    strcpy(tmp, str);

    log_printf(__func__, debug0, "Received command: %s\n", tmp);

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

            log_printf(__func__, debug0, "Parsed argument %d: %s\n", argc, argv[argc - 1]);
        }
    }

    int amount_commands = VECT_SIZE(commands_str);

    log_printf(__func__, debug0, "Amount of commands: %d\n", amount_commands);
    
    for (int i = 0; i < amount_commands; i++)
    {
        if(EQUAL_STRINGS(tmp, commands_str[i])){
            commands_fp[i](argc, argv);
        }
    }
}

void CMD_start(int argc, char argv[CMD_MAX_ARGS][CMD_STRLEN_ARGS]){
    log_printf(__func__, debug0, "In command start\n");
 
}

void CMD_stop(int argc, char argv[CMD_MAX_ARGS][CMD_STRLEN_ARGS]){
    log_printf(__func__, debug0, "In command stop\n");


}