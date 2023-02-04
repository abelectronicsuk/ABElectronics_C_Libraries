
# AB Electronics UK: C Libraries - Change Log

## 2023-02-04 Andrew Dorey  

* 1.3.3 Updates to IO Pi and Expander Pi documentation.

## 2022-04-23 Andrew Dorey  

* 1.3.2 Updated spelling and grammar in the Readme files and code comments.

## 2022-04-23 Andrew Dorey  

* 1.3.1 Added library for the new IO Zero 32 expansion board.

## 2021-02-25 Andrew Dorey  

* 1.3.0 Major Update.
* Moved demonstration files into demo sub folders.
* Created README.md for each library
* Added unit tests for ADC DAC Pi and IO Pi.
* Changed data type for functions to use fixed size unsigned types, uint8_t, uint16_t and uint32_t instead of char, int and long.  This should improve compatibility across processors and compilers which use different lengths for data types.
* **Note:** If you get a compiler error *"unknown type name ‘uint8_t’"* add **`#include <stdint.h>`** to your program.
* ADCDAC Pi - updated close_adc() and close_dac() from void to int.  Both functions will now return -1 if the SPI bus fails to close.
* IO Pi - refactored code to reduce repetition and added new options for setting and getting values for the whole bus.
* IO Pi - added reset parameter to IOPi_init to reset device to default values.  The IOPi_init previously reset the device automatically so with the reset parameter set to 0 you can now initialise the IO Pi without setting the ports as inputs.
* Expander Pi - refactored code to reduce repetition and added new options for setting and getting values for the whole IO bus.
* Expander Pi - added reset parameter to io_init to reset device to default values.  The io_init previously reset the device automatically so with the reset parameter set to 0 you can now initialise the IO without setting the ports as inputs.

## 2020-09-08 Andrew Dorey  

* 1.2.9 Updated SPI bus speeds for ADC and DAC and added speed test demo for the ADC.

## 2020-09-05 Andrew Dorey  

* 1.2.8 Update demo-adcread.c

## 2020-06-04 Andrew Dorey  

* 1.2.7 Create CODE_OF_CONDUCT.md

## 2020-04-22 Andrew Dorey  

* 1.2.6 Fixed bugs in the libraries and demo files for memory leaks and buffer overflows.

## 2020-03-27 Andrew Dorey  

* 1.2.5 Added new interrupt demo.

## 2020-02-27 Andrew Dorey  

* 1.2.4 Update README.md

## 2019-11-28 Andrew Dorey  

* 1.2.3 Added new I2C Switch library.

## 2018-09-24 Andrew Dorey  

* 1.2.2 A new demo to test the sample rate of the Expander Pi ADC. Minor comment fix.  

## 2018-04-24 Andrew Dorey  

* 1.2.1 Updated link for ADC Pi.

## 2018-03-07 Andrew Dorey  

* 1.2.0 Fixed several missing headers in the IO Pi library and demo files. Fixed several bugs with missing headers and functions in the wrong order.

## 2017-06-27 Andrew Dorey  

* 1.1.9 Added Expander Pi link.  Minor Bug fixes.

## 2017-06-26 Andrew Dorey  

* 1.1.8 A new library for our Expander Pi

## 2017-06-23 Andrew Dorey  

* 1.1.7 Fixed a bug with the DAC raw conversion and several errors in the
comments.  Fixed a bug with the speed test demo.

## 2017-06-18 Andrew Dorey  

* 1.1.6 Standardised the comments across all of the libraries.  Removed
unnecessary includes.  Integrated the ABE_helpers functions into the IO
Pi library to reduce the number of files to be linked during compiling.

## 2017-06-15 Andrew Dorey  

* 1.1.5 A new library for the RTC Pi and a few small performance improvements for the ADC Pi and ADC Differential Pi.

## 2016-06-26 Andrew Dorey  

* 1.1.4 Fixed a bug with the read_adc_raw() function.

## 2016-06-25 Andrew Dorey  

* 1.1.3 Added the ability to select differential mode when reading from the ADC inputs.

## 2016-03-27 Andrew Dorey  

* 1.1.2 A new library for the Servo PWM Pi and Servo PWM Pi Zero.

## 2016-02-27 Andrew Dorey  

* 1.1.1 Fixed a problem that stopped the IO Pi library from working properly
when more than 1 IO Pi was connected to the Raspberry Pi.

## 2016-02-10 Andrew Dorey  

* 1.1.0 A new demonstration file to test the sample rate of the ADC Pi on a
Raspberry Pi. Fixed a bug where the ADC was returning the wrong voltage.

## 2016-02-03 iri  

* 1.0.9 Update demo-dacwrite.c

## 2016-01-22 Andrew Dorey  

* 1.0.8 A new library for the ADCDAC Pi.

## 2016-01-12 Andrew Dorey  

* 1.0.7 A new library for the ADC Differential Pi.  Also made some optimisations to the ADC Pi library to reduce code size and improve speed.  Updated Readme.

## 2016-01-09 Andrew Dorey  

* 1.0.6 Added new ADC Pi library and removed old demo code.  Removed ABE_Helper library which is not needed in the ADC Pi library.

## 2015-09-01 Andrew Dorey  

* 1.0.5 Removed the old ADC Pi demo and replaced it with a new version from Stephan Callsen  

## 2015-07-07 Andrew Dorey  

* 1.0.4 Added a demonstration to show how to use a push button to toggle the output of an led.  

## 2015-05-14 Andrew Dorey  

* 1.0.3 Added a new library for the IO Pi  

## 2015-04-20 Andrew Dorey  

* 1.0.2 Fixed spelling mistake  

## 2014-05-17 Andrew Dorey  

* 1.0.1 Updates to readme and new code files  
* 1.0.0 Upload of initial code  
