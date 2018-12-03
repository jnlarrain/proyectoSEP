#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include "../ST7735/st7735.h"

char old_string1[20];
char old_string1b[10];
char old_string2a[10];
char old_string2b[10];
char old_string3a[10];
char old_string3b[10];
char old_string4[10];

// // recibe numero y lo pone en el cuadrante 1
// void first_grid(float number){
// 	static int flag;
// 	if (flag==1){
// 		SetPosition(25, 5);
// 		DrawString(old_string1, 0xffff, X2);
// 	}
// 	
// 	sprintf(old_string1, " %f", number);
// 	
// 	SetPosition(25, 5);	
// 	DrawString(old_string1, 0x0000, X2);
// 
// 	flag = 1;
// 	
// }


//recibe numero y lo pone en el cuadrante 1
void first_grid(int number){
	static int flag;
	if (flag==1){
		SetPosition(25, 5);
		DrawString(old_string1, 0xffff, X2);
	}
	
	int i;
	SetPosition(25, 5);
	while (number != 0){
		char digit;
		int aux;
		aux = number % 10;
		digit = transform(aux);
		DrawString(digit, 0x0000, X2);
	}	
	flag = 1;
	
}


char transform(int number){
	char digit[2]; 
	sprintf(digit, " %d", number);
	return digit;
}