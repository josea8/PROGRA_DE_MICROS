#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define LEDB PORTB
#define LEDC PORTC

#define BOTONINC PC0
#define BOTONDEC PC1


uint8_t contador = 0;
uint8_t lista7[] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0x77,0x7C,0x39,0x5E,0x79,0x71};
uint8_t adcV = 0;
uint8_t num1 = 0;
uint8_t num2 = 0;

void initADC(void);

void setup(void) {
	cli();
	// Configura los pines de los botones como entrada y habilita las resistencias de pull-up internas
	DDRC &= ~((1 << BOTONINC) | (1 << BOTONDEC));
	PORTC |= (1 << BOTONINC) | (1 << BOTONDEC);

	// Configura los pines de los LEDs como salida
	DDRB |= 0x3F; // PB0-PB5
	DDRC |= 0x1C; // PC2-PC4
	DDRD |= 0xFF; // PD0-PD7
	UCSR0B = 0;
	initADC();
	sei();
	
}
void initADC(void){
	ADMUX = 0; // INICIO EN 0
	// REFERENCIA AVCC = 5V
	ADMUX |= (1 << REFS0);
	ADMUX &= ~(1 << REFS1);
	// SELECCIONO EL ADC[5] = 0110
	ADMUX |= (1 << MUX2) | (1 << MUX1);
	
	// JUSTIFICACION A LA IZQUIERDA
	ADMUX |= (1 << ADLAR);
	
	ADCSRA = 0;
	
	
	// HABILITAR LA INTERRUPCION DEL ADC
	ADCSRA |= (1 << ADIE);
	
	// HABILITAMOS EL PRESCALER 128  FADC = 125 KHz
	ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
	
	// HABILITAMOS EL ADC
	ADCSRA |= (1 << ADEN);
	
}
void compara(void) {
	// Actualiza los LEDs para que muestren el valor actual del contador
	LEDB = (LEDB & 0xC0) | ((contador << 1) & 0x3E); // Actualiza los bits PB1-PB5 en el puerto B hasta llegar a cierto valor
	//valor
	LEDC = (LEDC & 0xE3) | ((contador >> 3) & 0x1C); // Actualiza los bits PC2-PC4 en el puerto C hasta llegar a cierto valor
}

int main(void) {
	setup();

	while (1) {
		ADCSRA |= (1<<ADSC);
		PORTD = lista7[num1];
		//para los transistores
		PORTD |= (1 << PD7);
		_delay_ms(2);
		PORTD &= ~(1 << PD7);
		
		PORTD = lista7[num2];
		//para los transistores
		PORTB |= (1 << PB0);
		_delay_ms(2);
		PORTB &= ~(1 << PB0);
		
		// Verifica si se presiona el botón de incremento
		if (!(PINC & (1 << BOTONINC))) {
			_delay_ms(1500); // Pequeña espera para evitar rebotes
			if (!(PINC & (1 << BOTONINC))) { // Verifica
				if (contador < 255) // Incrementa el contador si no ha alcanzado su valor máximo
				contador++;
				compara();
			}
		}

		// Verifica si se presiona el botón de decremento
		if (!(PINC & (1 << BOTONDEC))) {
			_delay_ms(1500); // antirebote
			if (!(PINC & (1 << BOTONDEC))) { // Verifica
				if (contador > 0) // Decrementa el contador si no ha alcanzado su valor mínimo
				contador--;
				compara();
			}
		}
	}
}
ISR(ADC_vect){
	// Guarda el valor del ADC en las variables dig1 y dig2
	adcV = ADCH;
	num1 = adcV >> 4;
	num2 = adcV & 0x0F;
	alarma();
	// APAGAR LA BANDERA DE INTERRUPCION DEL ADC
	ADCSRA |= (1<<ADIF); // Resetear la bandera de ADC
}

void alarma(){
	if (adcV >> contador)
	{
		PORTC |= (1 << PC5);
	}
	else {
		PORTC &= ~(1 << PC5);
	}
}
