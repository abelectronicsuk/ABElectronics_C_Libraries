/*
 ================================================
 AB Electronics UK IO Zero 32: 32-Channel Port Expander
 See CHANGELOG.md for the version number
 ================================================

 32-Channel Port Expander based on the PCA9535.
 The PCA9535 chip is split into two 8-bit ports.  Port 0 controls
 pins 1 to 8 while Port 1 controls pins 9 to 16.
 When writing to or reading from a bus or port the least significant bit
 represents the lowest numbered pin on the selected port.

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

#define INPUTPORT0  0x00 // Command byte Input port 0
#define INPUTPORT1  0x01 // Command byte Input port 1
#define OUTPUTPORT0 0x02 // Command byte Output port 0
#define OUTPUTPORT1 0x03 // Command byte Output port 1
#define INVERTPORT0 0x04 // Command byte Polarity Inversion port 0
#define INVERTPORT1 0x05 // Command byte Polarity Inversion port 1
#define CONFIGPORT0 0x06 // Command byte Configuration port 0
#define CONFIGPORT1 0x07 // Command byte Configuration port 1

// variables
static int i2cbus;
static const char *fileName = "/dev/i2c-1"; // change to /dev/i2c-0 if you are using a revision 0002 or 0003 model B
static uint8_t buf[10] = { 0 };

// private functions

static uint8_t read_byte_data(uint8_t address, uint8_t reg) {
	/*
	 internal method for reading one byte from the i2c bus
	*/
	if ((i2cbus = open(fileName, O_RDWR)) < 0) {
		printf("Failed to open i2c port for read %s \n", strerror(errno));

		exit(1);
	}

	if (ioctl(i2cbus, I2C_SLAVE, address) < 0) {
		printf("Failed to write to i2c port for read\n");
		exit(1);
	}

	buf[0] = reg;

	if ((write(i2cbus, buf, 1)) != 1) {
		printf("Failed to write to i2c device for read\n");
		exit(1);
	}

	if (read(i2cbus, buf, 1) != 1) { // Read back data into buf[]
		printf("Failed to read from slave\n");
		exit(1);
	}

	close(i2cbus);

	return (buf[0]);
}

static uint16_t read_word_data(uint8_t address, uint8_t reg) {
	/*
	 internal method for reading two bytes from the i2c bus
	*/
	if ((i2cbus = open(fileName, O_RDWR)) < 0) {
		printf("Failed to open i2c port for read %s \n", strerror(errno));

		exit(1);
	}

	if (ioctl(i2cbus, I2C_SLAVE, address) < 0) {
		printf("Failed to write to i2c port for read\n");
		exit(1);
	}

	buf[0] = reg;

	if ((write(i2cbus, buf, 1)) != 1) {
		printf("Failed to write to i2c device for read\n");
		exit(1);
	}

	if (read(i2cbus, buf, 2) != 2) { // Read back data into buf[]
		printf("Failed to read from slave\n");
		exit(1);
	}

	close(i2cbus);

	uint16_t value = (buf[1] << 8) | buf[0];
	return (value);
}

static void write_byte_data(uint8_t address, uint8_t reg, uint8_t value) {
	/*
	 internal method for writing one byte to the i2c bus
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

static void write_word_data(uint8_t address, uint8_t reg, uint16_t value) {
	/*
	 internal method for writing two bytes to the i2c bus
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
	buf[1] = (uint8_t)(value&(0xff)); // lower 8 bits
	buf[2] = (uint8_t)(value>>8) & 0xff; // upper 8 bits

	if ((write(i2cbus, buf, 3)) != 3) {
		printf("Failed to write to i2c device for write\n");
		exit(1);
	}

	close(i2cbus);
}

static uint8_t updatebyte(uint8_t byte, uint8_t bit, uint8_t value) {
	/*
	 internal method for setting the value of a single bit within a byte
	*/
	if (value == 0) {
		return (byte &= ~(1 << bit));

	} else {
		return (byte |= 1 << bit);
	}
}

static uint8_t checkbit(uint8_t byte, uint8_t bit) {
	/*
	 internal method for reading the value of a single bit within a byte
	*/
	if (byte & (1 << bit)) {
		return (1);
	} else {
		return (0);
	}
}

