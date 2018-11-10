#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <avr/io.h>
#include <util/delay.h>
#include "SPI.h"

#define BIT0 0x01
#define BIT1 0x02
#define BIT2 0x04
#define BIT3 0x08
#define BIT4 0x10
#define BIT5 0x20
#define BIT6 0x40
#define BIT7 0x80

/*
* PIN MAP
*
*
* SCK		PB5
* MISO		PB4
* MOSI		PB3
* DC		PB1
* RESET		PB0
* TFT		PD2
* SD		PD3
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
	DDRD  |= BIT2 | BIT3;
	PORTD |= BIT2 | BIT3;
	PORTB |= BIT4;
	SPCR  |= BIT6 | BIT4;// | BIT0 | BIT1;
	// SPI2X - Prescaler fclk/2 = 8MHz
	SPSR |= (1 << SPI2X);
}

uint8_t CommandSend(uint8_t data)
{
	// chip enable - active low
	PORTD &= ~BIT2;
	// command (active low)
	PORTB &= ~BIT1;
	// transmitting data
	SPDR = data;
	// wait till data transmit
	while (!(SPSR & (1 << SPIF)));
	// chip disable - idle high
	PORTD |= BIT2;
	// return received data
	return SPDR;
}

void SPI_transmit_char(uint8_t data)
{
	SPDR = data; //
	//PORTB &= ~BIT1;
	while ( !(SPSR & (1<<SPIF)) ); // Wait data transmission
	//PORTB |= BIT1;
}


char SPI_receive_char(void)
{
	uint8_t data;
	SPI_transmit_char(0xFF);
	data = SPDR;
	return data;
}