#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "Keyboard/keyboard.h"

int main(void)
{
    
	
	sei();
	
    while (1) 
    {
		calculadora(entrada);
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
	teclado();
	//debouncer out
}

ISR(PCINT1_vect) //seccion1 C
{
	//debouncer in
	if      (!(PINC & (1<<PINC2))) { presionado = linea + 4;}
	else if (!(PINC & (1<<PINC3))) { presionado = linea + 5;}
	else if (!(PINC & (1<<PINC4))) { presionado = linea + 6;}
	else if (!(PINC & (1<<PINC5))) { presionado = linea + 7;}
	teclado();
	//debouncer out
}
