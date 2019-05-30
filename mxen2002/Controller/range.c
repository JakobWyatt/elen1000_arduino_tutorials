#include <stdio.h>
#include "range.h"
#include "includes.h"
#include "utility.h"

double range(int pin) {
    //convert adc int to mV
    return distance_2D120X(adc_read(pin)*4.88);
}

double distance_2D120X(double voltage) {
    return 131635/voltage-11.22;
}

void display_range() {
    adc_init();
    lcd_init();
    lcd_clrscr();
    char topbuffer[50];
	char bottombuffer[50] = "";
	double rangeMes;
	DDRA = 0;
    while (true) {
		_delay_ms(200);
		rangeMes = range(0);
		sprintf(topbuffer, "%d", (int)rangeMes);
        if ((PINA)<<7 == 0) {
			sprintf(bottombuffer, "%d", (int)rangeMes);
        }
		lcd_clrscr();
		lcd_home();
        lcd_puts(topbuffer);
		lcd_goto(64);
		lcd_puts(bottombuffer);
    }
}
