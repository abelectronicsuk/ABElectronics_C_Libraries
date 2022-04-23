/*
write_pin.c

Version 1.0 Created 01/04/2022

      compile with "gcc write_pin.c ../ABE_IOZero32.c -Wall -Wextra -Wpedantic -Woverflow -o write_pin"
      run with "./write_pin"
 
This test validates the write_pin function in the ABE_IOZero32 library.

Hardware Required: IO Zero 32, Logic Analyser on I2C Pins

=== Expected Result ============================

> Console Output:

Logic output Started
Logic output Ended

> Logic Analyser Output:

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
#include "../ABE_IOZero32.h"

void clearscreen()
{
	printf("\033[2J\033[1;1H");
}

int main(int argc, char **argv)
{
	setvbuf(stdout, NULL, _IONBF, 0); // needed to print to the command line

	// Logic Analyser Check
    printf("Logic output Started\n");

    for (int x = 1; x < 17; x++)
	{
        write_pin(0x20, x, 1);
	}

    printf("Logic output Ended\n");

	(void)argc;
	(void)argv;
	return (0);
}
