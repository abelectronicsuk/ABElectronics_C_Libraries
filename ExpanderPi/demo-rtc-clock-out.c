/*
* demo-rtc-clock-out.c
*
*  Created on: 16 June 2017
*
*      compile with "gcc ABE_ExpanderPi.c demo-rtc-clock-out.c -o demo-rtc-clock-out"
*      run with "./demo-rtc-clock-out"
*
*  This demo shows how to enable the clock square wave output on the RTC Pi and set the frequency
*/

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>


#include "ABE_ExpanderPi.h"

int main(int argc, char **argv) {
	
	// set the frequency of the output. Options are : 1 = 1Hz, 2 = 4.096KHz, 3 = 8.192KHz, 4 = 32.768KHz
	rtc_set_frequency(3);

	// enable the square-wave output
	rtc_enable_output();

	return (0);
}
