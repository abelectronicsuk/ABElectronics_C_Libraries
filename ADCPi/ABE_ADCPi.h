/*
 ================================================
 ABElectronics UK ADC Pi 8-Channel Analogue to Digital Converter
 Version 1.1 Created 23/01/2015 - Updated 27/05/2015
 ================================================

Reads from the MCP3424 ADC on the ADC Pi and ADC Pi Plus.

Two functions are available to use.

read_raw(address,channel,bitrate,pga,conversionmode) returns the raw number from the ADC

read_voltage(address,channel,bitrate,pga,conversionmode) returns the voltage present at the ADC input


 Required package{
 apt-get install libi2c-dev
 */


 /**
 * Reads the raw value from the selected ADC channel
 * @param address - I2C address for the target device e.g. 0x68
 * @param channel - 1 to 4
 * @param bitrate - 12, 14, 16 or 18
 * @param pga - 1, 2, 4 or 8
 * @param conversionmode - 0 = one shot conversion, 1 = continuous conversion
 * @returns - raw long value from ADC buffer
 */
long read_raw(char address, char channel, int bitrate, int pga,	char conversionmode);


/**
* Returns the voltage from the selected ADC channel
* @param address - I2C address for the target device e.g. 0x68
* @param channel - 1 to 4
* @param bitrate - 12, 14, 16 or 18
* @param pga - 1, 2, 4 or 8
* @param conversionmode - 0 = one shot conversion, 1 = continuous conversion
* @returns - double voltage value from ADC
*/
double read_voltage(char address, char channel, int bitrate, int pga, char conversionmode);


