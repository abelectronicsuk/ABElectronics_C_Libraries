/*
================================================
ABElectronics UK RTC Pi real-time clock
Version 1.0 Created 12/06/2017
================================================

Required package{
apt-get install libi2c-dev
*/

/**
* Set the date on the RTC
* @param - date - struct tm formated date and time
*/
void rtc_set_date(struct tm date);


/**
* Read the date from the RTC.
* @returns - date as a tm struct
*/
struct tm rtc_read_date();


/**
* Enable the squarewave output pin
*/
void rtc_enable_output();


/**
* Disable the squarewave output pin
*/
void rtc_disable_output();


/**
* Set the squarewave output frequency
* @param - 1 = 1Hz, 2 = 4.096KHz, 3 = 8.192KHz, 4 = 32.768KHz
*/
void rtc_set_frequency(unsigned char frequency);


/**
* write to the memory on the ds1307
* the ds1307 contains 56 - Byte, battery - backed RAM with Unlimited Writes
* @param address - 0x08 to 0x3F
* @param valuearray - byte array containing data to be written to memory
*/
void rtc_write_memory(unsigned char address, unsigned char valuearray[]);

/**
* read from the memory on the ds1307
* the ds1307 contains 56-Byte, battery-backed RAM with Unlimited Writes
* @param address - 0x08 to 0x3F
* @param length - up to 32 bytes.  length can not exceed the avaiable address space.
* @returns - pointer to a byte array where the data will be saved
*/
unsigned char *rtc_read_memory(unsigned char address, int length);