/** 
 * Example of TFT controller st7735 / 1.8 TFT DISPLAY /
 *
 * Copyright (C) 2016 Marian Hrinko.
 * Written by Marian Hrinko (mato.hrinko@gmail.com)
 *
 * @author      Marian Hrinko
 * @datum       08.01.2016
 * @file        main.c
 * @tested      AVR Atmega16
 * @inspiration http://www.displayfuture.com/Display/datasheet/controller/ST7735.pdf
 *              https://github.com/adafruit/Adafruit-ST7735-Library
 *              http://w8bh.net/avr/AvrTFT.pdf
 *
 */
#ifndef F_CPU
  #define F_CPU 16000000UL
#endif 



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include "ST7735/st7735.h"
#include "USART/USART.h"


void main(void)
{ 
	St7735Init();
	USART_configuration config_57600_8N1 = {57600, 8, 0, 1};
	USART_Init(config_57600_8N1);
	buff_init(&TX_buffer, 60);
	buff_init(&RX_buffer, 60);
	sei();
	UpdateScreen();	
	ClearScreen(0xffff);
	//DrawString(char *str, uint16_t color, ESizes size)
	//SetPosition(uint8_t x, uint8_t y)
	
  // draw line
  DrawLine(23, 137, 20, 20, 0x04af);
  // draw line
  DrawLine(23, 137, 21, 21, 0x04af);
  // set text position
  SetPosition(41, 10);
  // draw text
  DrawString("ST7735 DRIVER", 0x007b, X1);
  // set text position
  SetPosition(53, 26);
  // draw text
  DrawString("ATMEGA 16", 0x0000, X1);
}


ISR(USART_RX_vect)
{
	char data = UDR0;
	//DrawChar(char character, uint16_t color, ESizes size)
	push(&RX_buffer, data);
}

ISR(USART_UDRE_vect)
{
	char data = pop(&TX_buffer);
	if (data == 0) UCSR0B &= ~(1<<UDRIE0); // UDR Empty Interrupt Disable
	else UDR0 = data;
}