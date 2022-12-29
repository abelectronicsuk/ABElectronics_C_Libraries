AB Electronics UK: IO Pi - C Library
=====

C Library to use with IO Pi Raspberry Pi expansion board from [AB Electronics UK](https://www.abelectronics.co.uk)

The example C programs can be found in /ABElectronics_C_Libraries/IOPi/demos

Downloading and Installing the library
----------

To download to your Raspberry Pi type in the terminal:  

```bash
git clone https://github.com/abelectronicsuk/ABElectronics_C_Libraries.git
```

___  

Functions
----------

```c
IOPi_init(uint8_t address, uint8_t reset = 1)
```

Initialise the MCP32017 IO chip with default values: ports are inputs, pull-up resistors are disabled and ports are not inverted.  
**Parameter:** (uint8_t) address: i2c address for the target device. 0x20 to 0x27  
**Parameter:** (uint8_t) reset: If set to 1 the IO Pi will reset to default values. Ports are inputs, pull-up resistors are disabled and ports are not inverted.  
**Returns:** null  
___

```c
set_pin_direction(uint8_t address, uint8_t pin, uint8_t direction):
```

Sets the IO direction for an individual pin  
**Parameter:** (uint8_t) address: I2C address for the target device  
**Parameter:** (uint8_t) pin: 1 to 16  
**Parameter:** (uint8_t) direction: 1 = input, 0 = output  
**Returns:** null
___

```c
get_pin_direction(uint8_t address, uint8_t pin)
```

Get the IO direction for an individual pin  
**Parameter:** (uint8_t) address: I2C address for the target device  
**Parameter:** (uint8_t) pin: pin to read, 1 to 16  
**Returns:** (uint8_t) 1 = input, 0 = output  
___

```c
set_port_direction(uint8_t address, uint8_t port, uint8_t direction): 
```

Sets the IO direction for the specified IO port  
**Parameter:** (uint8_t) address: I2C address for the target device  
**Parameter:** (uint8_t) port: 0 = pins 1 to 8, 1 = pins 9 to 16  
**Parameter:** (uint8_t) direction: number between 0 and 255 or 0x00 and 0xFF.  Each bit in the 8-bit number represents a pin on the port.  1 = input, 0 = output  
**Returns:** null
___

```c
get_port_direction(uint8_t address, uint8_t port): 
```

Get the direction from an IO port  
**Parameter:** (uint8_t) address: I2C address for the target device  
**Parameter:** (uint8_t) port: 0 = pins 1 to 8, 1 = pins 9 to 16  
**Returns:** (uint8_t) number between 0 and 255 (0xFF)  
___

```c
set_bus_direction(uint8_t address, uint16_t direction): 
```

Sets the IO direction for all pins on the bus  
**Parameter:** (uint8_t) address: I2C address for the target device  
**Parameter:** (uint8_t) direction: 16-bit number 0 to 65535 (0xFFFF).  For each bit 1 = input, 0 = output  
**Returns:** null
___

```c
get_bus_direction(uint8_t address)
```

Get the direction for an IO bus  
**Parameter:** (uint8_t) address: I2C address for the target device  
**Returns:** (uint16_t) 16-bit number 0 to 65535 (0xFFFF). For each bit 1 = input, 0 = output  
___

```c
set_pin_pullup(uint8_t address, uint8_t pin, uint8_t value)
```

Set the internal 100K pull-up resistors for an individual pin  
**Parameter:** (uint8_t) address: I2C address for the target device  
**Parameter:** (uint8_t) pin: pin to update, 1 to 16  
**Parameter:** (uint8_t) value: 1 = enabled, 0 = disabled  
**Returns:** null
___

```c
get_pin_pullup(uint8_t address, uint8_t pin)
```

Get the internal 100K pull-up resistors for an individual pin  
**Parameter:** (uint8_t) address: I2C address for the target device  
**Parameter:** (uint8_t) pin: pin to read, 1 to 16  
**Returns:** (uint8_t) 1 = enabled, 0 = disabled  
___

```c
set_port_pullups(uint8_t address, uint8_t port, uint8_t value)
```

Set the internal 100K pull-up resistors for the selected IO port  
**Parameter:** (uint8_t) address: I2C address for the target device  
**Parameter:** (uint8_t) port: 0 = pins 1 to 8, 1 = pins 9 to 16  
**Parameter:** (uint8_t) value: number between 0 and 255 or 0x00 and 0xFF.  Each bit in the 8-bit number represents a pin on the port.  1 = Enabled, 0 = Disabled  
**Returns:** null  
___

```c
get_port_pullups(uint8_t address, uint8_t port): 
```

Get the internal pull-up status for the selected IO port  
**Parameter:** (uint8_t) address: I2C address for the target device  
**Parameter:** (uint8_t) port: 0 = pins 1 to 8, 1 = pins 9 to 16  
**Returns:** (uint8_t) number between 0 and 255 (0xFF)  
___

```c
set_bus_pullups(uint8_t address, uint16_t value)
```

Set internal 100K pull-up resistors for an IO bus  
**Parameter:** (uint8_t) address: I2C address for the target device  
**Parameter:** (uint8_t) value: 16-bit number 0 to 65535 (0xFFFF). For each bit 1 = enabled, 0 = disabled  
**Returns:** null
___

```c
get_bus_pullups(uint8_t address)
```

Get the internal 100K pull-up resistors for an IO bus  
**Parameter:** (uint8_t) address: I2C address for the target device  
**Returns:** (uint16_t) 16-bit number 0 to 65535 (0xFFFF). For each bit 1 = enabled, 0 = disabled  
___

```c
write_pin(uint8_t address, uint8_t pin, uint8_t value)
```

Write to an individual pin 1 - 16  
**Parameter:** (uint8_t) address: I2C address for the target device  
**Parameter:** (uint8_t) pin: 1 to 16  
**Parameter:** (uint8_t) value: 1 = logic high, 0 = logic low  
**Returns:** null  
___

```c
write_port(uint8_t address, uint8_t port, uint8_t value)
```

Write to all pins on the selected port  
**Parameter:** (uint8_t) address: I2C address for the target device  
**Parameter:** (uint8_t) port: 0 = pins 1 to 8, 1 = pins 9 to 16  
**Parameter:** (uint8_t) value:  number between 0 and 255 or 0x00 and 0xFF.  Each bit in the 8-bit number represents a pin on the port.  1 = logic high, 0 = logic low  
**Returns:** null  
___

```c
write_bus(uint8_t address, uint16_t value)
```

Write to all pins on the selected bus  
**Parameter:** (uint8_t) address: I2C address for the target device  
**Parameter:** (uint8_t) value: 16-bit number 0 to 65535 (0xFFFF). For each bit 1 = logic high, 0 = logic low  
**Returns:** null  
___

```c
read_pin(uint8_t address, uint8_t pin)
```

Read the value of an individual pin 1 - 16  
**Parameter:** (uint8_t) address: I2C address for the target device  
**Parameter:** (uint8_t) pin: 1 to 16  
**Returns:** (uint8_t) 0 = logic low, 1 = logic high  
___

```c
read_port(uint8_t address, uint8_t port)
```

Read all pins on the selected port  
**Parameter:** (uint8_t) address: I2C address for the target device  
**Parameter:** (uint8_t) port: 0 = pins 1 to 8, 1 = pins 9 to 16  
**Returns:** (uint8_t) number between 0 and 255 or 0x00 and 0xFF.  Each bit in the 8-bit number represents a pin on the port.  0 = logic low, 1 = logic high
___

```c
read_bus(uint8_t address)
```

Read all pins on the bus  
**Parameter:** (uint8_t) address: I2C address for the target device  
**Returns:** (uint16_t) 16-bit number 0 to 65535 (0xFFFF) Each bit in the 16-bit number represents a pin on the port.  0 = logic low, 1 = logic high  
___

```c
invert_pin(uint8_t address, uint8_t pin, uint8_t polarity)
```

Invert the polarity of the selected pin  
**Parameter:** (uint8_t) address: I2C address for the target device  
**Parameter:** (uint8_t) pin: 1 to 16  
**Parameter:** (uint8_t) polarity: 0 = same logic state of the input pin, 1 = inverted logic state of the input pin  
**Returns:** null
___

```c
get_pin_polarity(uint8_t address, uint8_t pin)
```

Get the polarity of the selected pin  
**Parameter:** (uint8_t) address: I2C address for the target device  
**Parameter:** (uint8_t) pin: pin to read, 1 to 16  
**Returns:** (uint8_t) 0 = same logic state of the input pin, 1 = inverted logic state of the input pin  
___

```c
invert_port(uint8_t address, uint8_t port, uint8_t polarity)
```

Invert the polarity of the pins on a selected port  
**Parameter:** (uint8_t) address: I2C address for the target device  
**Parameter:** (uint8_t) port: 0 = pins 1 to 8, 1 = pins 9 to 16  
**Parameter:** (uint8_t) polarity: number between 0 and 255 or 0x00 and 0xFF.  Each bit in the 8-bit number represents a pin on the port.  0 = same logic state of the input pin, 1 = inverted logic state of the input pin  
**Returns:** null
___

```c
get_port_polarity(uint8_t address, uint8_t port): 
```

Get the polarity for the selected IO port  
**Parameter:** (uint8_t) address: I2C address for the target device  
**Parameter:** (uint8_t) port: 0 = pins 1 to 8, 1 = pins 9 to 16  
**Returns:** (uint8_t) number between 0 and 255 (0xFF)  
___

```c
invert_bus(uint8_t address, uint16_t polarity)
```

Invert the polarity of the pins on the bus  
**Parameter:** (uint8_t) address: I2C address for the target device  
**Parameter:** (uint8_t) polarity: 16-bit number 0 to 65535 (0xFFFF).  For each bit 0 = same logic state of the input pin, 1 = inverted logic state of the input pin  
**Returns:** null  
___

```c
get_bus_polarity(uint8_t address)
```

Get the polarity of the pins on the bus  
**Parameter:** (uint8_t) address: I2C address for the target device  
**Returns:** (uint16_t) 16-bit number 0 to 65535 (0xFFFF). For each bit 0 = same logic state of the input pin, 1 = inverted logic state of the input pin  
___

```c
mirror_interrupts(uint8_t address, uint8_t value)
```

Sets whether the interrupt pins INT A and INT B are independently connected to each port or internally connected together  
**Parameter:** (uint8_t) address: I2C address for the target device  
**Parameter:** (uint8_t) value: 1 = The INT pins are internally connected, 0 = The INT pins are not connected. INT A is associated with PortA and INT B is associated with PortB  
**Returns:** null
___

```c
set_interrupt_polarity(uint8_t address, uint8_t value)
```

Sets the polarity of the INT output pins  
**Parameter:** (uint8_t) address: I2C address for the target device  
**Parameter:** (uint8_t) value: 0 = Active Low, 1 = Active High  
**Returns:** null  
___

```c
get_interrupt_polarity(uint8_t address)
```

Get the polarity of the INT output pins  
**Parameter:** (uint8_t) address: I2C address for the target device  
**Returns:** (uint8_t) 1 = Active-high.  0 = Active-low.  
___

```c
set_interrupt_type(uint8_t address, uint8_t port, uint8_t value)
```

Sets the type of interrupt for each pin on the selected port  
**Parameter:** (uint8_t) address: I2C address for the target device  
**Parameter:** (uint8_t) port: 0 = pins 1 to 8, 1 = pins 9 to 16  
**Parameter:** (uint8_t) value: number between 0 and 255 or 0x00 and 0xFF.  Each bit in the 8-bit number represents a pin on the port.  1 = interrupt is fired when the pin matches the default value, 0 = the interrupt is fired on state change  
**Returns:** null  
___

```c
get_interrupt_type(uint8_t address, uint8_t port): 
```

Get the type of interrupt for each pin on the selected port  
**Parameter:** (uint8_t) address: I2C address for the target device  
**Parameter:** (uint8_t) port: 0 = pins 1 to 8, 1 = pins 9 to 16  
**Returns:** (uint8_t) number between 0 and 255 (0xFF) For each bit 1 = interrupt is fired when the pin matches the default value, 0 = interrupt fires on state change  
___

```c
set_interrupt_defaults(uint8_t address, uint8_t port, uint8_t value)
```

These bits set the compare value for pins configured for interrupt-on-change on the selected port.  
If the associated pin level is the opposite of the register bit, an interrupt occurs.  
**Parameter:** (uint8_t) address: I2C address for the target device  
**Parameter:** (uint8_t) port: 0 = pins 1 to 8, 1 = pins 9 to 16  
**Parameter:** (uint8_t) value: compare value between 0 and 255 or 0x00 and 0xFF.  Each bit in the 8-bit number represents a pin on the port.  
**Returns:** null  
___

```c
get_interrupt_defaults(uint8_t address, uint8_t port): 
```

Get the interrupt default value for each pin on the selected port  
**Parameter:** (uint8_t) address: I2C address for the target device  
**Parameter:** (uint8_t) port: 0 = pins 1 to 8, 1 = pins 9 to 16  
**Returns:** (uint8_t) number between 0 and 255 (0xFF)  
___

```c
set_interrupt_on_pin(uint8_t address, uint8_t pin, uint8_t value)
```

Enable interrupts for the selected pin  
**Parameter:** (uint8_t) address: I2C address for the target device  
**Parameter:** (uint8_t) pin: 1 to 16  
**Parameter:** (uint8_t) value: 0 = interrupt disabled, 1 = interrupt enabled  
**Returns:** null
___

```c
get_interrupt_on_pin(uint8_t address, uint8_t pin)
```

Gets whether the interrupt is enabled for the selected pin  
**Parameter:** (uint8_t) address: I2C address for the target device  
**Parameter:** (uint8_t) pin: pin to read, 1 to 16  
**Returns:** (uint8_t) 1 = enabled, 0 = disabled
___

```c
set_interrupt_on_port(uint8_t address, uint8_t port, uint8_t value)
```

Enable interrupts for the pins on the selected port  
**Parameter:** (uint8_t) address: I2C address for the target device  
**Parameter:** (uint8_t) port: 0 = pins 1 to 8, 1 = pins 9 to 16  
**Parameter:** (uint8_t) value: number between 0 and 255 or 0x00 and 0xFF.  Each bit in the 8-bit number represents a pin on the port.  
**Returns:** null
___

```c
get_interrupt_on_port(uint8_t address, uint8_t port): 
```

Gets whether the interrupts are enabled for the selected port  
**Parameter:** (uint8_t) address: I2C address for the target device  
**Parameter:** (uint8_t) port: 0 = pins 1 to 8, 1 = pins 9 to 16  
**Returns:** (uint8_t) number between 0 and 255 (0xFF).  For each bit 1 = enabled, 0 = disabled  
___

```c
set_interrupt_on_bus(uint8_t address, uint16_t value)
```

Enable interrupts for the pins on the bus  
**Parameter:** (uint8_t) address: I2C address for the target device  
**Parameter:** (uint8_t) value: 16-bit number 0 to 65535 (0xFFFF).  For each bit 1 = enabled, 0 = disabled  
**Returns:** null
___

```c
get_interrupt_on_bus(uint8_t address)
```

Gets whether the interrupts are enabled for the bus  
**Parameter:** (uint8_t) address: I2C address for the target device  
**Returns:** (uint16_t) 16-bit number 0 to 65535 (0xFFFF). For each bit 1 = enabled, 0 = disabled  
___

```c
read_interrupt_status(uint8_t address, uint8_t port)
```

Read the interrupt status for the pins on the selected port  
**Parameter:** (uint8_t) address: I2C address for the target device  
**Parameter:** (uint8_t) port: 0 = pins 1 to 8, 1 = pins 9 to 16  
**Returns:** (uint8_t) number between 0 and 255 or 0x00 and 0xFF.  Each bit in the 8-bit number represents a pin on the port.  1 = Enabled, 0 = Disabled
___

```c
read_interrupt_capture(uint8_t address, uint8_t port)
```

Read the value from the selected port at the time of the last interrupt trigger  
**Parameter:** (uint8_t) address: I2C address for the target device  
**Parameter:** (uint8_t) port: 0 = pins 1 to 8, 1 = pins 9 to 16  
**Returns:** (uint8_t) number between 0 and 255 or 0x00 and 0xFF.  Each bit in the 8-bit number represents a pin on the port.  1 = Enabled, 0 = Disabled
___

```c
reset_interrupts(uint8_t address)
```

Set the interrupts A and B to 0  
**Parameter:** (uint8_t) address: I2C address for the target device  
**Returns:** null
