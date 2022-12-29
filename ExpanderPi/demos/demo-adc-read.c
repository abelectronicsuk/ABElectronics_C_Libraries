/*
 * demo-adc-read.c
 *
 *  Created on: 16 June 2017
 *
 *      compile with "gcc ../ABE_ExpanderPi.c demo-adc-read.c -o demo-adc-read"
 *      run with "./demo-adc-read"
 */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>



#include "../ABE_ExpanderPi.h"

void clearscreen ()
{
    printf("\033[2J\033[1;1H");
}

int main(int argc, char **argv){
	setvbuf (stdout, NULL, _IONBF, 0); // needed to print to the command line

	if (adc_open() != 1){ // open the ADC SPI channel
			exit(1); // if the SPI bus fails to open exit the program
		}

	while (1){
		clearscreen();
		printf("Pin 1: %G \n", adc_read_voltage(1, 0)); // read the voltage from channel 1 in single-ended mode
		printf("Pin 2: %G \n", adc_read_voltage(2, 0)); // read the voltage from channel 2 in single-ended mode
		printf("Pin 3: %G \n", adc_read_voltage(3, 0)); // read the voltage from channel 3 in single-ended mode
		printf("Pin 4: %G \n", adc_read_voltage(4, 0)); // read the voltage from channel 4 in single-ended mode
		printf("Pin 5: %G \n", adc_read_voltage(5, 0)); // read the voltage from channel 5 in single-ended mode
		printf("Pin 6: %G \n", adc_read_voltage(6, 0)); // read the voltage from channel 6 in single-ended mode
		printf("Pin 7: %G \n", adc_read_voltage(7, 0)); // read the voltage from channel 7 in single-ended mode
		printf("Pin 8: %G \n", adc_read_voltage(8, 0)); // read the voltage from channel 8 in single-ended mode

		usleep(200000); // sleep 0.2 seconds

	}

	(void)argc;
	(void)argv;
	return (0);
}