static void set_pin(uint8_t address, uint8_t pin, uint8_t value, uint8_t a_register, uint8_t b_register)
{
	/**
	* internal method for setting the value of a single bit within the device registers
	*/
	uint8_t reg = 0;
	uint8_t p = 0;
	if (pin >= 1 && pin <= 8)
	{
		reg = a_register;
		p = pin - 1;
	}
	else if (pin >= 9 && pin <= 16)
	{
		reg = b_register;
		p = pin - 9;
	}
	else
	{
		printf("pin out of range: 1 to 16");
	}

	if (value > 1)
	{
		printf("value out of range: 0 or 1");
	}

	uint8_t newval = updatebyte(read_byte_data(address, reg), p, value);
	write_byte_data(address, reg, newval);
}

static uint8_t get_pin(uint8_t address, uint8_t pin, uint8_t a_register, uint8_t b_register)
{
	/**
	* internal method for getting the value of a single bit within the device registers
	*/

		uint8_t value = 0;

        if (pin >= 1 && pin <= 8)
		{
            value = checkbit(read_byte_data(address, a_register), pin - 1);
		}
        else if (pin >= 9 && pin <= 16)
		{
            value = checkbit(read_byte_data(address, b_register), pin - 9);
		}
        else
		{
            printf("pin out of range: 1 to 16");
		}

        return value;
}

static void set_port(uint8_t address, uint8_t port, uint8_t value, uint8_t a_register, uint8_t b_register)
{
	/**
	* internal method for setting the value of a device register
	*/
	if (port == 0)
	{
    	write_byte_data(address, a_register, value);
	}
    else if (port == 1)
	{
    	write_byte_data(address, b_register, value);
	}
	else
	{
		printf("port out of range: 0 or 1");
	}
}

static uint8_t get_port(uint8_t address, uint8_t port, uint8_t a_register, uint8_t b_register)
{
	/**
	* internal method for getting the value of a device register
	*/
	if (port == 0)
	{
    	return (read_byte_data(address, a_register));
	}
    else if (port == 1)
	{
    	return (read_byte_data(address, b_register));
	}
	else
	{
		printf("port out of range: 0 or 1");
		return (0);
	}
}

// public methods

void set_pin_direction(uint8_t address, uint8_t pin, uint8_t direction)
{
	/**
	* Set the IO direction for an individual pin
	* @param address - I2C address for the target device
	* @param pins - 1 to 16
	* @param direction - 1 = input, 0 = output
	*/
	set_pin(address, pin, direction, CONFIGPORT0, CONFIGPORT1);
}

uint8_t get_pin_direction(uint8_t address, uint8_t pin)
{
	/**
	* Get the IO direction for an individual pin
	* @param address - I2C address for the target device
	* @param pins - 1 to 16
	* @returns 1 = input, 0 = output
	*/
	return get_pin(address, pin, CONFIGPORT0, CONFIGPORT1);
}

void set_port_direction(uint8_t address, uint8_t port, uint8_t direction)
{
	/**
	* Set the direction for an IO port
	* @param address - I2C address for the target device
	* @param port - 0 = pins 1 to 8, port 1 = pins 9 to 16
	* @param direction - 0 to 255 (0xFF).  For each bit 1 = input, 0 = output
	*/
	set_port(address, port, direction, CONFIGPORT0, CONFIGPORT1);
}

uint8_t get_port_direction(uint8_t address, uint8_t port)
{
	/**
	* Get the direction for an IO port
	* @param address - I2C address for the target device
	* @param port - 0 = pins 1 to 8, port 1 = pins 9 to 16
	* @returns 0 to 255 (0xFF).  For each bit 1 = input, 0 = output
	*/
	return get_port(address, port, CONFIGPORT0, CONFIGPORT1);
}

void set_bus_direction(uint8_t address, uint16_t direction)
{
	/**
	* Set the direction for the IO bus
	* @param address - I2C address for the target device
	* @param direction - 0 to 65535 (0xFFFF).  For each bit 1 = input, 0 = output
	*/
	write_word_data(address, CONFIGPORT0, direction);
}

uint16_t get_bus_direction(uint8_t address)
{
	/**
	* Get the direction for the IO bus
	* @param address - I2C address for the target device
	* @returns 0 to 65535 (0xFFFF).  For each bit 1 = input, 0 = output
	*/
	return read_word_data(address, CONFIGPORT0);
}

