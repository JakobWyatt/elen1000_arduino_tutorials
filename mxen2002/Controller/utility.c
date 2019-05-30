/*
 * utility.c
 *
 * Created: 7/03/2019 4:15:04 PM
 *  Author: 19477143
 */ 

#include "utility.h"
//for debugging
#include "led.h"

void pullup(volatile unsigned char* DDR, volatile unsigned char* PORT, int pin) {
	//set to input mode
	*DDR &= ~(1<<(pin));
	*PORT |= 1<<pin;
}

bool isOff(volatile unsigned char* PIN, int pinNum) {
	return (*PIN)&(1<<7);
}
