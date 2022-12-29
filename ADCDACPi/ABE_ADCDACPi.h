/*
================================================
AB Electronics UK ADC DAC Pi Analogue to Digital / Digital to Analogue Converter

Based on the Microchip MCP3202 and MCP4822

See CHANGELOG.md for the version number
================================================
*/

#ifndef ADCDACPI_ABE_ADCDACPI_H_
#define ADCDACPI_ABE_ADCDACPI_H_


#endif /* ADCDACPI_ABE_ADCDACPI_H_ */

/**
* Open the ADC SPI bus channel
* This needs to be called before using the ADC
*/
int open_adc();

/**
* Close the ADC SPI bus channel
*/
int close_adc();

/**
* Open the DAC SPI bus channel
* This needs to be called before using the DAC
*/
int open_dac();

/**
* Close the DAC SPI bus channel
*/
int close_dac();

/**
* Read the voltage from the ADC
* @param channel - 1 or 2
* @param mode - 0 = Single Ended or 1 = Differential
* When in differential mode setting, channel to 1 will make IN1 = IN+ and IN2 = IN-
* When in differential mode setting, channel to 2 will make IN1 = IN- and IN2 = IN+
* @returns between 0V and the reference voltage
*/
double read_adc_voltage(uint8_t channel, uint8_t mode);

/**
* Read the raw value from the ADC
* @param channel -  1 to 8
* @param mode -  0 = Single Ended or 1 = Differential
* When in differential mode setting, channel to 1 will make IN1 = IN+ and IN2 = IN-
* When in differential mode setting, channel to 2 will make IN1 = IN- and IN2 = IN+
* @returns 12-bit value between 0 and 4096
*/
uint16_t read_adc_raw(uint8_t channel, uint8_t mode);

/**
* Set the reference voltage for the ADC
* @param ref - Set this value to be the same as the voltage measured on the 3.3V GPIO pin
*/
void set_adc_refvoltage(double ref);

/**
* Set the DAC voltage
* @param voltage - between 0 and 2.048 when the gain is set to 1,  0 and 3.3 when the gain is set to 2
* @param channel - 1 or 2
*/
void set_dac_voltage(double voltage, uint8_t channel);

/**
* Set the raw value from the selected channel on the DAC
* @param raw - between 0 and 4095
* @param channel - 1 or 2
*/
void set_dac_raw(uint16_t raw, uint8_t channel);

/**
* Set the DAC gain
* @param gain - 1 or 2 - The output voltage will be between 0 and 2.048V when the gain is set to 1,  0 and 3.3V when the gain is set to 2
*/
void set_dac_gain(uint8_t gain);

