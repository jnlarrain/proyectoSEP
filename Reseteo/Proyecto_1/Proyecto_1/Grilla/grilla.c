#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include "../ST7735/st7735.h"


char old_string1[20];
char old_string4[10];


//recibe numero y lo pone en el cuadrante 1
void first_grid(char letter){
	static int flag;
	static int pos = 0;
	if (flag==1){
		SetPosition(25, 5);
		DrawString(old_string1, 0xffff, X2);
		strcat(old_string1,"");
		SetPosition(25, 5);
		pos = 0;
	}
	old_string1[pos] = letter;
	pos +=1;
	SetPosition(25, 5);
	DrawString(old_string1, 0x0000, X2);	
	flag = 1;
}

