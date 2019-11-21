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

void InitHardware(void);
void TareaADC(void);
void TareaLeeSerie(void);

int main(void) {
    InitHardware();

    log_setLevel(debug0);

	//Default parameters
	handler.adc.start = 0;
	handler.adc.delay = 1000;
	handler.adc.pressure = 0;
	handler.adc.debug = 0;

    while(1) {
	    TareaADC();
    	TareaLeeSerie();

   		__WFI();
    }
    return 0 ;
}


#define	ADC_VECT_TOTAL	32
#define ADC_VECT_NOTAKE	8

void TareaADC(void)
{
	uint16_t val = 0;
	uint16_t samples[ADC_VECT_TOTAL];
	uint16_t accumulate = 0;

	delay(handler.adc.delay);
	log_printf(__func__, debug5, "ADC flag: %d", handler.adc.start);

	if(handler.adc.start)
	{
		uint8_t i = 0;
		
		//read till buffer is full
		while(i < ADC_VECT_TOTAL){
			if(Chip_ADC_ReadStatus(LPC_ADC, ADC_CH0, ADC_DR_DONE_STAT)){
				Chip_ADC_ReadValue(LPC_ADC, ADC_CH0, &val);

				samples[i] = val;

				i++;
				Chip_ADC_SetStartMode(LPC_ADC, ADC_START_NOW, ADC_TRIGGERMODE_RISING);
			}
		}

		//order buffer from MAX to min
		bubbleSort(samples, ADC_VECT_TOTAL);

		//accumulate values in the middle
		for (uint8_t i = ADC_VECT_NOTAKE; i < (ADC_VECT_TOTAL - ADC_VECT_NOTAKE); i++){
			accumulate += samples[i];
		}
		
		//mean value of accumulated
		val = accumulate / (ADC_VECT_TOTAL - ADC_VECT_NOTAKE*2);

		if(handler.adc.debug){
			UART_printf("%d %d\r\n", handler.adc.pressure, val);
		}
	}
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

				log_printf(__func__, debug3, "UART: %s\n", str);

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


void SysTick_Handler(void)
{
}

void InitHardware(void)
{
	/*Inicializo el Clock del sistema*/
	Chip_SetupXtalClocking();
	SystemCoreClockUpdate();

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
	uint16_t dummy;
	Chip_IOCON_PinMuxSet(LPC_IOCON,ADC0_PORT,ADC0_PIN,IOCON_FUNC1);
	Chip_ADC_Init(LPC_ADC,&adc_setup);
	Chip_ADC_EnableChannel(LPC_ADC,ADC_CH0,ENABLE);
	Chip_ADC_ReadValue(LPC_ADC,ADC_CH0,&dummy);

	// Inicializo el ADC5
	// uint16_t dummy;
	// Chip_IOCON_PinMuxSet(LPC_IOCON,1,31,IOCON_FUNC3);
	// Chip_ADC_Init(LPC_ADC, &adc_setup);
	// Chip_ADC_EnableChannel(LPC_ADC,ADC_CH5,ENABLE);
	// Chip_ADC_ReadValue(LPC_ADC,ADC_CH5,&dummy);
}


