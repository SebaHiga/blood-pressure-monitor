/*
 * cmd.h
 *
 *  Created on: Nov 17, 2019
 *      Author: seba
 */

#ifndef LIB_CMD_CMD_H_
#define LIB_CMD_CMD_H_

#include "chip.h"
#include "string.h"
#include "logger.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "logger.h"
#include "main.h"

#define CMD_MAX_ARGS    5
#define CMD_STRLEN_ARGS 20 

//cantidad de strings en array de strings
#define N_ELEMENTS(vect) sizeof(vect) / sizeof(vect[0])
//si son iguales manda un 1
#define EQUAL_STRINGS(str1, str2) strcmp(str1, str2) == 0

typedef void (*cmd_callback) (int, char[CMD_MAX_ARGS][CMD_STRLEN_ARGS]);

void CMD_parse(const char*);

void CMD_adc(int, char[CMD_MAX_ARGS][CMD_STRLEN_ARGS]);
void CMD_logger(int, char[CMD_MAX_ARGS][CMD_STRLEN_ARGS]);

#endif /* LIB_CMD_CMD_H_ */
