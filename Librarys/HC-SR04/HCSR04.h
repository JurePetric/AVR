/*
 * HC_SR04.h
 *
 * Created: 8. 05. 2020 11:06:26
 *  Author: Jure
 */ 


#ifndef HCSR04_H_
#define HCSR04_H_

#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>

#define RB_USONIC_DDR	DDRD
#define RB_USONIC_OUT	PORTD
#define RB_USONIC_IN	PIND
#define RB_USONIC_ECHO	3
#define RB_USONIC_TRIG	4

void RB_USonic_Init();
float RB_USonic_Get_Distance();

#endif /* HCSR04_H_ */