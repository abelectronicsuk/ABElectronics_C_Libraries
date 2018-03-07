/*
 * demo-iopiread.c
 *
 *  Created on: 23 Jan 2015
 *
 *      compile with "gcc ABE_IoPi.c demo-iopiread.c -o demo-iopiread"
 *      run with "./demo-iopiread"
 */

#include <string.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

#include "ABE_IoPi.h"

void clearscreen ()
{
    printf("\033[2J\033[1;1H");
}

int main(int argc, char **argv){
	setvbuf (stdout, NULL, _IONBF, 0); // needed to print to the command line

	IOPi_init(0x20); // initialise one of the io pi buses on i2c address 0x20, default address for bus 1

	set_port_direction(0x20,0, 0xFF); // set bank 0 to be inputs
	set_port_direction(0x20,1, 0xFF); // set bank 1 to be inputs
	set_port_pullups(0x20, 0, 0xFF); // enable internal pullups for bank 0
	invert_port(0x20,0,0xFF); // invert output so bank will read as 0
	set_port_pullups(0x20, 1, 0xFF); // enable internal pullups for bank 0
		invert_port(0x20,1,0xFF); // invert output so bank will read as 0


	while (1){
		clearscreen();
		printf("Port 0 Pin 1: %x \n", read_pin(0x20,1)); // grounding a pins 1 to 8 will change the value
		printf("Port 0 Pin 2: %x \n", read_pin(0x20,2)); // grounding a pins 1 to 8 will change the value
		printf("Port 0 Pin 3: %x \n", read_pin(0x20,3)); // grounding a pins 1 to 8 will change the value
		printf("Port 0 Pin 4: %x \n", read_pin(0x20,4)); // grounding a pins 1 to 8 will change the value
		printf("Port 0 Pin 5: %x \n", read_pin(0x20,5)); // grounding a pins 1 to 8 will change the value
		printf("Port 0 Pin 6: %x \n", read_pin(0x20,6)); // grounding a pins 1 to 8 will change the value
		printf("Port 0 Pin 7: %x \n", read_pin(0x20,7)); // grounding a pins 1 to 8 will change the value
		printf("Port 0 Pin 8: %x \n", read_pin(0x20,8)); // grounding a pins 1 to 8 will change the value
		printf("Port 0 Pin 9: %x \n", read_pin(0x20,9)); // grounding a pins 1 to 8 will change the value
		printf("Port 0 Pin 10: %x \n", read_pin(0x20,10)); // grounding a pins 1 to 8 will change the value
		printf("Port 0 Pin 11: %x \n", read_pin(0x20,11)); // grounding a pins 1 to 8 will change the value
		printf("Port 0 Pin 12: %x \n", read_pin(0x20,12)); // grounding a pins 1 to 8 will change the value
		printf("Port 0 Pin 13: %x \n", read_pin(0x20,13)); // grounding a pins 1 to 8 will change the value
		printf("Port 0 Pin 14: %x \n", read_pin(0x20,14)); // grounding a pins 1 to 8 will change the value
		printf("Port 0 Pin 15: %x \n", read_pin(0x20,15)); // grounding a pins 1 to 8 will change the value
		printf("Port 0 Pin 16: %x \n", read_pin(0x20,16)); // grounding a pins 1 to 8 will change the value
		usleep(200000); // sleep 0.2 seconds

	}

	return (0);
}
