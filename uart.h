#pragma once 
#define F_CPU 4000000

#define USART3_BAUD_RATE(BAUD_RATE)	((float)(64*F_CPU/(16*(float)BAUD_RATE))+0.5)

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>

void USART3_init(void);
static void USART3_sendChar(char c);
static int USART3_printChar(char c, FILE *stream);
void USART3_sendString(char *str);
uint8_t USART_read();
