/*
 * demo-speedtest.c
 *
 *  Created on: 07 Jan 2016
 *
 *      compile with "gcc ../ABE_ADCPi.c demo-speedtest.c -o demo-speedtest"
 *      run with "./demo-speedtest"
 */

#include <stdio.h>
#include <stdint.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

#include "../ABE_ADCPi.h"

#define numberofsamples 1000

void clearscreen()
{
	printf("\033[2J\033[1;1H");
}

int main(int argc, char **argv)
{
	setvbuf(stdout, NULL, _IONBF, 0); // Needed to print to the command line

	struct timeval t1, t2;
	double elapsedTime;

	double samplearray[numberofsamples];

	// Start timer
	gettimeofday(&t1, NULL);

	int x;

	for (x = 0; x < numberofsamples; x++)
	{
		samplearray[x] = read_voltage(0x68, 1, 12, 1, 1); // Read from ADC chip 1, channel 1, 12-bit, PGA gain set to 1 and continuous conversion mode
	}

	// Stop timer
	gettimeofday(&t2, NULL);

	// Calculate the average value
	double average = 0;
	double sum = 0;
	for (x = 0; x < numberofsamples; x++)
	{
		sum += samplearray[x];
	}

	average = sum / numberofsamples;

	// Compute and print the elapsed time in millisec
	elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0;	   // sec to ms
	elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0; // us to ms

	// Compute the sample rate
	double samplerate = (numberofsamples / elapsedTime) * 1000;

	printf("%d samples in %G ms.\nThe sample rate was %G samples per second\nThe average voltage was %Gv", numberofsamples, elapsedTime, samplerate, average);

	(void)argc;
	(void)argv;
	return (0);
}
