/*
 * demo-adcread.c
 *
 *  Created on: 07 Jan 2016
 *
 *      compile with "gcc ../ABE_ADCPi.c demo-adcread.c -o demo-adcread"
 *      run with "./demo-adcread"
 */

#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <unistd.h>


#include "../ABE_ADCPi.h"

void clearscreen ()
{
    printf("\033[2J\033[1;1H");
}

int main(int argc, char **argv){
	setvbuf (stdout, NULL, _IONBF, 0); // needed to print to the command line

	while (1){
		clearscreen();
		printf("Pin 1: %G \n", read_voltage(0x68,1, 18, 1, 1)); // read from ADC chip 1, channel 1, 18-bit, PGA gain set to 1 and continuous conversion mode
		printf("Pin 2: %G \n", read_voltage(0x68,2, 16, 1, 1)); // read from ADC chip 1, channel 2, 16-bit, PGA gain set to 1 and continuous conversion mode
		printf("Pin 3: %G \n", read_voltage(0x68,3, 14, 1, 1)); // read from ADC chip 1, channel 3, 14-bit, PGA gain set to 1 and continuous conversion mode
		printf("Pin 4: %G \n", read_voltage(0x68,4, 12, 1, 1)); // read from ADC chip 1, channel 4, 12-bit, PGA gain set to 1 and continuous conversion mode
		printf("Pin 5: %G \n", read_voltage(0x69,1, 12, 1, 1)); // read from ADC chip 2, channel 1, 12-bit, PGA gain set to 1 and continuous conversion mode
		printf("Pin 6: %G \n", read_voltage(0x69,2, 12, 1, 1)); // read from ADC chip 2, channel 2, 12-bit, PGA gain set to 1 and continuous conversion mode
		printf("Pin 7: %G \n", read_voltage(0x69,3, 12, 1, 1)); // read from ADC chip 2, channel 3, 12-bit, PGA gain set to 1 and continuous conversion mode
		printf("Pin 8: %G \n", read_voltage(0x69,4, 12, 1, 1)); // read from ADC chip 2, channel 4, 12-bit, PGA gain set to 1 and continuous conversion mode
		usleep(200000); // sleep 0.2 seconds

	}

	(void)argc;
	(void)argv;
	return (0);
}
