/*
 ================================================
 ABElectronics UK I2C Switch
 Version 1.0 Created 08/11/2019
 ================================================


 The PCA9546A I2C switch contains 4 channels.  Each channel can be
 switched on or off independently.

 Required package{
 sudo apt-get install libi2c-dev
 */

// public methods

/**
* Enable the specified I2C channel and disable other channels
* @param address - I2C address for the target device. Default = 0x70
* @param channel - 1 to 4
* @returns - 0 = success, -1 = error switching to channel
*/
char switch_channel(char address, char channel);


/**
* Sets the state of the specified I2C channel.
* All other channels keep their existing state.
* @param address - I2C address for the target device. Default = 0x70
* @param channel - 1 to 4
* @returns - 0 = success, -1 = error setting channel state
*/
char set_channel_state(char address, char channel, char state);


/**
* Gets the state of the specified I2C channel
* @param address - I2C address for the target device. Default = 0x70
* @param channel - 1 to 4
* @returns - 0 = channel off, 1 = channel on, -1 = error getting channel state
*/
char get_channel_state(char address, char channel);


/**
* Reset the I2C switch.  All channels are set to off.
* @returns - 0 = reset complete, -1 = reset failed
*/
char reset();


