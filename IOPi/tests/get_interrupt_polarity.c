/*
get_interrupt_polarity.c

Version 1.0 Created 04/03/2020

      compile with "gcc get_interrupt_polarity.c ../ABE_IoPi.c -Wall -Wextra -Wpedantic -Woverflow -o get_interrupt_polarity"
      run with "./get_interrupt_polarity"
 
This test validates the get_interrupt_polarity method in the IOPi class.

Hardware Required: IO Pi Plus or IO Pi Zero, Logic Analyser on I2C Pins

=== Expected Result ============================

> Console Output:

Test Passed

 
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

	bool passed = true;
	uint8_t x = 0;

    IOPi_init(0x20, 0);

	set_interrupt_polarity(0x20, 0);
    x = get_interrupt_polarity(0x20);
    if (x != 0){
        passed = false;
	}
    set_interrupt_polarity(0x20, 1);
    x = get_interrupt_polarity(0x20);
    if (x != 1){
        passed = false;
	}

    if (passed){
        printf("Test passed \n");
	}
    else{
        printf("Test Failed \n");
	}

	(void)argc;
	(void)argv;
	return (0);
}
