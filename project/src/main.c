/*
===============================================================================
 Name        : AdcUart.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
*/

#if defined (__USE_LPCOPEN)
#if defined(NO_BOARD_LIB)
#include "chip.h"
#include "string.h"
#include "board.h"
#include "uart.h"
#include "logger.h"
#include <stdio.h>
#include <stdarg.h>
#else
#include "board.h"
#endif
#endif

#include <cr_section_macros.h>

#define delay(ticks)			\
{								\
	static int _count = ticks;	\
	if(_count){					\
		_count--;				\
		return;					\
	}							\
	else {						\
		_count = ticks;			\
	}							\
}								\

#define TIC_1MS 		(1000)
#define TIC_LED_MS		(250)
#define LED_ROJO_PORT	(0)
#define LED_ROJO_PIN	(22)
#define UART1_PORT		(0)
#define UART1_TXD_PIN	(15)
#define UART1_RXD_PIN	(16)
#define UART1_BAUDRATE	(57600)
#define MSG_LEN			(11)

#define LED_VERDE_PORT	(2)
#define LED_VERDE_PIN	(12)

/*ADC*/
#define ADC0_PORT		(0)
#define ADC0_PIN		(23)

int tareaAdc_start = 0;

void InitHardware(void);
//void TareaTickLed(void);
void TareaADC(void);
void TareaLeeSerie(void);

int main(void) {

#if defined (__USE_LPCOPEN)
    // Read clock settings and update SystemCoreClock variable
    SystemCoreClockUpdate();
#if !defined(NO_BOARD_LIB)
    // Set up and initialize all required blocks and
    // functions related to the board hardware
    Board_Init();
    // Set the LED to the state of "On"
    Board_LED_Set(0, true);
#endif
#endif
    InitHardware();

    log_setLevel(info);



    while(1) {

//    TareaTickLed();
    TareaADC();
    TareaLeeSerie();

    __WFI();
    }
    return 0 ;
}



void TareaADC(void)
{
	uint16_t val=0;

	delay(1000);
	log_printf(__func__, info, "ADC flag: %d", tareaAdc_start);

	if(tareaAdc_start)
	{


			if(Chip_ADC_ReadStatus(LPC_ADC,ADC_CH5,ADC_DR_DONE_STAT)){
				Chip_ADC_ReadValue(LPC_ADC,ADC_CH5, &val);

					UART_printf("%d\r\n", val);
					Chip_ADC_SetStartMode(LPC_ADC,ADC_START_NOW,ADC_TRIGGERMODE_RISING);
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

				//Compare for each string
				if(strcmp(str, "start") == 0)
				{
					Chip_ADC_SetStartMode(LPC_ADC,ADC_START_NOW,ADC_TRIGGERMODE_RISING);
					tareaAdc_start= 1;
					Chip_GPIO_SetPinOutHigh(LPC_GPIO,LED_ROJO_PORT,LED_ROJO_PIN);
				}
				else if(strcmp(str, "stop") == 0)
				{
					tareaAdc_start= 0;
					Chip_GPIO_SetPinOutLow(LPC_GPIO,LED_ROJO_PORT,LED_ROJO_PIN);
				}

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

	/*
	ADC_CLOCK_SETUP_T adc_setup;
	uint16_t dummy;
	Chip_IOCON_PinMuxSet(LPC_IOCON,ADC0_PORT,ADC0_PIN,IOCON_FUNC1);
	Chip_ADC_Init(LPC_ADC,&adc_setup);
	Chip_ADC_EnableChannel(LPC_ADC,ADC_CH0,ENABLE);
	Chip_ADC_ReadValue(LPC_ADC,ADC_CH0,&dummy);
*/
	/*Inicializo el ADC5*/
	ADC_CLOCK_SETUP_T adc_setup;
	uint16_t dummy;
	Chip_IOCON_PinMuxSet(LPC_IOCON,1,31,IOCON_FUNC3);
	Chip_ADC_Init(LPC_ADC, &adc_setup);
	Chip_ADC_SetSampleRate(LPC_ADC, &adc_setup, 10000);
	Chip_ADC_EnableChannel(LPC_ADC,ADC_CH5,ENABLE);
	Chip_ADC_ReadValue(LPC_ADC,ADC_CH5,&dummy);
}


