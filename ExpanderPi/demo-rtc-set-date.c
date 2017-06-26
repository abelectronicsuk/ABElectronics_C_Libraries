/*
* demo-rtcsetdate.c
*
*  Created on: 16 June 2017
*
*      compile with "gcc ABE_ExpanderPi.c demo-rtc-set-date.c -o demo-rtc-set-date"
*      run with "./demo-rtc-set-date"
*
*  This demo shows how to write a date to the RTC and read it back again.
*/

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>


#include "ABE_ExpanderPi.h"

int main(int argc, char **argv) {
	
	struct tm datetime; // create a tm struct to store the date

	// set the date
	datetime.tm_sec = 00; // seconds
	datetime.tm_min = 20; // minutes
	datetime.tm_hour = 17;// hours
	datetime.tm_wday = 4;// dayofweek
	datetime.tm_mday = 10;// day
	datetime.tm_mon = 06;// month - 1
	datetime.tm_year = 117; // years since 1900

	rtc_set_date(datetime);  // send the date to the RTC Pi
	
	char buffer[80]; // used for converting the date to a string
	
	while (1) {
		datetime = rtc_read_date(); // read the date from the RTC into the tm struct

		if (strftime(buffer, sizeof buffer, "%Y-%m-%dT%H:%M:%S", &datetime)) { // convert the date into a string and print it to the console
			puts(buffer);
		}
		else {
			puts("strftime failed");
		}

		//usleep(1000000); // sleep for a second

		usleep(10000); // sleep for a second
	}

	return (0);
}
