/*
IOPi_init.c

Version 1.0 Created 04/03/2020

      compile with "gcc IOPi_init.c ../ABE_IoPi.c -Wall -Wextra -Wpedantic -Woverflow -o IOPi_init"
      run with "./IOPi_init"
 
This test validates the IOPi method in the ABE_IoPi library.

Hardware Required: IO Pi Plus or IO Pi Zero, Logic Analyser on I2C Pins

=== Expected Result ============================

> Console Output:

Logic output Started
Logic output Ended

> Logic Analyser Output:

W 0x20 0xA0 0x02

10ms delay

W 0x20 0xA0 0x02
W 0x20 0x00 0xFF 0xFF
W 0x20 0x0C 0x00 0x00
W 0x20 0x02 0x00 0x00

"""
 
 */

#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h> 
#include <time.h>
#include <unistd.h>
#include "../ABE_IoPi.h"

void clearscreen()
{
	printf("\033[2J\033[1;1H");
}

int main(int argc, char **argv)
{
	setvbuf(stdout, NULL, _IONBF, 0); // needed to print to the command line

	printf("Logic output Started \n");

	IOPi_init(0x20, 1);

	printf("Logic output Ended \n");

	(void)argc;
	(void)argv;
	return (0);
}
