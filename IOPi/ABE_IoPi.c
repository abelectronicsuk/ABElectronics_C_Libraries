/*
 ================================================
 ABElectronics UK IO Pi 32-Channel Port Expander
 Version 1.0 Created 23/01/2015
 ================================================


 Each MCP23017 chip is split into two 8-bit ports.  port 0 controls
 pins 1 to 8 while port 1 controls pins 9 to 16.
 When writing to or reading from a port the least significant bit represents
 the lowest numbered pin on the selected port.


 Required package{
 apt-get install libi2c-dev
 */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <linux/i2c-dev.h>
#include "ABE_helpers.h"

static char IODIRA = 0x00; // IO direction A - 1= input 0 = output
static char IODIRB = 0x01; // IO direction B - 1= input 0 = output
	// Input polarity A - If a bit is set, the corresponding GPIO register bit
	// will reflect the inverted value on the pin.
static char IPOLA = 0x02;
	// Input polarity B - If a bit is set, the corresponding GPIO register bit
	// will reflect the inverted value on the pin.
static char IPOLB = 0x03;
	// The GPINTEN register controls the interrupt-onchange feature for each
	// pin on port A.
static char GPINTENA = 0x04;
	// The GPINTEN register controls the interrupt-onchange feature for each
	// pin on port B.
static char GPINTENB = 0x05;
	// Default value for port A - These bits set the compare value for pins
	// configured for interrupt-on-change. If the associated pin level is the
	// opposite from the register bit, an interrupt occurs.
static char DEFVALA = 0x06;
	// Default value for port B - These bits set the compare value for pins
	// configured for interrupt-on-change. If the associated pin level is the
	// opposite from the register bit, an interrupt occurs.
static char DEFVALB = 0x07;
	// Interrupt control register for port A.  If 1 interrupt is fired when the
	// pin matches the default value, if 0 the interrupt is fired on state
	// change
static char INTCONA = 0x08;
	// Interrupt control register for port B.  If 1 interrupt is fired when the
	// pin matches the default value, if 0 the interrupt is fired on state
	// change
static char INTCONB = 0x09;
static char IOCON = 0x0A; // see datasheet for configuration register
static char GPPUA = 0x0C; // pull-up resistors for port A
static char GPPUB = 0x0D; // pull-up resistors for port B
	// The INTF register reflects the interrupt condition on the port A pins of
	// any pin that is enabled for interrupts. A set bit indicates that the
	// associated pin caused the interrupt.
static char INTFA = 0x0E;
	// The INTF register reflects the interrupt condition on the port B pins of
	// any pin that is enabled for interrupts. A set bit indicates that the
	// associated pin caused the interrupt.
static char INTFB = 0x0F;
	// The INTCAP register captures the GPIO port A value at the time the
	// interrupt occurred.
static char INTCAPA = 0x10;
	// The INTCAP register captures the GPIO port B value at the time the
	// interrupt occurred.
static char INTCAPB = 0x11;
static char GPIOA = 0x12; // data port A
static char GPIOB = 0x13; // data port B
static char OLATA = 0x14; // output latches A
static char OLATB = 0x15; // output latches B

	// variables
static char portaval = 0x00;
static char portbval = 0x00;
static char port_a_dir = 0x00; // port a direction
static char port_b_dir = 0x00; // port b direction
static char porta_pullup = 0x00; // port a pull-up resistors
static char portb_pullup = 0x00; // port a pull-up resistors
static char porta_polarity = 0x00; // input polarity for port a
static char portb_polarity = 0x00; // input polarity for port b
static char intA = 0x00; // interrupt control for port a
static char intB = 0x00; // interrupt control for port a
	// initial configuration - see IOCON page in the MCP23017 datasheet for
	// more information.
static char config = 0x22;




