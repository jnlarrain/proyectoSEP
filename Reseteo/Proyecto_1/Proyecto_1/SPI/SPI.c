#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <avr/io.h>
#include <util/delay.h>
#include "SPI.h"



/*
* PIN MAP
*
*
* SCK		PB5
* MISO		PB4
* MOSI		PB3
* DC		PB1
* RESET		PB0
* TFT		PB2
*/

void HardwareReset(void)
{
	// Actiavte pull-up register logical high on pin RST
	PORTB |= BIT0;
	// DDR as output
	DDRB  |= BIT0;
	// delay 200 ms
	_delay_ms(200);
	// Reset Low
	PORTB &= ~BIT0;
	// delay 200 ms
	_delay_ms(200);
	// Reset High
	PORTB |= BIT0;
}


void SpiInit(void)
{

	// Output: SCK, MOSI, CS_LD, DC_LD
	DDRB  |= BIT5 | BIT3 | BIT2 | BIT1;
	DDRB  &= ~BIT4;
	PORTB |= BIT2 | BIT4;
	SPCR  |= BIT6 | BIT4;// | BIT0 | BIT1;
	// SPI2X - Prescaler fclk/2 = 8MHz
	SPSR |= (1 << SPI2X);
}

uint8_t CommandSend(uint8_t data)
{
	// chip enable - active low
	PORTB &= ~BIT2;
	// command (active low)
	PORTB &= ~BIT1;
	// transmitting data
	SPDR = data;
	// wait till data transmit
	while (!(SPSR & (1 << SPIF)));
	// chip disable - idle high
	PORTB |= BIT2;
	// return received data
	return SPDR;
}

void SPI_transmit_char(uint8_t data)
{
	SPDR = data; //
	while ( !(SPSR & (1<<SPIF)) ); // Wait data transmission
}


char SPI_receive_char(void)
{
	uint8_t data;
	SPI_transmit_char(0xFF);
	data = SPDR;
	return data;
}