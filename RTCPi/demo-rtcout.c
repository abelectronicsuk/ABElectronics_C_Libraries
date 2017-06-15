/*
* demo-rtcout.c
*
*  Created on: 12 June 2017
*
*      compile with "gcc ABE_RTCPi.c demo-rtcout.c -o demo-rtcout"
*      run with "./demo-rtcout"
*
*  This demo shows how to enable the clock square wave output on the RTC Pi and set the frequency
*/

#include <stdio.h>
#include <time.h>


#include "ABE_RTCPi.h"

int main(int argc, char **argv) {
	
	// set the frequency of the output. Options are : 1 = 1Hz, 2 = 4.096KHz, 3 = 8.192KHz, 4 = 32.768KHz
	rtc_set_frequency(3);

	// enable the square-wave output
	rtc_enable_output();

	return (0);
}
