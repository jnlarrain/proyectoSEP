#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <math.h>
#include <string.h>
#include "teclado.h"
#include "Grilla/grilla.h"
#include "ST7735/st7735.h"
#include "SPI/SPI.h"


//calculadora
static float N1 = 0;
static float N2 = 0;
static float M = 0;
char operador = '+';
int estado = 0;
int punto = 0;
int contadorPunto = 0;
#define  pi 3.14159 //buscar pi con 5 digitos
int ovfe = 0;

//actualizacion pantalla
static char salida[10];
char pantalla1[10] = "";
char pantalla2[10] = "";
char pantalla3 =  ' ';
char pantalla4[10] = "";

static unsigned int presionado;
static unsigned int linea;

//debuoncing
#define tref 50		//referencia debounciong en ms
static int dx = 0;	     //coordenadas debouncer
int deb = 0;      //debouncing activado
static int tdeb = -1;     //tiempo debouncing
int modo = 1;

#define neutro PORTC |= (1 << PORTC0) | (1 << PORTC1); PORTD |= (1 << PORTD2) | (1 << PORTC3);
#define  p1  PORTC &= ~(1 << PORTC0); linea = 0;
#define  p2  PORTC &= ~(1 << PORTC1); linea = 8;
#define  p3  PORTD &= ~(1 << PORTD2); linea = 16;
#define  p4  PORTD &= ~(1 << PORTD3); linea = 24;

//barrido
static int t = 0;

//INICIO USART (hay que borrarlo eventualmente)
#define FOSC 16000000 	//clock
#define BAUD 57600		//ancho de banda que queremos
#define MYUBRR FOSC/16/BAUD-1	//calculo segun el data sheet
//FIN USART (hay que borrarlo eventualmente)


/*									INTERRUPCIONES

The Pin Change Interrupt Request 2 (PCI2) will trigger if any enabled PCINT[23:16] pin toggles. The Pin
Change Interrupt Request 1 (PCI1) will trigger if any enabled PCINT[14:8] pin toggles. The Pin Change
Interrupt Request 0 (PCI0) will trigger if any enabled PCINT[7:0] pin toggles.
PC2,3,4,5 -> PCINIT10,11,12,13 -> PCI1
PD4,5,6,7 -> PCINIT20,21,22,23 -> PCI2*/
ISR(PCINT2_vect) //seccion2 D
{
	if(!deb){
		if      (!(PIND & (1<<PIND4))) { presionado = linea + 0; dx = 0;}
		else if (!(PIND & (1<<PIND5))) { presionado = linea + 1; dx = 1;}
		else if (!(PIND & (1<<PIND6))) { presionado = linea + 2; dx = 2;}
		else if (!(PIND & (1<<PIND7))) { presionado = linea + 3; dx = 3;}
		deb = 1;
		tdeb = tref;
		teclado();
	}
	else{
		tdeb = tref;
	}
}

ISR(PCINT1_vect) //seccion1 C
{
	if(!deb){
		if      (!(PINC & (1<<PINC2))) { presionado = linea + 4; dx = 4;}
		else if (!(PINC & (1<<PINC3))) { presionado = linea + 5; dx = 5;}
		else if (!(PINC & (1<<PINC4))) { presionado = linea + 6; dx = 6;}
		else if (!(PINC & (1<<PINC5))) { presionado = linea + 7; dx = 7;}
		deb = 1;
		tdeb = tref;
		teclado();
	}
	else{
		tdeb = tref;
	}
}

ISR(TIMER2_COMPA_vect) { //timer cada 1ms
	if (tdeb > 0)
	{
		tdeb--;
	}
	else{
		deb = 0;
	}
	t++;
	if (t == 8)
	{
		t = 0;
	}
	//barrido
	if((t == 1)||(t == 3)||(t == 5)||(t == 7))	{ neutro }
	else{
		switch(t){
			case 0: p1 break;
			case 2: p2 break;
			case 4: p3 break;
			case 6: p4 break;
		}
	}
}

int main(void)
{
	St7735Init();
	UpdateScreen();
	tecladoInit();
	USART_Init(MYUBRR);
	USART_Transmit_String("HOLA");	
	sei();
		
	SetPosition(25, 55);
	DrawString("Inicio programa", 0x0000, X2);
	_delay_ms(10000);
	ClearScreen(0xffff);	

    while (1) 
    {
    }
}



