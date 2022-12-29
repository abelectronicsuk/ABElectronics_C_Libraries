/*
================================================
 AB Electronics UK Expander Pi
 See CHANGELOG.md for the version number
================================================

Required package: libi2c-dev
apt-get install libi2c-dev
*/

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>
#include <linux/i2c-dev.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>

// local variables

// SPI Bus Variables
#define adcdevice "/dev/spidev0.0"
#define dacdevice "/dev/spidev0.1"
static uint8_t mode = SPI_MODE_0; // SPI_MODE_0
static uint32_t adcspeed = 200000; // ADC SPI bus speed
static uint32_t dacspeed = 20000000; // DAC SPI bus speed

// I2C Bus Variables

const char *fileName = "/dev/i2c-1"; // change to /dev/i2c-0 if you are using a revision 0002 or 0003 model B
static int i2cbus;
static uint8_t buf[10] = { 0 };
static uint8_t writebuffer[60] = { 0 };
static uint8_t readbuffer[60] = { 0 };

// ADC Variables

static uint8_t adctx[] = { 0x01, 0x80, 0x00 }; // transmit buffer for the ADC
static uint8_t adcrx[3]; // receive buffer for the adc;
static int adc; // adc object
static double adcrefvoltage = 4.096; // reference voltage for the ADC chip.

// DAC Variables

static int dac; // dac object

// IO Definititions
#define IOADDRESS 0x20
#define IODIRA 0x00 // IO direction A - 1= input 0 = output
#define IODIRB 0x01 // IO direction B - 1= input 0 = output
#define IPOLA 0x02 // Input polarity A 
#define IPOLB 0x03 // Input polarity B
#define GPINTENA 0x04 // The GPINTEN register controls the interrupt-on-change feature for each pin on port A.
#define GPINTENB 0x05 // The GPINTEN register controls the interrupt-on-change feature for each pin on port B.
#define DEFVALA 0x06 // Default value for port A
#define DEFVALB 0x07 // Default value for port B
#define INTCONA 0x08 // Interrupt control register for port A
#define INTCONB 0x09 // Interrupt control register for port B
#define IOCON 0x0A // see datasheet for configuration register
#define GPPUA 0x0C // pullup resistors for port A
#define GPPUB 0x0D // pullup resistors for port B
#define INTFA 0x0E // The INTF register reflects the interrupt condition of any pin that is enabled for interrupts.
#define INTFB 0x0F
#define INTCAPA 0x10 // The INTCAP register captures the GPIO port value at the time the interrupt occurred.
#define INTCAPB 0x11
#define GPIOA 0x12 // data port A
#define GPIOB 0x13 // data port B
#define OLATA 0x14 // output latches A
#define OLATB 0x15 // output latches B

// IO Variables
static uint8_t config = 0x02;

// RTC Definitions
#define RTCADDRESS 0x68
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

// RTC Variables
uint8_t rtcConfig = 0x03;
uint16_t rtcCentury = 2000;


/*===================Local Functions===================*/

