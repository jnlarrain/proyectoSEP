/*
 * teclado.c
 *
 * Created: 11/30/2018 2:48:25 PM
 *  Author: Sebastian
 */ 

#include "teclado.h"
#include <avr/io.h>
#include "Grilla/grilla.h"


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
	
	//seteoTimmer2
	
	TCCR2A = 0;
	TCCR2B = 0;
	TCNT2 = 0;

	// 1000 Hz (16000000/((124+1)*128))
	OCR2A = 124;
	// CTC
	TCCR2A |= (1 << WGM21);
	// Prescaler 128
	TCCR2B |= (1 << CS22) | (1 << CS20);
	// Output Compare Match A Interrupt Enable
	TIMSK2 |= (1 << OCIE2A);
}


void USART_Transmit_char( unsigned char data )
{
	/* Wait for empty transmit buffer */
	while ( !( UCSR0A & (1<<UDRE0)) )
	;
	/* Put data into buffer, sends the data */
	UDR0 = data;
}

void USART_Transmit_String(char* StringPtr)
{
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
