AB Electronics UK: Servo PWM Pi - C Library
=====

C Library to use with Servo PWM Pi Raspberry Pi expansion board from [AB Electronics UK](https://www.abelectronics.co.uk)

The example C programs can be found in /ABElectronics_C_Libraries/ServoPi/demos

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
servopi_init(uint8_t address, uint8_t use_oe_pin)
```

Initialise the Servo Pi - run before calling any other methods  
**Parameter:** (uint8_t) address - I2C address  
**Parameter:** (uint8_t) use_oe_pin - Enable the Output Enable pin on the GPIO port  
**Returns:** (int) 0 = OK, 1 = GPIO enable failed, 2 = GPIO direction set failed 
___  

```c
set_pwm_freq(double freq, uint8_t address)
```

Set the PWM frequency  
**Parameter:** (double) freq - required frequency  
**Parameter:** (uint8_t) address - I2C address  
**Returns:** null  
___  

```c
set_pwm(uint8_t channel, uint16_t on, uint16_t off, uint8_t address)
```

Set the output on single channels  
**Parameter:** (uint8_t) channel - 1 to 16  
**Parameter:** (uint16_t) on - time period 0 to 4096  
**Parameter:** (uint16_t) off - time period 0 to 4096  
**Parameter:** (uint8_t) address - I2C address  
**Returns:** null  
___  

```c
set_all_pwm(uint16_t on, uint16_t off, uint8_t address)
```

Set the output on all channels  
**Parameter:** (uint16_t) on - time period 0 to 4096  
**Parameter:** (uint16_t) off - time period 0 to 4096  
**Parameter:** (uint8_t) address - I2C address  
**Returns:** null  
___  

```c
output_disable()
```

Disable the output via OE pin  
**Returns:** (int) 0 = OK, 1 = GPIO failed  
___  

```c
output_enable()
```

Enable the output via OE pin  
**Returns:** (int) 0 = OK, 1 = GPIO failed  
___  

```c
set_allcall_address(uint8_t allcalladdress, uint8_t address)
```

Set the I2C address for the All Call function    
**Parameter:** (uint8_t) allcalladdress - New I2C All Call address  
**Parameter:** (uint8_t) address - I2C address  
**Returns:** null  
___  

```c
enable_allcall_address(uint8_t address)
```

Enable the I2C address for the All Call function  
**Parameter:** (uint8_t) address - I2C address  
**Returns:** null  
___  

```c
disable_allcall_address(uint8_t address)
```

Disable the I2C address for the All Call function  
**Parameter:** (uint8_t) address - I2C address  
**Returns:** null  
___
