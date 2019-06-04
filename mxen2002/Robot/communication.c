#include "communication.h"

#include "includes.h"

void loop_comms()
{
	adc_init();
	motor_init();
	servo_init();
    /*COMMS INIT*/
    int control = 0;
    long recieved = 0;
    unsigned char id_r = NOSEND;
	bool autonomous = false;
    serial2_init();

    unsigned char id_tr = NOSEND;
    milliseconds_init();
    uint32_t currentMS = 0; 
	uint32_t lastSendMS = 0;
	uint32_t turnMS = 0;
	uint32_t directedMS = 0;
	long sending = 0;
	int ldist = 0;
	int rdist = 0;
	int cdist = 0;
	int lmes = 0;
	int rmes = 0;
	int cmes = 0;
	unsigned long lm = 0;
	unsigned long rm = 0;

    while (true)
    {
		lmes = (lmes*4 + adc_read(1))/5;
		rmes = (rmes*4 + adc_read(2))/5;
		cmes = (cmes*4 + adc_read(0))/5;
		ldist = (int)range(lmes);
		rdist = (int)range(rmes);
		cdist = (int)range(cmes);
		currentMS = milliseconds;

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
                if (id_r == AUTO_ON)
                {
                    autonomous = true;
				} else if (id_r == AUTO_OFF)
				{
					autonomous = false;
                } else if (id_r == RMOTOR)
                {
                    rm = recieved;
					directedMS = currentMS;
                } else if (id_r == LMOTOR)
				{
					lm = recieved;
					directedMS = currentMS;
				} else if (id_r == SERVO)
				{
					move_velocity_servo(-(recieved-512) + 512);
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
			if (id_tr == NOSEND || id_tr == DISTC)
			{
				id_tr = DISTL;
				sending = lmes;
			} else if (id_tr == DISTL) {
				id_tr = DISTR;
				sending = rmes;
			} else if (id_tr == DISTR)
			{
				id_tr = DISTC;
				sending = cmes;
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
		
		if (!autonomous && currentMS - directedMS < 350)
		{
			control_left_motor(lm);
			control_right_motor(rm);
			
		} else if (!autonomous) {
			control_left_motor(0);
			control_right_motor(0);
		} else if (currentMS - turnMS > 10) {
			
			turnMS = currentMS;
			if (cdist < 120)
			{
				if (ldist > rdist)
				{
					//turn left
					control_left_motor(-20);
					control_right_motor(50);
				} else {
					control_left_motor(50);
					control_right_motor(-20);
				}
			} else
			{
				if (ldist < rdist)
				{
					//follow left
					control_left_motor(50 - (clamp(ldist, 50, 170) - 110)/2);
					control_right_motor(50 + (clamp(ldist, 50, 170) - 110)/2);
				} else {
					control_left_motor(50 + (clamp(rdist, 50, 170) - 110)/2);
					control_right_motor(50 - (clamp(rdist, 50, 170) - 110)/2);
				}
			}
		}
    }
}

void motor_init() {
	//Clear control registers
	TCCR1A = 0; TCCR1B = 0;
	//Mode 8
	TCCR1B |= (1<<WGM13);
	//Compare mode 1 0 (Duty cycle = comp/top)
	TCCR1A |= (1 << COM1A1) | (1 << COM1B1);//CLR ON UP, SET ON DOWN
	//Set prescaler to 256
	TCCR1B |= (1 << CS12);
	//set TOP value
    //This is calculated by TOP = 16*10^6 / (2 * PRE * f) ==
	ICR1 = 104;
	
	//set PWN pins to output
	DDRB |= (1 << PB5) | (1 << PB6);
	
	//Control direction
	DDRA = 0xFF;
}

//n is an integer from -100 -> 100
void control_right_motor(int n) {
    //Switch position to backwards if n is negative
	//right motor has C=2, D=3
	//left motor has C=4, D=5
	if (n > 0)
	{
		PORTA |= (1<<PA3);
		PORTA &= ~(1<<PA2);
	} else 
	{
		PORTA &= ~(1<<PA3);
		PORTA |= (1<<PA2);
	}
	OCR1A = 1.04 * abs(n);
}

//n is an integer from -100 -> 100
void control_left_motor(int n) {
    //Switch position to backwards if n is negative
	if (n < 0)
	{
		PORTA |= (1<<PA0);
		PORTA &= ~(1<<PA1);
	} else
	{
		PORTA &= ~(1<<PA0);
		PORTA |= (1<<PA1);
	}
	OCR1B = 1.04 * abs(n);
}

void servo_init() {
	//Clear control registers
	TCCR3A = 0; TCCR3B = 0;
	//Mode 8
	TCCR3B |= (1<<WGM33);
	//Compare mode 1 0 (Duty cycle = comp/top)
	TCCR3A |= (1 << COM3A1) | (1 << COM3B1);//CLR ON UP, SET ON DOWN
	//Set prescaler to 8
	TCCR3B |= (1 << CS31);
	//set TOP value
	ICR3 = 20000;
	
	//set PWM pins to output
	DDRE |= (1 << PE3);
}

void move_velocity_servo(long n) {
	//Position with horizontal joystick 1
	OCR3A = n/4.55 + 1487;
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
