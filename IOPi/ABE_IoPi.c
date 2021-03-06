/*
 ================================================
 AB Electronics UK IO Pi 32-Channel Port Expander
 See CHANGELOG.md for version number
 ================================================

 Each MCP23017 chip is split into two 8-bit ports.  port 0 controls
 pins 1 to 8 while port 1 controls pins 9 to 16.
 When writing to or reading from a port the least significant bit represents
 the lowest numbered pin on the selected port.

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



#define IODIRA 0x00 // IO direction A - 1= input 0 = output
#define IODIRB 0x01 // IO direction B - 1= input 0 = output
	// Input polarity A - If a bit is set, the corresponding GPIO register bit
	// will reflect the inverted value on the pin.
#define IPOLA 0x02
	// Input polarity B - If a bit is set, the corresponding GPIO register bit
	// will reflect the inverted value on the pin.
#define IPOLB 0x03
	// The GPINTEN register controls the interrupt-onchange feature for each
	// pin on port A.
#define GPINTENA 0x04
	// The GPINTEN register controls the interrupt-onchange feature for each
	// pin on port B.
#define GPINTENB 0x05
	// Default value for port A - These bits set the compare value for pins
	// configured for interrupt-on-change. If the associated pin level is the
	// opposite from the register bit, an interrupt occurs.
#define DEFVALA 0x06
	// Default value for port B - These bits set the compare value for pins
	// configured for interrupt-on-change. If the associated pin level is the
	// opposite from the register bit, an interrupt occurs.
#define DEFVALB 0x07
	// Interrupt control register for port A.  If 1 interrupt is fired when the
	// pin matches the default value, if 0 the interrupt is fired on state
	// change
#define INTCONA 0x08
	// Interrupt control register for port B.  If 1 interrupt is fired when the
	// pin matches the default value, if 0 the interrupt is fired on state
	// change
#define INTCONB 0x09
#define IOCON 0x0A // see datasheet for configuration register
#define GPPUA 0x0C // pull-up resistors for port A
#define GPPUB 0x0D // pull-up resistors for port B
	// The INTF register reflects the interrupt condition on the port A pins of
	// any pin that is enabled for interrupts. A set bit indicates that the
	// associated pin caused the interrupt.
#define INTFA 0x0E
	// The INTF register reflects the interrupt condition on the port B pins of
	// any pin that is enabled for interrupts. A set bit indicates that the
	// associated pin caused the interrupt.
#define INTFB 0x0F
	// The INTCAP register captures the GPIO port A value at the time the
	// interrupt occurred.
#define INTCAPA 0x10
	// The INTCAP register captures the GPIO port B value at the time the
	// interrupt occurred.
#define INTCAPB 0x11
#define GPIOA 0x12 // data port A
#define GPIOB 0x13 // data port B
#define OLATA 0x14 // output latches A
#define OLATB 0x15 // output latches B

// variables
// initial configuration - see IOCON page in the MCP23017 datasheet for more information.
static uint8_t config = 0x02;
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
	 internal method for writing one bytes to the i2c bus
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
	* Set IO direction for an individual pin
	* @param address - I2C address for the target device
	* @param pins - 1 to 16
	* @param direction - 1 = input, 0 = output
	*/
	set_pin(address, pin, direction, IODIRA, IODIRB);
}

uint8_t get_pin_direction(uint8_t address, uint8_t pin)
{
	/**
	* Get IO direction for an individual pin
	* @param address - I2C address for the target device
	* @param pins - 1 to 16
	* @returns 1 = input, 0 = output
	*/
	return get_pin(address, pin, IODIRA, IODIRB);
}

void set_port_direction(uint8_t address, uint8_t port, uint8_t direction)
{
	/**
	* Set direction for an IO port
	* @param address - I2C address for the target device
	* @param port - 0 = pins 1 to 8, port 1 = pins 9 to 16
	* @param direction - 0 to 255 (0xFF).  For each bit 1 = input, 0 = output
	*/
	set_port(address, port, direction, IODIRA, IODIRB);
}

