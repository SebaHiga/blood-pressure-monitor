#include <logger.h>

static log_level_t _level;

static const char *_level_str[] = {
		"[MUTE]",
		"[ERR]",
		"[DEB]",
		"[INF]"
};

void log_setLevel(int level){
    _level = level;
}

int log_getLevel(void){
    return _level;
}

void log_printf(const char* func_name, int level, const char* format, ...){
	va_list arguments;

	if(_level < level){
        return;
    } 

	char tmp[255] = {0};
    char str[512] = {0};
	
	va_start(arguments, format);

	vsprintf(tmp, format, arguments);
    sprintf(str, "%s %s\t- %s\r\n", _level_str[level], func_name, tmp);

	va_end(arguments);

	UART_send_string(str);
}
