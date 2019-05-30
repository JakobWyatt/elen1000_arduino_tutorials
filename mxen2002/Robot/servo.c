/*
 * servo.c
 *
 * Created: 11/04/2019 3:03:59 PM
 *  Author: 19477143
 */ 

#include "servo.h"
#include "includes.h"

void servo_init() {
	//Clear control registers
	TCCR1A = 0; TCCR1B = 0;
	//Mode 8
	TCCR1B |= (1<<WGM13);
	//Compare mode 1 0 (Duty cycle = comp/top)
	TCCR1A |= (1 << COM1A1) | (1 << COM1B1);//CLR ON UP, SET ON DOWN
	//Set prescaler to 256
	TCCR1B |= (1 << CS12); //| (1 << CS10);
	//set TOP value
	ICR1 = 625;
	
	//set compare values for both pins
	//OCR1A = 6;
	//OCR1B = 6;
	
	//set PWN pins to output
	DDRB |= (1 << PB5) | (1 << PB6);
}

void music() {
	servo_init();
	while(true)
	{
		OCR1A = 18;
		OCR1B = 43;
		_delay_ms(400);
		OCR1A = 76;
		OCR1B = 50;
		_delay_ms(400);
		OCR1A = 18;
		OCR1B = 43;
		_delay_ms(400);
		OCR1A = 76;
		OCR1B = 50;
		_delay_ms(400);
		//second bar
		OCR1B = 43;
		_delay_ms(400);
		//fancy part
		OCR1A = 18;
		_delay_ms(100);
		OCR1A = 76;
		OCR1B = 50;
		_delay_ms(300);
		
		//fancy part
		OCR1A = 18;
		_delay_ms(100);
		OCR1A = 76;
		OCR1B = 43;
		_delay_ms(300);
		
		//fancy part
		OCR1A = 18;
		_delay_ms(100);
		OCR1A = 76;
		OCR1B = 50;
		_delay_ms(300);
	}
}

void control_servo() {
	servo_init();
	//joystick setup
	adc_init();

	while (true)
	{
		//Position with horizontal joystick 1
		OCR1A = adc_read(14)/18 + 17;
		//Velocity with horizontal joystick 2
		OCR1B = adc_read(1)/146 + 43;
	}
}

//n is a 10 bit integer, representing the position of the servo
void move_position_servo(long n) {
	//Position with horizontal joystick 1
	OCR1A = n/18 + 17;
}

void move_velocity_servo(long n) {
	//Position with horizontal joystick 1
	OCR1B = n/146 + 43;
}