uint8_t get_port_direction(uint8_t address, uint8_t port)
{
	/**
	* get direction for an IO port
	* @param address - I2C address for the target device
	* @param port - 0 = pins 1 to 8, port 1 = pins 9 to 16
	* @returns 0 to 255 (0xFF).  For each bit 1 = input, 0 = output
	*/
	return get_port(address, port, IODIRA, IODIRB);
}

void set_bus_direction(uint8_t address, uint16_t direction)
{
	/**
	* Set direction for the IO bus
	* @param address - I2C address for the target device
	* @param direction - 0 to 65535 (0xFFFF).  For each bit 1 = input, 0 = output
	*/
	write_word_data(address, IODIRA, direction);
}

uint16_t get_bus_direction(uint8_t address)
{
	/**
	* Get direction for the IO bus
	* @param address - I2C address for the target device
	* @returns 0 to 65535 (0xFFFF).  For each bit 1 = input, 0 = output
	*/
	return read_word_data(address, IODIRA);
}

void set_pin_pullup(uint8_t address, uint8_t pin, uint8_t value)
{
	/**
	* Set the internal 100K pull-up resistors for an individual pin
	* @param address - I2C address for the target device
	* @param pin - 1 to 16
	* @param value - 1 = enabled, 0 = disabled
	*/
	set_pin(address, pin, value, GPPUA, GPPUB);
}

uint8_t get_pin_pullup(uint8_t address, uint8_t pin)
{
	/**
	* Get the internal 100K pull-up resistors for an individual pin
	* @param address - I2C address for the target device
	* @param pin - 1 to 16
	* @returns 1 = enabled, 0 = disabled
	*/
	return get_pin(address, pin, GPPUA, GPPUB);
}

void set_port_pullups(uint8_t address, uint8_t port, uint8_t value)
{
	/**
	* Set the internal 100K pull-up resistors for the selected IO port
	* @param address - I2C address for the target device
	* @param port - 0 = pins 1 to 8, port 1 = pins 9 to 16
	* @param value - 0 to 255 (0xFF). For each bit 1 = enabled, 0 = disabled
	*/
	set_port(address, port, value, GPPUA, GPPUB);
}

uint8_t get_port_pullups(uint8_t address, uint8_t port)
{
	/**
	* Get the internal 100K pull-up resistors for the selected IO port
	* @param address - I2C address for the target device
	* @param port - 0 = pins 1 to 8, port 1 = pins 9 to 16
	* @returns 0 to 255 (0xFF). For each bit 1 = enabled, 0 = disabled
	*/
	return get_port(address, port, GPPUA, GPPUB);
}

void set_bus_pullups(uint8_t address, uint16_t value)
{
	/**
	* Set internal 100K pull-up resistors for the IO bus
	* @param address - I2C address for the target device
	* @param value - 0 to 65535 (0xFFFF).  For each bit 1 = enabled, 0 = disabled
	*/
	write_word_data(address, GPPUA, value);
}

uint16_t get_bus_pullups(uint8_t address)
{
	/**
	* Get internal 100K pull-up resistors for the IO bus
	* @param address - I2C address for the target device
	* @returns 0 to 65535 (0xFFFF).  For each bit 1 = enabled, 0 = disabled
	*/
	return read_word_data(address, GPPUA);
}

void write_pin(uint8_t address, uint8_t pin, uint8_t value)
{
	/**
	* Write to an individual pin 1 - 16
	* @param address - I2C address for the target device
	* @param pin - 1 to 16
	* @param value - 0 = logic low, 1 = logic high
	*/
	set_pin(address, pin, value, GPIOA, GPIOB);
}

