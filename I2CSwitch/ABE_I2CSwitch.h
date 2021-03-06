/*
 ================================================
 ABElectronics UK I2C Switch
 See CHANGELOG.md for version number
 ================================================


 The PCA9546A I2C switch contains 4 channels.  Each channel can be
 switched on or off independently.

 Required package: libi2c-dev
 sudo apt-get install libi2c-dev
 */

// public methods

/**
* Enable the specified I2C channel and disable other channels
* @param address: (uint8_t) I2C address for the target device. Default = 0x70
* @param channel: (uint8_t) 1 to 4
* @returns: (char) 0 = success, -1 = error switching to channel
*/
char switch_channel(uint8_t address, uint8_t channel);


/**
* Sets the state of the specified I2C channel.
* All other channels keep their existing state.
* @param address: (uint8_t) I2C address for the target device. Default = 0x70
* @param channel: (uint8_t) 1 to 4
* @param state: (uint8_t) 0 = channel off, 1 = channel on
* @returns: (char) 0 = success, -1 = error setting channel state
*/
char set_channel_state(uint8_t address, uint8_t channel, uint8_t state);


/**
* Gets the state of the specified I2C channel
* @param address: (uint8_t) I2C address for the target device. Default = 0x70
* @param channel: (uint8_t) 1 to 4
* @returns: (char) 0 = channel off, 1 = channel on, -1 = error getting channel state
*/
char get_channel_state(uint8_t address, uint8_t channel);


/**
* Reset the I2C switch.  All channels are set to off.
* @returns: (char) 0 = reset complete, -1 = reset failed
*/
char reset();


