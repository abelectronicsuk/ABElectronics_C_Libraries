/*
 ================================================
 AB Electronics UK I2C Switch
 See CHANGELOG.md for the version number
 ================================================


 The PCA9546A I2C switch contains 4 channels.  Each channel can be switched on or off independently.

 Required package: libi2c-dev
 sudo apt-get install libi2c-dev
 */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <time.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>

#define RESETPIN 27 // GPIO Reset Pin

// values used for I2C and GPIO communication
#define OUT 1
#define IN  0
#define LOW  0
#define HIGH 1
#define VALUE_MAX 30
#define BUFFER_MAX 3
#define DIRECTION_MAX 35

// local methods

static int i2cbus;
static const char *fileName = "/dev/i2c-1"; // change to /dev/i2c-0 if you are using a revision 0002 or 0003 model B
static uint8_t buf[10] = { 0 };

static uint8_t read_byte_data(uint8_t address) {

	if ((i2cbus = open(fileName, O_RDWR)) < 0) {
		printf("Failed to open I2C port for read %s \n", strerror(errno));
		exit(1);
	}

	if (ioctl(i2cbus, I2C_SLAVE, address) < 0) {
		printf("Failed to write to I2C port for read\n");
		exit(1);
	}

	if (read(i2cbus, buf, 1) != 1) { // Read back data into buf[]
		printf("Failed to read from slave\n");
		exit(1);
	}

	close(i2cbus);

	return (buf[0]);
}

static void write_byte_data(uint8_t address, uint8_t value) {
	if ((i2cbus = open(fileName, O_RDWR)) < 0) {
		printf("Failed to open I2C port for write\n");
		exit(1);
	}

	if (ioctl(i2cbus, I2C_SLAVE, address) < 0) {
		printf("Failed to write to I2C port for write\n");
		exit(1);
	}

	buf[0] = value;

	if ((write(i2cbus, buf, 1)) != 1) {
		printf("Failed to write to I2C device for write\n");
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

// public methods

char switch_channel(uint8_t address, uint8_t channel) {
	/**
	* Enable the specified I2C channel and disable other channels
	* @param address - I2C address for the target device. Default = 0x70
	* @param channel - 1 to 4
	* @returns - 0 = success, -1 = error setting state
	*/
	if (address < 0x70 || address > 0x77){
		fprintf(stderr, "I2C address out of range. 0x70 - 0x77\n");
		return(-1);
	}
	if (channel < 1 || channel > 4){
		fprintf(stderr, "channel out of range. 1 - 4\n");
		return(-1);
	}
	char cval = 0;
	cval = updatebyte(cval, channel - 1, 1);
	write_byte_data(address, cval);
	return(0);
}

char set_channel_state(uint8_t address, uint8_t channel, uint8_t state) {
	/**
	* Sets the state of the specified I2C channel.
	* All other channels keep their existing state.
	* @param address - I2C address for the target device. Default = 0x70
	* @param channel - 1 to 4
	* @param state - 0 or 1
	* @returns - 0 = success, -1 = error setting state
	*/
	if (address < 0x70 || address > 0x77){
		fprintf(stderr, "I2C address out of range. 0x70 - 0x77\n");
		return(-1);
	}
	if (channel < 1 || channel > 4){
		fprintf(stderr, "channel out of range. 1 - 4\n");
		return(-1);
	}
	if (state > 1){
		fprintf(stderr, "state out of range. 0 or 1\n");
		return(-1);
	}
	char cval = read_byte_data(address);
	cval = updatebyte(cval, channel -1, state);
	write_byte_data(address, cval);
	return(0);
}

char get_channel_state(uint8_t address, uint8_t channel) {
	/**
	* Gets the state of the specified I2C channel
	* @param address - I2C address for the target device. Default = 0x70
	* @param channel - 1 to 4
	* @returns - 1 = on, 0 = off, -1 = error getting state
	*/
	if (address < 0x70 || address > 0x77){
		fprintf(stderr, "I2C address out of range. 0x70 - 0x77\n");
		return(-1);
	}
	if (channel < 1 || channel > 4){
		fprintf(stderr, "channel out of range. 1 - 4\n");
		return(-1);
	}
	char cval = read_byte_data(address);
	return (checkbit(cval, channel - 1));
}

char reset(){
	/**
	* Reset the I2C switch.  All channels are set to off.
	* @returns - 0 = reset complete, -1 = reset failed
	*/
	char failed = 0;

	//Enable GPIO pins

		if (-1 == gpio_export(RESETPIN))
			failed = 1;

		/*
		 * Set GPIO directions
		 */
		if (-1 == gpio_direction(RESETPIN, OUT))
			failed = 1;
	
	// set reset pin low
	if (-1 == gpio_write(RESETPIN, 0)) {
		failed = 1;	
	}
	
	// wait 1ms before setting reset pin high
	usleep(1000);  

	if (-1 == gpio_write(RESETPIN, 1)) {
		failed = 1;	
	}

	// wait 1ms for the switch to reset
	usleep(1000);  

	if (failed == 1){
		fprintf(stderr, "Failed to reset switch. GPIO error!\n");
		return(-1);
	}
	else{
		return (0);
	}
}


