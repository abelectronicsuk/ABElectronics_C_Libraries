/*
 * demo-adcspeedtest.c
 *
 *  Created on: 08 Sep 2020
 *
 *      compile with "gcc ../ABE_ADCDACPi.c demo-adcspeedtest.c -o demo-adcspeedtest"
 *      run with "./demo-adcspeedtest"
 */

#define numberofsamples 100000

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>

#include "../ABE_ADCDACPi.h"

void clearscreen ()
{
    printf("\033[2J\033[1;1H");
}

int main(int argc, char **argv){
	setvbuf (stdout, NULL, _IONBF, 0); // needed to print to the command line

	if (open_adc() != 1){ // open the ADC spi channel
			exit(1); // if the SPI bus fails to open exit the program
		}

	struct timeval t1, t2;
	double elapsedTime;

	double samplearray[numberofsamples];

	printf("Starting speed test...");

	// start timer
	gettimeofday(&t1, NULL);

	int x;
	for (x = 0; x < numberofsamples; x++)
	{
		samplearray[x] = read_adc_voltage(1, 0); // read from adc channel 1
	}

	// stop timer
	gettimeofday(&t2, NULL);

	// calculate the average value
	double average = 0;
	double sum = 0;
	for (x = 0; x < numberofsamples; x++)
	{
		sum += samplearray[x];
	}

	average = sum / numberofsamples;

	// compute and print the elapsed time in millisec
	elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0;	// sec to ms
	elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0; // us to ms

	// compute the sample rate
	double samplerate = (numberofsamples / elapsedTime) * 1000;

	printf("%d samples in %G ms.\nThe sample rate was %G samples per second\nThe average voltage was %Gv", numberofsamples, elapsedTime, samplerate, average);

	(void)argc;
	(void)argv;
	return (0);
}
