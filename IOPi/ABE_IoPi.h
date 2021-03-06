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

// public functions

/**
* initialise the MCP32017 IO chip
* @param address - I2C address for the target device
* @param reset - If set to 1 the IO Pi will reset to default values. Ports are inputs, pull-up resistors are disabled and ports are not inverted.
*/
void IOPi_init(uint8_t address, uint8_t reset);

/**
* set IO direction for an individual pin
* @param address - I2C address for the target device
* @param pins - 1 to 16
* @param direction - 1 = input, 0 = output
*/
void set_pin_direction(uint8_t address, uint8_t pin, uint8_t direction);

/**
* Get IO direction for an individual pin
* @param address - I2C address for the target device
* @param pins - 1 to 16
* @returns 1 = input, 0 = output
*/
uint8_t get_pin_direction(uint8_t address, uint8_t pin);

/**
* set direction for an IO port
* @param address - I2C address for the target device
* @param port - 0 = pins 1 to 8, port 1 = pins 9 to 16
* @param direction - 0 to 255 (0xFF).  For each bit 1 = input, 0 = output
*/
void set_port_direction(uint8_t address, uint8_t port, uint8_t direction);

/**
* get direction for an IO port
* @param address - I2C address for the target device
* @param port - 0 = pins 1 to 8, port 1 = pins 9 to 16
* @returns 0 to 255 (0xFF).  For each bit 1 = input, 0 = output
*/
uint8_t get_port_direction(uint8_t address, uint8_t port);

/**
* set direction for the IO bus
* @param address - I2C address for the target device
* @param direction - 0 to 65535 (0xFFFF).  For each bit 1 = input, 0 = output
*/
void set_bus_direction(uint8_t address, uint16_t direction);

/**
* get direction for the IO bus
* @param address - I2C address for the target device
* @returns 0 to 65535 (0xFFFF).  For each bit 1 = input, 0 = output
*/
uint16_t get_bus_direction(uint8_t address);

/**
* set the internal 100K pull-up resistors for an individual pin
* @param address - I2C address for the target device
* @param pin - 1 to 16
* @param value - 1 = enabled, 0 = disabled
*/
void set_pin_pullup(uint8_t address, uint8_t pin, uint8_t value);

/**
* get the internal 100K pull-up resistors for an individual pin
* @param address - I2C address for the target device
* @param pin - 1 to 16
* @returns 1 = enabled, 0 = disabled
*/
uint8_t get_pin_pullup(uint8_t address, uint8_t pin);

/**
* set the internal 100K pull-up resistors for the selected IO port
* @param address - I2C address for the target device
* @param port - 0 = pins 1 to 8, port 1 = pins 9 to 16
* @param value - 0 to 255 (0xFF). For each bit 1 = enabled, 0 = disabled
*/
void set_port_pullups(uint8_t address, uint8_t port, uint8_t value);

/**
* get the internal 100K pull-up resistors for the selected IO port
* @param address - I2C address for the target device
* @param port - 0 = pins 1 to 8, port 1 = pins 9 to 16
* @returns 0 to 255 (0xFF). For each bit 1 = enabled, 0 = disabled
*/
uint8_t get_port_pullups(uint8_t address, uint8_t port);

/**
* set internal 100K pull-up resistors for the IO bus
* @param address - I2C address for the target device
* @param value - 0 to 65535 (0xFFFF).  For each bit 1 = enabled, 0 = disabled
*/
void set_bus_pullups(uint8_t address, uint16_t value);

/**
* get internal 100K pull-up resistors for the IO bus
* @param address - I2C address for the target device
* @returns 0 to 65535 (0xFFFF).  For each bit 1 = enabled, 0 = disabled
*/
uint16_t get_bus_pullups(uint8_t address);

/**
* write to an individual pin 1 - 16
* @param address - I2C address for the target device
* @param pin - 1 to 16
* @param value - 0 = logic low, 1 = logic high
*/
void write_pin(uint8_t address, uint8_t pin, uint8_t value);

