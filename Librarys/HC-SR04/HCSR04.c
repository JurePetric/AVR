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
	RB_USONIC_DDR |= (1<<RB_USONIC_TRIG);	//Define TRIGER as output	
	RB_USONIC_PORT &=~(1<<RB_USONIC_TRIG);	//Start TRIGER with 0 (for precaution)
	//Set interrupt INT1
	EICRA |= (1<<ISC10);					//Any change on INT1 (PORT D3) generate interrupt
	EIMSK |= (1<<INT1);						//Enable INT1 interrupt
	//Set Timer1 16-bit
	TCCR1A = 0x00;							//Set Timer1 register TCCR1A on 0 (no waveform generations)
	TCNT1 = 0;								//TCNT1 (counter) set on 0 (for precaution)
	sei();
}

float RB_USonic_Get_Distance(){
	float distance=0;	
	RB_USONIC_PORT |=(1<<RB_USONIC_TRIG);	//Start TRIGER pulse to begin measurment
	_delay_us(15);							//Pulse width min 10 uS
	RB_USONIC_PORT &=~(1<<RB_USONIC_TRIG);	//End pulse
	distance = (0.0323/2)*(float)count*0.5;	//Calculate distance distance=speed_of_sound*time/2
	count=0;								//Clear counter
	return distance;
}

ISR(INT1_vect){
	if(EXT_int_front == 0){					//When first change on INT1 ocure (feedback of ECHO); positive front of pulse
		EXT_int_front=1;					//Set flag that positive front ocure
		TCCR1B |= (1<<CS11);				//Start timer1 with setting the divider with 8
	}
	else if(EXT_int_front == 1){			//When second change on INT1 ocure (feedback of ECHO); negative front of pulse
		EXT_int_front=0;					//Set flag that negative front ocure
		TCCR1B = 0X00;						//Stop timer1 with setting the divider on 0
		count=TCNT1;						//Read counter (of timer1) into variable
		TCNT1=0;							//Reset counter on 0
	}
}
