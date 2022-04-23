/*
get_pin_direction.c

Version 1.0 Created 01/04/2022

      compile with "gcc get_pin_direction.c ../ABE_IOZero32.c -Wall -Wextra -Wpedantic -Woverflow -o get_pin_direction"
      run with "./get_pin_direction"
 
This test validates the get_pin_direction method in the ABE_IOZero32 library.

Hardware Required: IO Zero 32, Logic Analyser on I2C Pins

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
#include "../ABE_IOZero32.h"

void clearscreen()
{
	printf("\033[2J\033[1;1H");
}

int main(int argc, char **argv)
{
	setvbuf(stdout, NULL, _IONBF, 0); // needed to print to the command line

	bool passed = true;
	uint8_t x = 0;

	for (uint8_t a = 1; a < 17; a++){
         set_pin_direction(0x20, a, 0);
        x = get_pin_direction(0x20, a);
        if (x != 0){
            passed = false;
            break;
		}
        set_pin_direction(0x20, a, 1);
        x = get_pin_direction(0x20, a);
        if (x != 1){
            passed = false;
            break;
		}
	}

    if (passed){
        printf("Test passed \n");
	}
    else{
        printf("Test Passed \n");
	}

	(void)argc;
	(void)argv;
	return (0);
}
