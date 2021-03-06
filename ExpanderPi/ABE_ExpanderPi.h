/*
================================================
 AB Electronics UK Expander Pi
 See CHANGELOG.md for version number
================================================

Required package: libi2c-dev
apt-get install libi2c-dev
*/

#ifndef EXPANDERPI_ABE_EXPANDERPI_H_
#define EXPANDERPI_ABE_EXPANDERPI_H_

#endif /* EXPANDERPI_ABE_EXPANDERPI_H_ */

/*=================== ADC Functions ===================*/

/**
* Open the ADC SPI bus channel
* This needs to be called before using the ADC
*/
int adc_open();


/**
* Close the ADC SPI bus channel
*/
void adc_close();


/**
* Read the voltage from the ADC
* @param channel - 1 to 8
* @param mode - 0 = Single Ended or 1 = Differential
* @returns between 0V and the reference voltage
*/
double adc_read_voltage(uint8_t channel, uint8_t mode);


/**
* Read the raw value from the ADC
* @param channel -  1 to 8
* @param mode -  0 = Single Ended or 1 = Differential
* When in differential mode setting channel to 1 will make IN1 = IN+ and IN2 = IN-
* When in differential mode setting channel to 2 will make IN1 = IN- and IN2 = IN+
* @returns 12 bit value between 0 and 4096
*/
uint16_t adc_read_raw(uint8_t channel, uint8_t mode);


/**
* Set the reference voltage for the adc
* @param ref - Set this value to be the same as the voltage measured on the Vref pin on the Expander Pi
* If using the on board voltage reference then the value will be 4.096
*/
void adc_set_refvoltage(double ref);


/*=================== DAC Functions ===================*/

/**
* Open the DAC SPI bus channel
* This needs to be called before using the DAC
*/
int dac_open();


/**
* Close the DAC SPI bus channel
*/
void dac_close();


/**
* Set the raw value from the selected channel on the DAC
* @param raw - between 0 and 4095
* @param channel - 1 or 2
* @param gain - 1 or 2  - The output voltage will be between 0 and 2.048V when gain is set to 1,  0 and 4.096V when gain is set to 2
*/
void dac_set_raw(uint16_t raw, uint8_t channel, uint8_t gain);


/**
* Set the DAC voltage
* @param voltage - between 0 and 2.048 when gain is set to 1,  0 and 4.096 when gain is set to 2
* @param channel - 1 or 2
* @param gain - 1 or 2
*/
void dac_set_voltage(double voltage, uint8_t channel, uint8_t gain);


/*=================== IO Functions ===================*/

/**
* Set IO direction for an individual pin
* @param pins - 1 to 16
* @param direction - 1 = input, 0 = output
*/
void io_set_pin_direction(uint8_t pin, uint8_t direction);


/**
* Get IO direction for an individual pin
* @param pins - 1 to 16
* @returns 1 = input, 0 = output
*/
uint8_t io_get_pin_direction(uint8_t pin);


/**
* Set direction for an IO port
* @param port - 0 = pins 1 to 8, port 1 = pins 9 to 16
* @param direction - 0 to 255 (0xFF).  For each bit 1 = input, 0 = output
*/
void io_set_port_direction(uint8_t port, uint8_t direction);


/**
* Get direction for an IO port
* @param port - 0 = pins 1 to 8, port 1 = pins 9 to 16
* @returns 0 to 255 (0xFF).  For each bit 1 = input, 0 = output
*/
uint8_t io_get_port_direction(uint8_t port);


/**
* Set direction for the IO bus
* @param direction - 0 to 65535 (0xFFFF).  For each bit 1 = input, 0 = output
*/
void io_set_bus_direction(uint16_t direction);


/**
* Get direction for the IO bus
* @returns 0 to 65535 (0xFFFF).  For each bit 1 = input, 0 = output
*/
uint16_t io_get_bus_direction();


/**
* Set the internal 100K pull-up resistors for an individual pin
* @param pin - 1 to 16
* @param value - 1 = enabled, 0 = disabled
*/
void io_set_pin_pullup(uint8_t pin, uint8_t value);


/**
* Get the internal 100K pull-up resistors for an individual pin
* @param pin - 1 to 16
* @returns 1 = enabled, 0 = disabled
*/
uint8_t io_get_pin_pullup(uint8_t pin);


/**
* Set the internal 100K pull-up resistors for the selected IO port
* @param port - 0 = pins 1 to 8, port 1 = pins 9 to 16
* @param value - 0 to 255 (0xFF). For each bit 1 = enabled, 0 = disabled
*/
void io_set_port_pullups(uint8_t port, uint8_t value);


