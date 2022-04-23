/*
 * demo-ioread.c
 *
 *  Created on: 07 April 2022
 *
 *      compile with "gcc ../ABE_IOZero32.c demo-ioread.c -o demo-ioread"
 *      run with "./demo-ioread"
 * 
 *  This demo reads the value from all 16 pins on bus 0 (i2c address 0x20)
 *  Connect 100K pull-up resistors to each pin to pull them high.
 *  Connecting a pin to ground will pull it low and show on the console as changing from 0 to 1
 */

#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

#include "../ABE_IOZero32.h"

void clearscreen ()
{
    printf("\033[2J\033[1;1H");
}

int main(int argc, char **argv){
	setvbuf (stdout, NULL, _IONBF, 0); // needed to print to the command line

	set_bus_direction(0x20, 0xFFFF); // set all pins on the bus to be inputs
	set_bus_polarity(0x20,0xFFFF); // invert the output so a pin pulling high will read as 0

	while (1){
		clearscreen();
		// grounding a pin will change the value from 0 to 1
		printf("Pin  1: %x \n", read_pin(0x20,1)); 
		printf("Pin  2: %x \n", read_pin(0x20,2)); 
		printf("Pin  3: %x \n", read_pin(0x20,3)); 
		printf("Pin  4: %x \n", read_pin(0x20,4)); 
		printf("Pin  5: %x \n", read_pin(0x20,5)); 
		printf("Pin  6: %x \n", read_pin(0x20,6)); 
		printf("Pin  7: %x \n", read_pin(0x20,7)); 
		printf("Pin  8: %x \n", read_pin(0x20,8)); 
		printf("Pin  9: %x \n", read_pin(0x20,9)); 
		printf("Pin 10: %x \n", read_pin(0x20,10)); 
		printf("Pin 11: %x \n", read_pin(0x20,11)); 
		printf("Pin 12: %x \n", read_pin(0x20,12)); 
		printf("Pin 13: %x \n", read_pin(0x20,13)); 
		printf("Pin 14: %x \n", read_pin(0x20,14)); 
		printf("Pin 15: %x \n", read_pin(0x20,15)); 
		printf("Pin 16: %x \n", read_pin(0x20,16)); 
		usleep(200000); // sleep 0.2 seconds

	}
	
	(void)argc;
	(void)argv;
	return (0);
}
