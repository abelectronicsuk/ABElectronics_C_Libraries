/*
 * demo-switch.c
 *
 *  Created on: 25 Nov 2019
 * 
 *  Switch the I2C switch to a specified channel and read 
 *  the status of the switch to check that it has changed
 *
 *  compile with "gcc ABE_I2CSwitch.c demo-switch.c -o demo-switch"
 *  run with "sudo ./demo-switch"
 */

#include <stdio.h>
#include <stdint.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

#include "ABE_I2CSwitch.h"

void clearscreen ()
{
    printf("\033[2J\033[1;1H");
}

int main(int argc, char **argv){
	setvbuf (stdout, NULL, _IONBF, 0); // needed to print to the command line

	char i2caddress = 0x70; // default i2c address for the switch

	char channel = 1; // channel to select

	switch_channel(i2caddress, channel); // switch to the selected channel

	if (get_channel_state(i2caddress, channel) == 1){
    	printf("I2C switch set to channel %d \n", channel);
	}
	else{
		printf("Failed to change I2C switch to channel %d \n", channel);
	}

	(void)argc;
	(void)argv;
	return (0);
}