/**
* Get the internal 100K pull-up resistors for the selected IO port
* @param port - 0 = pins 1 to 8, port 1 = pins 9 to 16
* @returns 0 to 255 (0xFF). For each bit 1 = enabled, 0 = disabled
*/
uint8_t io_get_port_pullups(uint8_t port);


/**
* Set internal 100K pull-up resistors for the IO bus
* @param value - 0 to 65535 (0xFFFF).  For each bit 1 = enabled, 0 = disabled
*/
void io_set_bus_pullups(uint16_t value);


/**
* Get internal 100K pull-up resistors for the IO bus
* @returns 0 to 65535 (0xFFFF).  For each bit 1 = enabled, 0 = disabled
*/
uint16_t io_get_bus_pullups();


/**
* Write to an individual pin 1 - 16
* @param pin - 1 to 16
* @param value - 0 = logic low, 1 = logic high
*/
void io_write_pin(uint8_t pin, uint8_t value);


/**
* Write to all pins on the selected port
* @param port - 0 = pins 1 to 8, port 1 = pins 9 to 16
* @param value - 0 to 255 (0xFF)
*/
void io_write_port(uint8_t port, uint8_t value);


/**
* Write to all pins on the selected bus
* @param value - 0 to 65535 (0xFFFF). For each bit 1 = logic high, 0 = logic low
*/
void io_write_bus(uint16_t value);


/**
* Read the value of an individual pin
* @param pin - 1 to 16
* @returns - 0 = logic low, 1 = logic high
*/
uint8_t io_read_pin(uint8_t pin);


/**
* read all pins on the selected port
* @param port - 0 = pins 1 to 8, port 1 = pins 9 to 16
* @returns - 0 to 255 (0xFF). For each bit 1 = logic high, 0 = logic low
*/
uint8_t io_read_port(uint8_t port);


/**
* Read all pins on the bus
* @returns - 0 to 65535 (0xFFFF). For each bit 1 = logic high, 0 = logic low
*/
uint16_t io_read_bus();


/**
* Invert the polarity of the selected pin
* @param pin - 1 to 16
* @param polarity - 0 = non-inverted, 1 = inverted
*/
void io_invert_pin(uint8_t pin, uint8_t polarity);


/**
* Get the polarity of the selected pin
* @param pin - 1 to 16
* @returns - pin polarity 0 = non-inverted, 1 = inverted
*/
uint8_t io_get_pin_polarity(uint8_t pin);


/**
* Invert the polarity of the pins on a selected port
* @param port - 0 = pins 1 to 8, port 1 = pins 9 to 16
* @param polarity - 0 to 255 (0xFF). For each bit 0 = non-inverted, 1 = inverted
*/
void io_invert_port(uint8_t port, uint8_t polarity);


/**
* Get the polarity of the selected pin
* @param port - 0 = pins 1 to 8, port 1 = pins 9 to 16
* @returns - 0 to 255 (0xFF). For each bit 0 = non-inverted, 1 = inverted
*/
uint8_t io_get_port_polarity(uint8_t port);


/**
* Invert the polarity of the pins on a selected bus
* @param polarity - 0 to 65535 (0xFFFF). For each bit 0 = non-inverted, 1 = inverted
*/
void io_invert_bus(uint16_t polarity);


/**
* Get the polarity of the bus
* @returns - 0 to 65535 (0xFFFF). For each bit 0 = non-inverted, 1 = inverted
*/
uint16_t io_get_bus_polarity();


/**
* Set the interrupt pins to be mirrored or for separate ports
* @param value - 1 = The interrupt pins are internally connected, 0 = The interrupt pins are not connected. INTA is associated with PortA and INTB is associated with PortB
*/
void io_mirror_interrupts(uint8_t value);


/**
* This sets the polarity of the interrupt output pins.
* @param value - 1 = Active-high, 0 = Active-low.
*/
void io_set_interrupt_polarity(uint8_t value);


/**
* Get the polarity of the interrupt output pins.
* @returns 1 = Active-high, 0 = Active-low.
*/
uint8_t io_get_interrupt_polarity();


/**
* Sets the type of interrupt for each pin on the selected port
* @param port - 0 = pins 1 to 8, port 1 = pins 9 to 16
* @param value - 0 to 255 (0xFF). For each bit 1 = interrupt is fired when the pin matches the default value, 0 = the interrupt is fired on state change
*/
void io_set_interrupt_type(uint8_t port, uint8_t value);


/**
* Get the type of interrupt for each pin on the selected port
* @param port - 0 = pins 1 to 8, port 1 = pins 9 to 16
* @returns 0 to 255 (0xFF). For each bit 1 = interrupt is fired when the pin matches the default value, 0 = the interrupt is fired on state change
*/
uint8_t io_get_interrupt_type(uint8_t port);


