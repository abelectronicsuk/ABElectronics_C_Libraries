/*
 ================================================
 ABElectronics UK ADC Differential Pi 8-Channel Analogue to Digital Converter
 Version 1.0 Created 11/01/2016
 ================================================

Reads from the MCP3424 ADC on the ADC Differential Pi and Delta-Sigma Pi.

Two functions are available to use.

read_raw(address,channel,bitrate,pga,conversionmode) returns the raw number from the ADC

read_voltage(address,channel,bitrate,pga,conversionmode) returns the voltage present at the ADC input


 Required package{
 apt-get install libi2c-dev
 */


/// <summary>
/// Reads the raw value from the selected ADC channel
/// </summary>
/// <param name="address">I2C Address e.g.  0x68</param>
/// <param name="channel">1 to 4</param>
/// <param name="bitrate">12, 14, 16 or 18</param>
/// <param name="pga">1, 2, 4 or 8</param>
/// <param name="conversionmode">0 = one shot conversion, 1 = continuous conversion</param>
/// <returns>raw long value from ADC buffer</returns>
long read_raw(char address, char channel, int bitrate, int pga,	char conversionmode);


/// <summary>
/// Returns the voltage from the selected ADC channel
/// </summary>
/// <param name="address">I2C Address e.g.  0x68</param>
/// <param name="channel">1 to 4</param>
/// <param name="bitrate">12, 14, 16 or 18</param>
/// <param name="pga">1, 2, 4 or 8</param>
/// <param name="conversionmode">0 = one shot conversion, 1 = continuous conversion</param>
/// <returns>double voltage value from ADC</returns>
double read_voltage(char address, char channel, int bitrate, int pga, char conversionmode);


