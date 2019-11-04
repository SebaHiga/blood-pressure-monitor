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
#include <stdio.h>
#else
#include "board.h"
#endif
#endif

#include <cr_section_macros.h>

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

/*UART0*/
#define UART0_PORT		(0)
#define UART0_TXD_PIN	(2)
#define UART0_RXD_PIN	(3)
#define UART0_BAUDRATE	(57600)


/*ADC*/
#define ADC0_PORT		(0)
#define ADC0_PIN		(23)

int tareaAdc_start = 0;

void InitHardware(void);
//void TareaTickLed(void);
void TareaADC(void);
void TareaLeeSerie(void);

void UartSendData(char *c);

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

    while(1) {

//    TareaTickLed();
    TareaADC();
    TareaLeeSerie();

    __WFI();
    }
    return 0 ;
}

/*void TareaTickLed(void)
{
	static int espera = TIC_LED_MS;
	if(!--espera)
	{
		espera = TIC_LED_MS;
		Chip_GPIO_SetPinToggle(LPC_GPIO,LED_ROJO_PORT,LED_ROJO_PIN);
	}
}*/

void TareaADC(void)
{
	uint16_t val=0;
	char msg[5];

	if(tareaAdc_start)
	{
			if(Chip_ADC_ReadStatus(LPC_ADC,ADC_CH5,ADC_DR_DONE_STAT))
			{
				Chip_ADC_ReadValue(LPC_ADC,ADC_CH5,&val);

				if((Chip_UART_ReadLineStatus(LPC_UART0) & UART_LSR_THRE))
					memset(msg,'\0',5);
					sprintf(msg,"%d",val);
					UartSendData(msg);

				Chip_ADC_SetStartMode(LPC_ADC,ADC_START_NOW,ADC_TRIGGERMODE_RISING);
			}
	}
}


void TareaLeeSerie(void)
{
	uint8_t byte = '\0';

			if(Chip_UART_ReadLineStatus(LPC_UART0)&UART_LSR_RDR)
			{
				byte = Chip_UART_ReadByte(LPC_UART0);
				if(byte =='b')
				{
					Chip_ADC_SetStartMode(LPC_ADC,ADC_START_NOW,ADC_TRIGGERMODE_RISING);
					tareaAdc_start= 1;
					Chip_GPIO_SetPinOutHigh(LPC_GPIO,LED_ROJO_PORT,LED_ROJO_PIN);
				}
				else if(byte =='c')
				{
					tareaAdc_start= 0;
					Chip_GPIO_SetPinOutLow(LPC_GPIO,LED_ROJO_PORT,LED_ROJO_PIN);
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
	Chip_IOCON_PinMuxSet(LPC_IOCON,UART0_PORT,UART0_TXD_PIN,IOCON_FUNC1);
	Chip_IOCON_PinMuxSet(LPC_IOCON,UART0_PORT,UART0_RXD_PIN,IOCON_FUNC1);
	Chip_UART_Init(LPC_UART0);
	Chip_UART_SetBaud(LPC_UART0,UART0_BAUDRATE);
	Chip_UART_ConfigData(LPC_UART0, (UART_LCR_WLEN8 | UART_LCR_SBS_1BIT));
	Chip_UART_SetupFIFOS(LPC_UART0, (UART_FCR_FIFO_EN | UART_FCR_TRG_LEV3));
	Chip_UART_TXEnable(LPC_UART0);

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
	Chip_ADC_Init(LPC_ADC,&adc_setup);
	Chip_ADC_EnableChannel(LPC_ADC,ADC_CH5,ENABLE);
	Chip_ADC_ReadValue(LPC_ADC,ADC_CH5,&dummy);

}

void UartSendData(char *c)
{
	for(int i=0;i<strlen(c);i++)
		Chip_UART_SendByte(LPC_UART0,c[i]);
	Chip_UART_SendByte(LPC_UART0,'\r');
	Chip_UART_SendByte(LPC_UART0,'\n');
}
