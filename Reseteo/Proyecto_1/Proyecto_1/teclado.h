/*
 * teclado.h
 *
 * Created: 11/30/2018 2:48:09 PM
 *  Author: Sebastian
 */ 


#ifndef TECLADO_H_
#define TECLADO_H_

#define neutro _delay_ms(1); PORTC |= (1 << PORTC0) | (1 << PORTC1); PORTD |= (1 << PORTD2) | (1 << PORTC3);
#define  p1  PORTC &= ~(1 << PORTC0); neutro linea = 0;
#define  p2  PORTC &= ~(1 << PORTC1); neutro linea = 8;
#define  p3  PORTD &= ~(1 << PORTD2); neutro linea = 16;
#define  p4  PORTD &= ~(1 << PORTD3); neutro linea = 24;

unsigned char USART_Receive( void );
void USART_Init( unsigned int ubrr);
void USART_Transmit_String(char* StringPtr);
void USART_Transmit_char( unsigned char data );
void mostrar(int presionado);
void tecladoInit(void);


#endif /* TECLADO_H_ */