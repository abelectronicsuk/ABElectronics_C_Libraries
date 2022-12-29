/*
 ================================================
 AB Electronics UK ADC Pi 8-Channel Analogue to Digital Converter
 See CHANGELOG.md for the version number
 ================================================

 Reads from the MCP3424 ADC on the ADC Pi and ADC Pi Plus.

 Two functions are available to use.

 read_raw(address,channel,bitrate,pga,conversionmode) returns the raw number from the ADC

 read_voltage(address,channel,bitrate,pga,conversionmode) returns the voltage present at the ADC input

 Required package: libi2c-dev
 apt-get install libi2c-dev
 */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>

static int i2cbus;
static const char *fileName = "/dev/i2c-1"; // change to /dev/i2c-0 if you are using a revision 0002 or 0003 model B
static uint8_t writebuffer[10] = { 0 };
static uint8_t readbuffer[10] = { 0 };
static uint8_t signbit = 0;

// local methods

static void read_byte_array(uint8_t address, uint8_t reg) {

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

static uint8_t update_byte(uint8_t byte, uint8_t bit, uint8_t value) {
	/*
	 internal method for setting the value of a single bit within a byte
	 */
	if (value == 0) {
		return (byte &= ~(1 << bit));

	} else {
		return (byte |= 1 << bit);
	}

}

static uint8_t set_pga(uint8_t config, uint8_t gain) {
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

static uint8_t set_bit_rate(uint8_t config, uint8_t rate) {
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

static uint8_t set_conversion_mode(uint8_t config, uint8_t mode) {
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

static uint8_t set_channel(uint8_t config, uint8_t channel) {
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
* @param conversionmode - 0 = one-shot conversion, 1 = continuous conversion
* @returns - uint32_t value from ADC buffer
*/
uint32_t read_raw(uint8_t address, uint8_t channel, uint8_t bitrate, uint8_t pga, uint8_t conversionmode) {
	// variables for storing the raw bytes from the ADC
	uint8_t h = 0;
	uint8_t l = 0;
	uint8_t m = 0;
	uint8_t s = 0;
	uint8_t config = 0x9C;
	uint32_t t = 0;
	signbit = 0;

	// set the config based on the provided parameters
	config = set_channel(config, channel);
	config = set_conversion_mode(config, conversionmode);
	config = set_bit_rate(config, bitrate);
	config = set_pga(config, pga);

	// keep reading the ADC data until the conversion result is ready
	uint32_t timeout = 1000; // number of reads before a timeout occurs
	uint32_t x = 0;

	// open the i2c bus
	if ((i2cbus = open(fileName, O_RDWR)) < 0) {
		printf("Failed to open i2c port for read %s \n", strerror(errno));
		exit(1);
	}

	do {
		if (bitrate == 18) {
			read_byte_array(address, config);
			h = readbuffer[0];
			m = readbuffer[1];
			l = readbuffer[2];
			s = readbuffer[3];
		} else {
			read_byte_array(address, config);
			h = readbuffer[0];
			m = readbuffer[1];
			s = readbuffer[2];
		}

		if (!(s & (1 << 7))) break; // check bit 7 of s to see if the conversion result is ready

		if (x > timeout) return (0); // timeout occurred

		x++;
	} while (1);

	close(i2cbus);

	// extract the returned bytes and combine them in the correct order
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
* @param conversionmode - 0 = one-shot conversion, 1 = continuous conversion
* @returns - double voltage value from ADC
*/
double read_voltage(uint8_t address, uint8_t channel, uint8_t bitrate, uint8_t pga, uint8_t conversionmode) {
	uint32_t raw = read_raw(address, channel, bitrate, pga, conversionmode); // get the raw value

	// calculate the gain based on the pga value
	double gain = (double) pga / 2;
	double offset = 2.048 / (double) pga;

	// set the LSB value based on the bitrate
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

	if (signbit == 1) // if the sign bit is 1 the value is negative and most likely noise so it can be ignored.
			{
		return (0);
	} else {
		double voltage = ((double)raw * (lsb / gain)) * 2.471; // calculate the voltage and return it
		return (voltage);
	}
}

