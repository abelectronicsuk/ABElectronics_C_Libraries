/*
 * demo-toggleled.c
 *
 *  Created on: 07 July 2015
 *
 *  	This demonstration shows how to use a button on pin 15 of bus 2 to toggle the output of an led connected to pin 11 of bus 1.
 *  	A 300R resistor should be connected in series with the LED and the IO Pi to limit the current.
 *
 *      compile with "gcc ABE_IoPi.c demo-toggleled.c -o demo-toggleled"
 *      run with "./demo-toggleled"
 */

#include <stdio.h>
#include <time.h>


#include "ABE_IoPi.h"

int main(int argc, char **argv){
	IOPi_init(0x20); // initialise one of the io pi buses on i2c address 0x20
	IOPi_init(0x21); // initialise one of the io pi buses on i2c address 0x21

	set_port_direction(0x20,0, 0x00); // set the direction for bank 0 on address 0x20 to be outputs
	set_port_direction(0x20,1, 0x00); // set the direction for bank 1 on address 0x20 to be outputs

	set_port_direction(0x21,0, 0xFF); // set the direction for bank 0 on address 0x21 to be inputs
	set_port_direction(0x21,1, 0xFF); // set the direction for bank 1 on address 0x21 to be inputs

	set_port_pullups(0x21, 0, 0xFF); // enable internal pullups for bank 0
	invert_port(0x21,0,0xFF); // invert output so bank will read as 0
	set_port_pullups(0x21, 1, 0xFF); // enable internal pullups for bank 0
	invert_port(0x21,1,0xFF); // invert output so bank will read as 0

	int led = 0;
	int button_last_state = 0;

	write_pin(0x20,11, 0); // turn led on pin 11 off

	while (1){
		// read the input from the button on pin 15 and save it to the button_state variable

		if (read_pin(0x21,15) == 1){ // check to see if the button is pressed

			if (button_last_state == 0){ // check to see if the button has been released since the last loop

				if (led == 0){led = 1;} else {led = 0;} // toggle the state of the led variable

				write_pin(0x20,11,led); // set the output on pin 11 to be the same as the led variable

			}

			button_last_state = 1; // change the button state to 1 to show that the button has been pressed
		}
		else {
			button_last_state = 0; // change the button state to 0 to show that the button has been released
		}

		usleep(100000); // sleep for 100ms
	}

	return (0);
}
