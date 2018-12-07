#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include "../ST7735/st7735.h"


char old_string1[10];
char old_string2[10];
char old_string3[10];
char old_string4;

//recibe numero y lo pone en el cuadrante 1
void first_grid(char letter[10]){
	int i;
	if (letter=='a'){
		SetPosition(35, 25);
		DrawString(old_string1, 0xffff, X2);
	}
	else{
		SetPosition(35, 25);
		DrawString(old_string1, 0xffff, X2);
		for(i=0; i<10; i++){
		old_string1[i] = letter[i];
		}
		SetPosition(35, 25);
		DrawString(old_string1, 0x0000, X2);
	}
}



void second_grid(char letter[10]){
	int i;
	if (letter=='a'){
		SetPosition(35, 45);
		DrawString(old_string2, 0xffff, X2);
		for(i=0; i<10; i++){
			old_string2[i] = ' ';
		}
	}
	else{
		SetPosition(35, 45);
		DrawString(old_string2, 0xffff, X2);
		for(i=0; i<10; i++){
			old_string2[i] = letter[i];
		}
		SetPosition(35, 45);
		DrawString(old_string2, 0x0000, X2);
	}
}

void thrid_grid(char letter[10]){
	int i;
	if (letter=='a'){
		SetPosition(35, 65);
		DrawString(old_string3, 0xffff, X2);		
		for(i=0; i<10; i++){
			old_string3[i] = ' ';
		}
	}
	else{
		SetPosition(35, 65);
		DrawString(old_string3, 0xffff, X2);
		for(i=0; i<10; i++){
			old_string3[i] = letter[i];
		}
		SetPosition(35, 65);
		DrawString(old_string3, 0x0000, X2);
	}
}


void fourth_grid(char letter){
		if (letter=='a'){
		SetPosition(15, 45);
		DrawChar(letter, 0xffff, X2);	
		old_string4 = ' ';
		}
		else{
		SetPosition(15, 45);
		DrawChar(letter, 0xffff, X2);
		old_string4 = letter;
		SetPosition(15, 45);
		DrawChar(letter, 0x0000, X2);
		}
}



