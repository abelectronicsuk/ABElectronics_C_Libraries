/*
read_interrupt_status.c

Version 1.0 Created 04/03/2020

      compile with "gcc read_interrupt_status.c ../ABE_IoPi.c -Wall -Wextra -Wpedantic -Woverflow -o read_interrupt_status"
      run with "./read_interrupt_status"
 
This test validates the read_interrupt_status method in the ABE_IoPi library.

Hardware Required: IO Pi Plus or IO Pi Zero, Logic Analyser on I2C Pins

=== Expected Result ============================

> Console Output:

Logic output Started
Logic output Ended

> Logic Analyser Output:

W 0x20 0xA0 0x02

W 0x20 0x0E
R 0x20 0x00
W 0x20 0x0F
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

	read_interrupt_status(0x20, 0);
    read_interrupt_status(0x20, 1);

	printf("Logic output Ended \n");

	(void)argc;
	(void)argv;
	return (0);
}