void write_port(uint8_t address, uint8_t port, uint8_t value)
{
	/**
	* Write to all pins on the selected port
	* @param address - I2C address for the target device
	* @param port - 0 = pins 1 to 8, port 1 = pins 9 to 16
	* @param value - 0 to 255 (0xFF)
	*/
	set_port(address, port, value, GPIOA, GPIOB);
}

void write_bus(uint8_t address, uint16_t value)
{
	/**
	* Write to all pins on the selected bus
	* @param address - I2C address for the target device
	* @param value - 0 to 65535 (0xFFFF). For each bit 1 = logic high, 0 = logic low
	*/
	write_word_data(address, GPIOA, value);
}

uint8_t read_pin(uint8_t address, uint8_t pin)
{
	/**
	* Read the value of an individual pin
	* @param address - I2C address for the target device
	* @param pin - 1 to 16
	* @returns - 0 = logic low, 1 = logic high
	*/

	return get_pin(address, pin, GPIOA, GPIOB);
}

uint8_t read_port(uint8_t address, uint8_t port)
{
	/**
	* Read all pins on the selected port
	* @param address - I2C address for the target device
	* @param port - 0 = pins 1 to 8, port 1 = pins 9 to 16
	* @returns - 0 to 255 (0xFF). For each bit 1 = logic high, 0 = logic low
	*/
	return get_port(address, port, GPIOA, GPIOB);
}

uint16_t read_bus(uint8_t address)
{
	/**
	* Read all pins on the selected bus
	* @param address - I2C address for the target device
	* @returns - 0 to 65535 (0xFFFF). For each bit 1 = logic high, 0 = logic low
	*/
	return read_word_data(address, GPIOA);
}

void invert_pin(uint8_t address, uint8_t pin, uint8_t polarity)
{
	/**
	* Invert the polarity of the selected pin
	* @param address - I2C address for the target device
	* @param pin - 1 to 16
	* @param polarity - 0 = non-inverted, 1 = inverted
	*/
	set_pin(address, pin, polarity, IPOLA, IPOLB);
}

uint8_t get_pin_polarity(uint8_t address, uint8_t pin)
{
	/**
  	* Get the polarity of the selected pin
	* @param address - I2C address for the target device
  	* @param pin - 1 to 16
	* @returns - pin polarity 0 = non-inverted, 1 = inverted
  	*/
  	return get_pin(address, pin, IPOLA, IPOLB);
}

void invert_port(uint8_t address, uint8_t port, uint8_t polarity)
{
	/**
	* Invert the polarity of the pins on a selected port
	* @param address - I2C address for the target device
	* @param port - 0 = pins 1 to 8, port 1 = pins 9 to 16
	* @param polarity - 0 to 255 (0xFF). For each bit 0 = non-inverted, 1 = inverted
	*/
	set_port(address, port, polarity, IPOLA, IPOLB);
}

uint8_t get_port_polarity(uint8_t address, uint8_t port)
{
	/**
  	* Get the polarity of the selected pin
	* @param address - I2C address for the target device
  	* @param port - 0 = pins 1 to 8, port 1 = pins 9 to 16
	* @returns - 0 to 255 (0xFF). For each bit 0 = non-inverted, 1 = inverted
  	*/
  	return get_port(address, port, IPOLA, IPOLB);
}

void invert_bus(uint8_t address, uint16_t polarity)
{
	/**
	* Invert the polarity of the pins on a selected bus
	* @param address - I2C address for the target device
	* @param polarity - 0 to 65535 (0xFFFF). For each bit 0 = non-inverted, 1 = inverted
	*/
	write_word_data(address, IPOLA, polarity);
}

 uint16_t get_bus_polarity(uint8_t address)
 {
	 /**
  	* Get the polarity of the bus
	* @param address - I2C address for the target device
	* @returns - 0 to 65535 (0xFFFF). For each bit 0 = non-inverted, 1 = inverted
  	*/
  	return read_word_data(address, IPOLA);
 }

