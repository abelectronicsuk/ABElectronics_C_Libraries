/*
================================================
ABElectronics UK ADC-DAC Pi
Version 1.0 Created 16/06/2017
================================================

*/

#include <stdint.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>


// local variables
static const char *adcdevice = "/dev/spidev0.0";
static const char *dacdevice = "/dev/spidev0.1";
static uint8_t mode = SPI_MODE_0; // SPI_MODE_0
static uint32_t speed = 2500000; // SPI bus speed

static uint8_t adctx[] = { 0x01, 0x80, 0x00 }; // transmit buffer for the ADC
static char adcrx[3]; // receive buffer for the adc;
static uint8_t dactx[2];
static int adc; // adc object
static int dac; // dac object

static double adcrefvoltage = 3.3; // reference voltage for the ADC chip.
static int dacgain = 1; // gain setting for the DAC chip.
static double dacvoltage = 2.048; // maximum voltage for the DAC output

int open_adc() {
	/**
	* Open the ADC SPI bus channel
	* This needs to be called before using the DAC
	*/

	// Open SPI device
	if ((adc = open(adcdevice, O_RDWR)) < 0)
		return (0);

	if (ioctl(adc, SPI_IOC_WR_MAX_SPEED_HZ, &speed) == -1)
		return (0);
	// Set SPI mode
	if (ioctl(adc, SPI_IOC_WR_MODE, &mode) == -1)
		return (0);

	return (1);
}

void close_adc() {
	/**
	* Close the ADC SPI bus channel
	*/
	close(adc);
}

int open_dac() {
	/**
	* Open the DAC SPI bus channel
	* This needs to be called before using the DAC
	*/

	// Open SPI device
	if ((dac = open(dacdevice, O_RDWR)) < 0)
		return (0);

	if (ioctl(dac, SPI_IOC_WR_MAX_SPEED_HZ, &speed) == -1)
		return (0);
	// Set SPI mode
	if (ioctl(dac, SPI_IOC_WR_MODE, &mode) == -1)
		return (0);

	return (1);
}

void close_dac() {
	/**
	* Close the DAC SPI bus channel
	*/
	close(dac);
}

double read_adc_voltage(int channel, int mode) {
	/**
	* Read the voltage from the ADC
	* @param channel - 1 or 2
	* @param mode - 0 = Single Ended or 1 = Differential
	* When in differential mode setting channel to 1 will make IN1 = IN+ and IN2 = IN-
	* When in differential mode setting channel to 2 will make IN1 = IN- and IN2 = IN+
	* @returns between 0V and the reference voltage
	*/

	int rawval = read_adc_raw(channel, mode);
	return ((adcrefvoltage / 4096) * (double) rawval);
}

int read_adc_raw(int channel, int mode) {
	/**
	* Read the raw value from the ADC
	* @param channel -  1 to 8
	* @param mode -  0 = Single Ended or 1 = Differential
	* When in differential mode setting channel to 1 will make IN1 = IN+ and IN2 = IN-
	* When in differential mode setting channel to 2 will make IN1 = IN- and IN2 = IN+
	* @returns 12 bit value between 0 and 4096
	*/
	if (channel == 1) {
		if (mode == 0) {
			adctx[1] = 0x80;
		} else if (mode == 1) {
			adctx[1] = 0x00;
		} else {
			return (0);
		}
	} else if (channel == 2) {
		if (mode == 0) {
			adctx[1] = 0xC0;
		} else if (mode == 1) {
			adctx[1] = 0x40;
		} else {
			return (0);
		}
	} else {
		return (0);
	}

	struct spi_ioc_transfer tr = { .tx_buf = (uintptr_t) adctx, .rx_buf =
			(uintptr_t) adcrx, .len = 3, .delay_usecs = 0, .speed_hz = speed,
			.bits_per_word = 8, };

	int ret = ioctl(adc, SPI_IOC_MESSAGE(1), &tr);

	return (((adcrx[1] & 0x0F) << 8) + (adcrx[2]));

}

void set_adc_refvoltage(double ref) {
	/**
	* Set the reference voltage for the adc
	* @param ref - Set this value to be the same as the voltage measured on the 3.3V GPIO pin
	*/
	adcrefvoltage = ref;
}

void set_dac_raw(uint16_t raw, int channel) {
	/**
	* Set the raw value from the selected channel on the DAC
	* @param raw - between 0 and 4095
	* @param channel - 1 or 2
	*/


	dactx[1] = (raw & 0xff);
	dactx[0] = (((raw >> 8) & 0xff) | (channel - 1) << 7 | 0x1 << 5 | 1 << 4);

	if (dacgain == 2) {
		dactx[0] = (dactx[0] &= ~(1 << 5));
    }

	struct spi_ioc_transfer tr = { .tx_buf = (unsigned long) &dactx, .rx_buf =
			(unsigned long) NULL, .len = 2, .delay_usecs = 0, .speed_hz = speed,
			.bits_per_word = 8, .cs_change = 0, };

	// Write data
	if (ioctl(dac, SPI_IOC_MESSAGE(1), &tr) < 1) {
		return;
	}
}

void set_dac_voltage(double voltage, int channel) {
	/**
	* Set the DAC voltage
	* @param voltage - between 0 and 2.048 when gain is set to 1,  0 and 3.3 when gain is set to 2
	* @param channel - 1 or 2
	*/
	if (channel == 1) {
		adctx[1] = 0x80;
	} else if (channel == 2) {
		adctx[1] = 0xC0;
	} else {
		return;
	}
	if ((voltage >= 0.0) && (voltage < dacvoltage)) {
		uint16_t rawval = ((voltage / 2.048) * 4096) / dacgain;
		set_dac_raw(rawval, channel);
	} else {
		return;
	}
}

void set_dac_gain(int gain) {
	/**
	* Set the DAC gain
	* @param gain - 1 or 2 - The output voltage will be between 0 and 2.048V when gain is set to 1,  0 and 3.3V when gain is set to 2
	*/
	if (gain == 1) {
		dacgain = 1;
		dacvoltage = 2.048;
	}
	if (gain == 2) {
		dacgain = 2;
		dacvoltage = 3.3;
	}

}
