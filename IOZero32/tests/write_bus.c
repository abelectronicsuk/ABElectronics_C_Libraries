/*
write_bus.c

Version 1.0 Created 01/04/2022

      compile with "gcc write_bus.c ../ABE_IOZero32.c -Wall -Wextra -Wpedantic -Woverflow -o write_bus"
      run with "./write_bus"
 
This test validates the write_bus method in the ABE_IOZero32 library.

Hardware Required: IO Zero 32, Logic Analyser on I2C Pins

=== Expected Result ============================

> Console Output:

Logic output Started
Logic output Ended

> Logic Analyser Output:

W 0x20 0x02 0x00 0x00

looping to

W 0x20 0x02 0xFF 0xFF

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

    for (uint16_t a = 0; a < 65535; a++){
        write_bus(0x20, a);      
	}

    printf("Logic output Ended\n");

	(void)argc;
	(void)argv;
	return (0);
}
