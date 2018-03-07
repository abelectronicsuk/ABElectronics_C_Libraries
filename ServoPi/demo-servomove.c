/*
 * demo-servomove.c
 *
 *  Created on: 27 Mar 2016
 *
 *	Control a servo on channel 1 and change the position between three positions.
 *
 *	The demo needs to be run as sudo.  This is because the Servo Pi uses the GPIO port to control
 *	the Output Enable function and GPIO needs to be accessed as root.
 *
 *	To run without sudo privileges change "servopi_init(0x40, 1);" to "servopi_init(0x40, 0);"
 *	to disable the output enable pin and remove "output_enable(0x40);"
 *
 *  compile with "gcc -lm ABE_ServoPi.c demo-servomove.c -o demo-servomove"
 *  run with "sudo ./demo-servomove"
 */

#include <stdio.h>
#include <time.h>
#include <unistd.h>

#include "ABE_ServoPi.h"

// define the the servo minimum, centre and maximum limits
#define servoMin 250  // Minimum pulse length out of 4096
#define servoMed 400  // Medium pulse length out of 4096
#define servoMax 500  // Maximum pulse length out of 4096

void clearscreen() {
	printf("\033[2J\033[1;1H");
}

int main(int argc, char **argv) {
	setvbuf(stdout, NULL, _IONBF, 0); // needed to print to the command line

	int x;
	int a;

	// initialise the servo pi on I2C address 0x40 with the Output Enable pin enabled.
	// Check the returned value to ensure the Servo Pi initialised correctly

	a = servopi_init(0x40, 1);
	if (a != 0) {
		if (a == 1) {
			printf("Error enabling GPIO Pin");
		}
		if (a == 2) {
			printf("Error setting GPIO Pin direction");
		}
		return (0);
	}

	//Set PWM frequency to 60 Hz and enable the output
	set_pwm_freq(60, 0x40);
	output_enable();

	while (1) {
		set_pwm(1, 0, servoMin, 0x40); // set the pwm width to servoMin
		usleep(500000); // sleep 0.5 seconds
		set_pwm(1, 0, servoMed, 0x40); // set the pwm width to servoMed
		usleep(500000); // sleep 0.5 seconds
		set_pwm(1, 0, servoMax, 0x40); // set the pwm width to servoMax
		usleep(500000); // sleep 0.5 seconds
	}

	return (0);
}
