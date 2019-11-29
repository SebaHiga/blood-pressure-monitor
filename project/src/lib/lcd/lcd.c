#include "lcd.h"


void LCD_init (void){
    Chip_IOCON_PinMux(LPC_IOCON,LCD_E_PORT,LCD_E_PIN,IOCON_MODE_INACT,IOCON_FUNC0);
	Chip_GPIO_SetPinDIROutput(LPC_GPIO,LCD_E_PORT,LCD_E_PIN);
	Chip_IOCON_EnableOD(LPC_IOCON, LCD_E_PORT, LCD_E_PIN);
	Chip_GPIO_SetPinOutLow(LPC_GPIO,LCD_E_PORT,LCD_E_PIN);

	Chip_IOCON_PinMux(LPC_IOCON,LCD_D4_PORT,LCD_D4_PIN,IOCON_MODE_INACT,IOCON_FUNC0);
	Chip_GPIO_SetPinDIROutput(LPC_GPIO,LCD_D4_PORT,LCD_D4_PIN);
	Chip_IOCON_EnableOD(LPC_IOCON, LCD_D4_PORT, LCD_D4_PIN);
	Chip_GPIO_SetPinOutLow(LPC_GPIO,LCD_D4_PORT,LCD_D4_PIN);

	Chip_IOCON_PinMux(LPC_IOCON,LCD_D5_PORT,LCD_D5_PIN,IOCON_MODE_INACT,IOCON_FUNC0);
	Chip_GPIO_SetPinDIROutput(LPC_GPIO,LCD_D5_PORT,LCD_D5_PIN);
	Chip_IOCON_EnableOD(LPC_IOCON, LCD_D5_PORT, LCD_D5_PIN);
	Chip_GPIO_SetPinOutLow(LPC_GPIO,LCD_D5_PORT,LCD_D5_PIN);

	Chip_IOCON_PinMux(LPC_IOCON,LCD_D6_PORT,LCD_D6_PIN,IOCON_MODE_INACT,IOCON_FUNC0);
	Chip_GPIO_SetPinDIROutput(LPC_GPIO,LCD_D6_PORT,LCD_D6_PIN);
	Chip_IOCON_EnableOD(LPC_IOCON, LCD_D6_PORT, LCD_D6_PIN);
	Chip_GPIO_SetPinOutLow(LPC_GPIO,LCD_D6_PORT,LCD_D6_PIN);

	Chip_IOCON_PinMux(LPC_IOCON,LCD_D7_PORT,LCD_D7_PIN,IOCON_MODE_INACT,IOCON_FUNC0);
	Chip_GPIO_SetPinDIROutput(LPC_GPIO,LCD_D7_PORT,LCD_D7_PIN);
	Chip_IOCON_EnableOD(LPC_IOCON, LCD_D7_PORT, LCD_D7_PIN);
	Chip_GPIO_SetPinOutLow(LPC_GPIO,LCD_D7_PORT,LCD_D7_PIN);

	Chip_IOCON_PinMux(LPC_IOCON,LCD_RST_PORT,LCD_RST_PIN,IOCON_MODE_INACT,IOCON_FUNC0);
	Chip_GPIO_SetPinDIROutput(LPC_GPIO,LCD_RST_PORT,LCD_RST_PIN);
	Chip_IOCON_EnableOD(LPC_IOCON, LCD_RST_PORT, LCD_RST_PIN);
	Chip_GPIO_SetPinOutLow(LPC_GPIO,LCD_RST_PORT,LCD_RST_PIN);

	/*Secuencia de inicializacion LCD*/
	LCD_WaitLong(15);
	LCD_WComando4(0x30);

	LCD_WaitLong(15);
	LCD_WComando4(0x30);

	LCD_WaitShort(6);
	LCD_WComando4(0x30);

	LCD_WaitShort(6);
	LCD_WComando4(0x20);

	/*Function Set*/
	LCD_WaitShort(3);
	LCD_WComando4(0x20);
	LCD_WaitShort(3);
	LCD_WComando4(0x80);		// N=1 F=0

	/*Display on/off control*/
	LCD_WaitShort(12);
	LCD_WComando4(0x00);
	LCD_WaitShort(3);
	LCD_WComando4(0xC0);		// D=1 C=0 B=0

	/*Clear Display*/
	LCD_WaitShort(15);
	LCD_WComando8(LCD_CLEAR);	// Clear display
}
void LCD_WaitLong(uint16_t ciclos){
    uint16_t i;
	uint16_t x;

	for(i=0;i<ciclos;i++)
	{
		for(x = 0;x < 0x4FF6;x++);
	}
}
void LCD_WaitShort(uint8_t ciclos){
    uint8_t i;
	uint16_t x;

	for(i=0;i<ciclos;i++)
	{
		for(x=0;x<0x9F6;x++);
	}
}
void LCD_latch(void){
	Chip_GPIO_SetPinOutHigh(LPC_GPIO,LCD_E_PORT,LCD_E_PIN);
	//LCDDelay(2);
	LCD_WaitLong(1);
	Chip_GPIO_SetPinOutLow(LPC_GPIO,LCD_E_PORT,LCD_E_PIN);
}
void LCD_WComando4(uint8_t comando){
    Chip_GPIO_SetPinOutLow(LPC_GPIO,LCD_RST_PORT,LCD_RST_PIN);

	Chip_GPIO_SetPinState(LPC_GPIO,LCD_D4_PORT,LCD_D4_PIN, ((comando & 0x10) >> 4));
	Chip_GPIO_SetPinState(LPC_GPIO,LCD_D5_PORT,LCD_D5_PIN, ((comando & 0x20) >> 5));
	Chip_GPIO_SetPinState(LPC_GPIO,LCD_D6_PORT,LCD_D6_PIN, ((comando & 0x40) >> 6));
	Chip_GPIO_SetPinState(LPC_GPIO,LCD_D7_PORT,LCD_D7_PIN, ((comando & 0x80) >> 7));
	LCD_latch();
}
void LCD_WComando8(uint8_t comando){
    	Chip_GPIO_SetPinOutLow(LPC_GPIO,LCD_RST_PORT,LCD_RST_PIN);

	// Parte Alta del comando
	Chip_GPIO_SetPinState(LPC_GPIO,LCD_D4_PORT,LCD_D4_PIN, ((comando & 0x10) >> 4));
	Chip_GPIO_SetPinState(LPC_GPIO,LCD_D5_PORT,LCD_D5_PIN, ((comando & 0x20) >> 5));
	Chip_GPIO_SetPinState(LPC_GPIO,LCD_D6_PORT,LCD_D6_PIN, ((comando & 0x40) >> 6));
	Chip_GPIO_SetPinState(LPC_GPIO,LCD_D7_PORT,LCD_D7_PIN, ((comando & 0x80) >> 7));
	LCD_latch();

	// Parte Baja del comando
	Chip_GPIO_SetPinState(LPC_GPIO,LCD_D4_PORT,LCD_D4_PIN, ((comando & 0x01) ));
	Chip_GPIO_SetPinState(LPC_GPIO,LCD_D5_PORT,LCD_D5_PIN, ((comando & 0x02) >> 1));
	Chip_GPIO_SetPinState(LPC_GPIO,LCD_D6_PORT,LCD_D6_PIN, ((comando & 0x04) >> 2));
	Chip_GPIO_SetPinState(LPC_GPIO,LCD_D7_PORT,LCD_D7_PIN, ((comando & 0x08) >> 3));
	LCD_latch();
}
void LCD_WDato(uint8_t comando){
    Chip_GPIO_SetPinOutHigh(LPC_GPIO,LCD_RST_PORT,LCD_RST_PIN);

	// Parte Alta del comando
	Chip_GPIO_SetPinState(LPC_GPIO,LCD_D4_PORT,LCD_D4_PIN, ((comando & 0x10) >> 4));
	Chip_GPIO_SetPinState(LPC_GPIO,LCD_D5_PORT,LCD_D5_PIN, ((comando & 0x20) >> 5));
	Chip_GPIO_SetPinState(LPC_GPIO,LCD_D6_PORT,LCD_D6_PIN, ((comando & 0x40) >> 6));
	Chip_GPIO_SetPinState(LPC_GPIO,LCD_D7_PORT,LCD_D7_PIN, ((comando & 0x80) >> 7));
	LCD_latch();

	// Parte Baja del comando
	Chip_GPIO_SetPinState(LPC_GPIO,LCD_D4_PORT,LCD_D4_PIN, ((comando & 0x01) ));
	Chip_GPIO_SetPinState(LPC_GPIO,LCD_D5_PORT,LCD_D5_PIN, ((comando & 0x02) >> 1));
	Chip_GPIO_SetPinState(LPC_GPIO,LCD_D6_PORT,LCD_D6_PIN, ((comando & 0x04) >> 2));
	Chip_GPIO_SetPinState(LPC_GPIO,LCD_D7_PORT,LCD_D7_PIN, ((comando & 0x08) >> 3));
	LCD_latch();
}
void LCD_WString (uint8_t *ptr){
    	while(*ptr != 0x00)
	{
		LCD_WDato(*ptr++);
	}
}

