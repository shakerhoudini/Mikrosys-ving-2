#define F_CPU 4000000 //clockfrekvensen CPU-en går på 
#define RTC_PERIOD (511) //ADC
#define USART3_BAUD_RATE(BAUD_RATE)	((float)(64*F_CPU/(16*(float)BAUD_RATE))+0.5) //UART

//Inkludering av bibliotek 
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <string.h>
#include <stdio.h>


//Deklarasjon av funksjoner 
uint16_t adcVal;
void ADC0_init(void);
uint16_t ADC0_read(void);
void USART3_init(void);
static void USART3_sendChar(char c);
static int USART3_printChar(char c, FILE *stream);
void USART3_sendString(char *str);
uint8_t USART_read();

static FILE Usart_stream = FDEV_SETUP_STREAM(USART3_printChar,NULL,_FDEV_SETUP_WRITE); //Usart 

int main(void) //Hovedfunksjonen
{
	ADC0_init(); //Initialisering av ADC
	USART3_init(); //Initialisering av USART 
	
	adcVal = ADC0_read(); 
	//_delay_ms(10);
	while (1)
	{
		_delay_ms(100);
		adcVal = ADC0_read();
		if(!(PORTB.IN & PIN2_bm)){ //Dersom knappen trykkes inn 
			printf("Verdien er :%d\r\n",adcVal);
		}
	}
}

void ADC0_init(void) //Initialisering av ADC 
{
	/* Disable digital input buffer */
	PORTD.PIN6CTRL &= ~PORT_ISC_gm;
	PORTD.PIN6CTRL |= PORT_ISC_INPUT_DISABLE_gc;
	/* Disable pull-up resistor */
	PORTD.PIN6CTRL &= ~PORT_PULLUPEN_bm;
	
	PORTB.DIRCLR = PIN2_bm;
	PORTB.PIN2CTRL = PORT_PULLUPEN_bm;
	
	ADC0.CTRLC = ADC_PRESC_DIV4_gc;
	VREF.ADC0REF = VREF_REFSEL_VDD_gc;/* Internal reference */
	ADC0.CTRLA = ADC_ENABLE_bm /* ADC Enable: enabled */
	| ADC_RESSEL_10BIT_gc; /* 10-bit mode */
	/* Select ADC channel */
	ADC0.MUXPOS = ADC_MUXPOS_AIN6_gc;
}

uint16_t ADC0_read(void) //Leser av fra analog pins 
{
	/* Start ADC conversion */
	ADC0.COMMAND = ADC_STCONV_bm;
	/* Wait until ADC conversion done */
	while ( !(ADC0.INTFLAGS & ADC_RESRDY_bm) )
	{
		;
	}
	/* Clear the interrupt flag by writing 1: */
	ADC0.INTFLAGS = ADC_RESRDY_bm;
	return ADC0.RES;
}

void USART3_init(void) //initilisering av UART 
{
	PORTE.DIRSET = 0b11111111;
	
	PORTB.DIRCLR = PIN1_bm; //= PORTB.DIRSET = PIN1_bm;
	PORTB.DIRSET = PIN0_bm;  //= PORTB.DIRCLR = PIN0_bm;
	USART3.BAUD = (uint16_t)USART3_BAUD_RATE(9600);
	USART3.CTRLB |= USART_TXEN_bm | USART_RXEN_bm;
	
	stdout = &Usart_stream;
}
static void USART3_sendChar(char c)
{
	while (!(USART3.STATUS & USART_DREIF_bm))
	{
		;
	}
	USART3.TXDATAL = c;
}
void USART3_sendString(char *str)
{
	for(size_t i = 0; i < strlen(str); i++)
	{
		USART3_sendChar(str[i]);
	}
}

static int USART3_printChar(char c, FILE *stream){
	USART3_sendChar(c);
	return 0;
}
