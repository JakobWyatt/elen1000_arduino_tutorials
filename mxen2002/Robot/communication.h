#ifndef GUARD_COMMUNICATION_H
#define GUARD_COMMUNCATION_H

typedef enum {
    NOSEND,
    LMOTOR,
    RMOTOR,
    DISTL,
	DISTR,
	DISTC,
	SERVO,
	AUTO_ON,
	AUTO_OFF
} comms;

void loop_comms();
void motor_init();
void control_right_motor(int n);
void control_left_motor(int n);
void servo_init();
void move_velocity_servo(long n);
int clamp(int x, int min, int max);
double range(long read);
double distance_2D120X(double voltage);

#endif
