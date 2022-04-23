/*
write_port.c

Version 1.0 Created 04/03/2020

      compile with "gcc write_port.c ../ABE_IoPi.c -Wall -Wextra -Wpedantic -Woverflow -o write_port"
      run with "./write_port"
 
This test validates the write_port function in the ABE_IoPi library.

Hardware Required: IO Pi Plus or IO Pi Zero, Logic Analyser on I2C Pins

=== Expected Result ============================

> Console Output:

Logic output Started
Logic output Ended

> Logic Analyser Output:

W 0x20 0xA0 0x02
W 0x20 0x12 0x00 0x00

W 0x20 0x12 0x00
W 0x20 0x13 0x00

looping to

W 0x20 0x12 0xFF
W 0x20 0x13 0xFF


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

	// Logic Analyser Check
    printf("Logic output Started\n");

    for (int x = 0; x < 256; x++)
	{
        write_port(0x20, 0, x);
		write_port(0x20, 1, x);
	}

    printf("Logic output Ended\n");

	(void)argc;
	(void)argv;
	return (0);
}
