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

void loop();
double range(long pin);
double distance_2D120X(double voltage);

#endif