void write_pin(uint8_t address, uint8_t pin, uint8_t value)
{
	/**
	* Write to an individual pin 1 - 16
	* @param address - I2C address for the target device
	* @param pin - 1 to 16
	* @param value - 0 = logic low, 1 = logic high
	*/
	set_pin(address, pin, value, OUTPUTPORT0, OUTPUTPORT1);
}

void write_port(uint8_t address, uint8_t port, uint8_t value)
{
	/**
	* Write to all pins on the selected port
	* @param address - I2C address for the target device
	* @param port - 0 = pins 1 to 8, port 1 = pins 9 to 16
	* @param value - 0 to 255 (0xFF)
	*/
	set_port(address, port, value, OUTPUTPORT0, OUTPUTPORT1);
}

void write_bus(uint8_t address, uint16_t value)
{
	/**
	* Write to all pins on the selected bus
	* @param address - I2C address for the target device
	* @param value - 0 to 65535 (0xFFFF). For each bit 1 = logic high, 0 = logic low
	*/
	write_word_data(address, OUTPUTPORT0, value);
}

uint8_t read_pin(uint8_t address, uint8_t pin)
{
	/**
	* Read the value of an individual pin
	* @param address - I2C address for the target device
	* @param pin - 1 to 16
	* @returns - 0 = logic low, 1 = logic high
	*/

	return get_pin(address, pin, INPUTPORT0, INPUTPORT1);
}

uint8_t read_port(uint8_t address, uint8_t port)
{
	/**
	* Read all pins on the selected port
	* @param address - I2C address for the target device
	* @param port - 0 = pins 1 to 8, port 1 = pins 9 to 16
	* @returns - 0 to 255 (0xFF). For each bit 1 = logic high, 0 = logic low
	*/
	return get_port(address, port, INPUTPORT0, INPUTPORT1);
}

uint16_t read_bus(uint8_t address)
{
	/**
	* Read all pins on the selected bus
	* @param address - I2C address for the target device
	* @returns - 0 to 65535 (0xFFFF). For each bit 1 = logic high, 0 = logic low
	*/
	return read_word_data(address, INPUTPORT0);
}

void set_pin_polarity(uint8_t address, uint8_t pin, uint8_t polarity)
{
	/**
	* Set the polarity of the selected pin
	* @param address - I2C address for the target device
	* @param pin - 1 to 16
	* @param polarity - 0 = non-inverted, 1 = inverted
	*/
	set_pin(address, pin, polarity, INVERTPORT0, INVERTPORT1);
}

uint8_t get_pin_polarity(uint8_t address, uint8_t pin)
{
	/**
  	* Get the polarity of the selected pin
	* @param address - I2C address for the target device
  	* @param pin - 1 to 16
	* @returns - pin polarity 0 = non-inverted, 1 = inverted
  	*/
  	return get_pin(address, pin, INVERTPORT0, INVERTPORT1);
}

void set_port_polarity(uint8_t address, uint8_t port, uint8_t polarity)
{
	/**
	* Set the polarity of the pins on a selected port
	* @param address - I2C address for the target device
	* @param port - 0 = pins 1 to 8, port 1 = pins 9 to 16
	* @param polarity - 0 to 255 (0xFF). For each bit 0 = non-inverted, 1 = inverted
	*/
	set_port(address, port, polarity, INVERTPORT0, INVERTPORT1);
}

uint8_t get_port_polarity(uint8_t address, uint8_t port)
{
	/**
  	* Get the polarity of the selected pin
	* @param address - I2C address for the target device
  	* @param port - 0 = pins 1 to 8, port 1 = pins 9 to 16
	* @returns - 0 to 255 (0xFF). For each bit 0 = non-inverted, 1 = inverted
  	*/
  	return get_port(address, port, INVERTPORT0, INVERTPORT1);
}

void set_bus_polarity(uint8_t address, uint16_t polarity)
{
	/**
	* Set the polarity of the pins on a selected bus
	* @param address - I2C address for the target device
	* @param polarity - 0 to 65535 (0xFFFF). For each bit 0 = non-inverted, 1 = inverted
	*/
	write_word_data(address, INVERTPORT0, polarity);
}

 uint16_t get_bus_polarity(uint8_t address)
 {
	 /**
  	* Get the polarity of the bus
	* @param address - I2C address for the target device
	* @returns - 0 to 65535 (0xFFFF). For each bit 0 = non-inverted, 1 = inverted
  	*/
  	return read_word_data(address, INVERTPORT0);
 }
