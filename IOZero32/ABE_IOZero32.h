/*
 ================================================
 AB Electronics UK IO Zero 32 : 32-Channel Port Expander
 See CHANGELOG.md for version number
 ================================================

 32-Channel Port Expander based on the PCA9535.
 The PCA9535 chip is split into two 8-bit ports.  Port 0 controls
 pins 1 to 8 while Port 1 controls pins 9 to 16.
 When writing to or reading from a bus or port the least significant bit
 represents the lowest numbered pin on the selected port.

 Required package: libi2c-dev
 apt-get install libi2c-dev
 */

// public functions

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
* set the polarity of the selected pin
* @param address - I2C address for the target device
* @param pin - 1 to 16
* @param polarity - 0 = non-inverted, 1 = inverted
*/
void set_pin_polarity(uint8_t address, uint8_t pin, uint8_t polarity);

/**
* get the polarity of the selected pin
* @param address - I2C address for the target device
* @param pin - 1 to 16
* @returns - pin polarity 0 = non-inverted, 1 = inverted
*/
uint8_t get_pin_polarity(uint8_t address, uint8_t pin);

/**
* set the polarity of the pins on a selected port
* @param address - I2C address for the target device
* @param port - 0 = pins 1 to 8, port 1 = pins 9 to 16
* @param polarity - 0 to 255 (0xFF). For each bit 0 = non-inverted, 1 = inverted
*/
void set_port_polarity(uint8_t address, uint8_t port, uint8_t polarity);

/**
* get the polarity of the selected pin
* @param address - I2C address for the target device
* @param port - 0 = pins 1 to 8, port 1 = pins 9 to 16
* @returns - 0 to 255 (0xFF). For each bit 0 = non-inverted, 1 = inverted
*/
uint8_t get_port_polarity(uint8_t address, uint8_t port);

/**
* set the polarity of the pins on a selected bus
* @param address - I2C address for the target device
* @param polarity - 0 to 65535 (0xFFFF). For each bit 0 = non-inverted, 1 = inverted
*/
void set_bus_polarity(uint8_t address, uint16_t polarity);

/**
* get the polarity of the bus
* @param address - I2C address for the target device
* @returns - 0 to 65535 (0xFFFF). For each bit 0 = non-inverted, 1 = inverted
*/
uint16_t get_bus_polarity(uint8_t address);