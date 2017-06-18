/*
================================================
ABElectronics UK ADC Servo Pi 16 Channel PWM controller
Version 1.0 Created 11/01/2016
================================================

Required package{
apt-get install libi2c-dev
*/


/**
* Initialise the Servo Pi - run before calling any other methods
* @param address - I2C address
* @param use_oe_pin - Enable the Output Enable pin on the GPIO port
* @returns - 0 = OK, 1 = GPIO enable failed, 2 = GPIO direction set failed
*/
int servopi_init(char address, char use_oe_pin);


/**
* Set the PWM frequency
* @param freq - required frequency
* @param address - I2C address
*/
void set_pwm_freq(double freq, char address);

/**
* Set the output on single channels
* @param channel - 1 to 16
* @param on - time period 0 to 4096
* @param off - time period 0 to 4096
* @param address - I2C address
*/
void set_pwm(char channel, int on, int off, char address);

/**
* Set the output on all channels
* @param on - time period 0 to 4096
* @param off - time period 0 to 4096
* @param address - I2C address
*/
void set_all_pwm(int on, int off, char address);

/**
* Disable the output via OE pin
* @returns - 0 = OK, 1 = GPIO failed
*/
int output_disable();

/**
* Enable the output via OE pin
* @returns - 0 = OK, 1 = GPIO failed
*/
int output_enable();

/**
* Set the I2C address for the All Call function
* @param allcalladdress - I2C address
*/
void set_allcall_address(char allcalladdress, char address);

/**
* Enable the I2C address for the All Call function
* @param address - I2C address
*/
void enable_allcall_address(char address);

/**
* Disable the I2C address for the All Call function
* @param address - I2C address
*/
void disable_allcall_address(char address);

