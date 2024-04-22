#define F_CPU 16000000
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "PWM2/PWM2.h"
#include "PWM1/PWM1.h"
#define ADC_MAX 255
#define DUTY_MIN 0
#define DUTY_MAX 40

uint8_t dutyCycle = 0;
uint8_t dutyCycle1 = 0;

void setup(void){
	cli();
	UCSR0B = 0;
	sei();
	initADC();
}

uint8_t mapADCtoDC(uint8_t adcValue){
	return(uint8_t)(((float)adcValue / ADC_MAX) * (DUTY_MAX - DUTY_MIN) + DUTY_MIN);
}

int main(void)
{
	
	setup();
	
	initPWM1FastA(no_invertido, 1024);
	initPWM2Fast_16bit(no_invertido, 8);
    while (1) 
    {
		//dutyCycle1 = A; //(7.84*ADCH)+2000;
		dutyCycle = mapADCtoDC(ADCH);
		ADCSRA |= (1<<ADSC);
		updateDutyCicleA1(dutyCycle);
		updateDutyCicleA1(dutyCycle1);
    }
}

ISR(ADC_vect){
	dutyCycle = ADCH;
	
	//valorADC2 = ADCH;
	ADCSRA |=(1<<ADIF);
}