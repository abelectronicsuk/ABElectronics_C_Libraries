/*
 * ABE_ADCDACPI.h
 *
 *  Created on: 12 Jan 2016
 *      Author: andrew
 */

#ifndef ADCDACPI_ABE_ADCDACPI_H_
#define ADCDACPI_ABE_ADCDACPI_H_



#endif /* ADCDACPI_ABE_ADCDACPI_H_ */


int open_adc();

void close_adc();

int open_dac();

void close_dac();

double read_adc_voltage(int channel, int mode);

double read_adc_raw(int channel, int mode);

void set_adc_refvoltage(double ref);

void set_dac_voltage(double voltage, int channel);

void set_dac_raw(uint16_t raw, int channel);

void set_dac_gain(int gain);

