/*
 * demo-setallcalladdress.c
 *
 *  Created on: 27 Mar 2016
 *
 *  Set the All Call I2C address to be 0x60 and enable All Call functionality.  This allows you to
 *  access several Servo Pi boards at the same time from a single I2C address.
 *
 *	The demo needs to be run as sudo.  This is because the Servo Pi uses the GPIO port to control
 *	the Output Enable function and the GPIO needs to be accessed as root.
 *
 *	To run without sudo privileges change "servopi_init(0x40, 1);" to "servopi_init(0x40, 0);"
 *	to disable the output enable pin and remove "output_enable(0x40);"
 *
 *  Compile with "gcc -lm ../ABE_ServoPi.c demo-setallcalladdress.c -o demo-setallcalladdress"
 *  Run with "sudo ./demo-setallcalladdress"
 */

#include <stdio.h>
#include <stdint.h>

#include "../ABE_ServoPi.h"

void clearscreen() {
	printf("\033[2J\033[1;1H");
}



int main(int argc, char **argv) {
	setvbuf(stdout, NULL, _IONBF, 0); // Needed to print to the command line

	int a;

	// Initialise the servo pi on I2C address 0x40.  
	// Check the returned value to ensure the Servo Pi is initialised correctly

	a = servopi_init(0x40, 1);
	if (a != 0){
		if (a == 1){
			printf("Error enabling GPIO Pin");
		}
		if (a == 2){
			printf("Error setting GPIO Pin direction");
		}
		return (0);
	}

	set_allcall_address(0x60, 0x40);
	enable_allcall_address(0x40);

	(void)argc;
	(void)argv;
	return (0);
}
