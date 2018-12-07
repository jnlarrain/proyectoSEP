/*
 * teclado.h
 *
 * Created: 11/30/2018 2:48:09 PM
 *  Author: Sebastian
 */ 


#ifndef TECLADO_H_
#define TECLADO_H_

unsigned char USART_Receive( void );
void USART_Init( unsigned int ubrr);
void USART_Transmit_String(char* StringPtr);
void USART_Transmit_char( unsigned char data );
void tecladoInit(void);

#endif /* TECLADO_H_ */