/**
* write to all pins on the selected port
* @param address - I2C address for the target device
* @param port - 0 = pins 1 to 8, port 1 = pins 9 to 16
* @param value - 0 to 255 (0xFF)
*/
void write_port(uint8_t address, uint8_t port, uint8_t value);

/**
* write to all pins on the selected bus
* @param address - I2C address for the target device
* @param value - 0 to 65535 (0xFFFF). For each bit 1 = logic high, 0 = logic low
*/
void write_bus(uint8_t address, uint16_t value);

/**
* read the value of an individual pin
* @param address - I2C address for the target device
* @param pin - 1 to 16
* @returns - 0 = logic low, 1 = logic high
*/
uint8_t read_pin(uint8_t address, uint8_t pin);

/**
* read all pins on the selected port
* @param address - I2C address for the target device
* @param port - 0 = pins 1 to 8, port 1 = pins 9 to 16
* @returns - 0 to 255 (0xFF). For each bit 1 = logic high, 0 = logic low
*/
uint8_t read_port(uint8_t address, uint8_t port);

/**
* read all pins on the selected bus
* @param address - I2C address for the target device
* @returns - 0 to 65535 (0xFFFF). For each bit 1 = logic high, 0 = logic low
*/
uint16_t read_bus(uint8_t address);

/**
* invert the polarity of the selected pin
* @param address - I2C address for the target device
* @param pin - 1 to 16
* @param polarity - 0 = non-inverted, 1 = inverted
*/
void invert_pin(uint8_t address, uint8_t pin, uint8_t polarity);

/**
* get the polarity of the selected pin
* @param address - I2C address for the target device
* @param pin - 1 to 16
* @returns - pin polarity 0 = non-inverted, 1 = inverted
*/
uint8_t get_pin_polarity(uint8_t address, uint8_t pin);

/**
* invert the polarity of the pins on a selected port
* @param address - I2C address for the target device
* @param port - 0 = pins 1 to 8, port 1 = pins 9 to 16
* @param polarity - 0 to 255 (0xFF). For each bit 0 = non-inverted, 1 = inverted
*/
void invert_port(uint8_t address, uint8_t port, uint8_t polarity);

/**
* get the polarity of the selected pin
* @param address - I2C address for the target device
* @param port - 0 = pins 1 to 8, port 1 = pins 9 to 16
* @returns - 0 to 255 (0xFF). For each bit 0 = non-inverted, 1 = inverted
*/
uint8_t get_port_polarity(uint8_t address, uint8_t port);

/**
* invert the polarity of the pins on a selected bus
* @param address - I2C address for the target device
* @param polarity - 0 to 65535 (0xFFFF). For each bit 0 = non-inverted, 1 = inverted
*/
void invert_bus(uint8_t address, uint16_t polarity);

/**
* get the polarity of the bus
* @param address - I2C address for the target device
* @returns - 0 to 65535 (0xFFFF). For each bit 0 = non-inverted, 1 = inverted
*/
uint16_t get_bus_polarity(uint8_t address);

/**
* Set the interrupt pins to be mirrored or for separate ports
* @param address - I2C address for the target device
* @param value - 1 = The interrupt pins are internally connected, 0 = The interrupt pins are not connected. INTA is associated with PortA and INTB is associated with PortB
*/
void mirror_interrupts(uint8_t address, uint8_t value);

/**
* This sets the polarity of the interrupt output pins.
* @param address - I2C address for the target device
* @param value - 1 = Active-high, 0 = Active-low.
*/
void set_interrupt_polarity(uint8_t address, uint8_t value);

/**
* Get the polarity of the interrupt output pins.
* @param address - I2C address for the target device
* @returns 1 = Active-high, 0 = Active-low.
*/
uint8_t get_interrupt_polarity(uint8_t address);

/**
* Sets the type of interrupt for each pin on the selected port
* @param address - I2C address for the target device
* @param port - 0 = pins 1 to 8, port 1 = pins 9 to 16
* @param value - 0 to 255 (0xFF). For each bit 1 = interrupt is fired when the pin matches the default value, 0 = the interrupt is fired on state change
*/
void set_interrupt_type(uint8_t address, uint8_t port, uint8_t value);

