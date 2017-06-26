/*
 * demo-io-write.c
 *
 *  Created on: 16 June 2017
 *
 *      compile with "gcc ABE_ExpanderPi.c demo-io-write.c -o demo-io-write"
 *      run with "./demo-io-write"
 */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>


#include "ABE_ExpanderPi.h"

int main(int argc, char **argv){
	io_init(); // initialise one of the io pi buses on i2c address 0x20
	io_set_port_direction(0, 0x00); // set the direction for port 0 to be outputs
	io_set_port_direction(1, 0x00); // set the direction for port 1 to be outputs
	io_write_port(0, 0x00); // turn off all of the pins on port 0
	
	while (1){
		io_write_pin(1, 1); // turn pin 1 on and off at 1 second intervals
		usleep(1000000);
		io_write_pin(1, 0);
		usleep(1000000);
	}

	return (0);
}
