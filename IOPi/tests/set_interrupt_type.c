/*
set_interrupt_type.c

Version 1.0 Created 04/03/2020

      compile with "gcc set_interrupt_type.c ../ABE_IoPi.c -Wall -Wextra -Wpedantic -Woverflow -o set_interrupt_type"
      run with "./set_interrupt_type"
 
This test validates the set_interrupt_type function in the ABE_IoPi library.

Hardware Required: IO Pi Plus or IO Pi Zero, Logic Analyser on I2C Pins

=== Expected Result ============================

> Console Output:

Logic output Started
Logic output Ended

> Logic Analyser Output:

W 0x20 0xA0 0x02
W 0x20 0x08 0x00 0x00

W 0x20 0x08 0x00
W 0x20 0x09 0x00

looping to

W 0x20 0x08 0xFF
W 0x20 0x09 0xFF


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


    for (int x = 0; x < 256; x++)
	{
        set_interrupt_type(0x20, 0, x);
		set_interrupt_type(0x20, 1, x);
	}

    printf("Logic output Ended \n");

	(void)argc;
	(void)argv;
	return (0);
}
