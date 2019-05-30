#include "timer.h"
#include "includes.h"
#include <stdio.h>
#include <stdint.h>

//forces rvalue size promotion
//uint64_t overflow_count = 0;
int ms = 0;
int s = 0; 
int m = 0; 
char buffer[50];

void runTimer() {
    //setup lcd
    lcd_init();
    lcd_clrscr();
    setup_buttons();

    //CTC mode
    TCCR1A &= ~(1<<WGM10);
    TCCR1A &= ~(1<<WGM11);
    TCCR1B |= 1<<WGM12;
    //set prescaler to 1024
    clock_on();
	
    //enable compare interrupts
    TIMSK1 = 1<<OCIE1A;
	//set max to activate interrupt every 10ms
	OCR1A = 156;
    sei();
	
    
    while (true)
    {
        _delay_ms(5);
		if (ms == 1000) {
			++s;
			ms = 0;
		}
		if (s == 60)
		{
			++m;
			s = 0;
		}
		create_buffer();
		lcd_clrscr();
		lcd_home();
		lcd_puts(buffer);
    }
}

void create_buffer() {
	sprintf(buffer, "%d:%d.%d", m, s, ms);
}

void setup_buttons() {
    //setup buttons
    DDRD &= ~(1<<PD0);
    DDRD &= ~(1<<PD1);
    //trigger on falling
	EICRA &= ~(1<<ISC00);
    EICRA |= (1<<ISC01);
    EICRA &= ~(1<<ISC10);
    EICRA |= (1<<ISC11);
	EIMSK |= (1<<INT0);
    EIMSK |= (1<<INT1);
}

/*returns the timer time in ms
uint64_t time() {
    //16 bit timer
	uint64_t timer_max = 1<<16;
    return (TCNT1 + overflow_count * timer_max)/15.625;
}*/

void clock_on() {
    //turns clock back on to 1024 prescaler
    TCCR1B |= 1<<CS10;
    TCCR1B &= ~(1<<CS11);
    TCCR1B |= 1<<CS12;
}

//start/stop
ISR(INT0_vect) {
    //turn on/off clock by setting all bits to 0 and back to 1024 prescaler.
    static bool on = true;
    if (on) {
        //turn clock off
        TCCR1B &= ~(1<<CS10);
        TCCR1B &= ~(1<<CS11);
        TCCR1B &= ~(1<<CS12);
    } else {
        clock_on();
    }
    on = !on;
}

//reset
ISR(INT1_vect) {
    TCNT1 = 0;
    ms = 0;
	s = 0;
	m = 0;
}

/*overflow
ISR(TIMER1_OVF_vect) {
    ++overflow_count;
}*/

//called every 10ms
ISR(TIMER1_COMPA_vect) {
	ms += 10;
}
