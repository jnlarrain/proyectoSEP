

#ifndef INCFILE1_H_
#define INCFILE1_H_


#define  pi 3

#define neutro _delay_ms(1); PORTC |= (1 << PORTC0) | (1 << PORTC1); PORTD |= (1 << PORTD2) | (1 << PORTC3);
#define  p1  PORTC &= ~(1 << PORTC0); neutro linea = 0;
#define  p2  PORTC &= ~(1 << PORTC1); neutro linea = 8;
#define  p3  PORTD &= ~(1 << PORTD2); neutro linea = 16;
#define  p4  PORTD &= ~(1 << PORTD3); neutro linea = 24;

#define  deboncer 10


void Keyboard_init(void);

#endif /* INCFILE1_H_ */