/*
 * demo-dac-write.c
 *
 *  Created on: 16 June 2017
 *
 *      compile with "gcc ../ABE_ExpanderPi.c demo-dac-write.c -o demo-dac-write"
 *      run with "./demo-dac-write"
 */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "../ABE_ExpanderPi.h"



int main(int argc, char **argv){
	setvbuf (stdout, NULL, _IONBF, 0); // needed to print to the command line

	if (dac_open() != 1){ // open the DAC spi channel
		exit(1); // if the SPI bus fails to open exit the program
	}

	dac_set_voltage(1.2, 1, 2); // set the voltage on channel 1 to 1.2V with a gain of 2
	dac_set_voltage(2.5, 2, 2); // set the voltage on channel 2 to 2.5V with a gain of 2

	dac_close();

	(void)argc;
	(void)argv;
	return (0);
}