// local methods

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
	/*
	 set IO direction for an individual pin
	 pins 1 to 16
	 direction 1 = input, 0 = output
	 */
	pin = pin - 1;
	if (pin < 8) {
		port_a_dir = updatebyte(port_a_dir, pin, direction);
		write_byte_data(address, IODIRA, port_a_dir);
	} else {
		port_b_dir = updatebyte(port_b_dir, pin - 8, direction);
		write_byte_data(address, IODIRB, port_b_dir);
	}
}

void set_port_direction(char address, char port, char direction) {
	/*
	 set direction for an IO port
	 port 0 = pins 1 to 8, port 1 = pins 8 to 16
	 1 = input, 0 = output
	 */
	if (port == 1) {
		write_byte_data(address, IODIRB, direction);
		port_b_dir = direction;
	} else {
		write_byte_data(address, IODIRA, direction);
		port_a_dir = direction;
	}

}

void set_pin_pullup(char address, char pinval, char value) {
	/*
	 set the internal 100K pull-up resistors for an individual pin
	 pins 1 to 16
	 value 1 = enabled, 0 = disabled
	 */
	pinval = pinval - 1;
	if (pinval < 8) {
		porta_pullup = updatebyte(porta_pullup, pinval, value);
		write_byte_data(address, GPPUA, porta_pullup);
	} else {
		portb_pullup = updatebyte(portb_pullup, pinval - 8, value);
		write_byte_data(address, GPPUB, portb_pullup);
	}
}

void set_port_pullups(char address, char port, char value) {
	/*
	 set the internal 100K pull-up resistors for the selected IO port
	 */
	if (port == 1) {
		portb_pullup = value;
		write_byte_data(address, GPPUB, value);
	} else {
		porta_pullup = value;
		write_byte_data(address, GPPUA, value);
	}
}

void write_pin(char address, char pin, char value) {
	/*
	 write to an individual pin 1 - 16
	 */
	pin = pin - 1;

	if (pin < 8) {
		portaval = updatebyte(portaval, pin, value);
		write_byte_data(address, GPIOA, portaval);
	} else {
		portbval = updatebyte(portbval, pin - 8, value);
		write_byte_data(address, GPIOB, portbval);
	}
}

void write_port(char address, char port, char value) {
	/*
	 write to all pins on the selected port
	 port 0 = pins 1 to 8, port 1 = pins 8 to 16
	 value = number between 0 and 255 or 0x00 and 0xFF
	 */
	if (port == 1) {
		write_byte_data(address, GPIOB, value);
		portbval = value;
	} else {
		write_byte_data(address, GPIOA, value);
		portaval = value;
	}
}

int read_pin(char address, char pinval) {
	/*
	 read the value of an individual pin 1 - 16
	 returns 0 = logic level low, 1 = logic level high
	 */
	pinval = pinval - 1;
	if (pinval < 8) {
		portaval = read_byte_data(address, GPIOA);
		return (checkbit(portaval, pinval));
	} else {
		pinval = pinval - 8;
		portbval = read_byte_data(address, GPIOB);
		return (checkbit(portbval, pinval));
	}
}

char read_port(char address, char port) {
	/*
	 read all pins on the selected port
	 port 0 = pins 1 to 8, port 1 = pins 8 to 16
	 returns number between 0 and 255 or 0x00 and 0xFF
	 */
	if (port == 1) {
		portbval = read_byte_data(address, GPIOB);
		return (portbval);
	} else {
		portaval = read_byte_data(address, GPIOA);
		return (portaval);
	}
}

void invert_port(char address, char port, char polarity) {
	/*
	 invert the polarity of the pins on a selected port
	 port 0 = pins 1 to 8, port 1 = pins 8 to 16
	 polarity 0 = same logic state of the input pin, 1 = inverted logic
	 state of the input pin
	 */
	if (port == 1) {
		write_byte_data(address, IPOLB, polarity);
		portb_polarity = polarity;
	} else {
		write_byte_data(address, IPOLA, polarity);
		porta_polarity = polarity;
	}
}

