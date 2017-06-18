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

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <linux/i2c-dev.h>

static int i2cbus;
const char *fileName = "/dev/i2c-1"; // change to /dev/i2c-0 if you are using a revision 0002 or 0003 model B
unsigned char writebuffer[10] = { 0 };
unsigned char readbuffer[10] = { 0 };
static char signbit = 0;

// local methods

static void open_i2c_bus() {
	if ((i2cbus = open(fileName, O_RDWR)) < 0) {
		printf("Failed to open i2c port for read %s \n", strerror(errno));
		exit(1);
	}
}

static void close_i2c_bus() {
	close(i2cbus);
}

static void read_byte_array(char address, char reg, char length) {

	if (ioctl(i2cbus, I2C_SLAVE, address) < 0) {
		printf("Failed to write to i2c port for read\n");
		exit(1);
	}

	writebuffer[0] = reg;

	if ((write(i2cbus, writebuffer, 1)) != 1) {
		printf("Failed to write to i2c device for read\n");
		exit(1);
	}

	read(i2cbus, readbuffer, 4);
}

static char update_byte(char byte, char bit, char value) {
	/*
	 internal method for setting the value of a single bit within a byte
	 */
	if (value == 0) {
		return (byte &= ~(1 << bit));

	} else {
		return (byte |= 1 << bit);
	}

}

static char set_pga(char config, char gain) {
	/*
	 internal method for Programmable Gain Amplifier gain selection
	 */
	switch (gain) {
	case 1:
		config = update_byte(config, 0, 0);
		config = update_byte(config, 1, 0);
		break;
	case 2:
		config = update_byte(config, 0, 1);
		config = update_byte(config, 1, 0);
		break;
	case 4:
		config = update_byte(config, 0, 0);
		config = update_byte(config, 1, 1);
		break;
	case 8:
		config = update_byte(config, 0, 1);
		config = update_byte(config, 1, 1);
		break;
	default:
		break;
	}
	return (config);
}

static char set_bit_rate(char config, char rate) {
	/*
	 internal method for bit rate selection
	 */
	switch (rate) {
	case 12:
		config = update_byte(config, 2, 0);
		config = update_byte(config, 3, 0);
		break;
	case 14:
		config = update_byte(config, 2, 1);
		config = update_byte(config, 3, 0);
		break;
	case 16:
		config = update_byte(config, 2, 0);
		config = update_byte(config, 3, 1);

		break;
	case 18:
		config = update_byte(config, 2, 1);
		config = update_byte(config, 3, 1);

		break;
	default:
		break;
	}
	return (config);
}

static char set_conversion_mode(char config, char mode) {
	/*
	 internal method for setting the conversion mode
	 */
	if (mode == 1) {
		config = update_byte(config, 4, 1);
	} else {
		config = update_byte(config, 4, 0);
	}

	return (config);
}

static char set_channel(char config, char channel) {
	/*
	 internal method for setting the channel
	 */
	switch (channel) {
	case 1:
		config = update_byte(config, 5, 0);
		config = update_byte(config, 6, 0);
		break;
	case 2:
		config = update_byte(config, 5, 1);
		config = update_byte(config, 6, 0);
		break;
	case 3:
		config = update_byte(config, 5, 0);
		config = update_byte(config, 6, 1);
		break;
	case 4:
		config = update_byte(config, 5, 1);
		config = update_byte(config, 6, 1);
		break;
	}

	return (config);
}

/**
* Reads the raw value from the selected ADC channel
* @param address - I2C address for the target device e.g. 0x68
* @param channel - 1 to 4
* @param bitrate - 12, 14, 16 or 18
* @param pga - 1, 2, 4 or 8
* @param conversionmode - 0 = one shot conversion, 1 = continuous conversion
* @returns - raw long value from ADC buffer
*/
int read_raw(char address, char channel, int bitrate, int pga,
		char conversionmode) {
	// variables for storing the raw bytes from the ADC
	char h = 0;
	char l = 0;
	char m = 0;
	char s = 0;
	char config = 0x9C;
	long t = 0;
	signbit = 0;

	// set the config based on the provided parameters
	config = set_channel(config, channel);
	config = set_conversion_mode(config, conversionmode);
	config = set_bit_rate(config, bitrate);
	config = set_pga(config, pga);

	// keep reading the ADC data until the conversion result is ready
	int timeout = 1000; // number of reads before a timeout occurs
	int x = 0;

	open_i2c_bus();

	do {
		if (bitrate == 18) {
			read_byte_array(address, config, 3);
			h = readbuffer[0];
			m = readbuffer[1];
			l = readbuffer[2];
			s = readbuffer[3];
		} else {
			read_byte_array(address, config, 2);
			h = readbuffer[0];
			m = readbuffer[1];
			s = readbuffer[2];
		}

		// check bit 7 of s to see if the conversion result is ready
		if (!(s & (1 << 7))) {
			break;
		}

		if (x > timeout) {
			// timeout occurred
			return (0);
		}

		x++;
	} while (1);

	close_i2c_bus();

	// extract the returned bytes and combine in the correct order
	switch (bitrate) {
	case 18:
		t = ((h & 3) << 16) | (m << 8) | l;
		if ((t >> 17) & 1) {
			signbit = 1;
			t &= ~(1 << 17);
		}
		break;
	case 16:
		t = (h << 8) | m;
		if ((t >> 15) & 1) {
			signbit = 1;
			t &= ~(1 << 15);
		}
		break;
	case 14:
		t = ((h & 63) << 8) | m;
		if ((t >> 13) & 1) {
			signbit = 1;
			t &= ~(1 << 13);
		}
		break;
	case 12:
		t = ((h & 15) << 8) | m;
		if ((t >> 11) & 1) {
			signbit = 1;
			t &= ~(1 << 11);
		}
		break;
	default:
		break;
	}

	return (t);
}

/**
* Returns the voltage from the selected ADC channel
* @param address - I2C address for the target device e.g. 0x68
* @param channel - 1 to 4
* @param bitrate - 12, 14, 16 or 18
* @param pga - 1, 2, 4 or 8
* @param conversionmode - 0 = one shot conversion, 1 = continuous conversion
* @returns - double voltage value from ADC
*/
double read_voltage(char address, char channel, int bitrate, int pga,
		char conversionmode) {
	int raw = read_raw(address, channel, bitrate, pga, conversionmode); // get the raw value

	// calculate the gain based on the pga value
	double gain = (double) pga / 2;
	double offset = 2.048 / (double) pga;

	// set the lsb value based on the bitrate
	double lsb = 0;

	switch (bitrate) {
	case 12:
		lsb = 0.0005;
		break;
	case 14:
		lsb = 0.000125;
		break;
	case 16:
		lsb = 0.00003125;
		break;
	case 18:
		lsb = 0.0000078125;
		break;
	default:
		return (9999);
		break;
	}

	if (signbit == 1) // if the signbit is 1 convert it back to positive and subtract 2.048.
			{
		double voltage = (double) raw * (lsb / gain) - offset; // calculate the voltage and return it
		return (voltage);
	} else {
		double voltage = (double) raw * (lsb / gain); // calculate the voltage and return it
		return (voltage);
	}
}

