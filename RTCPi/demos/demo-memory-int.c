/*
* demo-memory-int.c
*
*  Created on: 12 June 2017
*
*      compile with "gcc ../ABE_RTCPi.c demo-memory-int.c -o demo-memory-int"
*      run with "./demo-memory-int"
*
*  This demo shows how to write to and read from the internal battery-backed memory on the DS1307 RTC chip
*/

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "../ABE_RTCPi.h"


int main(int argc, char **argv) {


	int inval = 6254; // declare the input value

	printf("Number written to SRAM: %d\n", inval);

	uint8_t bytearray[sizeof(inval)]; // byte array to be stored in the RTC SRAM

	int a = 0;

	for (a = 0; a < (int)sizeof(inval); a++) {
		bytearray[a] = (inval >> a * 8) & 0xFF;
	}

	rtc_write_memory(0x08, bytearray); // write the byte array to the RTC SRAM
	
	
	int outval = 0; // declare the output value

	// read the RTC SRAM into a 4-byte array.  
	// The array is allocated by the rtc_read_memory function, use a pointer to point to the memory location of this array.
	uint8_t *readarray = rtc_read_memory(0x08, sizeof(inval));

	for (a = 0; a < (int)sizeof(outval); a++) { // convert the bytes from the readarray into a number
		outval |= readarray[a] << (a * 8);
	}

	free(readarray); // free the memory allocated to readarray

	printf("Number read from SRAM: %d", outval);

	(void)argc;
	(void)argv;
	return (0);
}
