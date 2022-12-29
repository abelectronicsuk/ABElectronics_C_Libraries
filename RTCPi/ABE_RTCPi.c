/*
================================================
 AB Electronics UK RTC Pi real-time clock
 See CHANGELOG.md for the version number
================================================

Required package:libi2c-dev
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
#include <time.h> 

// Define registers values from the datasheet
#define SECONDS 0x00
#define MINUTES 0x01
#define HOURS 0x02
#define DAYOFWEEK 0x03
#define DAY 0x04
#define MONTH 0x05
#define YEAR 0x06
#define CONTROL 0x07

#define HI_NIBBLE(b) (((b) >> 4) & 0x0F)
#define LO_NIBBLE(b) ((b) & 0x0F)

// Define local variables
uint8_t rtcAddress = 0x68;
uint8_t rtcConfig = 0x03;
uint16_t rtcCentury = 2000;

static int i2cbus;
static const char *fileName = "/dev/i2c-1"; // change to /dev/i2c-0 if you are using a revision 0002 or 0003 model B
static uint8_t buf[10] = { 0 };

static uint8_t writebuffer[60] = { 0 };
static uint8_t readbuffer[60] = { 0 };

static void read_byte_array(uint8_t address, uint8_t reg, uint8_t length) {

	if ((i2cbus = open(fileName, O_RDWR)) < 0) {
		printf("Failed to open i2c port for read %s \n", strerror(errno));

		exit(1);
	}

	if (ioctl(i2cbus, I2C_SLAVE, address) < 0) {
		printf("Failed to write to i2c port for read\n");
		exit(1);
	}

	writebuffer[0] = reg;

	if ((write(i2cbus, writebuffer, 1)) != 1) {
		printf("Failed to write to i2c device for read\n");
		exit(1);
	}

	read(i2cbus, readbuffer, length);

	close(i2cbus);
}

void write_byte_data(uint8_t address, uint8_t reg, uint8_t value) {
	/*
	Internal method for writing data to the i2c bus
	*/
	if ((i2cbus = open(fileName, O_RDWR)) < 0) {
		printf("Failed to open i2c port for write\n");
		exit(1);
	}

	if (ioctl(i2cbus, I2C_SLAVE, address) < 0) {
		printf("Failed to write to i2c port for write\n");
		exit(1);
	}

	buf[0] = reg;
	buf[1] = value;

	if ((write(i2cbus, buf, 2)) != 2) {
		printf("Failed to write to i2c device for write\n");
		exit(1);
	}

	close(i2cbus);
}

void write_byte_array(uint8_t address, uint8_t buffer[], uint8_t length) {
	/*
	Internal method for writing data to the i2c bus
	*/
	if ((i2cbus = open(fileName, O_RDWR)) < 0) {
		printf("Failed to open i2c port for write\n");
		exit(1);
	}

	if (ioctl(i2cbus, I2C_SLAVE, address) < 0) {
		printf("Failed to write to i2c port for write\n");
		exit(1);
	}

	if ((write(i2cbus, buffer, length)) != length) {
		printf("Failed to write to i2c device for write\n");
		exit(1);
	}

	close(i2cbus);
}

static uint8_t bcd_to_dec(uint8_t bcd) {
	/*
	Internal method for converting a BCD formatted number to decimal
	*/

	return (uint8_t)((HI_NIBBLE(bcd) * 10) + (LO_NIBBLE(bcd)));
}

static uint8_t dec_to_bcd(char dec) {
	/*
	Internal method for converting a decimal formatted number to BCD
	*/
	return (uint8_t)((dec / 10) * 16) + (dec % 10);
}

static uint8_t updatebyte(uint8_t byte, uint8_t bit, uint8_t value) {
	/*
	Internal method for setting the value of a single bit within a byte
	*/
	if (value == 0) {
		return (byte &= ~(1 << bit));

	}
	else {
		return (byte |= 1 << bit);
	}

}

void rtc_set_date(struct tm date) {
	/**
	* Set the date on the RTC
	* @param - date - struct tm formatted date and time
	*/
	writebuffer[0] = SECONDS; // register address for seconds
	writebuffer[1] = dec_to_bcd(date.tm_sec); // seconds
	writebuffer[2] = dec_to_bcd(date.tm_min); // minutes
	writebuffer[3] = dec_to_bcd(date.tm_hour);// hours
	writebuffer[4] = dec_to_bcd(date.tm_wday);// dayofweek
	writebuffer[5] = dec_to_bcd(date.tm_mday);// day
	writebuffer[6] = dec_to_bcd(date.tm_mon) + 1;// month
	writebuffer[7] = dec_to_bcd(date.tm_year % 100); // strip the century from the date

	write_byte_array(rtcAddress, writebuffer, 8);
}

struct tm rtc_read_date() {
	/**
	* Read the date from the RTC.
	* @returns - date as a tm struct
	*/

