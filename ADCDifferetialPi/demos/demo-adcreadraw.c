/*
 * demo-adcread.c
 *
 *  Created on: 03 Mar 2020
 *
 *      compile with "gcc ../ABE_ADCDifferentialPi.c demo-adcreadraw.c -o demo-adcreadraw"
 *      run with "./demo-adcreadraw"
 */

#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <unistd.h>

#include "../ABE_ADCDifferentialPi.h"

void clearscreen ()
{
    printf("\033[2J\033[1;1H");
}

int main(int argc, char **argv){
	setvbuf (stdout, NULL, _IONBF, 0); // needed to print to the command line

	while (1){
		clearscreen();
		printf("Pin 1: %lu \n", read_raw(0x68,1, 18, 1, 1)); // read from adc chip 1, channel 1, 18 bit, pga gain set to 1 and continuous conversion mode
		printf("Pin 2: %lu \n", read_raw(0x68,2, 16, 1, 1)); // read from adc chip 1, channel 2, 16 bit, pga gain set to 1 and continuous conversion mode
		printf("Pin 3: %lu \n", read_raw(0x68,3, 14, 1, 1)); // read from adc chip 1, channel 3, 14 bit, pga gain set to 1 and continuous conversion mode
		printf("Pin 4: %lu \n", read_raw(0x68,4, 12, 1, 1)); // read from adc chip 1, channel 4, 12 bit, pga gain set to 1 and continuous conversion mode
		printf("Pin 5: %lu \n", read_raw(0x69,1, 12, 1, 1)); // read from adc chip 2, channel 1, 12 bit, pga gain set to 1 and continuous conversion mode
		printf("Pin 6: %lu \n", read_raw(0x69,2, 12, 1, 1)); // read from adc chip 2, channel 2, 12 bit, pga gain set to 1 and continuous conversion mode
		printf("Pin 7: %lu \n", read_raw(0x69,3, 12, 1, 1)); // read from adc chip 2, channel 3, 12 bit, pga gain set to 1 and continuous conversion mode
		printf("Pin 8: %lu \n", read_raw(0x69,4, 12, 1, 1)); // read from adc chip 2, channel 4, 12 bit, pga gain set to 1 and continuous conversion mode
		usleep(200000); // sleep 0.2 seconds

	}

	(void)argc;
	(void)argv;
	return (0);
}