void invert_pin(char address, char pin, char polarity) {
	/*
	 invert the polarity of the selected pin
	 pins 1 to 16
	 polarity 0 = same logic state of the input pin, 1 = inverted logic
	 state of the input pin
	 */
	pin = pin - 1;
	if (pin < 8) {
		porta_polarity = updatebyte(portaval, pin, polarity);
		write_byte_data(address, IPOLA, porta_polarity);
	} else {
		portb_polarity = updatebyte(portbval, pin - 8, polarity);
		write_byte_data(address, IPOLB, portb_polarity);
	}
}

void mirror_interrupts(char address, char value) {
	/*
	 1 = The char pins are internally connected, 0 = The char pins are not
	 connected. INTA is associated with PortA and INTB is associated with
	 PortB
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
	/*
	 This sets the polarity of the char output pins - 1 = Active-high.
	 0 = Active-low.
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
	/*
	 Sets the type of interrupt for each pin on the selected port
	 1 = interrupt is fired when the pin matches the default value
	 0 = the interrupt is fired on state change
	 */
	if (port == 0) {
		write_byte_data(address, INTCONA, value);
	} else {
		write_byte_data(address, INTCONB, value);
	}
}

void set_interrupt_defaults(char address, char port, char value) {
	/*
	 These bits set the compare value for pins configured for
	 interrupt-on-change on the selected port.
	 If the associated pin level is the opposite from the register bit, an
	 interrupt occurs.
	 */
	if (port == 0) {
		write_byte_data(address, DEFVALA, value);
	} else {
		write_byte_data(address, DEFVALB, value);
	}
}

void set_interrupt_on_port(char address, char port, char value) {
	/*
	 Enable interrupts for the pins on the selected port
	 port 0 = pins 1 to 8, port 1 = pins 8 to 16
	 value = number between 0 and 255 or 0x00 and 0xFF
	 */
	if (port == 0) {
		write_byte_data(address, GPINTENA, value);
		intA = value;
	} else {
		write_byte_data(address, GPINTENB, value);
		intB = value;
	}
}

void set_interrupt_on_pin(char address, char pin, char value) {
	/*
	 Enable interrupts for the selected pin
	 Pin = 1 to 16
	 Value 0 = interrupt disabled, 1 = interrupt enabled
	 */
	pin = pin - 1;
	if (pin < 8) {
		intA = updatebyte(intA, pin, value);
		write_byte_data(address, GPINTENA, intA);
	} else {
		intB = updatebyte(intB, pin - 8, value);
		write_byte_data(address, GPINTENB, intB);
	}
}

char read_interrupt_status(char address, char port) {
	/*
	 read the interrupt status for the pins on the selected port
	 port 0 = pins 1 to 8, port 1 = pins 8 to 16
	 */
	if (port == 0) {
		return (read_byte_data(address, INTFA));
	} else {
		return (read_byte_data(address, INTFB));
	}
}

char read_interrupt_capture(char address, char port) {
	/*
	 read the value from the selected port at the time of the last
	 interrupt trigger
	 port 0 = pins 1 to 8, port 1 = pins 8 to 16
	 */
	if (port == 0) {
		return (read_byte_data(address, INTCAPA));
	} else {
		return (read_byte_data(address, INTCAPB));
	}
}

void reset_interrupts(char address) {
	/*
	 set the interrupts A and B to 0
	 */
	read_interrupt_capture(address, 0);
	read_interrupt_capture(address, 1);
}

void IOPi_init(char address) {
	write_byte_data(address, IOCON, config);
	portaval = read_byte_data(address, GPIOA);
	portbval = read_byte_data(address, GPIOB);
	write_byte_data(address, IODIRA, 0xFF);
	write_byte_data(address, IODIRB, 0xFF);
	set_port_pullups(address, 0, 0x00);
	set_port_pullups(address, 1, 0x00);
	invert_port(address, 0, 0x00);
	invert_port(address, 1, 0x00);
}
