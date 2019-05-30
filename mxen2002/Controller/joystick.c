/*
 * joystick.c
 *
 * Created: 7/03/2019 3:40:03 PM
 *  Author: 19477143
 */ 

#include "joystick.h"
#include "includes.h"
#include "led.h"
#include "utility.h"

void displayJoystick(int pin) {
	adc_init();
	while(true) {
		int ledNum = adc_read(pin)/113;
		fillBack(&DDRA, &PORTA, ledNum );
	}
}

void switchJoystickDirOnPress() {
	adc_init();
	pullup(&DDRC, &PORTC, 7);
	int dir = 0;
	while (true) {
		if (!isOff(&PINC, 7)) {
			if (dir == 0) {
				dir = 1;
			} else {
				dir = 0;
			}
		}
		int ledNum = adc_read(dir)/113;
		fillBack(&DDRA, &PORTA, ledNum );
	}
}

void detectTemp() {
	adc_init();
	while (true) {
		int ledNum = (adc_read(0)-522)/8;
		fillBack(&DDRA, &PORTA, ledNum);
	}
}
