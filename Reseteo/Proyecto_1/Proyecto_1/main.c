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
#include "teclado.h"
#include "Grilla/grilla.h"
#include "ST7735/st7735.h"
#include "SPI/SPI.h"

static unsigned int linea;
static unsigned int presionado;

#define  pi 3
#define  deboncer 10

//INICIO USART (hay que borrarlo eventualmente)
#define FOSC 16000000 	//clock
#define BAUD 57600		//ancho de banda que queremos
#define MYUBRR FOSC/16/BAUD-1	//calculo segun el data sheet
//FIN USART (hay que borrarlo eventualmente)


/*									INTERRUPCIONES

The Pin Change Interrupt Request 2 (PCI2) will trigger if any enabled PCINT[23:16] pin toggles. The Pin
Change Interrupt Request 1 (PCI1) will trigger if any enabled PCINT[14:8] pin toggles. The Pin Change
Interrupt Request 0 (PCI0) will trigger if any enabled PCINT[7:0] pin toggles.
PC2,3,4,5 -> PCINIT10,11,12,13 -> PCI1
PD4,5,6,7 -> PCINIT20,21,22,23 -> PCI2*/

//agregar bool ocupado para no accionar multiples veces interrupcion!
ISR(PCINT2_vect) //seccion2 D
{
	//INICIO OCUPADO
	//Debounder in
	if      (!(PIND & (1<<PIND4))) { presionado = linea + 0;}
	else if (!(PIND & (1<<PIND5))) { presionado = linea + 1;}
	else if (!(PIND & (1<<PIND6))) { presionado = linea + 2;}
	else if (!(PIND & (1<<PIND7))) { presionado = linea + 3;}
	mostrar(presionado);
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
	mostrar(presionado);
	//debouncer out
}

int main(void)
{
	St7735Init();
	UpdateScreen();
	tecladoInit();
	USART_Init(MYUBRR);
	USART_Transmit_String("HOLA");	
	sei();
	
	
	SetPosition(25, 5);
	DrawString("Inicio programa", 0x0000, X2);
	_delay_ms(10000);
	SetPosition(25, 5);
	DrawString("Inicio programa", 0xffff, X2);
	
	ClearScreen(0xffff);
	
	first_grid(11);
	
	
	
    while (1) 
    {
		p1
		p2
		p3
		p4
    }
}

