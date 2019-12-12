/*
===============================================================================
 Name        : AdcUart.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
*/

#include "main.h"

handler_t handler;

int main(void) {
    // InitHardware();

	SYSTEM_Init();
	
    log_setLevel(debug0);

	//Default parameters
	handler.adc.start = 1;
	handler.adc.delay = ADC_FREQ_500Hz;
	handler.adc.pressure = 0;
	handler.adc.debug = 0;
	handler.adc.new_val = 0;
	strcpy(handler.adc.debug_mode, "all");

	//signal process
	handler.sp.status = 0;
	handler.sp.offset = 0;
	handler.sp.status = idle;

	//pulse
	handler.sp.pulse_param.upper = UPPER_VAL;
	handler.sp.pulse_param.middle = MIDDLE_VAL;
	handler.sp.pulse_param.fall = FALL_VAL;
	handler.sp.pulse_param.min_lenght = MIN_LENGHT;
	handler.sp.pulse_param.max_height = MAX_HEIGHT;
	
	handler.sp.pulse_param.map2sys = MAP_2_SYS_RATIO;
	handler.sp.pulse_param.map2dia = MAP_2_DIA_RATIO;

	handler.display.delay = DISPLAY_DELAY;

	SCH_Start();

	LCD_printf(row1, "Ready");



    while(1) {
    	SCH_Dispatch_Tasks();
    }
    return 0 ;
}

void TareaLeeSerie(void)
{
	char byte = '\0';
	static char str[255] = {'\0'};
	static int index = 0;
	static bool sending = false;

	if(Chip_UART_ReadLineStatus(LPC_UART0)&UART_LSR_RDR)
	{
		//Start reading serial
		byte = Chip_UART_ReadByte(LPC_UART0);
		if(!sending){
			if(byte != '\n'){
				sending = true;
				index = 0;
				str[index] = byte;
			}
		}
		//Keep reading till \n
		else{
			index++;
			//Found stop of serial string
			if(byte == '\n'){
				str[index] = '\0';

				_log(debug3, "UART: %s\n", str);

				CMD_parse(str);

				memset(str, '\0', 255);
				sending = false;
				index = 0;
			}
			//Append byte on string
			else{
				str[index] = byte;
			}
		}
	}
}


void InitHardware(void)
{
	/*Inicializo el Clock del sistema*/
	// Chip_SetupXtalClocking();
	// SystemCoreClockUpdate();

	/*Inicializo los perifericos de pines e IOCON*/
	Chip_GPIO_Init(LPC_GPIO);
	Chip_IOCON_Init(LPC_IOCON);

	/*Inicializo el led rojo */
	Chip_IOCON_PinMuxSet(LPC_IOCON,LED_ROJO_PORT,LED_ROJO_PIN,IOCON_FUNC0);
	Chip_GPIO_SetPinDIROutput(LPC_GPIO,LED_ROJO_PORT,LED_ROJO_PIN);
	Chip_GPIO_SetPinOutHigh(LPC_GPIO,LED_ROJO_PORT,LED_ROJO_PIN);

	/*Inicializo el led verde */
	Chip_IOCON_PinMuxSet(LPC_IOCON,LED_VERDE_PORT,LED_VERDE_PIN,IOCON_FUNC0);
	Chip_GPIO_SetPinDIROutput(LPC_GPIO,LED_VERDE_PORT,LED_VERDE_PIN);
	Chip_GPIO_SetPinOutHigh(LPC_GPIO,LED_VERDE_PORT,LED_VERDE_PIN);

	/*Inicializo la UART*/
	UART_init();

	/*Inicializo el systick*/
	SysTick_Config(SystemCoreClock/TIC_1MS);

	/*Apago el led rojo*/
	Chip_GPIO_SetPinOutLow(LPC_GPIO,LED_ROJO_PORT,LED_ROJO_PIN);
	
	// ADC_CLOCK_SETUP_T adc_setup;
	ADC_CLOCK_SETUP_T adc_setup;
	Chip_IOCON_PinMuxSet(LPC_IOCON,ADC0_PORT,ADC0_PIN,IOCON_FUNC1);
	Chip_IOCON_PinMuxSet(LPC_IOCON, ADC1_PORT, ADC1_PIN, IOCON_FUNC1);
	Chip_ADC_Init(LPC_ADC,&adc_setup);

	LCD_init();
}