void mirror_interrupts(uint8_t address, uint8_t value)
{
	/**
	* Set the interrupt pins to be mirrored or for separate ports
	* @param address - I2C address for the target device
	* @param value - 1 = The interrupt pins are internally connected, 0 = The interrupt pins are not connected. INTA is associated with PortA and INTB is associated with PortB
	*/
	if (value == 0)
	{
		config = updatebyte(config, 6, 0);
		write_byte_data(address, IOCON, config);
	}
	else if (value == 1)
	{
		config = updatebyte(config, 6, 1);
		write_byte_data(address, IOCON, config);
	}
	else
	{
		printf("mirror_interrupts value out of range: 0 or 1");
	}
}

void set_interrupt_polarity(uint8_t address, uint8_t value)
{
	/**
	* This sets the polarity of the interrupt output pins.
	* @param address - I2C address for the target device
	* @param value - 1 = Active-high, 0 = Active-low.
	*/
	if (value == 0)
	{
		config = updatebyte(config, 1, 0);
		write_byte_data(address, IOCON, config);
	}
	else if (value == 1)
	{
		config = updatebyte(config, 1, 1);
		write_byte_data(address, IOCON, config);
	}
	else
	{
		printf("set_interrupt_polarity value out of range: 0 or 1");
	}
}

uint8_t get_interrupt_polarity(uint8_t address)
{
	/**
  	* Get the polarity of the interrupt output pins.
	* @param address - I2C address for the target device
	* @returns 1 = Active-high, 0 = Active-low.
  	*/
  	return checkbit(read_byte_data(address, IOCON), 1);
}

void set_interrupt_type(uint8_t address, uint8_t port, uint8_t value)
{
	/**
	* Sets the type of interrupt for each pin on the selected port
	* @param address - I2C address for the target device
	* @param port - 0 = pins 1 to 8, port 1 = pins 9 to 16
	* @param value - 0 to 255 (0xFF). For each bit 1 = interrupt is fired when the pin matches the default value, 0 = the interrupt is fired on state change
	*/
	set_port(address, port, value, INTCONA, INTCONB);
}

uint8_t get_interrupt_type(uint8_t address, uint8_t port)
{
	/**
	* Get the type of interrupt for each pin on the selected port
	* @param address - I2C address for the target device
	* @param port - 0 = pins 1 to 8, port 1 = pins 9 to 16
	* @returns 0 to 255 (0xFF). For each bit 1 = interrupt is fired when the pin matches the default value, 0 = the interrupt is fired on state change
	*/
	return get_port(address, port, INTCONA, INTCONB);
}

void set_interrupt_defaults(uint8_t address, uint8_t port, uint8_t value)
{
	/**
	* These bits set the compare value for pins configured for interrupt-on-change on the selected port.
	* If the associated pin level is the opposite from the register bit, an interrupt occurs.
	* @param address - I2C address for the target device
	* @param port - 0 = pins 1 to 8, port 1 = pins 9 to 16
	* @param value - default state for the port. 0 to 255 (0xFF).
	*/
	set_port(address, port, value, DEFVALA, DEFVALB);
}

uint8_t get_interrupt_defaults(uint8_t address, uint8_t port)
{
	/**
  	* Get the compare value for pins configured for interrupt-on-change on the selected port.
  	* If the associated pin level is the opposite from the register bit, an interrupt occurs.
  	* @param address - I2C address for the target device
	* @param port - 0 = pins 1 to 8, port 1 = pins 9 to 16
	* @returns default state for the port. 0 to 255 (0xFF).
  	*/
  	return get_port(address, port, DEFVALA, DEFVALB);
}

