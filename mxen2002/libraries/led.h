/*
 * led.h
 *
 * Created: 28/02/2019 5:25:23 PM
 *  Author: 19477143
 */ 


#ifndef LED_H_
#define LED_H_

void alternate(volatile unsigned char* DDR, volatile unsigned char* PORT);
void flash(volatile unsigned char* DDR, volatile unsigned char* PORT);
void sequence(volatile unsigned char* DDR, volatile unsigned char* PORT);
void fillBack(volatile unsigned char* DDR, volatile unsigned char* PORT, int fillNum);
void allOn(volatile unsigned char* DDR, volatile unsigned char* PORT);
void allOff(volatile unsigned char* DDR, volatile unsigned char* PORT);

#endif /* LED_H_ */