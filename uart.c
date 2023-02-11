#include "usart.h"

void USART3_init(void) //Initialisering av UART 
{
	PORTE.DIRSET = 0b11111111;
	PORTD.DIR = 0b00000111;
	
	PORTB.DIRCLR = PIN1_bm; //= PORTB.DIRSET = PIN1_bm;
	PORTB.DIRSET = PIN0_bm;  //= PORTB.DIRCLR = PIN0_bm;
	USART3.BAUD = (uint16_t)USART3_BAUD_RATE(9600);
	USART3.CTRLB |= USART_TXEN_bm | USART_RXEN_bm;
	
	static FILE Usart_stream = FDEV_SETUP_STREAM(USART3_printChar,NULL,_FDEV_SETUP_WRITE);
	stdout = &Usart_stream;
}
static void USART3_sendChar(char c) //Sender char 
{
	while (!(USART3.STATUS & USART_DREIF_bm))
	{
		;
	}
	USART3.TXDATAL = c;
}
void USART3_sendString(char *str) //Sender string 
{
	for(size_t i = 0; i < strlen(str); i++)
	{
		USART3_sendChar(str[i]);
	}
}

static int USART3_printChar(char c, FILE *stream){ //Printer char 
	USART3_sendChar(c);
	return 0;
}
