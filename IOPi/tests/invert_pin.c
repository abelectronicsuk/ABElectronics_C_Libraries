/*
invert_pin.c

Version 1.0 Created 04/03/2020

      compile with "gcc invert_pin.c ../ABE_IoPi.c -Wall -Wextra -Wpedantic -Woverflow -o invert_pin"
      run with "./invert_pin"
 
This test validates the invert_pin function in the ABE_IoPi library.

Hardware Required: IO Pi Plus or IO Pi Zero, Logic Analyser on I2C Pins

=== Expected Result ============================

> Console Output:

Logic output Started
Logic output Ended

> Logic Analyser Output:

W 0x20 0xA0 0x02
W 0x20 0x02 0x00 0x00

W 0x20 0x02
R 0x20 0x02 0x00
W 0x20 0x02 0x01

looping to

W 0x20 0x02
R 0x20 0x02 0x7F
W 0x20 0x02 0xFF

W 0x20 0x03
R 0x20 0x03 0x00
W 0x20 0x03 0x01

looping to

W 0x20 0x03
R 0x20 0x03 0x7F
W 0x20 0x03 0xFF


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
    printf("Logic output Started \n");

	invert_bus(0x20, 0x0000);

    for (int x = 1; x < 17; x++)
	{
        invert_pin(0x20, x, 1);
	}

    printf("Logic output Ended \n");

	(void)argc;
	(void)argv;
	return (0);
}
