/*
 * utility.h
 *
 * Created: 7/03/2019 4:14:45 PM
 *  Author: 19477143
 */ 


#ifndef UTILITY_H_
#define UTILITY_H_

#include "includes.h"

void pullup(volatile unsigned char* DDR, volatile unsigned char* PORT, int pin);
bool isOff(volatile unsigned char* PIN, int pinNum);


#endif /* UTILITY_H_ */