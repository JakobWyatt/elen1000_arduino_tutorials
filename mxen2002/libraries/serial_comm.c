/*
 * serial_comm.c
 *
 * Created: 18/04/2019 3:22:02 PM
 *  Author: 19477143
 */ 
#include "serial_comm.h"
#include "includes.h"
#include "servo.h"

void recieve_control() {
	serial2_init();
	servo_init();
	
	int control = 0;
	long recieved = 0;
	unsigned char id = 0;
	
	while (true) {
		if (UCSR2A&(1<<RXC2)) {
			unsigned char serial_byte_in = UDR2;
			if (serial_byte_in == 0xFF && control == 0) {
				control = 1;
			} else if (control == 1) {
				control = 2;
				id = serial_byte_in;
			} else if (control > 1 && control < 6) {
				control++;
				recieved = (recieved<<8) + serial_byte_in;
			} else if (serial_byte_in == 0xFE && control == 6) {
				move_position_servo(recieved);
				control = 0;
			}
		}
	}
}

void send_control() {
	serial2_init();
	adc_init();
	milliseconds_init(); 
	
	int currentMS = 0; 
	int lastSendMS = 0;
	long sending = 0; 
	
	while(true)
	{
		currentMS = milliseconds;
			
		if (lastSendMS - currentMS >= 100)
		{
			lastSendMS = currentMS;
			sending = adc_read(14);
			serial2_write_byte(0xFF);
			serial2_write_byte(0x00);
			serial2_write_byte(sending>>24);
			serial2_write_byte(sending>>16);
			serial2_write_byte(sending>>8);
			serial2_write_byte(sending);
			serial2_write_byte(0xFE);
		}
	} 
}