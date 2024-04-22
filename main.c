/*
 * laboratorio6.c
 *
 * Created: 21/04/2024 18:26:51
 * Author : Jose
 */ 
#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>

void initUART9600(void);
void writeUART(char caracter);
void WriteTextUART(char * Texto);
void setup(void);
uint8_t ValorTX;


void setup(){
	DDRB |= (1<<DDB0) | (1<<DDB1) | (1<<DDB2) | (1<<DDB3) | (1<<DDB4) | (1<<DDB5);//salidas en en el puerto B
	initUART9600();
	sei();
}

int main(void)
{
	setup();
	
	writeUART('m');
	writeUART('e');
	writeUART('n');
	writeUART('s');
	writeUART('a');
	writeUART('j');
	writeUART('e');
	writeUART('\n');
	/*
	writeUART(10);
	writeUART(13);*/
	WriteTextUART("Dame un valor para los leds:\r\n");
    while (1) 
    {
		
    }
}

void initUART9600(void){
	//rx como entrada y yx como salida
	DDRD &= ~(1<<DDD0);
	DDRD |= (1<<DDD1);
	//CONFIGURACION DEL UCSR0A
	UCSR0A = 0;
	//CONFIGURACION DEL UCSR0B > DONDE HABILITAMOS ISR DE RECEPCION, HABILITAMOS EL TX Y RX
	UCSR0B = 0;
	UCSR0B = (1 << RXCIE0) | (1 << RXEN0) | (1 << TXEN0);
	//CONFIGURAR EL UCSR0C > ASINCRONO, SIN PARIDAD, 1 SOLO BIT DE STOP, DATA BITS: 8
	UCSR0C = 0;
	UCSR0C |= (1 << UCSZ01)|(1 << UCSZ00);
	//CONFIGURACION DEL BAUDRATE: 9600
	UBRR0 = 103;
}
void WriteTextUART(char * Texto){
	uint8_t i;
	for (i=0; Texto[i]!= '\0'; i++){ //para poder dar valores conforme el simbolo que estamos recibiendo de la terminal
		while (!(UCSR0A & (1<<UDRE0)));
		UDR0 = Texto[i];
	}
}



void writeUART(char caracter){
	while(!(UCSR0A & (1 << UDRE0)));
	UDR0 = caracter;
}

ISR(USART_RX_vect){
	ValorTX = UDR0;
	while(!(UCSR0A & (1 << UDRE0)));
	UDR0 = ValorTX;
	PORTB = ValorTX;
}

	