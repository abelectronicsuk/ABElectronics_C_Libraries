/*

Readout abelectronics ADC Pi board inputs via C


Copyright (C) 2012 Stephan Callsen

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
associated documentation files (the "Software"), to deal in the Software without restriction,
including without limitation the rights to use, copy, modify, merge, publish, distribute,
sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial
portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT
LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

Autor:	Stephan Callsen
Date:	Oct. 24nd 2012
Version:	1.3


	Required package:
	apt-get install libi2c-dev

	Compile with gcc:
	gcc adcpi.c -o adcpi

	Execute with:
	./adcpi [channel]
	default is channel 1



Taken from Microchip Datasheet:
bit 7 RDY: Ready Bit
	This bit is the data ready flag. In read mode, this bit indicates if the output register has been updated
	with a latest conversion result. In One-Shot Conversion mode, writing this bit to “1” initiates a new
	conversion.

Reading RDY bit with the read command:
	1 = Output register has not been updated.
	0 = Output register has been updated with the latest conversion result.

Writing RDY bit with the write command:
Continuous Conversion mode: No effect
One-Shot Conversion mode:
	1 = Initiate a new conversion.
	0 = No effect.

bit 6-5 C1-C0: Channel Selection Bits
	00 = Select Channel 1 (Default)
	01 = Select Channel 2
	10 = Select Channel 3 (MCP3428 only, treated as “00” by the MCP3426/MCP3427)
	11 = Select Channel 4 (MCP3428 only, treated as “01” by the MCP3426/MCP3427)

bit 4 O/C: Conversion Mode Bit
	1 = Continuous Conversion Mode (Default). The device performs data conversions continuously.
	0 = One-Shot Conversion Mode. The device performs a single conversion and enters a low power
	standby mode until it receives another write or read command.

bit 3-2 S1-S0: Sample Rate Selection Bit
	00 = 240 SPS (12 bits) (Default)
	01 = 60 SPS (14 bits)
	10 = 15 SPS (16 bits)

bit 1-0 G1-G0: PGA Gain Selection Bits
	00 = x1 (Default)
	01 = x2
	10 = x4
	11 = x8

Mostly all functions from i2c-dev.h
    i2c_smbus_write_quick( int file, __u8 value)
    i2c_smbus_read_byte(int file)
    i2c_smbus_write_byte(int file, __u8 value)
    i2c_smbus_read_byte_data(int file, __u8 command)
    i2c_smbus_write_byte_data(int file, __u8 command, __u8 value)
    i2c_smbus_read_word_data(int file, __u8 command)
    i2c_smbus_write_word_data(int file, __u8 command, __u16 value)
    i2c_smbus_process_call(int file, __u8 command, __u16 value)
    i2c_smbus_read_block_data(int file, __u8 command, __u8 *values)
    i2c_smbus_write_block_data(int file, __u8 command, __u8 length, __u8 *values)
    i2c_smbus_read_i2c_block_data(int file, __u8 command, __u8 *values)
    i2c_smbus_write_i2c_block_data(int file, __u8 command, __u8 length, __u8 *values)
    i2c_smbus_block_process_call(int file, __u8 command, __u8 length, __u8 *values)
*/

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <linux/i2c-dev.h>

#define ADC_1 		0x68
#define ADC_2 		0x69
#define ADC_CHANNEL1	0x98
#define ADC_CHANNEL2	0xB8
#define ADC_CHANNEL3 	0xD8
#define ADC_CHANNEL4	0xF8

//Prototypes
float getadc (int chn);

//DEMO MAIN
int main(int argc, char **argv) {
  int i;
  float val;
  int channel;
  if (argc>1) channel=atoi(argv[1]);
  if (channel <1|channel>8) channel=1;
  for (i=0;i<500;i++){
    val = getadc (channel);
    printf ("Channel: %d  - %2.4fV\n",channel,val);
    sleep (0.5);
  }
  return 0;
}

float getadc (int chn){
  unsigned int fh,dummy, adc, adc_channel;
  float val;
  __u8  res[4];
  switch (chn){
    case 1: { adc=ADC_1; adc_channel=ADC_CHANNEL1; }; break;
    case 2: { adc=ADC_1; adc_channel=ADC_CHANNEL2; }; break;
    case 3: { adc=ADC_1; adc_channel=ADC_CHANNEL3; }; break;
    case 4: { adc=ADC_1; adc_channel=ADC_CHANNEL4; }; break;
    case 5: { adc=ADC_2; adc_channel=ADC_CHANNEL1; }; break;
    case 6: { adc=ADC_2; adc_channel=ADC_CHANNEL2; }; break;
    case 7: { adc=ADC_2; adc_channel=ADC_CHANNEL3; }; break;
    case 8: { adc=ADC_2; adc_channel=ADC_CHANNEL4; }; break;
    default: { adc=ADC_1; adc_channel=ADC_CHANNEL1; }; break;
  }
  fh = open("/dev/i2c-1", O_RDWR);
  ioctl(fh,I2C_SLAVE,adc);
  i2c_smbus_write_byte (fh, adc_channel);
  usleep (50000);
  i2c_smbus_read_i2c_block_data(fh,adc_channel,4,res);
  usleep(50000);
  close (fh);
  dummy = (res[0]<<8|res[1]);
  if (dummy>=32768) dummy=65536-dummy;
  val = dummy * 0.000154;
  return val;
}
