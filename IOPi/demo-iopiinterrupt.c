/*
 * demo-iopiinterrupt.c
 *
 *  Created on: 27 March 2020
 *
 *      compile with "gcc ABE_IoPi.c demo-iopiinterrupt.c -o demo-iopiinterrupt"
 *      run with "./demo-iopiinterrupt"
 */

#include <string.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

#include "ABE_IoPi.h"

void clearscreen ()
{
    printf("\033[2J\033[1;1H");
}

int main(int argc, char **argv)
{
    setvbuf(stdout, NULL, _IONBF, 0); // needed to print to the command line

    // initialise one of the io pi buses on i2c address 0x20, default address for bus 1
    IOPi_init(0x20); 

    // Set all pins on the IO bus to be inputs with internal pull-ups enabled.

    set_port_pullups(0x20, 0, 0xFF);
    set_port_pullups(0x20, 1, 0xFF);
    set_port_direction(0x20, 0, 0xFF);
    set_port_direction(0x20, 1, 0xFF);

    // Invert both ports so pins will show 1 when grounded
    invert_port(0x20, 0, 0xFF);
    invert_port(0x20, 1, 0xFF);

    // Set the interrupt polarity to be active high and mirroring disabled, so
    // pins 1 to 8 trigger INT A and pins 9 to 16 trigger INT B
    set_interrupt_polarity(0x20, 1);
    mirror_interrupts(0x20, 0);

    // Set the interrupts default value to 0x00 so the interrupt will trigger when any pin registers as true
    set_interrupt_defaults(0x20, 0, 0x00);
    set_interrupt_defaults(0x20, 1, 0x00);

    // Set the interrupt type to be 1 for ports A and B so an interrupt is
    // fired when the pin does not match the default value
    set_interrupt_type(0x20, 0, 0xFF);
    set_interrupt_type(0x20, 1, 0xFF);

    // Enable interrupts for all pins
    set_interrupt_on_port(0x20, 0, 0xFF);
    set_interrupt_on_port(0x20, 1, 0xFF);

    while (1){

        // read the interrupt status for each port.  
        // If the status is not 0 then an interrupt has occured on one of the pins 
        // so read the value from the interrupt capture.

        if (read_interrupt_status(0x20, 0) != 0){
            printf("Port 0: %x \n", read_interrupt_capture(0x20,0));
        }
        if (read_interrupt_status(0x20, 1) != 0){
            printf("Port 1: %x \n", read_interrupt_capture(0x20,1));
        }

        usleep(200000); // sleep 0.2 seconds

    }

    (void)argc;
	(void)argv;
    return (0);
}