void teclado(void) //interpretacion de seleccion, mapea input
{
	//presionado es el input
	//mapear presionado con respectiva entrada en entradas
	//cambiar orden en este string segun se necesite
	char entradas[32] = "=396xtln/-*+apg^0174osqm.285fced";
	char ent = entradas[presionado];
	//calculadora(entrada); //le pasa la entrada a la calculadora
	
	//FIN OCUPADO
	//MOSTRAR PRESIONADO POR USART (por ahora)
	char str[2];
	calculadora(ent);
	if (ent == 'o'){
		ClearScreen(0xffff);
	}
	else if(ent == 'f') {
		ClearScreen(0x0000);
	}
	sprintf(str, "%d", presionado);
	USART_Transmit_String(str);
	USART_Transmit_String(" -> ");
	USART_Transmit_char(ent);
	USART_Transmit_char('\n');
	}
	
	


//a partir de aca -> esto va en avr
void mostrar(int posicion)
{
	int ver = 1;
	switch(posicion)
	{
		case 0: //limpiamos //0 limpia la pantalla!
		//strcpy(pantalla1,"         ");
		strcpy(pantalla2,"         ");
		pantalla3 = ' ';
		strcpy(pantalla4,"         ");
		ver = 0; break;
		case 1: floatToString(N1,salida);strcpy(pantalla1,salida); break;
		case 2: floatToString(N2,salida);strcpy(pantalla2,salida); break;
		case 3: pantalla3 = operador; break;		
		case 4: floatToString(N1,salida);strcpy(pantalla4,salida); break;		
		case 5: ClearScreen(0x0000);break;
		case 6: //error
		strcpy(pantalla1,"Error");
		break;
	}
	if(ver)
	{
		//actualizar pantalla con respectivos strings aca [borrar estos Serial.println(), son de arduino]
	
	if(pantalla1!= "         ") {first_grid(pantalla1);} second_grid(pantalla2); thrid_grid(pantalla4);fourth_grid(pantalla3);
	
}
}

void floatToString(float numero, char* str) //recibe un float y el string donde guardar
{
	int decimales = 0;
	if((numero > 9999999) || (numero < -9999999)) //mas de 7 digitos
	{
		strcpy(str,"       ovf");
		ovfe = 1;
	}
	else if (numero >= 0) //positivos
	{
		if     (numero <     10) {decimales = 5;}
		else if(numero <    100) {decimales = 4;}
		else if(numero <   1000) {decimales = 3;}
		else if(numero <  10000) {decimales = 2;}
		else if(numero < 100000) {decimales = 1;}
		dtostrf(numero,10, decimales, str);
	}
	else if (numero < 0)  //negativos
	{
		if     (numero >    -10) {decimales = 5;}
		else if(numero >   -100) {decimales = 4;}
		else if(numero >  -1000) {decimales = 3;}
		else if(numero > -10000) {decimales = 2;}
		else if(numero >-100000) {decimales = 1;}
		dtostrf(numero,10, decimales, str);
	}
}

void actualizarNumero(int n, char num) //actualiza numero n con num
{
	if(contadorPunto < 7){
		if(n == 1)//N1
		{
			float sumar = 10;
			if (punto){ sumar = 1; }
			switch (num){
				case '0': N1 = N1*sumar + 0/pow(10,contadorPunto); break;
				case '1': N1 = N1*sumar + 1/pow(10,contadorPunto); break;
				case '2': N1 = N1*sumar + 2/pow(10,contadorPunto); break;
				case '3': N1 = N1*sumar + 3/pow(10,contadorPunto); break;
				case '4': N1 = N1*sumar + 4/pow(10,contadorPunto); break;
				case '5': N1 = N1*sumar + 5/pow(10,contadorPunto); break;
				case '6': N1 = N1*sumar + 6/pow(10,contadorPunto); break;
				case '7': N1 = N1*sumar + 7/pow(10,contadorPunto); break;
				case '8': N1 = N1*sumar + 8/pow(10,contadorPunto); break;
				case '9': N1 = N1*sumar + 9/pow(10,contadorPunto); break;
			case '.': punto = 1;  break;}
			if (punto){ contadorPunto++; }
			mostrar(0); mostrar(1);
			}else{ //N2
			float sumar = 10;
			if (punto){ sumar = 1; }
			switch (num){
				case '0': N2 = N2*sumar + 0/pow(10,contadorPunto); break;
				case '1': N2 = N2*sumar + 1/pow(10,contadorPunto); break;
				case '2': N2 = N2*sumar + 2/pow(10,contadorPunto); break;
				case '3': N2 = N2*sumar + 3/pow(10,contadorPunto); break;
				case '4': N2 = N2*sumar + 4/pow(10,contadorPunto); break;
				case '5': N2 = N2*sumar + 5/pow(10,contadorPunto); break;
				case '6': N2 = N2*sumar + 6/pow(10,contadorPunto); break;
				case '7': N2 = N2*sumar + 7/pow(10,contadorPunto); break;
				case '8': N2 = N2*sumar + 8/pow(10,contadorPunto); break;
				case '9': N2 = N2*sumar + 9/pow(10,contadorPunto); break;
			case '.': punto = 1;  break;}
			if (punto){ contadorPunto++; }
		mostrar(2);}
	}
}

