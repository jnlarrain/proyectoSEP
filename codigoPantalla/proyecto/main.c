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


void main(void)
{ 
	St7735Init();
	UpdateScreen();	
	ClearScreen(0xffff);
	//DrawString(char *str, uint16_t color, ESizes size)
	//SetPosition(uint8_t x, uint8_t y)
	
  // draw line
  DrawLine(23, 137, 20, 20, 0x04af);
  // draw line
  DrawLine(23, 137, 21, 21, 0x04af);
  // set text position
  SetPosition(5, 2);
  // draw text
  DrawString("ST7735 DRIVER test", 0x007b, X2);
  // set text position

}

