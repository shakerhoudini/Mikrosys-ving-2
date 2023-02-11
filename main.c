#define F_CPU 4000000 //Frekvens for mikrokontrolleren
#include "adc.h" //Inkludering av egendefinerte 
#include "usart.h"


void ledCnt(uint16_t val);


int main(void)
{
	ADC0_init();
	ADC0_start();
	USART3_init();
	int counter = 0;
	adcVal = ADC0_read();
	//_delay_ms(10);
	uint16_t temp = 0;
	while (1)
	{
		uint16_t r1 = 10000;
		uint16_t r2= (adcVal*3.3)/(3.3-adcVal*3.3)*r1;
		uint16_t r = r1/r2;
		uint16_t m1 = log(r);
		uint16_t m2 = 3950/m1;
		ADC0_start();	
		temp = 25-m2;
		
		if(ADC0_conersionDone()){
			counter++;
			if(counter>100){
				printf("balls %d\r\n",temp);
				ledCnt(temp);
				counter = 0;
			}
			adcVal = ADC0_read();		
			
		}
	}
}

void ledCnt(uint16_t val){ //Termometerfunksjon
	PORTD.OUT = 0;
	/*	printf("%d",val);*/
	if(val > 14){
		PORTD.OUT |= PIN0_bm;
		if(val > 20){
			PORTD.OUT |= PIN1_bm;
			if(val > 25){
				PORTD.OUT |= PIN2_bm;
			}
		}
	}
}



