/*
set_pin_direction.c

Version 1.0 Created 01/04/2022

      compile with "gcc set_pin_direction.c ../ABE_IOZero32.c -Wall -Wextra -Wpedantic -Woverflow -o set_pin_direction"
      run with "./set_pin_direction"
 
This test validates the set_pin_direction function in the ABE_IOZero32 library.

Hardware Required: IO Zero 32, Logic Analyser on I2C Pins

=== Expected Result ============================

> Console Output:

Logic output Started
Logic output Ended

> Logic Analyser Output:

W 0x20 0x06
R 0x20 0x00 0x00
W 0x20 0x06 0x01

looping to

W 0x20 0x07
R 0x20 0x00 0x7F
W 0x20 0x07 0xFF


"""
 
 */

#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h> 
#include <time.h>
#include <unistd.h>
#include "../ABE_IOZero32.h"

void clearscreen()
{
	printf("\033[2J\033[1;1H");
}

int main(int argc, char **argv)
{
	setvbuf(stdout, NULL, _IONBF, 0); // needed to print to the command line
	
	// Logic Analyser Check
    printf("Logic output Started \n");

    for (int x = 1; x < 17; x++)
	{
        set_pin_direction(0x20, x, 1);
	}

    printf("Logic output Ended \n");

	(void)argc;
	(void)argv;
	return (0);
}
