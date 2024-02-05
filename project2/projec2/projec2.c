/*
 * projec2.c
 *
 *  Created on: jun 28, 2024
 *      Author: MARWAN REDA
 */
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
unsigned char second=0,hour=0,minute=0;
void TIMER1_init(void){
	TCCR1A |=(1<<FOC1A); 						//non PWM
	TCCR1B |=(1<<CS12)|(1<<CS10)|(1<<WGM12) ;	//select compare and no prescalar 1024
	TCNT1=0;									//INTIAL VALUE FOR TIMER
	OCR1A=977;									// MAX COMPARE VALUE
	TIMSK |=(1<<OCIE1A);							// ENABLE interRupt with compare A
}
ISR(TIMER1_COMPA_vect)
{
	second++;
	if(second==60){
		second=0;
		minute++;
	}
	if(minute==60){
		minute=0;
		second=0;
		hour++;
	}
	if(hour==24){
		minute=0;
		second=0;
		hour=0;
	}
}
void INT0_init(void){
	GICR  |=(1<<INT0); 	//ACTIVATE MODULE INT0
	MCUCR |=(1<<ISC01);	//FALLING EDGE
	DDRD &=~(1<<PD2); 	//ACT AS INPUT READ ONLY FROM BUTTON
	PORTD |=(1<<PD2); 	//ACTIVAT PULL UP RESISTOR
}
ISR(INT0_vect)
{
	second=0;
	minute=0;
	hour=0;
	//MAKE TIMER AT ZERO
}
void INT1_init(void){
	DDRD &=~(1<<PD3);				//ACT AS INPUT READ ONLY FROM BUTTON
	MCUCR |=(1<<ISC10)|(1<<ISC11); 	// SELECT RAISING EDGE MODE
	GICR |=(1<<INT1);				//ACTIVATE MODULE INT1
}
ISR(INT1_vect)
{
	TCCR1B &=~(1<<CS10)&~(1<<CS12)&~(1<<CS11);
	//STOP CLOCK
}
void INT2_init(void){
	GICR |=(1<<INT2);	//ACTIVATE MODULE INT2
	MCUCSR &=~(1<<ISC2);	// SELECT FALLING EDGE MODE
	DDRB &=~(1<<PB2);	//ACT AS INPUT READ ONLY FROM BUTTON
	PORTB |=(1<<PB2);	//ACTIVAT PULL UP RESISTOR

}
ISR(INT2_vect)
{
	TCCR1B|=(1<<CS10)|(1<<CS12) | (1<<WGM12);
	//ENABLE CLOCK
}
void show(void){
			PORTA =0X01;  		//ENABLE FIRST PIIN
			PORTC =second%10;	//SHOW FIRST DIGIT IN SECOND VARIABLE
			_delay_ms(2);
			PORTA =0X02;		//ENABLE SECOND PIIN
			PORTC =second/10;	//SHOW SECOND DIGIT IN SECOND VARIABLE
			_delay_ms(2);
			PORTA =0X04;
			PORTC = minute%10;
			_delay_ms(2);
			PORTA =0X08;
			PORTC = minute/10;
			_delay_ms(2);
			PORTA =0X10;
			PORTC = hour%10;
			_delay_ms(2);
			PORTA =0X20;
			PORTC = hour/10;
			_delay_ms(2);

}
int main(void){
		SREG |=(1<<7);	//ENABLE GLOBAL BIT INTRRUPT
		DDRC |= 0x0F;
		PORTC &=0xF0;
		DDRA |=0xF3 ;
		PORTA&=0xF3;
		TIMER1_init();
		INT0_init();
		INT1_init();
		INT2_init();
		while(1)
		{
			show();

}
}

