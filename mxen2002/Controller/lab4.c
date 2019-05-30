/*
 * lab4.c
 *
 * Created: 21/03/2019 3:03:17 PM
 *  Author: 19477143
 */ 
#include "lab4.h"
#include "includes.h"

void blink_lamp() {
	DDRA |= (1<<PA2);
	while (true) {
		_delay_ms(250);
		PORTA ^= (1<<PA2);
	}
}

void setup_lamp() {
	DDRA |= (1<<PA2);
	PORTA ^= (1<<PA2);
}

void setup_interrupt() {
	DDRD = 0;
	EICRA |= (1<<ISC01);
	EICRA &= ~(1<<ISC00);
	EIMSK |= (1<<INT0);
	sei();
}
/*
ISR(INT0_vect) {
	PORTA ^= (1<<PA2);
}
*/