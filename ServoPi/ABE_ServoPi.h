/*
 ================================================
 ABElectronics UK Servo Pi 16-Channel PWM Servo Controller
 See CHANGELOG.md for version number
 ================================================


 Required package: libi2c-dev
 apt-get install libi2c-dev
 */


/**
* Initialise the Servo Pi - run before calling any other methods
* @param address - I2C address
* @param use_oe_pin - Enable the Output Enable pin on the GPIO port
* @returns - 0 = OK, 1 = GPIO enable failed, 2 = GPIO direction set failed
*/
int servopi_init(uint8_t address, uint8_t use_oe_pin);


/**
* Set the PWM frequency
* @param freq - required frequency
* @param address - I2C address
*/
void set_pwm_freq(double freq, uint8_t address);

/**
* Set the output on single channels
* @param channel - 1 to 16
* @param on - time period 0 to 4096
* @param off - time period 0 to 4096
* @param address - I2C address
*/
void set_pwm(uint8_t channel, uint16_t on, uint16_t off, uint8_t address);

/**
* Set the output on all channels
* @param on - time period 0 to 4096
* @param off - time period 0 to 4096
* @param address - I2C address
*/
void set_all_pwm(uint16_t on, uint16_t off, uint8_t address);

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
* @param allcalladdress - New I2C All Call address
* @param address - I2C address
*/
void set_allcall_address(uint8_t allcalladdress, uint8_t address);

/**
* Enable the I2C address for the All Call function
* @param address - I2C address
*/
void enable_allcall_address(uint8_t address);

/**
* Disable the I2C address for the All Call function
* @param address - I2C address
*/
void disable_allcall_address(uint8_t address);

