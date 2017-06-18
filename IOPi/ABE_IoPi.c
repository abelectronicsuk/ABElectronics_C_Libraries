/*
 ================================================
 ABElectronics UK IO Pi 32-Channel Port Expander
 Version 1.1 Created 23/01/2015 - Updated 27/05/2015
 ================================================


 Each MCP23017 chip is split into two 8-bit ports.  port 0 controls
 pins 1 to 8 while port 1 controls pins 9 to 16.
 When writing to or reading from a port the least significant bit represents
 the lowest numbered pin on the selected port.


 Required package{
 apt-get install libi2c-dev
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
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
static char config = 0x22;




// local methods

static int i2cbus;
const char *fileName = "/dev/i2c-1"; // change to /dev/i2c-0 if you are using a revision 0002 or 0003 model B
unsigned char buf[10] = { 0 };

int read_byte_data(char address, char reg) {

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

void write_byte_data(char address, char reg, char value) {
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

static char updatebyte(char byte, char bit, char value) {
	/*
	 internal method for setting the value of a single bit within a byte
	 */
	if (value == 0) {
		return (byte &= ~(1 << bit));

	} else {
		return (byte |= 1 << bit);
	}

}

static char checkbit(char byte, char bit) {
	/*
	 internal method for reading the value of a single bit within a byte
	 */
	if (byte & (1 << bit)) {
		return (1);
	} else {
		return (0);
	}
}

// public methods

void set_pin_direction(char address, char pin, char direction) {
	/**
	* set IO direction for an individual pin
	* @param address - I2C address for the target device
	* @param pins - 1 to 16
	* @param direction - 1 = input, 0 = output
	*/
	pin = pin - 1;
	char cval = 0;
	if (pin < 8) {
		cval = read_byte_data(address, IODIRA);
		cval = updatebyte(cval, pin, direction);
		write_byte_data(address, IODIRA, cval);
	} else {
		cval = read_byte_data(address, IODIRB);
		cval = updatebyte(cval, pin - 8, direction);
		write_byte_data(address, IODIRB, cval);
	}
}

void set_port_direction(char address, char port, char direction) {
	/**
	* set direction for an IO port
	* @param address - I2C address for the target device
	* @param port - 0 = pins 1 to 8, port 1 = pins 9 to 16
	* @param direction - 1 = input, 0 = output
	*/
	if (port == 1) {
		write_byte_data(address, IODIRB, direction);
	} else {
		write_byte_data(address, IODIRA, direction);
	}

}

void set_pin_pullup(char address, char pinval, char value) {
	/**
	* set the internal 100K pull-up resistors for an individual pin
	* @param address - I2C address for the target device
	* @param pin - 1 to 16
	* @param value - 1 = enabled, 0 = disabled
	*/
	pinval = pinval - 1;
	char cval = 0;
	if (pinval < 8) {
		cval = read_byte_data(address, GPPUA);
		cval = updatebyte(cval, pinval, value);
		write_byte_data(address, GPPUA, cval);
	} else {
		cval = read_byte_data(address, GPPUB);
		cval = updatebyte(cval, pinval - 8, value);
		write_byte_data(address, GPPUB, cval);
	}
}

void set_port_pullups(char address, char port, char value) {
	/**
	* set the internal 100K pull-up resistors for the selected IO port
	* @param address - I2C address for the target device
	* @param port - 0 = pins 1 to 8, port 1 = pins 9 to 16
	* @param value - number between 0 and 255 or 0x00 and 0xFF
	*/
	if (port == 1) {
		write_byte_data(address, GPPUB, value);
	} else {
		write_byte_data(address, GPPUA, value);
	}
}

void write_pin(char address, char pin, char value) {
	/**
	* write to an individual pin 1 - 16
	* @param address - I2C address for the target device
	* @param pin - 1 to 16
	* @param value - 0 = logic level low, 1 = logic level high
	*/
	pin = pin - 1;
	char cval = 0;
	if (pin < 8) {
		cval = read_byte_data(address, GPIOA);
		cval = updatebyte(cval, pin, value);
		write_byte_data(address, GPIOA, cval);
	} else {
		cval = read_byte_data(address, GPIOB);
		cval = updatebyte(cval, pin - 8, value);
		write_byte_data(address, GPIOB, cval);
	}
}

void write_port(char address, char port, char value) {
	/**
	* write to all pins on the selected port
	* @param address - I2C address for the target device
	* @param port - 0 = pins 1 to 8, port 1 = pins 9 to 16
	* @param value - number between 0 and 255 or 0x00 and 0xFF
	*/
	if (port == 1) {
		write_byte_data(address, GPIOB, value);
	} else {
		write_byte_data(address, GPIOA, value);
	}
}

int read_pin(char address, char pinval) {
	/**
	* read the value of an individual pin
	* @param address - I2C address for the target device
	* @param pin - 1 to 16
	* @returns - 0 = logic level low, 1 = logic level high
	*/
	pinval = pinval - 1;
	if (pinval < 8) {
		return (checkbit(read_byte_data(address, GPIOA), pinval));
	} else {
		pinval = pinval - 8;
		return (checkbit(read_byte_data(address, GPIOB), pinval));
	}
}

char read_port(char address, char port) {
	/**
	* read all pins on the selected port
	* @param address - I2C address for the target device
	* @param port - 0 = pins 1 to 8, port 1 = pins 9 to 16
	* @returns - number between 0 and 255 or 0x00 and 0xFF
	*/
	if (port == 1) {
		return (read_byte_data(address, GPIOB));
	} else {
		return (read_byte_data(address, GPIOA));
	}
}

