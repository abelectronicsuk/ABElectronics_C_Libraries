/*
 * ABE_IoPi.h
 *
 *  Created on: 27 Mar 2016
 */



/* Initialise the Servo Pi - run before calling any other methods
 * Parameters:
 * address - I2C address
 * use_oe_pin - Enable the Output Enable pin on the GPIO port
 *
 * Returns
 * 0 = OK
 * 1 = GPIO enable failed
 * 2 = GPIO direction set failed
*/
int servopi_init(char address, char use_oe_pin);


/*
* Set the PWM frequency
* Parameters:
* freq - required frequency
* address - I2C address
 */
void set_pwm_freq(double freq, char address);

/*
* Set the output on single channels
* Parameters:
* channel - 1 to 16
* on - time period
* off - time period
* address - I2C address
*/
void set_pwm(char channel, int on, int off, char address);

/*
* Set the output on all channels
* Parameters:
* on - time period
* off - time period
* address - I2C address
*/
void set_all_pwm(int on, int off, char address);

/*
 * Disable the output via OE pin
 * returns
 * 0 = OK
 * 1 = GPIO failed
 */
int output_disable();

/*
 * Enable the output via OE pin
 * returns
 * 0 = OK
 * 1 = GPIO failed
 */
int output_enable();

/*
 * Set the I2C address for the All Call function
 * Parameters:
 * address - I2C address
 */
void set_allcall_address(char allcalladdress, char address);

/*
 * Enable the I2C address for the All Call function
 * Parameters:
 * address - I2C address
 */
void enable_allcall_address(char address);

/*
 * Disable the I2C address for the All Call function
 * Parameters:
 * address - I2C address
 */
void disable_allcall_address(char address);