	read_byte_array(rtcAddress, 0, 7);
	struct tm date;
	date.tm_sec = bcd_to_dec(readbuffer[0]); // seconds
	date.tm_min = bcd_to_dec(readbuffer[1]); // minutes
	date.tm_hour = bcd_to_dec(readbuffer[2]);// hours
	date.tm_wday = bcd_to_dec(readbuffer[3]);// dayofweek
	date.tm_mday = bcd_to_dec(readbuffer[4]);// day
	date.tm_mon = bcd_to_dec(readbuffer[5]) - 1;// month
	date.tm_year = bcd_to_dec(readbuffer[6]) + (rtcCentury - 1900);// year

	return date;
}

void rtc_enable_output() {
	/**
	* Enable the squarewave output pin
	*/
	rtcConfig = updatebyte(rtcConfig, 7, 1);
	rtcConfig = updatebyte(rtcConfig, 4, 1);
	write_byte_data(rtcAddress, CONTROL, rtcConfig);
}

void rtc_disable_output() {
	/**
	* Disable the squarewave output pin
	*/
	rtcConfig = updatebyte(rtcConfig, 7, 0);
	rtcConfig = updatebyte(rtcConfig, 4, 0);
	write_byte_data(rtcAddress, CONTROL, rtcConfig);
}

void rtc_set_frequency(uint8_t frequency) {
	/**
	* Set the squarewave output frequency
	* @param - 1 = 1Hz, 2 = 4.096KHz, 3 = 8.192KHz, 4 = 32.768KHz
	*/
	switch (frequency) {
		case 1:
			rtcConfig = updatebyte(rtcConfig, 0, 0);
			rtcConfig = updatebyte(rtcConfig, 1, 0);
			write_byte_data(rtcAddress, CONTROL, rtcConfig);
			break;
		case 2:
			rtcConfig = updatebyte(rtcConfig, 0, 1);
			rtcConfig = updatebyte(rtcConfig, 1, 0);
			write_byte_data(rtcAddress, CONTROL, rtcConfig);
			break;
		case 3:
			rtcConfig = updatebyte(rtcConfig, 0, 0);
			rtcConfig = updatebyte(rtcConfig, 1, 1);
			write_byte_data(rtcAddress, CONTROL, rtcConfig);
			break;
		case 4:
			rtcConfig = updatebyte(rtcConfig, 0, 1);
			rtcConfig = updatebyte(rtcConfig, 1, 1);
			write_byte_data(rtcAddress, CONTROL, rtcConfig);
			break;
		default:
			printf("Error: rtc_set_frequency() - value must be between 1 and 4");

	}
}

void rtc_write_memory(uint8_t address, uint8_t *valuearray) {
	/**
	* Write to the memory on the DS1307
	* The DS1307 contains a 56-byte, battery-backed RAM with unlimited writes
	* @param address - 0x08 to 0x3F
	* @param valuearray - byte array containing data to be written to memory
	*/

	if (address >= 0x08 && address <= 0x3F) {
		if (address + sizeof(valuearray) <= 0x3F) {

			uint8_t length = sizeof(valuearray);

			uint8_t *writearray = malloc(length + 1);

			if (errno == ENOMEM) { // Fail!!!!
				free(writearray);
				printf("memory allocation error: not enough system memory to allocate array");
			}
			else {

				writearray[0] = address;

				int a;
				for (a = 0; a < length; a++) {
					writearray[a + 1] = valuearray[a];
				}

				write_byte_array(rtcAddress, writearray, (uint8_t)length + 1);

				free(writearray);
			}

		}
		else {
			printf("memory overflow error: address + length exceeds 0x3F");
		}
	}
	else{
		printf("address out of range");
	}
}

uint8_t *rtc_read_memory(uint8_t address, uint8_t length) {
	/**
	* Read from the memory on the DS1307
	* The DS1307 contains a 56-Byte, battery-backed RAM with unlimited writes
	* @param address - 0x08 to 0x3F
	* @param length - up to 32 bytes. length can not exceed the available address space.
	* @returns - pointer to a byte array where the data will be saved
	*/

	if (address >= 0x08 && address <= 0x3F) {
		if (address <= (0x3F - length)) {
			//write_i2c_block_data(_rtcAddress, address, valuearray);

			uint8_t *writearray = malloc(length);

			if (errno == ENOMEM) { // Fail!!!!
				free(writearray);
				printf("memory allocation error: not enough system memory to allocate array");
				return NULL;
			}
			else {
				read_byte_array(rtcAddress, address, length); // read the values from the SRAM into the read buffer

				// copy the read buffer into the writearray
				uint8_t i = 0;
				for (i = 0; i < length; i++) {
					writearray[i] = readbuffer[i];
				}

				return writearray;
			}

		}
		else {
			printf("memory overflow error: address + length exceeds 0x3F");
			return NULL;
		}
	}
	else {
		printf("address out of range");
		return NULL;
	}	
}