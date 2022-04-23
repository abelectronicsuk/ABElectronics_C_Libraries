/*
 * adcdactest.c
 *
 *  Created on: 01 Mar 2021
 *
 *      compile with "gcc ../ABE_ADCDACPi.c adcdactest.c  -Wall -Wextra -Wpedantic -Woverflow -o adcdactest "
 *      run with "./adcdactest"
 * 
 *  This test checks each of the functions in the ABE_ADCDACPi.c file.
 *  To perform this test the ADC inputs on the ADC DAC Pi will need to be connected to the DAC outputs.
 *  IN1 connects to O1
 *  IN2 connects to O2
 */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

#include "../ABE_ADCDACPi.h"

void clearscreen ()
{
    printf("\033[2J\033[1;1H");
}

int main(int argc, char **argv){
	setvbuf (stdout, NULL, _IONBF, 0); // needed to print to the command line
	int passed = 1;
	uint16_t adcraw_error = 20; // error margin for adc raw readings to allow for noise and variation in the 3.3V power rail.

	double adcvolt_error = 0.1; // error margin for adc voltage readings to allow for noise and variation in the 3.3V power rail.

	/* functions tested

	open_adc()
	close_adc()
	open_dac()
	close_dac()
	read_adc_raw(uint8_t channel, uint8_t mode)
	read_adc_voltage(uint8_t channel, uint8_t mode)
	set_adc_refvoltage(double ref)
	set_dac_raw(uint16_t raw, uint8_t channel)
	set_dac_voltage(double voltage, uint8_t channel)
	set_dac_gain(uint8_t gain)

	*/

	printf("Starting Test\n");

	// open the spi channels for the ADC and DAC

	if (open_adc() == 1){ // open the ADC spi channel
		printf("open_adc(): PASSED\n");
	}
	else{
		printf("open_adc(): FAILED\n");
		passed = 0;
	}

	if (open_dac() == 1){ // open the DAC spi channel
		printf("open_dac(): PASSED\n");
	}
	else{
		printf("open_dac(): FAILED\n");
		passed = 0;
	}

	// Set the DAC gain to 1 and ADC voltage reference to 3.3

	set_dac_gain(1);
	set_adc_refvoltage(3.3);

	// Set the DAC raw value on channel 1 to 4095, 2.048V and channel 2 to 2047, 1.024V

	set_dac_raw(4095, 1);
	set_dac_raw(2047, 2);

	usleep(200000); // sleep 0.2 seconds

	// Read the raw value from ADC channel 1 in single ended mode. Expecting 2.048V or 2542

	uint16_t adcval1 = read_adc_raw(1, 0);
	
	if (adcval1 > (2542 - adcraw_error) && adcval1 < (2542 + adcraw_error)){
		printf("set_dac_raw() channel 1: PASSED\n");
		printf("read_adc_raw() channel 1: PASSED\n");
	}
	else{
		printf("set_dac_raw() channel 1: FAILED\n");
		printf("read_adc_raw() channel 1: FAILED\n");
		passed = 0;
	}

	// Read the raw value from ADC channel 2 in single ended mode. Expecting 1.024V or 1271

	uint16_t adcval2 = read_adc_raw(2, 0);

	if (adcval2 > (1271 - adcraw_error) && adcval2 < (1271 + adcraw_error)){
		printf("set_dac_raw() channel 2: PASSED\n");
		printf("read_adc_raw() channel 2: PASSED\n");
	}
	else{
		printf("set_dac_raw() channel 2: FAILED\n");
		printf("read_adc_raw() channel 2: FAILED\n");
		passed = 0;
	}

	// Set the DAC gain to 2

	set_dac_gain(2);

	// Set the DAC voltage value on channel 1 to 1.8V and channel 2 to 3.0V

	set_dac_voltage(1.8, 1);
	set_dac_voltage(3.0, 2);

	// Read the voltage from ADC channel 1 in single ended mode. Expecting 1.8V

	double adcvolt1 = read_adc_voltage(1, 0);
	
	if (adcvolt1 > (1.8 - adcvolt_error) && adcvolt1 < (1.8 + adcvolt_error)){
		printf("set_dac_voltage() channel 1: PASSED\n");
		printf("read_adc_voltage() channel 1: PASSED\n");
	}
	else{
		printf("set_dac_voltage() channel 1: FAILED\n");
		printf("read_adc_voltage() channel 1: FAILED\n");
		passed = 0;
	}

	// Read the voltage from ADC channel 2 in single ended mode. Expecting 3.0V

	double adcvolt2 = read_adc_voltage(2, 0);

	if (adcvolt2 > (3.0 - adcvolt_error) && adcvolt2 < (3.0 + adcvolt_error)){
		printf("set_dac_voltage() channel 2: PASSED\n");
		printf("read_adc_voltage() channel 2: PASSED\n");
	}
	else{
		printf("set_dac_voltage() channel 2: FAILED\n");
		printf("read_adc_voltage() channel 2: FAILED\n");
		passed = 0;
	}

	// Read the differential voltage from ADC channels 2 and 1. 
	// Measures the difference of channel 2 - channel 1
	//Expecting 1.2V

	double adcdiffvolt = read_adc_voltage(2, 1);

	if (adcdiffvolt > (1.2 - adcvolt_error) && adcdiffvolt < (1.2 + adcvolt_error)){
		printf("read_adc_voltage() Differential Mode Channel 2 - 1: PASSED\n");
	}
	else{
		printf("read_adc_voltage() Differential Mode Channel 2 - 1: FAILED\n");
		passed = 0;
	}

	// Change the DAC voltage for channels 1 to 3.0V and 2 to 1.2V
	// Read the differential voltage from ADC channels 1 and 2. 
	// Measures the difference of channel 1 - channel 2
	//Expecting 1.8V

	set_dac_voltage(1.2, 2);
	set_dac_voltage(3.0, 1);

	adcdiffvolt = read_adc_voltage(1, 1);

	if (adcdiffvolt > (1.8 - adcvolt_error) && adcdiffvolt < (1.8 + adcvolt_error)){
		printf("read_adc_voltage() Differential Mode Channel 1 - 2: PASSED\n");
	}
	else{
		printf("read_adc_voltage() Differential Mode Channel 1 - 2: FAILED\n");
		passed = 0;
	}

	// close the spi channels for the ADC and DAC

	if (close_adc() == 0){
		printf("close_adc(): PASSED\n");
	}
	else{
		printf("close_adc(): FAILED - Error %d\n", errno);
		passed = 0;
	}

	if (close_dac() == 0){
		printf("close_dac(): PASSED\n");
	}
	else{
		printf("close_dac(): FAILED - Error %d\n", errno);
		passed = 0;
	}

	if (passed == 1){
		printf("All Tests: PASSED\n");
	}
	else{
		printf("One or more Tests FAILED\n");
	}

	(void)argc;
	(void)argv;
	return (0);
}
