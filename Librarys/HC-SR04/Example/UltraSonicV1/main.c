/*
 * main.c
 *
 *  Created on: May 7, 2020
 *      Author: jure
 */


/*
 * UltraSonicV1.c
 *
 * Created: 28/04/2020 19:56:38
 * Author : Jure
 */


#include <avr/io.h>
#include "HCSR04.h"


void USART_Init( unsigned int ubrr);
void USART_Transmit( unsigned char data, FILE *stream);

static FILE mystdout = FDEV_SETUP_STREAM(USART_Transmit, NULL, _FDEV_SETUP_WRITE);

int main(){
	USART_Init(9600);
	RB_USonic_Init();
	while(1){
		printf("D: %.4f cm\n\r", RB_USonic_Get_Distance());
		_delay_ms(200);
	}
}

void USART_Init( unsigned int baud){
	unsigned int ubrr=F_CPU/16/baud-1;
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)ubrr;
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	UCSR0C = (1<<USBS0)|(3<<UCSZ00);
	stdout = &mystdout;
}

void USART_Transmit( unsigned char data, FILE *stream){
	while ( !( UCSR0A & (1<<UDRE0)) );
	UDR0 = data;
}


