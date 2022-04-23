/*
get_interrupt_defaults.c

Version 1.0 Created 04/03/2020

      compile with "gcc get_interrupt_defaults.c ../ABE_IoPi.c -Wall -Wextra -Wpedantic -Woverflow -o get_interrupt_defaults"
      run with "./get_interrupt_defaults"
 
This test validates the get_interrupt_defaults method in the ABE_IoPi library.

Hardware Required: IO Pi Plus or IO Pi Zero, Logic Analyser on I2C Pins

=== Expected Result ============================

> Console Output:

Test Passed

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

	bool passed = true;
	uint8_t x = 0;

    IOPi_init(0x20, 0);

	for (uint8_t a = 0; a < 255; a++){
         set_interrupt_defaults(0x20, 0, a);
        x = get_interrupt_defaults(0x20, 0);
        if (x != a){
            passed = false;
            break;
		}
        set_interrupt_defaults(0x20, 1, a);
        x = get_interrupt_defaults(0x20, 1);
        if (x != a){
            passed = false;
            break;
		}
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
