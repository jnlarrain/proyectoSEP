/*
 * Proyecto_sep.c
 *
 * Created: 11/5/2018 10:10:22 PM
 * Author : Sebastian Diaz
 */ 

#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "keyboard.h"


// The Pin Change Interrupt Request 2 (PCI2) will trigger if any enabled PCINT[23:16] pin toggles. The Pin
// Change Interrupt Request 1 (PCI1) will trigger if any enabled PCINT[14:8] pin toggles. The Pin Change
// Interrupt Request 0 (PCI0) will trigger if any enabled PCINT[7:0] pin toggles.
//PC2,3,4,5 -> PCINIT10,11,12,13 -> PCI1
//PD4,5,6,7 -> PCINIT20,21,22,23 -> PCI2
void teclado(void) //interpretacion de seleccion
{
	//presionado es el input
	//mapear presionado con respectiva entrada en entradas
	char entradas[31] = "0123456789.+-*/^qlesctgxpofamdn="; 
	char ent = entradas[presionado];
	//calculadora(entrada)
	
	//MOSTRAR PRESIONADO POR USART (por ahora)
	char str[2];
	sprintf(str, "%d", presionado);
}

void actualizarNumero(float n, char num) //actualiza numero 
{
	switch (num){
		case '0': n = n*10 + 0; break;
		case '1': n = n*10 + 1; break;
		case '2': n = n*10 + 2; break;
		case '3': n = n*10 + 3; break;
		case '4': n = n*10 + 4; break;
		case '5': n = n*10 + 5; break;
		case '6': n = n*10 + 6; break;
		case '7': n = n*10 + 7; break;
		case '8': n = n*10 + 8; break;
		case '9': n = n*10 + 9; break;
		case '.': punto = 1;  break;}
		
	if (punto){ n = n/10; }
	mostrarNumero(N1);
}

void calcular1(float n, char op) //opera op con n
{
	switch (op){
		case 'q': N1 = n; break;  //raiz cuadrada
		case 'l': N1 = n; break;  //logaritmo natural
		case 'e': N1 = n; break;  //exp(x)
		case 's': N1 = n; break;  //sen(x)  //ocupar % pi
		case 'c': N1 = n; break;  //cos(x)
		case 't': N1 = n; break;  //tg(x)
		case 'g': N1 = n; break;  //log(x)
		case 'x': N1 = n; break;} //x^-1
	N2 = 0;
}

void calcular2(float n1, float n2, char op) //opera n1 op n2
{
	switch (op){
		case '+': N1 = n1 + n2; break;  // +
		case '-': N1 = n1 - n2; break;  // -
		case '*': N1 = n1 * n2; break;  // *
		case '/': N1 = n1 / n2; break;  // /
		case '^': N1 = n1; break;}   // ^
	N2 = 0;
	USART_Transmit_String("calcular N1 y N2 con");
	USART_Transmit_char(op);
	USART_Transmit_char('\n');
}

void mostrarNumero(float num)
{
	char str[6];
	sprintf(str, "%d", num);
	USART_Transmit_String(str);
	USART_Transmit_char('\n');
}

