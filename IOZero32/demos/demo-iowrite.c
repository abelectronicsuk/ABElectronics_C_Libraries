/*
 * demo-iowrite.c
 *
 *  Created on: 07 April 2022
 *
 *      compile with "gcc ../ABE_IOZero32.c demo-iowrite.c -o demo-iowrite"
 *      run with "./demo-iowrite"
 */

#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <unistd.h>

#include "../ABE_IOZero32.h"

int main(int argc, char **argv){
	set_bus_direction(0x20, 0x0000); // set the direction for all pins on the bus to be outputs

	write_bus(0x20,0x0000); // turn off all pins on the bus
	while (1){
		write_pin(0x20,1, 1); // turn pin 1 on and off at 1 second intervals
		usleep(1000000);
		write_pin(0x20,1, 0);
		usleep(1000000);
	}

	(void)argc;
	(void)argv;
	return (0);
}
