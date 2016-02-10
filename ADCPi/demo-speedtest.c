/*
 * demo-speedtest.c
 *
 *  Created on: 07 Jan 2016
 *
 *      compile with "gcc ABE_ADCPi.c demo-speedtest.c -o demo-speedtest"
 *      run with "./demo-speedtest"
 */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <time.h>


#include "ABE_ADCPi.h"

void clearscreen ()
{
    printf("\033[2J\033[1;1H");
}

int main(int argc, char **argv){
	setvbuf (stdout, NULL, _IONBF, 0); // needed to print to the command line

	struct timeval t1, t2;
	double elapsedTime;

	int numberofsamples = 240;
	double samplearray[numberofsamples];

	// start timer
	gettimeofday(&t1, NULL);

	int x;
	for (x = 0; x <= numberofsamples; x++){
		samplearray[x] = read_voltage(0x68,1, 12, 1, 1); // read from adc chip 1, channel 1, 12 bit, pga gain set to 1 and continuous conversion mode
	}

	// stop timer
	    gettimeofday(&t2, NULL);

	    // calculate the average value
	    double average, sum;
	    for (x = 0; x <= numberofsamples; x++){
	    	sum += samplearray[x];
	    }

	    average = sum/numberofsamples;

	    // compute and print the elapsed time in millisec
	    elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0;      // sec to ms
	    elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0;   // us to ms

	    printf("%d samples in %G ms.\nAverage value was %Gv",numberofsamples,elapsedTime,average);


	return (0);
}

