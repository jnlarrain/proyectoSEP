#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <string.h>
#include <avr/pgmspace.h>
#include "Keyboard/keyboard.h"
#include "ST7735/st7735.h"
#include "Grid/grid.h"
#include "SPI/SPI.h"


static unsigned int linea;
static unsigned int presionado;
static unsigned int estado = 0;

static float N1 = 0;
static float N2 = 0;
static float M = 0;
static char operador = '+';
int punto = 0;

void main(void)
{
	St7735Init();
	Keyboard_init();
	
	UpdateScreen();
	ClearScreen(0xffff);
	
	first_grid(11);
	sei();
	
	while (1)
	{
		//calculadora(entrada);
	}
}


//agregar bool ocupado para no accionar multiples veces interrupcion!
ISR(PCINT2_vect) //seccion2 D
{
	//Debouncer in
	if      (!(PIND & (1<<PIND4))) { presionado = linea + 0;}
	else if (!(PIND & (1<<PIND5))) { presionado = linea + 1;}
	else if (!(PIND & (1<<PIND6))) { presionado = linea + 2;}
	else if (!(PIND & (1<<PIND7))) { presionado = linea + 3;}
	//teclado();
	//debouncer out
	first_grid(2222);
}

ISR(PCINT1_vect) //seccion1 C
{
	//debouncer in
	if      (!(PINC & (1<<PINC2))) { presionado = linea + 4;}
	else if (!(PINC & (1<<PINC3))) { presionado = linea + 5;}
	else if (!(PINC & (1<<PINC4))) { presionado = linea + 6;}
	else if (!(PINC & (1<<PINC5))) { presionado = linea + 7;}
	//teclado();
	//debouncer out
	first_grid(1111);
}
