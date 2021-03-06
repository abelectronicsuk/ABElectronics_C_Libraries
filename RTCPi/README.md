AB Electronics UK: RTC Pi - C Library
=====

C Library to use with RTC Pi Raspberry Pi expansion board from [AB Electronics UK](https://www.abelectronics.co.uk)

The example C programs can be found in /ABElectronics_C_Libraries/RTCPi/demos

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
rtc_set_date(struct tm date)
```

Set the date on the RTC  
**Parameter:** (struct tm) date - struct tm formatted date and time  
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

Enable the squarewave output pin  
**Returns:** null  
___  

```c
rtc_disable_output()
```

Disable the squarewave output pin  
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
rtc_write_memory(uint8_t address, uint8_t valuearray[])
```

Write to the memory on the ds1307.  
The DS1307 contains 56 - Byte, battery - backed RAM with Unlimited Writes.  
**Parameter:** (uint8_t) address - 0x08 to 0x3F  
**Parameter:** (uint8_t[]) valuearray - byte array containing data to be written to memory  
**Returns:** null  
___  

```c
rtc_read_memory(uint8_t address, uint8_t length)
```

Read from the memory on the ds1307  
The DS1307 contains 56-Byte, battery-backed RAM with Unlimited Writes  
**Parameter:** (uint8_t) address - 0x08 to 0x3F  
**Parameter:** (uint8_t) length - up to 32 bytes.  length can not exceed the available address space.  
**Returns:** (uint8_t *) pointer to a byte array where the data will be saved  
___
