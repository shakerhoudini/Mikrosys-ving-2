#pragma once
#define F_CPU 4000000
#define RTC_PERIOD (511)
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>


uint16_t ADC0_read(void);
void ADC0_start(void);
bool ADC0_conersionDone(void);
uint16_t adcVal;
void ADC0_init(void);
