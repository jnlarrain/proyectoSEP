
#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include <avr/interrupt.h>

static unsigned int linea;
static unsigned int presionado;
static unsigned int estado = 0;

//calculadora
static float N1 = 0;
static float N2 = 0;
static float M = 0;
static char operador = '+';
int punto = 0;
#define  pi 3

#define neutro _delay_ms(1); PORTC |= (1 << PORTC0) | (1 << PORTC1); PORTD |= (1 << PORTD2) | (1 << PORTC3);
#define  p1  PORTC &= ~(1 << PORTC0); neutro linea = 0;
#define  p2  PORTC &= ~(1 << PORTC1); neutro linea = 8;
#define  p3  PORTD &= ~(1 << PORTD2); neutro linea = 16;
#define  p4  PORTD &= ~(1 << PORTD3); neutro linea = 24;

#define  deboncer 10

//INICIO USART (hay que borrarlo eventualmente)
#define FOSC 16000000 	//clock
#define BAUD 57600		//ancho de banda que queremos
#define MYUBRR FOSC/16/BAUD-1	//calculo segun el data sheet

void USART_Transmit_char( unsigned char data )
{
	/* Wait for empty transmit buffer */
	while ( !( UCSR0A & (1<<UDRE0)) )
	;
	/* Put data into buffer, sends the data */
	UDR0 = data;
}

void USART_Transmit_String(char* StringPtr){
	while(*StringPtr != 0x00){
		USART_Transmit_char(*StringPtr);
		StringPtr++;
	}
}

void USART_Init( unsigned int ubrr)
{
	/*Set baud rate */
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)ubrr;
	/*Enable receiver and transmitter */
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);			//habilitamos tx y rx
	/* Set frame format: 8data, 1stop bit */
	UCSR0C = (3<<UCSZ00);					//decimos cuantos stopbit queremos y que data -> 8bits, 1stpobit
}

unsigned char USART_Receive( void )
{
	/* Wait for data to be received */
	while ( !(UCSR0A & (1<<RXC0)) )
	;
	/* Get and return received data from buffer */
	return UDR0;
}

//FIN USART (hay que borrarlo eventualmente)

// The Pin Change Interrupt Request 2 (PCI2) will trigger if any enabled PCINT[23:16] pin toggles. The Pin
// Change Interrupt Request 1 (PCI1) will trigger if any enabled PCINT[14:8] pin toggles. The Pin Change
// Interrupt Request 0 (PCI0) will trigger if any enabled PCINT[7:0] pin toggles.
//PC2,3,4,5 -> PCINIT10,11,12,13 -> PCI1
//PD4,5,6,7 -> PCINIT20,21,22,23 -> PCI2

//agregar bool ocupado para no accionar multiples veces interrupcion!
ISR(PCINT2_vect) //seccion2 D
{
	//INICIO OCUPADO
	//Debounder in
	if      (!(PIND & (1<<PIND4))) { presionado = linea + 0;}
	else if (!(PIND & (1<<PIND5))) { presionado = linea + 1;}
	else if (!(PIND & (1<<PIND6))) { presionado = linea + 2;}
	else if (!(PIND & (1<<PIND7))) { presionado = linea + 3;}
	teclado();
	//debounceer out
}

ISR(PCINT1_vect) //seccion1 C
{
	//INICIO OCUPADO
	//debouncer in
	if      (!(PINC & (1<<PINC2))) { presionado = linea + 4;}
	else if (!(PINC & (1<<PINC3))) { presionado = linea + 5;}
	else if (!(PINC & (1<<PINC4))) { presionado = linea + 6;}
	else if (!(PINC & (1<<PINC5))) { presionado = linea + 7;}
	teclado();
	//debouncer out
}

void teclado(void) //interpretacion de seleccion, mapea input
{
	//presionado es el input
	//mapear presionado con respectiva entrada en entradas
	//cambiar orden en este string segun se necesite
	char entradas[31] = "m123456789.+-*/^qlesctgxpofa0dn=";
	char ent = entradas[presionado];
	//calculadora(entrada); //le pasa la entrada a la calculadora
	
	//FIN OCUPADO
	//MOSTRAR PRESIONADO POR USART (por ahora)
	char str[2];
	sprintf(str, "%d", presionado);
	USART_Transmit_String(str);
	USART_Transmit_String(" -> ");
	USART_Transmit_char(ent);
	USART_Transmit_char('\n');
	}


	int main(void)
	{
	//OUTPUTS
	DDRC |= (1 << DDC0) | (1 << DDC1);
	DDRD |= (1 << DDD2) | (1 << DDD3);
	
	//INPUTS
	DDRC &= ~((1 << DDC2) | (1 << DDC3) | (1 << DDC4) | (1 << DDC5));
	DDRD &= ~((1 << DDD4) | (1 << DDD5) | (1 << DDD6) | (1 << DDD7));
	
	//PULL-UP
	PORTC |= (1 << PORTC2) | (1 << PORTC3) | (1 << PORTC4) | (1 << PORTC5);
	PORTD |= (1 << PORTD4) | (1 << PORTD5) | (1 << PORTD6) | (1 << PORTD7);
	
	//OUTPUTS HIGH (neutro)
	PORTC |= (1 << PORTC0) | (1 << PORTC1);
	PORTD |= (1 << PORTD2) | (1 << PORTC3);
	
	PCMSK1 |= 0x3C; //PC2,3,4,5
	PCMSK2 |= 0xF0; //PD4,5,6,7
	PCICR |= (1<<PCIE1) | (1<<PCIE2);   //interrupts de cambio habilitados
	
	USART_Init(MYUBRR);
	USART_Transmit_String("HOLA");
	
	sei();
	
	while (1)
	{
		p1
		p2
		p3
		p4
	}
}