#include "uart.h"


void UART_init(void){
	Chip_IOCON_PinMuxSet(LPC_IOCON,UART0_PORT,UART0_TXD_PIN,IOCON_FUNC1);
	Chip_IOCON_PinMuxSet(LPC_IOCON,UART0_PORT,UART0_RXD_PIN,IOCON_FUNC1);
	Chip_UART_Init(LPC_UART0);
	Chip_UART_SetBaud(LPC_UART0,UART0_BAUDRATE);
	Chip_UART_ConfigData(LPC_UART0, (UART_LCR_WLEN8 | UART_LCR_SBS_1BIT));
	Chip_UART_SetupFIFOS(LPC_UART0, (UART_FCR_FIFO_EN | UART_FCR_TRG_LEV3));
	Chip_UART_TXEnable(LPC_UART0);
}

void UART_printf(const char* format, ...){
	va_list arguments;
	char tmp[255] = {0};
	
	va_start(arguments, format);

	vsprintf(tmp, format, arguments);

	va_end(arguments);

	UART_send_string(tmp);
}


void UART_send_string(char *c){
	int len = strlen(c);

	//ojo tarea bloqueante escondeeeer
	Chip_UART_SendBlocking(LPC_UART0, c, len);

}
