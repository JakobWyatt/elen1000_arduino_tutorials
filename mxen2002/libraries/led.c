/*
 * LED.c
 *
 * Created: 28/02/2019 5:24:14 PM
 *  Author: 19477143
 */ 

#include "includes.h"

void alternate(unsigned char* DDR, unsigned char* PORT)
{
	*DDR = 0xFF;//put PORTA into output mode
	*PORT = (1<<3);
	_delay_ms(500);
	while(1)//main loop
	{
		//Use XOR to toggle bit. If input is 1, the bit is flipped.
		//If it is 0, the bit is conserved.
		*PORT ^= (1<<3)|(1<<6);
		_delay_ms(500);
	}
}

void flash(unsigned char* DDR, unsigned char* PORT)
{
	*DDR = 0xFF;//put PORTA into output mode
	*PORT = 0x00;
	while(1)//main loop
	{
		_delay_ms(500);     //500 millisecond delay
		*PORT |= (1<<3);  // note here PA3 is just an alias for the number 3
		// this line is equivalent to PORTA = PORTA | 0b00001000   which writes a HIGH to pin 3 of PORTA
		_delay_ms(500);
		*PORT &= ~(1<<3); // this line is equivalent to PORTA = PORTA & (0b11110111)  which writes a HIGH to pin 3 of PORTA
	}

}

void sequence(unsigned char* DDR, unsigned char* PORT)
{
	*DDR = 0xFF;
	*PORT = 0x01;
	//1 if going up, -1 if going down
	int direction = 1;
	int counter = 0;
	while(1)
	{
		//set direction
		if (counter == 7) {
			direction = -1;
			} else if (counter == 0) {
			direction = 1;
		}
		//turn off current pin
		*PORT &= ~(1<<counter);
		//set next pin
		counter += direction;
		//turn on next pin
		*PORT |= (1<<counter);
		_delay_ms(100);
	}
}

void allOn(unsigned char* DDR, unsigned char* PORT)
{
	*DDR = 0xFF;
	*PORT = 0xFF;
}

void allOff(unsigned char* DDR, unsigned char* PORT)
{
	*DDR = 0xFF;
	*PORT = 0x00;
}


void fillBack(unsigned char* DDR, unsigned char* PORT, int fillNum) {
	*DDR = 0xFF;
	*PORT = 0x00;
	for (int i = 0; i != fillNum; ++i ) {
		*PORT |= (1<<i);
	}
}
