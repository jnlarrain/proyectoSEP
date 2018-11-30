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



void Keyboard_init(void)
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

