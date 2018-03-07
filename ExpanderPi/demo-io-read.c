/*
 * demo-io-read.c
 *
 *  Created on: 16 June 2017
 *
 *      compile with "gcc ABE_ExpanderPi.c demo-io-read.c -o demo-io-read"
 *      run with "./demo-io-read"
 */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>


#include "ABE_ExpanderPi.h"

void clearscreen ()
{
    printf("\033[2J\033[1;1H");
}

int main(int argc, char **argv){
	setvbuf (stdout, NULL, _IONBF, 0); // needed to print to the command line

	io_init(); // initialise the io bus

	io_set_port_direction(0, 0xFF); // set bank 0 to be inputs
	io_set_port_direction(1, 0xFF); // set bank 1 to be inputs
	io_set_port_pullups(0, 0xFF); // enable internal pullups for bank 0
	io_invert_port(0,0xFF); // invert output so bank will read as 0
	io_set_port_pullups(1, 0xFF); // enable internal pullups for bank 0
	io_invert_port(1,0xFF); // invert output so bank will read as 0


	while (1){
		clearscreen();
		printf("Pin 1: %x \n", io_read_pin(1)); // grounding a pins 1 to 8 will change the value
		printf("Pin 2: %x \n", io_read_pin(2)); // grounding a pins 1 to 8 will change the value
		printf("Pin 3: %x \n", io_read_pin(3)); // grounding a pins 1 to 8 will change the value
		printf("Pin 4: %x \n", io_read_pin(4)); // grounding a pins 1 to 8 will change the value
		printf("Pin 5: %x \n", io_read_pin(5)); // grounding a pins 1 to 8 will change the value
		printf("Pin 6: %x \n", io_read_pin(6)); // grounding a pins 1 to 8 will change the value
		printf("Pin 7: %x \n", io_read_pin(7)); // grounding a pins 1 to 8 will change the value
		printf("Pin 8: %x \n", io_read_pin(8)); // grounding a pins 1 to 8 will change the value
		printf("Pin 9: %x \n", io_read_pin(9)); // grounding a pins 1 to 8 will change the value
		printf("Pin 10: %x \n", io_read_pin(10)); // grounding a pins 1 to 8 will change the value
		printf("Pin 11: %x \n", io_read_pin(11)); // grounding a pins 1 to 8 will change the value
		printf("Pin 12: %x \n", io_read_pin(12)); // grounding a pins 1 to 8 will change the value
		printf("Pin 13: %x \n", io_read_pin(13)); // grounding a pins 1 to 8 will change the value
		printf("Pin 14: %x \n", io_read_pin(14)); // grounding a pins 1 to 8 will change the value
		printf("Pin 15: %x \n", io_read_pin(15)); // grounding a pins 1 to 8 will change the value
		printf("Pin 16: %x \n", io_read_pin(16)); // grounding a pins 1 to 8 will change the value
		usleep(200000); // sleep 0.2 seconds

	}

	return (0);
}