static uint8_t read_byte_data(uint8_t address, uint8_t reg) {

	if ((i2cbus = open(fileName, O_RDWR)) < 0) {
		printf("Failed to open I2C port for read %s \n", strerror(errno));

		exit(1);
	}

	if (ioctl(i2cbus, I2C_SLAVE, address) < 0) {
		printf("Failed to write to I2C port for read\n");
		exit(1);
	}

	buf[0] = reg;

	if ((write(i2cbus, buf, 1)) != 1) {
		printf("Failed to write to I2C device for read\n");
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

	if ((i2cbus = open(fileName, O_RDWR)) < 0) {
		printf("Failed to open I2C port for read %s \n", strerror(errno));

		exit(1);
	}

	if (ioctl(i2cbus, I2C_SLAVE, address) < 0) {
		printf("Failed to write to I2C port for read\n");
		exit(1);
	}

	buf[0] = reg;

	if ((write(i2cbus, buf, 1)) != 1) {
		printf("Failed to write to I2C device for read\n");
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

static void read_byte_array(uint8_t address, uint8_t reg, uint8_t length) {

	if ((i2cbus = open(fileName, O_RDWR)) < 0) {
		printf("Failed to open I2C port for read %s \n", strerror(errno));

		exit(1);
	}

	if (ioctl(i2cbus, I2C_SLAVE, address) < 0) {
		printf("Failed to write to I2C port for read\n");
		exit(1);
	}

	writebuffer[0] = reg;

	if ((write(i2cbus, writebuffer, 1)) != 1) {
		printf("Failed to write to I2C device for read\n");
		exit(1);
	}

	read(i2cbus, readbuffer, length);

	close(i2cbus);
}

static void write_byte_data(uint8_t address, uint8_t reg, uint8_t value) {
	if ((i2cbus = open(fileName, O_RDWR)) < 0) {
		printf("Failed to open I2C port for write\n");
		exit(1);
	}

	if (ioctl(i2cbus, I2C_SLAVE, address) < 0) {
		printf("Failed to write to I2C port for write\n");
		exit(1);
	}

	buf[0] = reg;
	buf[1] = value;

	if ((write(i2cbus, buf, 2)) != 2) {
		printf("Failed to write to I2C device for write\n");
		exit(1);
	}

	close(i2cbus);
}

static void write_word_data(uint8_t address, uint8_t reg, uint16_t value) {
	if ((i2cbus = open(fileName, O_RDWR)) < 0) {
		printf("Failed to open I2C port for write\n");
		exit(1);
	}

	if (ioctl(i2cbus, I2C_SLAVE, address) < 0) {
		printf("Failed to write to I2C port for write\n");
		exit(1);
	}

	buf[0] = reg;
	buf[1] = (uint8_t)(value&(0xff)); // lower 8 bits
	buf[2] = (uint8_t)(value>>8) & 0xff; // upper 8 bits

	if ((write(i2cbus, buf, 3)) != 3) {
		printf("Failed to write to I2C device for write\n");
		exit(1);
	}

	close(i2cbus);
}

void write_byte_array(uint8_t address, uint8_t buffer[], uint8_t length) {
	/*	internal function for writing data to the i2c bus	*/
	if ((i2cbus = open(fileName, O_RDWR)) < 0) {
		printf("Failed to open I2C port for write\n");
		exit(1);
	}

	if (ioctl(i2cbus, I2C_SLAVE, address) < 0) {
		printf("Failed to write to I2C port for write\n");
		exit(1);
	}

	if ((write(i2cbus, buffer, length)) != length) {
		printf("Failed to write to I2C device for write\n");
		exit(1);
	}

	close(i2cbus);
}

static unsigned char bcd_to_dec(unsigned char bcd) {
	// internal RTC function for converting a BCD formatted number to decimal
	return (unsigned char)((HI_NIBBLE(bcd) * 10) + (LO_NIBBLE(bcd)));
}

static unsigned char dec_to_bcd(char dec) {
	// internal RTC function for converting a decimal formatted number to BCD
	return (unsigned char)((dec / 10) * 16) + (dec % 10);
}

static char updatebyte(char byte, char bit, char value) {
	// internal function for setting the value of a single bit within a byte
	if (value == 0) {
		return (byte &= ~(1 << bit));

	}
	else {
		return (byte |= 1 << bit);
	}

}

static char checkbit(char byte, char bit) {
	// internal function for reading the value of a single bit within a byte
	if (byte & (1 << bit)) {
		return (1);
	}
	else {
		return (0);
	}
}

static void set_pin(uint8_t pin, uint8_t value, uint8_t a_register, uint8_t b_register)
{
	/**
	* internal IO function for setting the value of a single bit within the device registers
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

	uint8_t newval = updatebyte(read_byte_data(IOADDRESS, reg), p, value);
	write_byte_data(IOADDRESS, reg, newval);
}

static uint8_t get_pin(uint8_t pin, uint8_t a_register, uint8_t b_register)
{
	/**
	* internal IO function for getting the value of a single bit within the device registers
	*/

		uint8_t value = 0;

        if (pin >= 1 && pin <= 8)
		{
            value = checkbit(read_byte_data(IOADDRESS, a_register), pin - 1);
		}
        else if (pin >= 9 && pin <= 16)
		{
            value = checkbit(read_byte_data(IOADDRESS, b_register), pin - 9);
		}
        else
		{
            printf("pin out of range: 1 to 16");
		}

        return value;
}

static void set_port(uint8_t port, uint8_t value, uint8_t a_register, uint8_t b_register)
{
	/**
	* internal IO function for setting the value of a device register
	*/
	if (port == 0)
	{
    	write_byte_data(IOADDRESS, a_register, value);
	}
    else if (port == 1)
	{
    	write_byte_data(IOADDRESS, b_register, value);
	}
	else
	{
		printf("port out of range: 0 or 1");
	}
}

static uint8_t get_port(uint8_t port, uint8_t a_register, uint8_t b_register)
{
	/**
	* internal IO function for getting the value of a device register
	*/
	if (port == 0)
	{
    	return (read_byte_data(IOADDRESS, a_register));
	}
    else if (port == 1)
	{
    	return (read_byte_data(IOADDRESS, b_register));
	}
	else
	{
		printf("port out of range: 0 or 1");
		return (0);
	}
}


/*===============================Public Functions===============================*/


/*===================ADC Functions Begin ===================*/

int adc_open() {
	/**
	* Open the ADC SPI bus channel
	* This needs to be called before using the ADC
	*/

	// Open SPI device
	if ((adc = open(adcdevice, O_RDWR)) < 0)
		return (0);

	if (ioctl(adc, SPI_IOC_WR_MAX_SPEED_HZ, &adcspeed) == -1)
		return (0);
	// Set SPI mode
	if (ioctl(adc, SPI_IOC_WR_MODE, &mode) == -1)
		return (0);

	return (1);
}

void adc_close() {
	/**
	* Close the ADC SPI bus channel
	*/
	close(adc);
}

uint16_t adc_read_raw(uint8_t channel, uint8_t mode) {
	/**
	* Read the raw value from the ADC
	* @param channel -  1 to 8
	* @param mode -  0 = Single Ended or 1 = Differential
	* When in differential mode, setting channel to 1 will make IN1 = IN+ and IN2 = IN-
	* When in differential mode, setting channel to 2 will make IN1 = IN- and IN2 = IN+
	* @returns 12 bit value between 0 and 4096
	*/

	channel = channel - 1;
	if (mode == 0) {
		adctx[0] = 6 + (channel >> 2);
		adctx[1] = (channel & 3) << 6;
	}
	else if (mode == 1) {
		adctx[0] = 4 + (channel >> 2);
		adctx[1] = (channel & 3) << 6;
	}
	else {
		return (0);
	}

	struct spi_ioc_transfer tr = { .tx_buf = (uintptr_t)adctx,.rx_buf =
		(uintptr_t)adcrx,.len = 3,.delay_usecs = 0,.speed_hz = adcspeed,
		.bits_per_word = 8, };

	if (ioctl(adc, SPI_IOC_MESSAGE(1), &tr) == -1){ return 0;}

	return (((adcrx[1] & 0x0F) << 8) + (adcrx[2]));
}

double adc_read_voltage(uint8_t channel, uint8_t mode) {
	/**
	* Read the voltage from the ADC
	* @param channel - 1 to 8
	* @param mode - 0 = Single Ended or 1 = Differential
	* @returns between 0V and the reference voltage
	*/
	uint16_t rawval = adc_read_raw(channel, mode);
	return ((adcrefvoltage / 4096) * (double)rawval);
}

void adc_set_refvoltage(double ref) {
	/**
	* Set the reference voltage for the ADC
	* @param ref - Set this value to be the same as the voltage measured on the Vref pin on the Expander Pi
	* If using the on-board voltage reference then the value will be 4.096
	*/
	adcrefvoltage = ref;
}

/*===================ADC Functions END ===================*/

/*===================DAC Functions Begin ===================*/

int dac_open() {
	/**
	* Open the DAC SPI bus channel
	* This needs to be called before using the DAC
	*/

	// Open SPI device
	if ((dac = open(dacdevice, O_RDWR)) < 0)
		return (0);

	if (ioctl(dac, SPI_IOC_WR_MAX_SPEED_HZ, &dacspeed) == -1)
		return (0);
	// Set SPI mode
	if (ioctl(dac, SPI_IOC_WR_MODE, &mode) == -1)
		return (0);

	return (1);
}

void dac_close() {
	/**
	* Close the DAC SPI bus channel
	*/
	close(dac);
}

void dac_set_raw(uint16_t raw, uint8_t channel, uint8_t gain) {
	/**
	* Set the raw value from the selected channel on the DAC
	* @param raw - between 0 and 4095
	* @param channel - 1 or 2
	* @param gain - 1 or 2  - The output voltage will be between 0 and 2.048V when the gain is set to 1,  0 and 4.096V when the gain is set to 2
	*/

	uint16_t tx;

	if (channel == 1) {
		raw = (raw & 0x0FFF) | 0x3000;

	}
	else if (channel == 2) {
		raw = raw | 0xF000;
	}
	else {
		return;
	}

	if (gain == 2) {
		uint16_t x = raw;
		raw = (x &= ~(1 << 13));
	}

	tx = (raw << 8) | (raw >> 8);

	struct spi_ioc_transfer tr = { .tx_buf = (unsigned long)&tx,.rx_buf =
		(unsigned long)NULL,.len = 2,.delay_usecs = 0,.speed_hz = dacspeed,
		.bits_per_word = 8,.cs_change = 0, };

	// Write data
	if (ioctl(dac, SPI_IOC_MESSAGE(1), &tr) < 1) {
		return;
	}
}

void dac_set_voltage(double voltage, uint8_t channel, uint8_t gain) {
	/**
	* Set the DAC voltage
	* @param voltage - between 0 and 2.048 when the gain is set to 1,  0 and 4.096 when the gain is set to 2
	* @param channel - 1 or 2
	* @param gain - 1 or 2
	*/
	double dacvoltage = 2.048;

	if (gain == 2) {
		dacvoltage = 4.096;
	}
	
	adctx[1] = 0x80;
	if (channel == 2) {
		adctx[1] = 0xC0;
	}
	
	if ((voltage >= 0.0) && (voltage < dacvoltage)) {
		uint16_t rawval = ((voltage / 2.048) * 4096) / gain;
		dac_set_raw(rawval, channel, gain);
	}
	else {
		return;
	}
}


/*===================DAC Functions END ===================*/

/*===================IO Functions Begin ===================*/

void io_set_pin_direction(uint8_t pin, uint8_t direction)
{
	/**
	* Set IO direction for an individual pin
	* @param pins - 1 to 16
	* @param direction - 1 = input, 0 = output
	*/
	set_pin(pin, direction, IODIRA, IODIRB);
}

uint8_t io_get_pin_direction(uint8_t pin)
{
	/**
	* Get IO direction for an individual pin
	* @param pins - 1 to 16
	* @returns 1 = input, 0 = output
	*/
	return get_pin(pin, IODIRA, IODIRB);
}

void io_set_port_direction(uint8_t port, uint8_t direction)
{
	/**
	* Set direction for an IO port
	* @param port - 0 = pins 1 to 8, port 1 = pins 9 to 16
	* @param direction - 0 to 255 (0xFF).  For each bit 1 = input, 0 = output
	*/
	set_port(port, direction, IODIRA, IODIRB);
}

uint8_t io_get_port_direction(uint8_t port)
{
	/**
	* Get the direction for an IO port
	* @param port - 0 = pins 1 to 8, port 1 = pins 9 to 16
	* @returns 0 to 255 (0xFF).  For each bit 1 = input, 0 = output
	*/
	return get_port(port, IODIRA, IODIRB);
}

void io_set_bus_direction(uint16_t direction)
{
	/**
	* Set direction for the IO bus
	* @param direction - 0 to 65535 (0xFFFF).  For each bit 1 = input, 0 = output
	*/
	write_word_data(IOADDRESS, IODIRA, direction);
}

uint16_t io_get_bus_direction()
{
	/**
	* Get the direction for the IO bus
	* @returns 0 to 65535 (0xFFFF).  For each bit 1 = input, 0 = output
	*/
	return read_word_data(IOADDRESS, IODIRA);
}

void io_set_pin_pullup(uint8_t pin, uint8_t value)
{
	/**
	* Set the internal 100K pull-up resistors for an individual pin
	* @param pin - 1 to 16
	* @param value - 1 = enabled, 0 = disabled
	*/
	set_pin(pin, value, GPPUA, GPPUB);
}

uint8_t io_get_pin_pullup(uint8_t pin)
{
	/**
	* Get the internal 100K pull-up resistors for an individual pin
	* @param pin - 1 to 16
	* @returns 1 = enabled, 0 = disabled
	*/

	return get_pin(pin, GPPUA, GPPUB);
}

void io_set_port_pullups(uint8_t port, uint8_t value)
{
	/**
	* Set the internal 100K pull-up resistors for the selected IO port
	* @param port - 0 = pins 1 to 8, port 1 = pins 9 to 16
	* @param value - 0 to 255 (0xFF). For each bit 1 = enabled, 0 = disabled
	*/
	set_port(port, value, GPPUA, GPPUB);
}

uint8_t io_get_port_pullups(uint8_t port)
{
	/**
	* Get the internal 100K pull-up resistors for the selected IO port
	* @param port - 0 = pins 1 to 8, port 1 = pins 9 to 16
	* @returns 0 to 255 (0xFF). For each bit 1 = enabled, 0 = disabled
	*/
	return get_port(port, GPPUA, GPPUB);
}

void io_set_bus_pullups(uint16_t value)
{
	/**
	* Set internal 100K pull-up resistors for the IO bus
	* @param value - 0 to 65535 (0xFFFF).  For each bit 1 = enabled, 0 = disabled
	*/
	write_word_data(IOADDRESS, GPPUA, value);
}

uint16_t io_get_bus_pullups()
{
	/**
	* Get internal 100K pull-up resistors for the IO bus
	* @returns 0 to 65535 (0xFFFF).  For each bit 1 = enabled, 0 = disabled
	*/
	return read_word_data(IOADDRESS, GPPUA);
}

void io_write_pin(uint8_t pin, uint8_t value)
{
	/**
	* Write to an individual pin 1 - 16
	* @param pin - 1 to 16
	* @param value - 0 = logic low, 1 = logic high
	*/
	set_pin(pin, value, GPIOA, GPIOB);
}

void io_write_port(uint8_t port, uint8_t value)
{
	/**
	* Write to all pins on the selected port
	* @param port - 0 = pins 1 to 8, port 1 = pins 9 to 16
	* @param value - 0 to 255 (0xFF)
	*/
	set_port(port, value, GPIOA, GPIOB);
}

void io_write_bus(uint16_t value)
{
	/**
	* Write to all pins on the selected bus
	* @param value - 0 to 65535 (0xFFFF). For each bit 1 = logic high, 0 = logic low
	*/
	write_word_data(IOADDRESS, GPIOA, value);
}

uint8_t io_read_pin(uint8_t pin)
{
	/**
	* Read the value of an individual pin
	* @param pin - 1 to 16
	* @returns - 0 = logic low, 1 = logic high
	*/

	return get_pin(pin, GPIOA, GPIOB);
}

uint8_t io_read_port(uint8_t port)
{
	/**
	* Read all pins on the selected port
	* @param port - 0 = pins 1 to 8, port 1 = pins 9 to 16
	* @returns - 0 to 255 (0xFF). For each bit 1 = logic high, 0 = logic low
	*/
	return get_port(port, GPIOA, GPIOB);
}

uint16_t io_read_bus()
{
	/**
	* Read all pins on the selected bus
	* @returns - 0 to 65535 (0xFFFF). For each bit 1 = logic high, 0 = logic low
	*/
	return read_word_data(IOADDRESS, GPIOA);
}

void io_invert_pin(uint8_t pin, uint8_t polarity)
{
	/**
	* Invert the polarity of the selected pin
	* @param pin - 1 to 16
	* @param polarity - 0 = non-inverted, 1 = inverted
	*/
	set_pin(pin, polarity, IPOLA, IPOLB);
}

uint8_t io_get_pin_polarity(uint8_t pin)
{
	/**
  	* Get the polarity of the selected pin
  	* @param pin - 1 to 16
	* @returns 0 = non-inverted, 1 = inverted
  	*/
  	return get_pin(pin, IPOLA, IPOLB);
}

void io_invert_port(uint8_t port, uint8_t polarity)
{
	/**
	* Invert the polarity of the pins on a selected port
	* @param port - 0 = pins 1 to 8, port 1 = pins 9 to 16
	* @param polarity - 0 to 255 (0xFF). For each bit 0 = non-inverted, 1 = inverted
	*/
	set_port(port, polarity, IPOLA, IPOLB);
}

uint8_t io_get_port_polarity(uint8_t port)
{
	/**
  	* Get the polarity of the selected pin
  	* @param port - 0 = pins 1 to 8, port 1 = pins 9 to 16
	* @returns 0 to 255 (0xFF). For each bit 0 = non-inverted, 1 = inverted
  	*/
  	return get_port(port, IPOLA, IPOLB);
}

void io_invert_bus(uint16_t polarity)
{
	/**
	* Invert the polarity of the pins on a selected bus
	* @param polarity - 0 to 65535 (0xFFFF). For each bit 0 = non-inverted, 1 = inverted
	*/
	write_word_data(IOADDRESS, IPOLA, polarity);
}

 uint16_t io_get_bus_polarity()
 {
	 /**
  	* Get the polarity of the bus
	* @returns 0 to 65535 (0xFFFF). For each bit 0 = non-inverted, 1 = inverted
  	*/
  	return read_word_data(IOADDRESS, IPOLA);
 }

void io_mirror_interrupts(uint8_t value)
{
	/**
	* Set the interrupt pins to be mirrored or for separate ports
	* @param value - 1 = The interrupt pins are internally connected, 0 = The interrupt pins are not connected. INTA is associated with PortA and INTB is associated with PortB
	*/
	if (value == 0)
	{
		config = updatebyte(config, 6, 0);
		write_byte_data(IOADDRESS, IOCON, config);
	}
	else if (value == 1)
	{
		config = updatebyte(config, 6, 1);
		write_byte_data(IOADDRESS, IOCON, config);
	}
	else
	{
		printf("mirror_interrupts value out of range: 0 or 1");
	}
}

void io_set_interrupt_polarity(uint8_t value)
{
	/**
	* This sets the polarity of the interrupt output pins.
	* @param value - 1 = Active-high, 0 = Active-low.
	*/
	if (value == 0)
	{
		config = updatebyte(config, 1, 0);
		write_byte_data(IOADDRESS, IOCON, config);
	}
	else if (value == 1)
	{
		config = updatebyte(config, 1, 1);
		write_byte_data(IOADDRESS, IOCON, config);
	}
	else
	{
		printf("set_interrupt_polarity value out of range: 0 or 1");
	}
}

uint8_t io_get_interrupt_polarity()
{
	/**
  	* Get the polarity of the interrupt output pins.
	* @returns 1 = Active-high, 0 = Active-low.
  	*/
  	return checkbit(read_byte_data(IOADDRESS, IOCON), 1);
}

void io_set_interrupt_type(uint8_t port, uint8_t value)
{
	/**
	* Sets the type of interrupt for each pin on the selected port
	* @param port - 0 = pins 1 to 8, port 1 = pins 9 to 16
	* @param value - 0 to 255 (0xFF). For each bit 1 = interrupt is fired when the pin matches the default value, 0 = the interrupt is fired on state change
	*/
	set_port(port, value, INTCONA, INTCONB);
}

uint8_t io_get_interrupt_type(uint8_t port)
{
	/**
	* Get the type of interrupt for each pin on the selected port
	* @param port - 0 = pins 1 to 8, port 1 = pins 9 to 16
	* @returns 0 to 255 (0xFF). For each bit 1 = interrupt is fired when the pin matches the default value, 0 = the interrupt is fired on state change
	*/
	return get_port(port, INTCONA, INTCONB);
}

void io_set_interrupt_defaults(uint8_t port, uint8_t value)
{
	/**
	* These bits set the compare value for pins configured for interrupt-on-change on the selected port.
	* If the associated pin level is the opposite of the register bit, an interrupt occurs.
	* @param port - 0 = pins 1 to 8, port 1 = pins 9 to 16
	* @param value - default state for the port. 0 to 255 (0xFF).
	*/
	set_port(port, value, DEFVALA, DEFVALB);
}

uint8_t io_get_interrupt_defaults(uint8_t port)
{
	/**
  	* Get the compare value for pins configured for interrupt-on-change on the selected port.
  	* If the associated pin level is the opposite of the register bit, an interrupt occurs.
	* @param port - 0 = pins 1 to 8, port 1 = pins 9 to 16
	* @returns default state for the port. 0 to 255 (0xFF).
  	*/
  	return get_port(port, DEFVALA, DEFVALB);
}

void io_set_interrupt_on_pin(uint8_t pin, uint8_t value)
{
	/**
	* Enable interrupts for the selected pin
	* @param pin - 1 to 16
	* @param value - 0 = interrupt disabled, 1 = interrupt enabled
	*/
	set_pin(pin, value, GPINTENA, GPINTENB);
}

 uint8_t io_get_interrupt_on_pin(uint8_t pin)
 {
	 /**
  	* Get the interrupt-enable status for the selected pin
  	* @param pin - 1 to 16
	* @returns 0 = interrupt disabled, 1 = interrupt enabled
  	*/
  	return get_pin(pin, GPINTENA, GPINTENB);
 }

void io_set_interrupt_on_port(uint8_t port, uint8_t value)
{
	/**
	* Enable interrupts for the pins on the selected port
	* @param port - 0 = pins 1 to 8, port 1 = pins 9 to 16
	* @param value - 0 to 255 (0xFF). For each bit 0 = interrupt disabled, 1 = interrupt enabled
	*/
	set_port(port, value, GPINTENA, GPINTENB);
}

uint8_t io_get_interrupt_on_port(uint8_t port)
{
	/**
  	* Get the interrupt-enable status for the selected port
  	* @param port - 0 = pins 1 to 8, port 1 = pins 9 to 16
	* @returns 0 to 255 (0xFF). For each bit 0 = interrupt disabled, 1 = interrupt enabled
  	*/
  	return get_port(port, GPINTENA, GPINTENB);
}

void io_set_interrupt_on_bus(uint16_t value)
{
	/**
	* Enable interrupts for the pins on the selected bus
	* @param value - 0 to 65535 (0xFFFF). For each bit 0 = interrupt disabled, 1 = interrupt enabled
	*/
	write_word_data(IOADDRESS, GPINTENA, value);
}

uint16_t io_get_interrupt_on_bus()
 {
	 /**
  	* Get the interrupt-enable status for the selected bus
	* @returns 0 to 65535 (0xFFFF). For each bit 0 = interrupt disabled, 1 = interrupt enabled
  	*/
  	return read_word_data(IOADDRESS, GPINTENA);
 }

uint8_t io_read_interrupt_status(uint8_t port)
{
	/**
	* Read the interrupt status for the pins on the selected port
	* @param port - 0 = pins 1 to 8, port 1 = pins 9 to 16
	* @returns - 0 to 255 (0xFF). For each bit 1 = interrupt triggered, 0 = interrupt not triggered
	*/
	return get_port(port, INTFA, INTFB);
}

uint8_t io_read_interrupt_capture(uint8_t port)
{
	/**
	* Read the value from the selected port at the time of the last interrupt trigger
	* @param port - 0 = pins 1 to 8, port 1 = pins 9 to 16
	* @returns - 0 to 255 (0xFF). For each bit 1 = interrupt triggered, 0 = interrupt not triggered
	*/
	return get_port(port, INTCAPA, INTCAPB);
}

void io_reset_interrupts()
{
	/**
	* Reset the interrupts A and B to 0
	*/
	io_read_interrupt_capture(0);
	io_read_interrupt_capture(1);
}

void io_init(uint8_t reset) {
	/**
	* Initialise the MCP32017 IO chip
	* @param reset - If set to 1 the IO Pi will reset to default values. Ports are inputs, pull-up resistors are disabled and ports are not inverted.
	*/
	write_byte_data(IOADDRESS, IOCON, config);
	if (reset == 1){
		write_word_data(IOADDRESS, IODIRA, 0xFFFF);
		write_word_data(IOADDRESS, GPPUA, 0x0000);
		write_word_data(IOADDRESS, IPOLA, 0x0000);
	}
}

/*===================IO Functions END ===================*/

/*===================RTC Functions Begin ===================*/


void rtc_set_date(struct tm date) {
	/**
	* Set the date on the RTC
	* @param date - struct tm formated date and time
	*/
	writebuffer[0] = SECONDS; // register address for seconds
	writebuffer[1] = dec_to_bcd(date.tm_sec); // seconds
	writebuffer[2] = dec_to_bcd(date.tm_min); // minutes
	writebuffer[3] = dec_to_bcd(date.tm_hour);// hours
	writebuffer[4] = dec_to_bcd(date.tm_wday);// dayofweek
	writebuffer[5] = dec_to_bcd(date.tm_mday);// day
	writebuffer[6] = dec_to_bcd(date.tm_mon) + 1;// month
	writebuffer[7] = dec_to_bcd(date.tm_year % 100); // strip the century from the date

	write_byte_array(RTCADDRESS, writebuffer, 8);
}

struct tm rtc_read_date() {
	/**
	* Read the date from the RTC.
	* @returns - date as a tm struct
	*/

	read_byte_array(RTCADDRESS, 0, 7);
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
	write_byte_data(RTCADDRESS, CONTROL, rtcConfig);
}

void rtc_disable_output() {
	/**
	* Disable the squarewave output pin
	*/
	rtcConfig = updatebyte(rtcConfig, 7, 0);
	rtcConfig = updatebyte(rtcConfig, 4, 0);
	write_byte_data(RTCADDRESS, CONTROL, rtcConfig);
}

void rtc_set_frequency(uint8_t frequency) {
	/**
	* Set the squarewave output frequency
	* @param frequency - 1 = 1Hz, 2 = 4.096KHz, 3 = 8.192KHz, 4 = 32.768KHz
	*/
	switch (frequency) {
	case 1:
		rtcConfig = updatebyte(rtcConfig, 0, 0);
		rtcConfig = updatebyte(rtcConfig, 1, 0);
		write_byte_data(RTCADDRESS, CONTROL, rtcConfig);
		break;
	case 2:
		rtcConfig = updatebyte(rtcConfig, 0, 1);
		rtcConfig = updatebyte(rtcConfig, 1, 0);
		write_byte_data(RTCADDRESS, CONTROL, rtcConfig);
		break;
	case 3:
		rtcConfig = updatebyte(rtcConfig, 0, 0);
		rtcConfig = updatebyte(rtcConfig, 1, 1);
		write_byte_data(RTCADDRESS, CONTROL, rtcConfig);
		break;
	case 4:
		rtcConfig = updatebyte(rtcConfig, 0, 1);
		rtcConfig = updatebyte(rtcConfig, 1, 1);
		write_byte_data(RTCADDRESS, CONTROL, rtcConfig);
		break;
	default:
		printf("Error: rtc_set_frequency() - value must be between 1 and 4");

	}
}

void rtc_write_memory(uint8_t address, uint8_t *valuearray) {
	/**
	* write to the memory on the DS1307.  The DS1307 contains a 56-byte, battery-backed RAM with unlimited writes
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

				write_byte_array(RTCADDRESS, writearray, (uint8_t)length + 1);

				free(writearray);
			}

		}
		else {
			printf("memory overflow error: address + length exceeds 0x3F");
		}
	}
	else {
		printf("address out of range");
	}
}

uint8_t *rtc_read_memory(uint8_t address, uint8_t length) {
	/**
	* Read from the memory on the DS1307
	* The DS1307 contains 56-Byte, battery-backed RAM with Unlimited Writes
	* @param address - 0x08 to 0x3F
	* @param length - up to 32 bytes.  length can not exceed the available address space.
	* @returns - pointer to a byte array where the data will be saved
	*/

	if (address >= 0x08 && address <= 0x3F) {
		if (address <= (0x3F - length)) {
			//write_i2c_block_data(_RTCADDRESS, address, valuearray);

			uint8_t *writearray = malloc(length);

			if (errno == ENOMEM) { // Fail!!!!
				free(writearray);
				printf("memory allocation error: not enough system memory to allocate array");
				return NULL;
			}
			else {
				read_byte_array(RTCADDRESS, address, length); // read the values from the SRAM into the read buffer

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

/*===================RTC Functions End ===================*/