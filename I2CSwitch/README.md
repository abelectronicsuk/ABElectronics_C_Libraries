AB Electronics UK: I2C Switch - C Library
=====

C Library to use with I2C Switch Raspberry Pi expansion board from [AB Electronics UK](https://www.abelectronics.co.uk)

The example C programs can be found in /ABElectronics_C_Libraries/I2CSwitch/demos

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
switch_channel(uint8_t address, uint8_t channel)
```

Enable the specified I2C channel and disable other channels  
**Parameter:** (uint8_t) address - I2C address for the target device. Default = 0x70  
**Parameter:** (uint8_t) channel - 1 to 4  
**Returns:** (char) 0 = success, -1 = error switching to channel  
___  

```c
set_channel_state(uint8_t address, uint8_t channel, uint8_t state)
```

Sets the state of the specified I2C channel.  
All other channels keep their existing state.  
**Parameter:** (uint8_t) address - I2C address for the target device. Default = 0x70  
**Parameter:** (uint8_t) channel - 1 to 4  
**Parameter:** (uint8_t) state - 0 = channel off, 1 = channel on  
**Returns:** (char) 0 = success, -1 = error setting channel state  
___  

```c
get_channel_state(uint8_t address, uint8_t channel)
```

Gets the state of the specified I2C channel  
**Parameter:** (uint8_t) address - I2C address for the target device. Default = 0x70  
**Parameter:** (uint8_t) channel - 1 to 4  
**Returns:** (char) 0 = channel off, 1 = channel on, -1 = error getting channel state  
___  

```c
reset()
```

Reset the I2C switch.  All channels are set to off.  
**Returns:** (char) 0 = reset complete, -1 = reset failed  
___
