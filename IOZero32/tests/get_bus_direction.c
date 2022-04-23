/*
get_bus_direction.c

Version 1.0 Created 01/04/2022

      compile with "gcc get_bus_direction.c ../ABE_IOZero32.c -Wall -Wextra -Wpedantic -Woverflow -o get_bus_direction"
      run with "./get_bus_direction"
 
This test validates the get_bus_direction function in the ABE_IOZero32 library.

Hardware Required: IO Zero 32

=== Expected Result ============================

> Console Output:

Testing get_bus_direction
65534 of 65535
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
	uint16_t x = 0;
    
    printf("Testing get_bus_direction\n");
    for (uint16_t a = 0; a < 65535; a++){

        printf("\r%d of 65534", a);
        set_bus_direction(0x20, a);
        x = get_bus_direction(0x20);
        if (x != a){
            passed = false;
            break;
		}
	}

    if (!passed){
        printf("\nTest Failed\n");
	}
    else{
        printf("\nTest Passed\n");
	}

	(void)argc;
	(void)argv;
	return (0);
}