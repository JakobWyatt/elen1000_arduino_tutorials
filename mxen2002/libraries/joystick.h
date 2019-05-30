/*
 * joystick.h
 *
 * Created: 7/03/2019 3:40:26 PM
 *  Author: 19477143
 */ 


#ifndef JOYSTICK_H_
#define JOYSTICK_H_

//typedef enum {VERT, HORZ} joystickDir;

void displayJoystick(int pin);
void detectTemp();
void switchJoystickDirOnPress();


#endif /* JOYSTICK_H_ */