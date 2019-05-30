#include "communication.h"

#include "includes.h"
#include <string.h>

void loop()
{
    /*USER INIT*/
	adc_init();
    lcd_init();
	lcd_clrscr();
    char buffer[50];
	
	int lmotor_val = 0;
	int rmotor_val = 0;
	int v = 0;
	int h = 0;
	
	DDRD &= ~(1<<PD2);
	PORTD |= 1<<PD2;

    /*COMMS INIT*/
    int control = 0;
    long recieved = 0;
    unsigned char id_r = 0;
	bool autonomous = false;
    serial2_init();

    unsigned char id_tr = NOSEND;
    milliseconds_init();
    uint32_t currentMS = 0; 
	uint32_t lastSendMS = 0;
	long sending = 0;
	uint32_t lastSendAuto = 0;

    while (true)
    {
    /*
    USER CODE: DO STUFF HERE
    */
	h = adc_read(15);
	v = adc_read(14);
	lmotor_val = (v + h - 1023)/10;
	rmotor_val = -(v - h)/10;

    /*RECIEVE THE DATA*/
	
        while (UCSR2A&(1<<RXC2)) {
			unsigned char serial_byte_in = UDR2;
			if (serial_byte_in == 0xFF && control == 0) {
				control = 1;
			} else if (control == 1) {
				control = 2;
				id_r = serial_byte_in;
			} else if (control > 1 && control < 6) {
				control++;
				recieved = (recieved<<8) + serial_byte_in;
			} else if (serial_byte_in == 0xFE && control == 6) {
				/*USER CODE: SWITCH ON ID, read from recieved*/
                if (id_r == DISTL)
                {
                    //print into buffer
				    sprintf(buffer, "%s: %4lu ", "L", (unsigned long)range(recieved));
					strcpy(buffer + 7, " ");
				    //write the buffer
				    lcd_home();
				    lcd_puts(buffer);
                } else if (id_r == DISTC)
				{
					//print into buffer
					sprintf(buffer, "%s: %4lu ", "C", (unsigned long)range(recieved));
					strcpy(buffer + 7, " ");
					//write the buffer
					lcd_goto(8);
					lcd_puts(buffer);
				} else if (id_r == DISTR)
				{
					//print into buffer
					sprintf(buffer, "%s: %4lu ", "R", (unsigned long)range(recieved));
					strcpy(buffer + 7, " ");
					//write the buffer
					lcd_goto(64);
					lcd_puts(buffer);
				}
                /*END USER CODE*/
				control = 0;
			}
	    }
		
        /*Transmit the data*/
        currentMS = milliseconds;
        if (currentMS - lastSendMS >= 100)
		{
            /*USER CODE here: assign 'sending' to data sent, and 'id_tr'
            to the corresponding variable id*/
			if ((PIND&(1 << PIND2)) == 0 && currentMS - lastSendAuto > 300)
			{
				sending = 0;
				lastSendAuto = currentMS;
				if (autonomous)
				{
					autonomous = false;
					id_tr = AUTO_OFF;
					//print into buffer
					sprintf(buffer, "A: F    ");
					//write the buffer
					lcd_goto(72);
					lcd_puts(buffer);
				} else {
					autonomous = true;
					id_tr = AUTO_ON;
					//print into buffer
					sprintf(buffer, "A: T    ");
					//write the buffer
					lcd_goto(72);
					lcd_puts(buffer);
				}
			} else if (autonomous)
			{
				id_tr = NOSEND;
			
			} else if (id_tr == NOSEND || id_tr == SERVO || id_tr == AUTO_OFF)
            {
                id_tr = LMOTOR;
                sending = lmotor_val;
            } else if (id_tr == LMOTOR) {
                id_tr = RMOTOR;
                sending = rmotor_val;
            } else if (id_tr == RMOTOR)
			{
				id_tr = SERVO;
				sending = adc_read(1);
			}
            /*END USER CODE*/
            if (id_tr != NOSEND)
            {
                lastSendMS = currentMS;
		        serial2_write_byte(0xFF);
		        serial2_write_byte(id_tr);
		        serial2_write_byte(sending>>24);
		        serial2_write_byte(sending>>16);
		        serial2_write_byte(sending>>8);
		        serial2_write_byte(sending);
		        serial2_write_byte(0xFE);
            }
		}
    }
}

int clamp(int x, int min, int max)
{
	if (x < min)
	{
		x = min;
	}
	if (x > max)
	{
		x = max;
	}
	return x;
}

double range(long read) {
	//convert adc int to mV
	return distance_2D120X(read*4.88);
}

double distance_2D120X(double voltage) {
	return 131635/voltage-11.22;
}

