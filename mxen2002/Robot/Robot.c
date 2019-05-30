//Example ATmega2560 Project
//File: ATmega2560Project.c
//Author: Robert Howie
//Created: 26 February 2016
//An example file for second year mechatronics project

//include this .c file's header file
#include "includes.h"
#include "communication.h"

//file scope variables
//static char serial_string[200] = {0};



int main(void)
{
	loop_comms();
	return 0;
}//end main
