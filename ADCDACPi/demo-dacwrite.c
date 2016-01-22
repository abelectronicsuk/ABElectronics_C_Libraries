/*
 * demo-dacwrite.c
 *
 *  Created on: 20 Jan 2016
 *
 *      compile with "gcc ABE_ADCDACPi.c demo-adcread.c -o demo-dacwrite"
 *      run with "./demo-dacwrite"
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

#include "ABE_ADCDACPi.h"



int main(int argc, char **argv){
	setvbuf (stdout, NULL, _IONBF, 0); // needed to print to the command line

	if (open_dac() != 1){ // open the DAC spi channel
		exit(1); // if the SPI bus fails to open exit the program
	}

	set_dac_gain(2); // set the dac gain to 2 which will give a voltage range of 0 to 3.3V


	set_dac_voltage(1.2, 1); // set the voltage on channel 1 to 1.2V
	set_dac_voltage(2.5, 2); // set the voltage on channel 2 to 2.5V

	close_dac();
	return (0);
}
