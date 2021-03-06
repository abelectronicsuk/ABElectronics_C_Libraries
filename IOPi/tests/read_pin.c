/*
read_pin.c

Version 1.0 Created 04/03/2020

      compile with "gcc read_pin.c ../ABE_IoPi.c -Wall -Wextra -Wpedantic -Woverflow -o read_pin"
      run with "./read_pin"
 
This test validates the read_pin method in the ABE_IoPi library.

Hardware Required: IO Pi Plus or IO Pi Zero, Logic Analyser on I2C Pins

=== Expected Result ============================

> Console Output:

Logic output Started
Logic output Ended

> Logic Analyser Output:

W 0x20 0xA0 0x02
W 0x20 0x12 0xFF 0xFF
W 0x20 0x0C 0x00 0x00
W 0x20 0x02 0x00 0x00

W 0x20 0x12
R 0x20 0x00

looping 8 times

W 0x20 0x13
R 0x20 0x00

looping 8 times

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

	IOPi_init(0x20, 0);

	for (uint8_t a = 1; a < 17; a++){
        read_pin(0x20, a);        
	}

	(void)argc;
	(void)argv;
	return (0);
}
