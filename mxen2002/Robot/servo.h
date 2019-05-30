/*
 * servo.h
 *
 * Created: 11/04/2019 3:04:17 PM
 *  Author: 19477143
 */ 


#ifndef SERVO_H_
#define SERVO_H_


void control_servo();
void servo_init();
void music();
void move_position_servo(long n);
void move_velocity_servo(long n);

#endif /* SERVO_H_ */