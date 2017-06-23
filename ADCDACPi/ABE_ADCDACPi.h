/*
================================================
ABElectronics UK ADC-DAC Pi
Version 1.0 Created 16/06/2017
================================================

*/

#ifndef ADCDACPI_ABE_ADCDACPI_H_
#define ADCDACPI_ABE_ADCDACPI_H_



#endif /* ADCDACPI_ABE_ADCDACPI_H_ */

/**
* Open the ADC SPI bus channel
* This needs to be called before using the DAC
*/
int open_adc();

/**
* Close the ADC SPI bus channel
*/
void close_adc();

/**
* Open the DAC SPI bus channel
* This needs to be called before using the DAC
*/
int open_dac();

/**
* Close the DAC SPI bus channel
*/
void close_dac();

/**
* Read the voltage from the ADC
* @param channel - 1 or 2
* @param mode - 0 = Single Ended or 1 = Differential
* When in differential mode setting channel to 1 will make IN1 = IN+ and IN2 = IN-
* When in differential mode setting channel to 2 will make IN1 = IN- and IN2 = IN+
* @returns between 0V and the reference voltage
*/
double read_adc_voltage(int channel, int mode);

/**
* Read the raw value from the ADC
* @param channel -  1 to 8
* @param mode -  0 = Single Ended or 1 = Differential
* When in differential mode setting channel to 1 will make IN1 = IN+ and IN2 = IN-
* When in differential mode setting channel to 2 will make IN1 = IN- and IN2 = IN+
* @returns 12 bit value between 0 and 4096
*/
double read_adc_raw(int channel, int mode);

/**
* Set the reference voltage for the adc
* @param ref - Set this value to be the same as the voltage measured on the 3.3V GPIO pin
*/
void set_adc_refvoltage(double ref);

/**
* Set the DAC voltage
* @param voltage - between 0 and 2.048 when gain is set to 1,  0 and 3.3 when gain is set to 2
* @param channel - 1 or 2
*/
void set_dac_voltage(double voltage, int channel);

/**
* Set the raw value from the selected channel on the DAC
* @param raw - between 0 and 4095
* @param channel - 1 or 2
*/
void set_dac_raw(uint16_t raw, int channel);

/**
* Set the DAC gain
* @param gain - 1 or 2 - The output voltage will be between 0 and 2.048V when gain is set to 1,  0 and 3.3V when gain is set to 2
*/
void set_dac_gain(int gain);

