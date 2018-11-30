/*
 * teclado.c
 *
 * Created: 11/30/2018 2:48:25 PM
 *  Author: Sebastian
 */ 

#include "teclado.h"
#include <avr/io.h>


void tecladoInit(void)
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
}


void mostrar(int presionado) //interpretacion de seleccion, mapea input
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