/**
* These bits set the compare value for pins configured for interrupt-on-change on the selected port.
* If the associated pin level is the opposite from the register bit, an interrupt occurs.
* @param port - 0 = pins 1 to 8, port 1 = pins 9 to 16
* @param value - default state for the port. 0 to 255 (0xFF).
*/
void io_set_interrupt_defaults(uint8_t port, uint8_t value);


/**
* Get the compare value for pins configured for interrupt-on-change on the selected port.
* If the associated pin level is the opposite from the register bit, an interrupt occurs.
* @param port - 0 = pins 1 to 8, port 1 = pins 9 to 16
* @returns default state for the port. 0 to 255 (0xFF).
*/
uint8_t io_get_interrupt_defaults(uint8_t port);


/**
* Enable interrupts for the selected pin
* @param pin - 1 to 16
* @param value - 0 = interrupt disabled, 1 = interrupt enabled
*/
void io_set_interrupt_on_pin(uint8_t pin, uint8_t value);


/**
* Get the interrupt enable status for the selected pin
* @param pin - 1 to 16
* @returns 0 = interrupt disabled, 1 = interrupt enabled
*/
uint8_t io_get_interrupt_on_pin(uint8_t pin);


/**
* Enable interrupts for the pins on the selected port
* @param port - 0 = pins 1 to 8, port 1 = pins 9 to 16
* @param value - 0 to 255 (0xFF). For each bit 0 = interrupt disabled, 1 = interrupt enabled
*/
void io_set_interrupt_on_port(uint8_t port, uint8_t value);


/**
* Get the interrupt enable status for the selected port
* @param port - 0 = pins 1 to 8, port 1 = pins 9 to 16
* @returns 0 to 255 (0xFF). For each bit 0 = interrupt disabled, 1 = interrupt enabled
*/
uint8_t io_get_interrupt_on_port(uint8_t port);


/**
* Enable interrupts for the pins on the selected bus
* @param value - 0 to 65535 (0xFFFF). For each bit 0 = interrupt disabled, 1 = interrupt enabled
*/
void io_set_interrupt_on_bus(uint16_t value);


/**
* Get the interrupt enable status for the selected bus
* @returns 0 to 65535 (0xFFFF). For each bit 0 = interrupt disabled, 1 = interrupt enabled
*/
uint16_t io_get_interrupt_on_bus();


/**
* Read the interrupt status for the pins on the selected port
* @param port - 0 = pins 1 to 8, port 1 = pins 9 to 16
* @returns - 0 to 255 (0xFF). For each bit 1 = interrupt triggered, 0 = interrupt not triggered
*/
uint8_t io_read_interrupt_status(uint8_t port);


/**
* Read the value from the selected port at the time of the last interrupt trigger
* @param port - 0 = pins 1 to 8, port 1 = pins 9 to 16
* @returns - 0 to 255 (0xFF). For each bit 1 = interrupt triggered, 0 = interrupt not triggered
*/
uint8_t io_read_interrupt_capture(uint8_t port);


/**
* Reset the interrupts A and B to 0
*/
void io_reset_interrupts();


/**
* Initialise the MCP32017 IO chip
* @param reset - If set to 1 the IO Pi will reset to default values. Ports are inputs, pull-up resistors are disabled and ports are not inverted.
*/
void io_init(uint8_t reset);


/*=================== RTC Functions ===================*/

/**
* Set the date on the RTC
* @param date - struct tm formatted date and time
*/
void rtc_set_date(struct tm date);


/**
* Read the date from the RTC.
* @returns - date as a tm struct
*/
struct tm rtc_read_date();


/**
* Enable the square wave output pin
*/
void rtc_enable_output();


/**
* Disable the square wave output pin
*/
void rtc_disable_output();


/**
* Set the square wave output frequency
* @param frequency - 1 = 1Hz, 2 = 4.096KHz, 3 = 8.192KHz, 4 = 32.768KHz
*/
void rtc_set_frequency(uint8_t frequency);


/**
* Write to the memory on the DS1307.  
* The DS1307 contains 56-Byte, battery-backed RAM with Unlimited Writes
* @param address - 0x08 to 0x3F
* @param valuearray - byte array containing data to be written to memory
*/
void rtc_write_memory(uint8_t address, uint8_t *valuearray);


/**
* Read from the memory on the DS1307
* The DS1307 contains 56-Byte, battery-backed RAM with Unlimited Writes
* @param address - 0x08 to 0x3F
* @param length - up to 32 bytes.  length can not exceed the available address space.
* @returns - pointer to a byte array where the data will be saved
*/
uint8_t *rtc_read_memory(uint8_t address, uint8_t length);
