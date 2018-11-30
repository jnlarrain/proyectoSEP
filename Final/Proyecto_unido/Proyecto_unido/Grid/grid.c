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

//recibe numero y lo pone en el cuadrante 1
void first_grid(uint32_t number){
	static int flag;
	int i;
	char data;
	if (flag==1){
	SetPosition(5, 5);
	DrawString(old_string1, 0xffff, X2);
	}
	SetPosition(25, 5);
	for(i=0; i<20; i++){
		int aux = number % 10;
		number -= aux;
		number /= 10;
		data = get_char(aux);
		DrawString( data, 0x0000, X2);
	}
	flag = 1;
	
}


//recibe numero y lo pone en el cuadrante 2
void second_grid(uint16_t number){
		static int flag;
		int i;
		char data;
		if (flag==1){
			SetPosition(5, 5);
			DrawString(old_string1, 0xffff, X2);
		}
		SetPosition(25, 25);
		for(i=0; i<20; i++){
			int aux = number % 10;
			number -= aux;
			number /= 10;
			data = get_char(aux);
			DrawString( data, 0x0000, X2);
		}
		flag = 1;
}

//recibe numero y lo pone en el cuadrante 3
void third_grid(uint16_t number){
		static int flag;
		int i;
		char data;
		if (flag==1){
			SetPosition(5, 5);
			DrawString(old_string1, 0xffff, X2);
		}
		SetPosition(5, 5);
		for(i=0; i<20; i++){
			int aux = number % 10;
			number -= aux;
			number /= 10;
			data = get_char(aux);
			DrawString( data, 0x0000, X2);
		}
		flag = 1;
}

//recibe operador y lo pone en el cuadrante 4
void fourth_grid(uint16_t number){
		static int flag;
		int i;
		char data;
		if (flag==1){
			SetPosition(5, 5);
			DrawString(old_string1, 0xffff, X2);
		}
		SetPosition(5, 5);
		for(i=0; i<20; i++){
			int aux = number % 10;
			number -= aux;
			number /= 10;
			data = get_char(aux);
			DrawString( data, 0x0000, X2);
		}
		flag = 1;
}



char get_char(uint8_t number){
	char data[2];		
	sprintf(data, "%d", number);
	return data;	
}