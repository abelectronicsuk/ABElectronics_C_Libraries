/*
read_bus.c

Version 1.0 Created 01/04/2022

      compile with "gcc read_bus.c ../ABE_IOZero32.c -Wall -Wextra -Wpedantic -Woverflow -o read_bus"
      run with "./read_bus"
 
This test validates the read_bus function in the ABE_IOZero32 library.

Hardware Required: IO Zero 32, Logic Analyser on I2C Pins

=== Expected Result ============================

> Console Output:

Logic output Started
Logic output Ended

> Logic Analyser Output:

W 0x20 0x02 0x00 0x00
W 0x20 0x06 0xFF 0xFF

W 0x20 0x00
R 0x20 0x00 0x00

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

	// Reset to 0
	write_bus(0x20, 0x0000);

	// Set Bus to inputs
    set_bus_direction(0x20, 0xFFFF);

	// Logic Analyser Check

	printf("Logic output Started \n");

	uint16_t x = read_bus(0x20);

	printf("Logic output Ended \n");
	(void)x;
	(void)argc;
	(void)argv;
	return (0);
}
