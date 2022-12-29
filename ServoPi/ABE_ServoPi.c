/*
 ================================================
 AB Electronics UK Servo Pi 16-Channel PWM Servo Controller
 See CHANGELOG.md for the version number
 ================================================


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
#include <math.h>

// Define registers values from the datasheet
#define MODE1 0x00
#define MODE2 0x01
#define SUBADR1 0x02
#define SUBADR2 0x03
#define SUBADR3 0x04
#define ALLCALLADR 0x05
#define LED0_ON_L 0x06
#define LED0_ON_H 0x07
#define LED0_OFF_L 0x08
#define LED0_OFF_H 0x09
#define ALL_LED_ON_L 0xFA
#define ALL_LED_ON_H 0xFB
#define ALL_LED_OFF_L 0xFC
#define ALL_LED_OFF_H 0xFD
#define PRE_SCALE 0xFE

// Values used for I2C and GPIO communication
#define OUT 1
#define IN  0
#define LOW  0
#define HIGH 1
#define VALUE_MAX 30
#define BUFFER_MAX 3
#define DIRECTION_MAX 35

static int i2cbus;
static const char *fileName = "/dev/i2c-1"; // Change to /dev/i2c-0 if you are using a revision 0002 or 0003 model B
static uint8_t buf[10] = { 0 };

// Output enable pin.  Change this if you connect the GPIO pin to something other than 4
#define ENABLE_PIN 4

// Private methods

static int read_byte_data(uint8_t address, uint8_t reg) {

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

static void write_byte_data(uint8_t address, uint8_t reg, uint8_t value) {
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

static int gpio_export(int pin) {

	char buffer[BUFFER_MAX];
	ssize_t bytes_written;
	int fd;

	fd = open("/sys/class/gpio/export", O_WRONLY);
	if (-1 == fd) {
		fprintf(stderr, "Failed to open export for writing!\n");
		return(-1);
	}

	bytes_written = snprintf(buffer, BUFFER_MAX, "%d", pin);
	write(fd, buffer, bytes_written);
	close(fd);
	return(0);
}

static int gpio_direction(int pin, int dir) {
	static const char s_directions_str[]  = "in\0out";
	char path[DIRECTION_MAX];
	int fd;

	snprintf(path, DIRECTION_MAX, "/sys/class/gpio/gpio%d/direction", pin);
	fd = open(path, O_WRONLY);
	if (-1 == fd) {
		fprintf(stderr, "Failed to open gpio direction for writing!\n");
		return(-1);
	}

	if (-1 == write(fd, &s_directions_str[IN == dir ? 0 : 3], IN == dir ? 2 : 3)) {
		fprintf(stderr, "Failed to set direction!\n");
		return(-1);
	}

	close(fd);
	return(0);
}

static int gpio_write(int pin, int value) {
	static const char s_values_str[] = "01";

	char path[VALUE_MAX];
	int fd;

	snprintf(path, VALUE_MAX, "/sys/class/gpio/gpio%d/value", pin);
	fd = open(path, O_WRONLY);
	if (-1 == fd) {
		fprintf(stderr, "Failed to open gpio value for writing!\n");
		return(-1);
	}

	if (1 != write(fd, &s_values_str[LOW == value ? 0 : 1], 1)) {
		fprintf(stderr, "Failed to write value!\n");
		return(-1);
	}

	close(fd);
	return(0);
}

// Public methods

int servopi_init(uint8_t address, uint8_t use_oe_pin) {
	/**
	* Initialise the Servo Pi - run before calling any other methods
	* @param address - I2C address
	* @param use_oe_pin - Enable the Output Enable pin on the GPIO port
	* @returns - 0 = OK, 1 = GPIO enable failed, 2 = GPIO direction set failed
	*/
	write_byte_data(address, MODE1, 0x00);

	// set the GPIO pin as an output for the Output Enable Pin if use_oe_pin = 1

	if (use_oe_pin == 1){
		//Enable GPIO pins

		if (-1 == gpio_export(ENABLE_PIN))
			return (1);

		/*
		 * Set GPIO directions
		 */
		if (-1 == gpio_direction(ENABLE_PIN, OUT))
			return (2);
	}

	return (0);

}