void calcular1(float n, char op) //opera el operador op con un solo numero n y lo guarda en N1
{
	switch (op){
		case 'q': N1 = sqrt(N1); break;  //raiz cuadrada
		case 'l': N1 = log(N1); break;   //logaritmo natural
		case 'e': N1 = exp(N1); break;   //exp(x)
		case 's': N1 = sin(N1); break;   //sen(x)
		case 'c': N1 = cos(N1); break;   //cos(x)
		case 't': N1 = tan(N1); break;   //tg(x)
		case 'g': N1 = log10(N1); break; //log(x)
	case 'x': N1 = pow(N1,(-1.0)); break;} //x^-1
	N2 = 0;
	operador = op;
	mostrar(3);
	mostrar(4);
}



void calcular2(float n1, float n2, char op) //opera n1 op n2 y lo guarda en N1
{
	switch (op){
		case '+': N1 = N1 + N2; break;  // +
		case '-': N1 = N1 - N2; break;  // -
		case '*': N1 = N1 * N2; break;  // *
		case '/': N1 = N1 / N2; break;  // /  ver el caso N2 = 0!!!
	case '^': N1 = pow(N1,N2); break;}   // ^ Falta aca
	N2 = 0;
	mostrar(4);
}

void calculadora(char entrada) //basado en diagrama estados calculadora
{
	if(ovfe)
	{
		estado = 4;
	}
	switch (estado)
	{
		case 0: //estado 0
		if((entrada == '0')||
		(entrada == '1')||
		(entrada == '2')||
		(entrada == '3')||
		(entrada == '4')||
		(entrada == '5')||
		(entrada == '6')||
		(entrada == '7')||
		(entrada == '8')||
		(entrada == '9')||
		(entrada == '.')) { actualizarNumero(1,entrada); estado = 0;}
		else if ((entrada == '+')||
		(entrada == '-')||
		(entrada == '*')||
		(entrada == '/')||
		(entrada == '^')) {operador = entrada; estado = 1;mostrar(3);}
		else if ((entrada == 'q')||
		(entrada == 'l')||
		(entrada == 'e')||
		(entrada == 's')||
		(entrada == 'c')||
		(entrada == 't')||
		(entrada == 'g')||
		(entrada == 'x')) {calcular1(N1,entrada);   estado = 3;}
		else{
			switch (entrada){
				case 'p': N1 = pi;              estado = 3; mostrar(0); mostrar(1); break;
				case 'm': N1 = M; N2 = 0;       estado = 3; mostrar(0); mostrar(1); break;
				case 'd': M = M + N1; N1 = 0; N2 = 0; estado = 0; mostrar(0); break;
				case 'n': M = M - N1; N1 = 0; N2 = 0; estado = 0; mostrar(0); break;
				case 'a': M = 0;  N1 = 0; N2 = 0;     estado = 0; mostrar(0); break;
				case 'f': mostrar(5); estado = 4; break;
			}
		}break;
		
		case 1: //estado 1
		punto = 0;
		contadorPunto = 0;
		if((entrada == '0')||
		(entrada == '1')||
		(entrada == '2')||
		(entrada == '3')||
		(entrada == '4')||
		(entrada == '5')||
		(entrada == '6')||
		(entrada == '7')||
		(entrada == '8')||
		(entrada == '9')||
		(entrada == '.')) { N2 = 0; actualizarNumero(2,entrada); estado = 2;}
		else if ((entrada == '+')||
		(entrada == '-')||
		(entrada == '*')||
		(entrada == '/')||
		(entrada == '^')) {operador = entrada; estado = 1; mostrar(3);}
		else if ((entrada == 'q')||
		(entrada == 'l')||
		(entrada == 'e')||
		(entrada == 's')||
		(entrada == 'c')||
		(entrada == 't')||
		(entrada == 'g')||
		(entrada == 'x')||
		(entrada == 'm')||
		(entrada == 'd')||
		(entrada == 'n')) { estado = 4; mostrar(0); mostrar(6);} //error
		else{
			switch (entrada){
				case 'p': N2 = pi;  calcular2(N1,N2,operador);  estado = 3; mostrar(2); break;
				case 'a': M = 0;  N1 = 0; N2 = 0; punto = 0;    contadorPunto = 0; estado = 0; mostrar(0);   break;
				case '=': calcular2(N1,N1,operador);    estado = 0; break;
				case 'f': mostrar(5); estado = 4; break;
			}
		}
		break;
		
		case 2: //estado 2
		if((entrada == '0')||
		(entrada == '1')||
		(entrada == '2')||
		(entrada == '3')||
		(entrada == '4')||
		(entrada == '5')||
		(entrada == '6')||
		(entrada == '7')||
		(entrada == '8')||
		(entrada == '9')||
		(entrada == '.')) { actualizarNumero(2,entrada); estado = 2;}
		else if ((entrada == '+')||
		(entrada == '-')||
		(entrada == '*')||
		(entrada == '/')||
		(entrada == '^')) { calcular2(N1,N2,operador); estado = 1;  operador = entrada; mostrar(0); mostrar(1); mostrar(3); }
		else if ((entrada == 'q')||
		(entrada == 'l')||
		(entrada == 'e')||
		(entrada == 's')||
		(entrada == 'c')||
		(entrada == 't')||
		(entrada == 'g')||
		(entrada == 'x')) {calcular2(N1,N2,operador); calcular1(N1,entrada); estado = 3;}
		else{
			switch (entrada){
				case 'p': N2 = pi;  calcular2(N1,N2,operador);  estado = 3; mostrar(2); break;
				case 'a': M = 0;  N1 = 0; N2 = 0; punto = 0;  contadorPunto = 0; estado = 0; mostrar(0);  break;
				case '=': calcular2(N1,N2,operador);  estado = 3; mostrar(2);   break;
				case 'm': N1 = M; N2 = 0;      estado = 3;  mostrar(0); mostrar(1);     break;
				case 'd': calcular2(N1,N2,operador); M = M + N1; N1 = 0; N2 = 0;  estado = 0; mostrar(0);  break;
				case 'n': calcular2(N1,N2,operador); M = M - N1; N1 = 0; N2 = 0;  estado = 0; mostrar(0);  break;
				case 'f': mostrar(5); estado = 4; break;
			}
		}
		break;
		
		case 3: //estado 3
		punto = 0;
		contadorPunto = 0;
		if((entrada == '0')||
		(entrada == '1')||
		(entrada == '2')||
		(entrada == '3')||
		(entrada == '4')||
		(entrada == '5')||
		(entrada == '6')||
		(entrada == '7')||
		(entrada == '8')||
		(entrada == '9')||
		(entrada == '.')) { N1 = 0; actualizarNumero(1,entrada); estado = 0;}
		else if ((entrada == '+')||
		(entrada == '-')||
		(entrada == '*')||
		(entrada == '/')||
		(entrada == '^')) {operador = entrada; mostrar(0); estado = 1; mostrar(1); mostrar(3);  }
		else if ((entrada == 'q')||
		(entrada == 'l')||
		(entrada == 'e')||
		(entrada == 's')||
		(entrada == 'c')||
		(entrada == 't')||
		(entrada == 'g')||
		(entrada == 'x')) {calcular1(N1,entrada);   estado = 3;}
		else{
			switch (entrada){
				case 'p': N1 = pi;              estado = 3; mostrar(0); mostrar(1); break;
				case 'm': N1 = M; N2 = 0;       estado = 3; mostrar(0); mostrar(1); break;
				case 'd': M = M + N1; N1 = 0; N2 = 0;   estado = 0; mostrar(0); break;
				case 'n': M = M - N1; N1 = 0; N2 = 0;   estado = 0; mostrar(0); break;
				case 'a': M = 0;  N1 = 0; N2 = 0; punto = 0; contadorPunto = 0; estado = 0; mostrar(0); break;
				case 'f': mostrar(5); estado = 4; break;
			}
		}
		break;
		
		case 4: //estado 4
		if ((entrada == 'o'))
		{
			M = 0;  N1 = 0; N2 = 0; estado = 0; punto = 0; contadorPunto = 0; mostrar(0); ovfe = 0;
			//prender pantalla
		}
		else if ((entrada == 'f'))
		{
			mostrar(5); estado = 4;
		}
		else{
			mostrar(0); mostrar(6); estado = 4; //error
		}
		break;
	}
}
//hasta aca va en avr

	