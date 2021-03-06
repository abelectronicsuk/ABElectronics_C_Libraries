AB Electronics UK: Expander Pi - C Library
=====

C Library to use with Expander Pi Raspberry Pi expansion board from [AB Electronics UK](https://www.abelectronics.co.uk)

The example C programs can be found in /ABElectronics_C_Libraries/ExpanderPi/demos

Downloading and Installing the library
----------

To download to your Raspberry Pi type in terminal:  

```bash
git clone https://github.com/abelectronicsuk/ABElectronics_C_Libraries.git
```

___  

ADC Functions
----------

```c
adc_open()
```

Open the ADC SPI bus channel  
This needs to be called before using the ADC  
**Returns:** (int) 1 = open successful, 0 = SPI fails to open.  
___  

```c
adc_close()
```

Close the ADC SPI bus channel  
**Returns:** null  
___  

```c
adc_read_raw(uint8_t channel, uint8_t mode)
```

Read the raw value from the ADC  
**Parameter:** (uint8_t) channel -  1 to 8  
**Parameter:** (uint8_t) mode - 0 = Single Ended or 1 = Differential  
When in differential mode setting channel to 1 will make IN1 = IN+ and IN2 = IN-  
When in differential mode setting channel to 2 will make IN1 = IN- and IN2 = IN+  
**Returns:** (uint16_t) 12 bit value between 0 and 4096  
___  

```c
adc_read_voltage(uint8_t channel, uint8_t mode)
```

Read the voltage from the ADC  
**Parameter:** (uint8_t) channel -  1 to 8  
**Parameter:** (uint8_t) mode - 0 = Single Ended or 1 = Differential  
When in differential mode setting channel to 1 will make IN1 = IN+ and IN2 = IN-  
When in differential mode setting channel to 2 will make IN1 = IN- and IN2 = IN+  
**Returns:** (double) between 0V and the reference voltage  
___  

```c
adc_set_refvoltage(double ref)
```

Set the reference voltage for the ADC  
**Parameter:** (double) ref - Set this value to be the same as the voltage measured on the Vref pin on the Expander Pi  
If using the on board voltage reference then the value will be 4.096  
**Returns:** null  
___  

DAC Functions
----------

```c
dac_open()
```

Open the DAC SPI bus channel  
This needs to be called before using the DAC  
**Returns:** (int) 1 = open successful, 0 = SPI fails to open.  
___  

```c
dac_close()
```

Close the DAC SPI bus channel  
**Returns:** null  
___  

```c
dac_set_raw(uint16_t raw, uint8_t channel, uint8_t gain)
```

Set the raw value from the selected channel on the DAC  
**Parameter:** (uint16_t) raw - between 0 and 4095  
**Parameter:** (uint8_t) channel - 1 or 2  
**Parameter:** (uint8_t) gain - 1 or 2  - The output voltage will be between 0 and 2.048V when gain is set to 1,  0 and 4.096V when gain is set to 2  
**Returns:** null  
___  

```c
dac_set_voltage(double voltage, uint8_t channel, uint8_t gain)
```

Set the DAC voltage  
**Parameter:** (double) voltage - between 0 and 2.048 when gain is set to 1,  0 and 4.096 when gain is set to 2  
**Parameter:** (uint8_t) channel - 1 or 2  
**Parameter:** (uint8_t) gain - 1 or 2  
**Returns:** null  
___  

IO Functions
----------

```c
io_set_pin_direction(uint8_t pin, uint8_t direction)
```

Set IO direction for an individual pin  
**Parameter:** (uint8_t) pins - 1 to 16  
**Parameter:** (uint8_t) direction - 1 = input, 0 = output  
**Returns:** null  
___  

```c
io_get_pin_direction(uint8_t pin)
```

Get IO direction for an individual pin  
**Parameter:** (uint8_t) pins - 1 to 16  
**Returns:** (uint8_t) 1 = Input, 0 = Output  
___  

```c
io_set_port_direction(uint8_t port, uint8_t direction)
```

Set direction for an IO port  
**Parameter:** (uint8_t) port - 0 = pins 1 to 8, port 1 = pins 9 to 16  
**Parameter:** (uint8_t) direction - 0 to 255 (0xFF).  For each bit 1 = input, 0 = output  
**Returns:** null  
___  

```c
io_get_port_direction(uint8_t port)
```

Get direction for an IO port  
**Parameter:** (uint8_t) port - 0 = pins 1 to 8, port 1 = pins 9 to 16  
**Returns:** (uint8_t) 0 to 255 (0xFF).  For each bit 1 = input, 0 = output  
___  

```c
io_set_bus_direction(uint16_t direction)
```

Set direction for the IO bus  
**Parameter:** (uint16_t) direction - 0 to 65535 (0xFFFF).  For each bit 1 = input, 0 = output  
**Returns:** null  
___  

```c
io_get_bus_direction()
```

Get direction for the IO bus  
**Returns:** (uint16_t) 0 to 65535 (0xFFFF).  For each bit 1 = input, 0 = output  
___  

```c
io_set_pin_pullup(uint8_t pin, uint8_t value)
```

Set the internal 100K pull-up resistors for an individual pin  
**Parameter:** (uint8_t) pin - 1 to 16  
**Parameter:** (uint8_t) value - 1 = enabled, 0 = disabled  
**Returns:** null  
___  

```c
io_get_pin_pullup(uint8_t pin)
```

Get the internal 100K pull-up resistors for an individual pin  
**Parameter:** (uint8_t) pin - 1 to 16  
**Returns:** (uint8_t) 1 = enabled, 0 = disabled  
___  

```c
io_set_port_pullups(uint8_t port, uint8_t value)
```

Set the internal 100K pull-up resistors for the selected IO port  
**Parameter:** (uint8_t) port - 0 = pins 1 to 8, port 1 = pins 9 to 16  
**Parameter:** (uint8_t) value - 0 to 255 (0xFF). For each bit 1 = enabled, 0 = disabled  
**Returns:** null  
___  

```c
io_get_port_pullups(uint8_t port)
```

get the internal 100K pull-up resistors for the selected IO port  
**Parameter:** (uint8_t) port - 0 = pins 1 to 8, port 1 = pins 9 to 16  
**Returns:** (uint8_t) 0 to 255 (0xFF). For each bit 1 = enabled, 0 = disabled  
___  

```c
io_set_bus_pullups(uint16_t value)
```

Set internal 100K pull-up resistors for the IO bus  
**Parameter:** (uint16_t) value - 0 to 65535 (0xFFFF).  For each bit 1 = enabled, 0 = disabled  
**Returns:** null  
___  

```c
io_get_bus_pullups()
```

Get internal 100K pull-up resistors for the IO bus  
**Returns:** (uint16_t) 0 to 65535 (0xFFFF).  For each bit 1 = enabled, 0 = disabled  
___  

```c
io_write_pin(uint8_t pin, uint8_t value)
```

Write to an individual pin 1 - 16  
**Parameter:** (uint8_t) pin - 1 to 16  
**Parameter:** (uint8_t) value - 0 = logic low, 1 = logic high  
**Returns:** null  
___  

```c
io_write_port(uint8_t port, uint8_t value)
```

Write to all pins on the selected port  
**Parameter:** (uint8_t) port - 0 = pins 1 to 8, port 1 = pins 9 to 16  
**Parameter:** (uint8_t) value - 0 to 255 (0xFF)  
**Returns:** null  
___  

```c
io_write_bus(uint16_t value)
```

Write to all pins on the selected bus  
**Parameter:** (uint8_t) value - 0 to 65535 (0xFFFF). For each bit 1 = logic high, 0 = logic low  
**Returns:** null  
___  

```c
io_read_pin(uint8_t pin)
```

Read the value of an individual pin  
**Parameter:** (uint8_t) pin - 1 to 16  
**Returns:** (uint8_t) 0 = logic low, 1 = logic high  
___  

```c
io_read_port(uint8_t port)
```

Read all pins on the selected port  
**Parameter:** (uint8_t) port - 0 = pins 1 to 8, port 1 = pins 9 to 16  
**Returns:** (uint8_t) 0 to 255 (0xFF). For each bit 1 = logic high, 0 = logic low  
___  

```c
io_read_bus()
```

Read all pins on the bus  
**Returns:** (uint16_t) 0 to 65535 (0xFFFF). For each bit 1 = logic high, 0 = logic low  
___  

```c
io_invert_pin(uint8_t pin, uint8_t polarity)
```

Invert the polarity of the selected pin  
**Parameter:** (uint8_t) pin - 1 to 16  
**Parameter:** (uint8_t) polarity - 0 = non-inverted, 1 = inverted  
**Returns:** null  
___  

```c
io_get_pin_polarity(uint8_t pin)
```

Get the polarity of the selected pin  
**Parameter:** (uint8_t) pin - 1 to 16  
**Returns:** (uint8_t) pin polarity 0 = non-inverted, 1 = inverted  
___  

```c
io_invert_port(uint8_t port, uint8_t polarity)
```

Invert the polarity of the pins on a selected port  
**Parameter:** (uint8_t) port - 0 = pins 1 to 8, port 1 = pins 9 to 16  
**Parameter:** (uint8_t) polarity - 0 to 255 (0xFF). For each bit 0 = non-inverted, 1 = inverted  
**Returns:** null  
___  

```c
io_get_port_polarity(uint8_t port)
```

Get the polarity of the selected pin  
**Parameter:** (uint8_t) port - 0 = pins 1 to 8, port 1 = pins 9 to 16  
**Returns:** (uint8_t) 0 to 255 (0xFF). For each bit 0 = non-inverted, 1 = inverted  
___  

```c
io_invert_bus(uint16_t polarity)
```

Invert the polarity of the pins on a selected bus  
**Parameter:** (uint16_t) polarity - 0 to 65535 (0xFFFF). For each bit 0 = non-inverted, 1 = inverted  
**Returns:** null  
___  

```c
io_get_bus_polarity()
```

Get the polarity of the bus  
**Returns:** (uint16_t) 0 to 65535 (0xFFFF). For each bit 0 = non-inverted, 1 = inverted  
___  

```c
io_mirror_interrupts(uint8_t value)
```

Set the interrupt pins to be mirrored or for separate ports  
**Parameter:** (uint8_t) value - 1 = The interrupt pins are internally connected, 0 = The interrupt pins are not connected. INTA is associated with PortA and INTB is associated with PortB  
**Returns:** null  
___  

```c
io_set_interrupt_polarity(uint8_t value)
```

This sets the polarity of the interrupt output pins.  
**Parameter:** (uint8_t) value - 1 = Active-high, 0 = Active-low.  
**Returns:** null  
___  

```c
io_get_interrupt_polarity()
```

Get the polarity of the interrupt output pins.  
**Returns:** (uint8_t) 1 = Active-high, 0 = Active-low.  
___  

```c
io_set_interrupt_type(uint8_t port, uint8_t value)
```

Sets the type of interrupt for each pin on the selected port  
**Parameter:** (uint8_t) port - 0 = pins 1 to 8, port 1 = pins 9 to 16  
**Parameter:** (uint8_t) value - 0 to 255 (0xFF). For each bit 1 = interrupt is fired when the pin matches the default value, 0 = the interrupt is fired on state change  
**Returns:** null  
___  

```c
io_get_interrupt_type(uint8_t port)
```

Get the type of interrupt for each pin on the selected port  
**Parameter:** (uint8_t) port - 0 = pins 1 to 8, port 1 = pins 9 to 16  
**Returns:** (uint8_t) 0 to 255 (0xFF). For each bit 1 = interrupt is fired when the pin matches the default value, 0 = the interrupt is fired on state change  
___  

```c
io_set_interrupt_defaults(uint8_t port, uint8_t value)
```

These bits set the compare value for pins configured for interrupt-on-change on the selected port.  
If the associated pin level is the opposite from the register bit, an interrupt occurs.  
**Parameter:** (uint8_t) port - 0 = pins 1 to 8, port 1 = pins 9 to 16  
**Parameter:** (uint8_t) value - default state for the port. 0 to 255 (0xFF).  
**Returns:** null  
___  

```c
io_get_interrupt_defaults(uint8_t port)
```

Get the compare value for pins configured for interrupt-on-change on the selected port.  
If the associated pin level is the opposite from the register bit, an interrupt occurs.  
**Parameter:** (uint8_t) port - 0 = pins 1 to 8, port 1 = pins 9 to 16  
**Returns:** (uint8_t) default state for the port. 0 to 255 (0xFF).  
___  

```c
io_set_interrupt_on_pin(uint8_t pin, uint8_t value)
```

Enable interrupts for the selected pin  
**Parameter:** (uint8_t) pin - 1 to 16  
**Parameter:** (uint8_t) value - 0 = interrupt disabled, 1 = interrupt enabled  
**Returns:** null  
___  

```c
io_get_interrupt_on_pin(uint8_t pin)
```

Get the interrupt enable status for the selected pin  
**Parameter:** (uint8_t) pin - 1 to 16  
**Returns:** (uint8_t) 0 = interrupt disabled, 1 = interrupt enabled  
___  

```c
io_set_interrupt_on_port(uint8_t port, uint8_t value)
```

Enable interrupts for the pins on the selected port  
**Parameter:** (uint8_t) port - 0 = pins 1 to 8, port 1 = pins 9 to 16  
**Parameter:** (uint8_t) value - 0 to 255 (0xFF). For each bit 0 = interrupt disabled, 1 = interrupt enabled  
**Returns:** null  
___  

```c
io_get_interrupt_on_port(uint8_t port)
```

Get the interrupt enable status for the selected port  
**Parameter:** (uint8_t) port - 0 = pins 1 to 8, port 1 = pins 9 to 16  
**Returns:** (uint8_t) 0 to 255 (0xFF). For each bit 0 = interrupt disabled, 1 = interrupt enabled  
___  

```c
io_set_interrupt_on_bus(uint16_t value)
```

Enable interrupts for the pins on the selected bus  
**Parameter:** (uint16_t) value - 0 to 65535 (0xFFFF). For each bit 0 = interrupt disabled, 1 = interrupt enabled  
**Returns:** null  
___  

```c
io_get_interrupt_on_bus()
```

Get the interrupt enable status for the selected bus  
**Returns:** (uint16_t) 0 to 65535 (0xFFFF). For each bit 0 = interrupt disabled, 1 = interrupt enabled  
___  

```c
io_read_interrupt_status(uint8_t port)
```

Read the interrupt status for the pins on the selected port  
**Parameter:** (uint8_t) port - 0 = pins 1 to 8, port 1 = pins 9 to 16  
**Returns:** (uint8_t) 0 to 255 (0xFF). For each bit 1 = interrupt triggered, 0 = interrupt not triggered  
___  

```c
io_read_interrupt_capture(uint8_t port)
```

Read the value from the selected port at the time of the last interrupt trigger  
**Parameter:** (uint8_t) 0 = pins 1 to 8, port 1 = pins 9 to 16  
**Returns:** (uint8_t) 0 to 255 (0xFF). For each bit 1 = interrupt triggered, 0 = interrupt not triggered  
___  

```c
io_reset_interrupts()
```

Reset the interrupts A and B to 0  
**Returns:** null  
___  

```c
io_init(uint8_t reset)
```

Initialise the MCP32017 IO chip  
**Parameter:** (uint8_t) reset - If set to 1 the IO Pi will reset to default values. Ports are inputs, pull-up resistors are disabled and ports are not inverted.  
**Returns:** null  
___  

RTC Functions
----------

```c
rtc_set_date(struct tm date)
```

Set the date on the RTC  
**Parameter:** (uint8_t) date - struct tm formatted date and time  
**Returns:** null  
___  

```c
rtc_read_date()
```

Read the date from the RTC.  
**Returns:** (struct tm) date as a tm struct  
___  

```c
rtc_enable_output()
```

Enable the square wave output pin  
**Returns:** null  
___  

```c
rtc_disable_output()
```

Disable the square wave output pin  
**Returns:** null  
___  

```c
rtc_set_frequency(uint8_t frequency)
```

Set the square wave output frequency  
**Parameter:** (uint8_t) frequency - 1 = 1Hz, 2 = 4.096KHz, 3 = 8.192KHz, 4 = 32.768KHz  
**Returns:** null  
___  

```c
rtc_write_memory(uint8_t address, uint8_t *valuearray)
```

Write to the memory on the DS1307.  The DS1307 contains 56 - Byte, battery - backed RAM with Unlimited Writes  
**Parameter:** (uint8_t) address - 0x08 to 0x3F  
**Parameter:** (uint8_t) valuearray - byte array containing data to be written to memory  
**Returns:** null  
___  

```c
rtc_read_memory(uint8_t address, uint8_t length)
```

Read from the memory on the DS1307  
The DS1307 contains 56-Byte, battery-backed RAM with Unlimited Writes  
**Parameter:** (uint8_t) address - 0x08 to 0x3F  
**Parameter:** (uint8_t) length - up to 32 bytes.  length can not exceed the available address space.  
**Returns:** (uint8_t *) pointer to a byte array where the data will be saved  
___  
