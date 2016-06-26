/*
 * demo-adcread.c
 *
 *  Created on: 20 Jan 2016
 *
 *      compile with "gcc ABE_ADCDACPi.c demo-adcread.c -o demo-adcread"
 *      run with "./demo-adcread"
 */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <time.h>
#include <getopt.h>


#include "ABE_ADCDACPi.h"

void clearscreen ()
{
    printf("\033[2J\033[1;1H");
}

int main(int argc, char **argv){
	setvbuf (stdout, NULL, _IONBF, 0); // needed to print to the command line

	if (open_adc() != 1){ // open the DAC spi channel
			exit(1); // if the SPI bus fails to open exit the program
		}

	while (1){
		clearscreen();
		printf("Pin 1: %G \n", read_adc_voltage(1, 0)); // read the voltage from channel 1 in single ended mode
		printf("Pin 2: %G \n", read_adc_voltage(2, 0)); // read the voltage from channel 2 in single ended mode

		usleep(200000); // sleep 0.2 seconds

	}

	return (0);
}