void set_pwm_freq(double freq, uint8_t address) {
	/**
	* Set the PWM frequency
	* @param freq - required frequency
	* @param address - I2C address
	*/

	double scaleval = 25000000.0; // 25MHz
	scaleval /= 4096.0; // 12-bit
	scaleval /= freq;
	scaleval -= 1.0;
	uint8_t prescale = (uint8_t) floor(scaleval + 0.5);
	uint8_t oldmode = read_byte_data(address, MODE1);
	uint8_t newmode = (oldmode & 0x7F) | 0x10;

	write_byte_data(address, MODE1, newmode);
	write_byte_data(address, PRE_SCALE, prescale);
	write_byte_data(address, MODE1, oldmode);
	//usleep(5000); // sleep 5 milliseconds
	write_byte_data(address, MODE1, oldmode | 0x80);

}

void set_pwm(uint8_t channel, uint16_t on, uint16_t off, uint8_t address) {
	/**
	* Set the output on single channels
	* @param channel - 1 to 16
	* @param on - period 0 to 4096
	* @param off - period 0 to 4096
	* @param address - I2C address
	*/
	channel -= 1;
	write_byte_data(address, LED0_ON_L + 4 * channel, on & 0xFF);
	write_byte_data(address, LED0_ON_H + 4 * channel, on >> 8);
	write_byte_data(address, LED0_OFF_L + 4 * channel, off & 0xFF);
	write_byte_data(address, LED0_OFF_H + 4 * channel, off >> 8);

}

void set_all_pwm(uint16_t on, uint16_t off, uint8_t address) {
	/**
	* Set the output on all channels
	* @param on - period 0 to 4096
	* @param off - period 0 to 4096
	* @param address - I2C address
	*/

	write_byte_data(address, ALL_LED_ON_L, on & 0xFF);
	write_byte_data(address, ALL_LED_ON_H, on >> 8);
	write_byte_data(address, ALL_LED_OFF_L, off & 0xFF);
	write_byte_data(address, ALL_LED_OFF_H, off >> 8);

}

int output_disable() {
	/**
	* Disable the output via the OE pin
	* @returns - 0 = OK, 1 = GPIO failed
	*/
	if (-1 == gpio_write(ENABLE_PIN, 1)) {
		return (1);
	}
	return (0);
}

int output_enable() {
	/**
	* Enable the output via the OE pin
	* @returns - 0 = OK, 1 = GPIO failed
	*/
	if (-1 == gpio_write(ENABLE_PIN, 0)) {
		return (1);
	}
	return (0);

}

void set_allcall_address(uint8_t allcalladdress, uint8_t address) {
	/**
	* Set the I2C address for the All Call function
	* @param allcalladdress - New I2C All Call address
	* @param address - I2C address
	*/
	uint8_t oldmode = read_byte_data(address, MODE1);
	uint8_t newmode = oldmode | (1 << 0);
	write_byte_data(address, MODE1, newmode);
	write_byte_data(address, ALLCALLADR, allcalladdress << 1);

}

void enable_allcall_address(uint8_t address) {
	/**
	* Enable the I2C address for the All Call function
	* @param address - I2C address
	*/
	uint8_t oldmode = read_byte_data(address, MODE1);
	uint8_t newmode = oldmode | (1 << 0);
	write_byte_data(address, MODE1, newmode);

}

void disable_allcall_address(uint8_t address) {
	/**
	* Disable the I2C address for the All Call function
	* @param address - I2C address
	*/
	uint8_t oldmode = read_byte_data(address, MODE1);
	uint8_t newmode = oldmode & ~(1 << 0);
	write_byte_data(address, MODE1, newmode);
}

