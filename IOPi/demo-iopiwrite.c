/*
 * demo-iopiwrite.c
 *
 *  Created on: 23 Jan 2015
 *
 *      compile with "gcc ABE_IoPi.c demo-iopiwrite.c -o demo-iopiwrite"
 *      run with "./demo-iopiwrite"
 */

#include <stdio.h>
#include <time.h>


#include "ABE_IoPi.h"

int main(int argc, char **argv){
	IOPi_init(0x20); // initialise one of the io pi buses on i2c address 0x20
	set_port_direction(0x20,0, 0x00); // set the direction for bank 0 to be outputs
	set_port_direction(0x20,1, 0x00); // set the direction for bank 1 to be outputs
	write_port(0x20,1, 0xFF);
	while (1){
		write_pin(0x20,1, 1); // turn pin 1 on and off at 1 second intervals
		usleep(1000000);
		write_pin(0x20,1, 0);
		usleep(1000000);
	}

	return (0);
}
