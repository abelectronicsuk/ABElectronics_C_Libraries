/*
reset_interrupts.c

Version 1.0 Created 04/03/2020

      compile with "gcc reset_interrupts.c ../ABE_IoPi.c -Wall -Wextra -Wpedantic -Woverflow -o reset_interrupts"
      run with "./reset_interrupts"
 
This test validates the reset_interrupts method in the ABE_IoPi library.

Hardware Required: IO Pi Plus or IO Pi Zero, Logic Analyser on I2C Pins

=== Expected Result ============================

> Console Output:

Logic output Started
Logic output Ended

> Logic Analyser Output:

W 0x20 0xA0 0x02

W 0x20 0x10
R 0x20 0x00
W 0x20 0x11
R 0x20 0x00
 
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

	printf("Logic output Started \n");

	reset_interrupts(0x20);

	printf("Logic output Ended \n");

 
	(void)argc;
	(void)argv;
	return (0);
}