void Display_lcd( char *msg , char r , char p ){
    if( r )
        p = p + 0xc0 ;
	else
		p = p + 0x80 ;

	LCD_WComando8(p);
    LCD_WString( (uint8_t *) msg);
}


/**
 * 
 *  |   0 mmHg  -> corrido 4-len posiciones 
 *  |  10 mmHg
 *  | 100 mmHg
 * 	|1000 mmHg
 */
void LCD_InstantPressure (int value){
    char num[LCD_N_CHARACTERS];
    char msg[LCD_N_CHARACTERS];

    sprintf(num, "%d", value);

    //set message with NULL then set spaces to offset the number
    memset(msg, 0, LCD_N_CHARACTERS);
    memset(msg, ' ', 4 - strlen(num));

    //concatenate all to message
    strcat(msg, num);
    strcat(msg, " mmHg");

    LCD_diffPrint(row2, msg);
}

/**
 *  This functions only prints for different characters in given string 
 * 
 */
void LCD_diffPrint(lcd_row_t row, const char *str){
    static char msg_previous[2][LCD_N_CHARACTERS] = {0};
    char msg[LCD_N_CHARACTERS];
    int position = 0;

    if(strlen(str) > 16) return;

    memset(msg, 0, LCD_N_CHARACTERS);
    strcpy(msg, str);

    switch (row){
        case row1:
            position = 0x80;
            break;
        case row2:
            position = 0xc0;
            break;
    }

	//replace null for spaces
	for(int i = 0; i < LCD_N_CHARACTERS; i++){
		if(msg[i] == '\0'){
			msg[i] = ' ';
		}
	}

    //check for differences
    for (int i = 0; i < LCD_N_CHARACTERS; i++)
    {
        if(msg_previous[row][i] != msg[i]){
			LCD_WComando8(i + position);
			LCD_WDato( (uint8_t) msg[i]);
        }
    }
    
    strcpy(msg_previous[row], msg);
}

/*

    Be careful to NOT surpass 16 characters

*/
void LCD_printf(lcd_row_t row, const char *format, ...){
    char str[125] = {0};
    va_list args;

    va_start(args, format);
    vsprintf(str, format, args);
    va_end(args);

    if(strlen(str) < LCD_N_CHARACTERS){
        LCD_diffPrint(row, str);
    }
}