void set_interrupt_on_pin(uint8_t address, uint8_t pin, uint8_t value)
{
	/**
	* Enable interrupts for the selected pin
	* @param address - I2C address for the target device
	* @param pin - 1 to 16
	* @param value - 0 = interrupt disabled, 1 = interrupt enabled
	*/
	set_pin(address, pin, value, GPINTENA, GPINTENB);
}

 uint8_t get_interrupt_on_pin(uint8_t address, uint8_t pin)
 {
	 /**
  	* Get the interrupt enable status for the selected pin
	* @param address - I2C address for the target device
  	* @param pin - 1 to 16
	* @returns 0 = interrupt disabled, 1 = interrupt enabled
  	*/
  	return get_pin(address, pin, GPINTENA, GPINTENB);
 }

void set_interrupt_on_port(uint8_t address, uint8_t port, uint8_t value)
{
	/**
	* Enable interrupts for the pins on the selected port
	* @param address - I2C address for the target device
	* @param port - 0 = pins 1 to 8, port 1 = pins 9 to 16
	* @param value - 0 to 255 (0xFF). For each bit 0 = interrupt disabled, 1 = interrupt enabled
	*/
	set_port(address, port, value, GPINTENA, GPINTENB);
}

uint8_t get_interrupt_on_port(uint8_t address, uint8_t port)
{
	/**
  	* Get the interrupt enable status for the selected port
	* @param address - I2C address for the target device
  	* @param port - 0 = pins 1 to 8, port 1 = pins 9 to 16
	* @returns 0 to 255 (0xFF). For each bit 0 = interrupt disabled, 1 = interrupt enabled
  	*/
  	return get_port(address, port, GPINTENA, GPINTENB);
}

void set_interrupt_on_bus(uint8_t address, uint16_t value)
{
	/**
	* Enable interrupts for the pins on the selected bus
	* @param address - I2C address for the target device
	* @param value - 0 to 65535 (0xFFFF). For each bit 0 = interrupt disabled, 1 = interrupt enabled
	*/
	write_word_data(address, GPINTENA, value);
}

 uint16_t get_interrupt_on_bus(uint8_t address)
 {
	 /**
  	* Get the interrupt enable status for the selected bus
	* @param address - I2C address for the target device
	* @returns 0 to 65535 (0xFFFF). For each bit 0 = interrupt disabled, 1 = interrupt enabled
  	*/
  	return read_word_data(address, GPINTENA);
 }

uint8_t read_interrupt_status(uint8_t address, uint8_t port)
{
	/**
	* Read the interrupt status for the pins on the selected port
	* @param address - I2C address for the target device
	* @param port - 0 = pins 1 to 8, port 1 = pins 9 to 16
	* @returns - 0 to 255 (0xFF). For each bit 1 = interrupt triggered, 0 = interrupt not triggered
	*/
	return get_port(address, port, INTFA, INTFB);
}

uint8_t read_interrupt_capture(uint8_t address, uint8_t port)
{
	/**
	* Read the value from the selected port at the time of the last interrupt trigger
	* @param address - I2C address for the target device
	* @param port - 0 = pins 1 to 8, port 1 = pins 9 to 16
	* @returns - 0 to 255 (0xFF). For each bit 1 = interrupt triggered, 0 = interrupt not triggered
	*/
	return get_port(address, port, INTCAPA, INTCAPB);
}

void reset_interrupts(uint8_t address)
{
	/**
	* Reset the interrupts A and B to 0
	* @param address - I2C address for the target device
	*/
	read_interrupt_capture(address, 0);
	read_interrupt_capture(address, 1);
}

void IOPi_init(uint8_t address, uint8_t reset) {
	/**
	* Initialise the MCP32017 IO chip
	* @param address - I2C address for the target device
	* @param reset - If set to 1 the IO Pi will reset to default values. Ports are inputs, pull-up resistors are disabled and ports are not inverted.
	*/
	write_byte_data(address, IOCON, config);
	if (reset == 1){
		write_word_data(address, IODIRA, 0xFFFF);
		write_word_data(address, GPPUA, 0x0000);
		write_word_data(address, IPOLA, 0x0000);
	}
}