/**
* Get the type of interrupt for each pin on the selected port
* @param address - I2C address for the target device
* @param port - 0 = pins 1 to 8, port 1 = pins 9 to 16
* @returns 0 to 255 (0xFF). For each bit 1 = interrupt is fired when the pin matches the default value, 0 = the interrupt is fired on state change
*/
uint8_t get_interrupt_type(uint8_t address, uint8_t port);

/**
* These bits set the compare value for pins configured for interrupt-on-change on the selected port.
* If the associated pin level is the opposite from the register bit, an interrupt occurs.
* @param address - I2C address for the target device
* @param port - 0 = pins 1 to 8, port 1 = pins 9 to 16
* @param value - default state for the port. 0 to 255 (0xFF).
*/
void set_interrupt_defaults(uint8_t address, uint8_t port, uint8_t value);

/**
* Get the compare value for pins configured for interrupt-on-change on the selected port.
* If the associated pin level is the opposite from the register bit, an interrupt occurs.
* @param address - I2C address for the target device
* @param port - 0 = pins 1 to 8, port 1 = pins 9 to 16
* @returns default state for the port. 0 to 255 (0xFF).
*/
uint8_t get_interrupt_defaults(uint8_t address, uint8_t port);

/**
* Enable interrupts for the selected pin
* @param address - I2C address for the target device
* @param pin - 1 to 16
* @param value - 0 = interrupt disabled, 1 = interrupt enabled
*/
void set_interrupt_on_pin(uint8_t address, uint8_t pin, uint8_t value);

/**
* Get the interrupt enable status for the selected pin
* @param address - I2C address for the target device
* @param pin - 1 to 16
* @returns 0 = interrupt disabled, 1 = interrupt enabled
*/
uint8_t get_interrupt_on_pin(uint8_t address, uint8_t pin);

/**
* Enable interrupts for the pins on the selected port
* @param address - I2C address for the target device
* @param port - 0 = pins 1 to 8, port 1 = pins 9 to 16
* @param value - 0 to 255 (0xFF). For each bit 0 = interrupt disabled, 1 = interrupt enabled
*/
void set_interrupt_on_port(uint8_t address, uint8_t port, uint8_t value);

/**
* Get the interrupt enable status for the selected port
* @param address - I2C address for the target device
* @param port - 0 = pins 1 to 8, port 1 = pins 9 to 16
* @returns 0 to 255 (0xFF). For each bit 0 = interrupt disabled, 1 = interrupt enabled
*/
uint8_t get_interrupt_on_port(uint8_t address, uint8_t port);

/**
* Enable interrupts for the pins on the selected bus
* @param address - I2C address for the target device
* @param value - 0 to 65535 (0xFFFF). For each bit 0 = interrupt disabled, 1 = interrupt enabled
*/
void set_interrupt_on_bus(uint8_t address, uint16_t value);

/**
* Get the interrupt enable status for the selected bus
* @param address - I2C address for the target device
* @returns 0 to 65535 (0xFFFF). For each bit 0 = interrupt disabled, 1 = interrupt enabled
*/
uint16_t get_interrupt_on_bus(uint8_t address);

/**
* read the interrupt status for the pins on the selected port
* @param address - I2C address for the target device
* @param port - 0 = pins 1 to 8, port 1 = pins 9 to 16
* @returns - 0 to 255 (0xFF). For each bit 1 = interrupt triggered, 0 = interrupt not triggered
*/
uint8_t read_interrupt_status(uint8_t address, uint8_t port);

/**
* read the value from the selected port at the time of the last interrupt trigger
* @param address - I2C address for the target device
* @param port - 0 = pins 1 to 8, port 1 = pins 9 to 16
* @returns - 0 to 255 (0xFF). For each bit 1 = interrupt triggered, 0 = interrupt not triggered
*/
uint8_t read_interrupt_capture(uint8_t address, uint8_t port);

/**
* Reset the interrupts A and B to 0
* @param address - I2C address for the target device
*/
void reset_interrupts(uint8_t address);