void invert_port(char address, char port, char polarity) {
	/**
	* invert the polarity of the pins on a selected port
	* @param address - I2C address for the target device
	* @param port - 0 = pins 1 to 8, port 1 = pins 9 to 16
	* @param polarity - 0 = same logic state of the input pin, 1 = inverted logic	state of the input pin
	*/
	if (port == 1) {
		write_byte_data(address, IPOLB, polarity);
	} else {
		write_byte_data(address, IPOLA, polarity);
	}
}

void invert_pin(char address, char pin, char polarity) {
	/**
	* invert the polarity of the selected pin
	* @param address - I2C address for the target device
	* @param pin - 1 to 16
	* @param polarity - 0 = same logic state of the input pin, 1 = inverted logic	state of the input pin
	*/
	pin = pin - 1;
	char cval = 0;
	if (pin < 8) {
		cval = read_byte_data(address, IPOLA);
		cval = updatebyte(cval, pin, polarity);
		write_byte_data(address, IPOLA, cval);
	} else {
		cval = read_byte_data(address, IPOLB);
		cval = updatebyte(cval, pin - 8, polarity);
		write_byte_data(address, IPOLB, cval);
	}
}

void mirror_interrupts(char address, char value) {
	/**
	* Set the interrupt pins to be mirrored or for separate ports
	* @param address - I2C address for the target device
	* @param value - 1 = The char pins are internally connected, 0 = The char pins are not connected. INTA is associated with PortA and INTB is associated with PortB
	*/
	if (value == 0) {
		config = updatebyte(config, 6, 0);
		write_byte_data(address, IOCON, config);
	} else {
		config = updatebyte(config, 6, 1);
		write_byte_data(address, IOCON, config);
	}
}

void set_interrupt_polarity(char address, char value) {
	/**
	* This sets the polarity of the char output pins.
	* @param address - I2C address for the target device
	* @param value - 1 = Active-high, 0 = Active-low.
	*/
	if (value == 0) {
		config = updatebyte(config, 1, 0);
		write_byte_data(address, IOCON, config);
	} else {
		config = updatebyte(config, 1, 1);
		write_byte_data(address, IOCON, config);
	}
}

void set_interrupt_type(char address, char port, char value) {
	/**
	* Sets the type of interrupt for each pin on the selected port
	* @param address - I2C address for the target device
	* @param port - 0 = pins 1 to 8, port 1 = pins 9 to 16
	* @param value - 1 = interrupt is fired when the pin matches the default value, 0 = the interrupt is fired on state change
	*/
	if (port == 0) {
		write_byte_data(address, INTCONA, value);
	} else {
		write_byte_data(address, INTCONB, value);
	}
}

void set_interrupt_defaults(char address, char port, char value) {
	/**
	* These bits set the compare value for pins configured for interrupt-on-change on the selected port.
	* If the associated pin level is the opposite from the register bit, an interrupt occurs.
	* @param address - I2C address for the target device
	* @param port - 0 = pins 1 to 8, port 1 = pins 9 to 16
	* @param value - default state for the port
	*/
	if (port == 0) {
		write_byte_data(address, DEFVALA, value);
	} else {
		write_byte_data(address, DEFVALB, value);
	}
}

void set_interrupt_on_port(char address, char port, char value) {
	/**
	* Enable interrupts for the pins on the selected port
	* @param address - I2C address for the target device
	* @param port - 0 = pins 1 to 8, port 1 = pins 9 to 16
	* @param value - number between 0 and 255 or 0x00 and 0xFF
	*/
	if (port == 0) {
		write_byte_data(address, GPINTENA, value);
	} else {
		write_byte_data(address, GPINTENB, value);
	}
}

void set_interrupt_on_pin(char address, char pin, char value) {
	/**
	* Enable interrupts for the selected pin
	* @param address - I2C address for the target device
	* @param pin - 1 to 16
	* @param value - 0 = interrupt disabled, 1 = interrupt enabled
	*/
	pin = pin - 1;
	char cval = 0;
	if (pin < 8) {
		cval = read_byte_data(address, GPINTENA);
		cval = updatebyte(cval, pin, value);
		write_byte_data(address, GPINTENA, cval);
	} else {
		cval = read_byte_data(address, GPINTENB);
		cval = updatebyte(cval, pin - 8, value);
		write_byte_data(address, GPINTENB, cval);
	}
}

char read_interrupt_status(char address, char port) {
	/**
	* read the interrupt status for the pins on the selected port
	* @param address - I2C address for the target device
	* @param port - 0 = pins 1 to 8, port 1 = pins 9 to 16
	*/
	if (port == 0) {
		return (read_byte_data(address, INTFA));
	} else {
		return (read_byte_data(address, INTFB));
	}
}

char read_interrupt_capture(char address, char port) {
	/**
	* read the value from the selected port at the time of the last
	* interrupt trigger
	* @param address - I2C address for the target device
	* @param port - 0 = pins 1 to 8, port 1 = pins 9 to 16
	*/
	if (port == 0) {
		return (read_byte_data(address, INTCAPA));
	} else {
		return (read_byte_data(address, INTCAPB));
	}
}

void reset_interrupts(char address) {
	/**
	* set the interrupts A and B to 0
	* @param address - I2C address for the target device
	*/
	read_interrupt_capture(address, 0);
	read_interrupt_capture(address, 1);
}

void IOPi_init(char address) {
	/**
	* initialise the MCP32017 IO chip with default values: ports are inputs, pull-up resistors are disabled and ports are not inverted
	* @param address - I2C address for the target device
	*/
	write_byte_data(address, IOCON, config);
	write_byte_data(address, IODIRA, 0xFF);
	write_byte_data(address, IODIRB, 0xFF);
	set_port_pullups(address, 0, 0x00);
	set_port_pullups(address, 1, 0x00);
	invert_port(address, 0, 0x00);
	invert_port(address, 1, 0x00);
}
