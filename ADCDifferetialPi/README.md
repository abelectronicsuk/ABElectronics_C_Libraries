AB Electronics UK: ADC Differential Pi - C Library
=====

C Library to use with ADC Differential Pi Raspberry Pi expansion board from [AB Electronics UK](https://www.abelectronics.co.uk)

The example C programs can be found in /ABElectronics_C_Libraries/ADCDifferetialPi/demos

Downloading and Installing the library
----------

To download to your Raspberry Pi type in terminal:  

```bash
git clone https://github.com/abelectronicsuk/ABElectronics_C_Libraries.git
```

___  

Functions
----------

```c
read_raw(uint8_t address, uint8_t channel, uint8_t bitrate, uint8_t pga,	uint8_t conversionmode)
```

Reads the raw value from the selected ADC channel  
**Parameter:** (uint8_t) address - I2C address for the target device e.g. 0x68  
**Parameter:** (uint8_t) channel - 1 to 4  
**Parameter:** (uint8_t) bitrate - 12, 14, 16 or 18  
**Parameter:** (uint8_t) pga - 1, 2, 4 or 8  
**Parameter:** (uint8_t) conversionmode - 0 = one shot conversion, 1 = continuous conversion  
**Returns:** (uint32_t) value from ADC buffer  
___  

```c
read_voltage(uint8_t address, uint8_t channel, uint8_t bitrate, uint8_t pga, uint8_t conversionmode);
```

Reads the voltage from the selected ADC channel  
**Parameter:** (uint8_t) address - I2C address for the target device e.g. 0x68  
**Parameter:** (uint8_t) channel - 1 to 4  
**Parameter:** (uint8_t) bitrate - 12, 14, 16 or 18  
**Parameter:** (uint8_t) pga - 1, 2, 4 or 8  
**Parameter:** (uint8_t) conversionmode - 0 = one shot conversion, 1 = continuous conversion  
**Returns:** (double) voltage value from ADC