void calculadora(char entrada) //estados calculadora
{
	switch (estado)
	{
		case 0: //estado 0
		if((entrada == '0')||
		   (entrada == '1')||
		   (entrada == '2')||
		   (entrada == '3')||
		   (entrada == '4')||
		   (entrada == '5')||
		   (entrada == '6')||
		   (entrada == '7')||
		   (entrada == '8')||
		   (entrada == '9')||
		   (entrada == '.')) { actualizarNumero(N1,entrada); estado = 0;}
		else if ((entrada == '+')||
				 (entrada == '-')||
				 (entrada == '*')||
				 (entrada == '/')||
				 (entrada == '^')) {operador = entrada;	USART_Transmit_char(operador);		estado = 1;}
		else if ((entrada == 'q')||
				 (entrada == 'l')||
				 (entrada == 'e')||
				 (entrada == 's')||
				 (entrada == 'c')||
				 (entrada == 't')||
				 (entrada == 'g')||
				 (entrada == 'x')) {calcular1(N1,entrada);		estado = 3;}
		else{
			switch (entrada){
				case 'p': N1 = pi;						estado = 3; break;
				case 'm': N1 = M; N2 = 0;				estado = 3; break;
				case 'd': M = M + N1; N1 = 0; N2 = 0;	estado = 0; break;
				case 'n': M = M - N1; N1 = 0; N2 = 0;	estado = 0; break;
				case 'a': M = 0;  N1 = 0; N2 = 0;	    estado = 1; break;
				//case 'f': //terminar calculadora
			}
		}break;
		
		case 1: //estado 1
		punto = 0;
		if((entrada == '0')||
			(entrada == '1')||
			(entrada == '2')||
			(entrada == '3')||
			(entrada == '4')||
			(entrada == '5')||
			(entrada == '6')||
			(entrada == '7')||
			(entrada == '8')||
			(entrada == '9')||
			(entrada == '.')) { N2 = 0; actualizarNumero(N2,entrada); estado = 2;}
		else if ((entrada == '+')||
				(entrada == '-')||
				(entrada == '*')||
				(entrada == '/')||
				(entrada == '^')) {operador = entrada;	USART_Transmit_char(operador);		estado = 1;}
		else if ((entrada == 'q')||
				(entrada == 'l')||
				(entrada == 'e')||
				(entrada == 's')||
				(entrada == 'c')||
				(entrada == 't')||
				(entrada == 'g')||
				(entrada == 'x')||
				(entrada == 'm')||
				(entrada == 'd')||
				(entrada == 'n')) {	estado = 4;} //error
		else{
			switch (entrada){
				case 'p': N2 = pi;	calcular2(N1,N2,operador);	estado = 3; break;
				case 'a': M = 0;  N1 = 0; N2 = 0; punto = 0;    estado = 0; break;
				case '=': M = 0;  calcular2(N1,N1,operador);    estado = 0; break;
				//case 'f': //terminar calculadora
			}
		}
		break;
		
		case 2: //estado 2
		if((entrada == '0')||
			(entrada == '1')||
			(entrada == '2')||
			(entrada == '3')||
			(entrada == '4')||
			(entrada == '5')||
			(entrada == '6')||
			(entrada == '7')||
			(entrada == '8')||
			(entrada == '9')||
			(entrada == '.')) { actualizarNumero(N2,entrada); estado = 2;}
		else if ((entrada == '+')||
			(entrada == '-')||
			(entrada == '*')||
			(entrada == '/')||
			(entrada == '^')) { calcular2(N1,N2,operador); operador = entrada; USART_Transmit_char(operador); estado = 1;}
		else if ((entrada == 'q')||
			(entrada == 'l')||
			(entrada == 'e')||
			(entrada == 's')||
			(entrada == 'c')||
			(entrada == 't')||
			(entrada == 'g')||
			(entrada == 'x')) {calcular2(N1,N2,operador); calcular1(N1,entrada); estado = 3;}
		else{
			switch (entrada){
				case 'p': N2 = pi;	calcular2(N1,N2,operador);	estado = 3; break;
				case 'a': M = 0;  N1 = 0; N2 = 0; punto = 0;    estado = 0; break;
				case '=': M = 0;  calcular2(N1,N2,operador);    estado = 3; break;
				case 'm': N1 = M; N2 = 0;						estado = 3; break;
				case 'd': calcular2(N1,N2,operador); M = M + N1; N1 = 0; N2 = 0;	estado = 0; break;
				case 'n': calcular2(N1,N2,operador); M = M - N1; N1 = 0; N2 = 0;	estado = 0; break;
				//case 'f': //terminar calculadora
			}
		}
		break;
		
		case 3: //estado 3
		punto = 0;
		if((entrada == '0')||
		(entrada == '1')||
		(entrada == '2')||
		(entrada == '3')||
		(entrada == '4')||
		(entrada == '5')||
		(entrada == '6')||
		(entrada == '7')||
		(entrada == '8')||
		(entrada == '9')||
		(entrada == '.')) { N1 = 0; actualizarNumero(N1,entrada); estado = 0;}
		else if ((entrada == '+')||
		(entrada == '-')||
		(entrada == '*')||
		(entrada == '/')||
		(entrada == '^')) {operador = entrada;	USART_Transmit_char(operador);		estado = 1;}
		else if ((entrada == 'q')||
		(entrada == 'l')||
		(entrada == 'e')||
		(entrada == 's')||
		(entrada == 'c')||
		(entrada == 't')||
		(entrada == 'g')||
		(entrada == 'x')) {calcular1(N1,entrada);		estado = 3;}
		else{
			switch (entrada){
				case 'p': N1 = pi;							estado = 3; break;
				case 'm': N1 = M; N2 = 0;					estado = 3; break;
				case 'd': M = M + N1; N1 = 0; N2 = 0;		estado = 0; break;
				case 'n': M = M - N1; N1 = 0; N2 = 0;		estado = 0; break;
				case 'a': M = 0;  N1 = 0; N2 = 0; punto = 0; estado = 1; break;
				//case 'f': //terminar calculadora
			}
		}
		break;
		
		case 4: //estado 4
		if ((entrada == 'o')||(entrada == 'a'))
		{
			M = 0;  N1 = 0; N2 = 0; estado = 0; punto = 0;
		}
		else if ((entrada == 'f'))
		{
			//fin
		}
		else{
			//error
		}
		break;
	}
	
}


int Keyboard_init(void)
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

