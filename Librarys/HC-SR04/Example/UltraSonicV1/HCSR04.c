/*
 * HC_SR04.c
 *
 * Created: 8. 05. 2020 11:06:03
 *  Author: Jure
 */ 

#include "HCSR04.h"

volatile unsigned int count=0;
volatile int EXT_int_front=0;

void RB_USonic_Init(){
	RB_USONIC_DDR |= (1<<RB_USONIC_TRIG);
	RB_USONIC_OUT &=~(1<<RB_USONIC_TRIG);
	//Set interrupt INT1
	EICRA |= (1<<ISC10);	//Any change on INT1 generate interrupt
	EIMSK |= (1<<INT1);		//Enable INT1 interrupt
	//Set Timer1 16-bit
	TCCR1A = 0x00;
	TCNT1 = 0;
	sei();
}

float RB_USonic_Get_Distance(){
	float distance=0;
	PORTD |=(1<<RB_USONIC_TRIG);
	_delay_us(15);
	PORTD &=~(1<<RB_USONIC_TRIG);
	distance=(0.0323/2)*(float)count*0.5;
	count=0;
	return distance;
}

ISR(INT1_vect){
	if(EXT_int_front == 0){
		EXT_int_front=1;
		TCCR1B |= (1<<CS11);
	}
	else if(EXT_int_front == 1){
		EXT_int_front=0;
		TCCR1B = 0X00;
		count=TCNT1;
		TCNT1=0;
	}
}
