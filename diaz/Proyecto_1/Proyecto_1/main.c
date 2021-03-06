/*
 * Proyecto_1.c
 *
 * Created: 11/5/2018 10:10:22 PM
 * Author : Sebastian
 */ 

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

void actualizarNumero(float n, char num) //actualiza numero n con num
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
	//ver tema del punto	
	if (punto){ n = n/(10); } //este 10 tiene que ir aumentado (elevado) segun un contador que se resetea cuando punto pasa a 0
	mostrarNumero(N1);
}

void calcular1(float n, char op) //opera el operador op con un solo numero n y lo guarda en N1
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
	USART_Transmit_String("calcular N1  con ");
	USART_Transmit_char(op);
	USART_Transmit_char('\n');
}

void calcular2(float n1, float n2, char op) //opera n1 op n2 y lo guarda en N1
{
	switch (op){
		case '+': N1 = n1 + n2; break;  // +
		case '-': N1 = n1 - n2; break;  // -
		case '*': N1 = n1 * n2; break;  // *
		case '/': N1 = n1 / n2; break;  // /
		case '^': N1 = n1; break;}   // ^ Falta aca
	N2 = 0;
	USART_Transmit_String("calcular N1 y N2 con");
	USART_Transmit_char(op);
	USART_Transmit_char('\n');
}

void mostrarNumero(float num) //mustra el numero
{
	char str[6];
	sprintf(str, "%d", num);
	USART_Transmit_String(str);
	USART_Transmit_char('\n');
}

void calculadora(char entrada) //basado en diagrama estados calculadora
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
			USART_Transmit_String("Error\n"); //error
		}
		break;
	}
	
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
		//para test por usar comentar p1, p2, p3 y p4
// 		char entrada = USART_Receive();
// 		calculadora(entrada);
    }
}

