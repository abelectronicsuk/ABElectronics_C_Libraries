/*
 abelectronics IO Pi Expander board output demo led matrix example using smbus for C
 Writes "IO Pi" to a 24 x 8 led matrix
 Requries Python 2 and smbus
 I2C API depends on I2C support in the kernel

 Version 1.0  - 24/12/2012
 Version History:
 1.0 - Initial Release

Required package:
apt-get install libi2c-dev

Compile with gcc:
gcc iopimatrix.c -o iopimatrix

Execute with:
./iopimatrix

*/
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <linux/i2c-dev.h>


int fd;
char *fileName = "/dev/i2c-1";
int expanderaddress1 = 0x20;
int expanderaddress2 = 0x21;
unsigned char buf[10];

void writetoIO(int, int, int ) ;
void ClearAll();
void ClearBus1();
void ClearBus2();
void ClearBus3();

int main(int argc, char **argv) {
	printf("*** led matrix demo ***\n");

	if ((fd = open(fileName, O_RDWR)) < 0) {
			printf("Failed to open 12c port\n");
			exit(1);
	}

	// set both chips to be outputs each bus


	writetoIO(expanderaddress1, 0x00, 0x00);
	writetoIO(expanderaddress1, 0x01, 0x00);
	writetoIO(expanderaddress2, 0x00, 0x00);
	writetoIO(expanderaddress2, 0x01, 0x00);
	ClearAll();

	while(1) {
		// first 8 columns
		writetoIO(expanderaddress1, 0x13, 0x80);
		writetoIO(expanderaddress1, 0x12, 0x7E);
		ClearBus1();

		writetoIO(expanderaddress1, 0x13, 0x40);
		writetoIO(expanderaddress1, 0x12, 0x00);
		ClearBus1();

		writetoIO(expanderaddress1, 0x13, 0x20);
		writetoIO(expanderaddress1, 0x12, 0x7E);
		ClearBus1();

		writetoIO(expanderaddress1, 0x13, 0x10);
		writetoIO(expanderaddress1, 0x12, 0xFF);
		ClearBus1();

		writetoIO(expanderaddress1, 0x13, 0x08);
		writetoIO(expanderaddress1, 0x12, 0xC3);
		ClearBus1();

		writetoIO(expanderaddress1, 0x13, 0x04);
		writetoIO(expanderaddress1, 0x12, 0xBD);
		ClearBus1();

		writetoIO(expanderaddress1, 0x13, 0x02);
		writetoIO(expanderaddress1, 0x12, 0x7E);
		ClearBus1();

		writetoIO(expanderaddress1, 0x13, 0x01);
		writetoIO(expanderaddress1, 0x12, 0x7E);
		ClearBus1();
		// second 8 columns
		writetoIO(expanderaddress2, 0x12, 0x80);
		writetoIO(expanderaddress1, 0x12, 0x7E);
		ClearBus2();

		writetoIO(expanderaddress2, 0x12, 0x40);
		writetoIO(expanderaddress1, 0x12, 0xBD);
		ClearBus2();

		writetoIO(expanderaddress2, 0x12, 0x20);
		writetoIO(expanderaddress1, 0x12, 0xC3);
		ClearBus2();

		writetoIO(expanderaddress2, 0x12, 0x02);
		writetoIO(expanderaddress1, 0x12, 0x00);
		ClearBus2();

		writetoIO(expanderaddress2, 0x12, 0x01);
		writetoIO(expanderaddress1, 0x12, 0x6F);
		ClearBus2();
		// third 8 columns
		writetoIO(expanderaddress2, 0x13, 0x80);
		writetoIO(expanderaddress1, 0x12, 0x6F);
		ClearBus3();

		writetoIO(expanderaddress2, 0x13, 0x40);
		writetoIO(expanderaddress1, 0x12, 0x6F);
		ClearBus3();

		writetoIO(expanderaddress2, 0x13, 0x20);
		writetoIO(expanderaddress1, 0x12, 0x9F);
		ClearBus3();


		writetoIO(expanderaddress2, 0x13, 0x04);
		writetoIO(expanderaddress1, 0x12, 0x7E);
		ClearBus3();

		writetoIO(expanderaddress2, 0x13, 0x02);
		writetoIO(expanderaddress1, 0x12, 0x00);
		ClearBus3();

		writetoIO(expanderaddress2, 0x13, 0x01);
		writetoIO(expanderaddress1, 0x12, 0x7E);
		ClearBus3();
	}

}

void writetoIO(int address, int val1, int val2) {
	ioctl(fd,I2C_SLAVE,address);
	buf[0] = val1;
	buf[1] = val2;
	write(fd, buf, 2);


}
void ClearAll() {
	writetoIO(expanderaddress1, 0x12, 0xFF);
	writetoIO(expanderaddress1, 0x13, 0x00);
	writetoIO(expanderaddress2, 0x12, 0x00);
	writetoIO(expanderaddress2, 0x13, 0x00);
}


void ClearBus1(){;
	writetoIO(expanderaddress1, 0x12, 0xFF);
	writetoIO(expanderaddress1, 0x13, 0x00);
}

void ClearBus2(){
	writetoIO(expanderaddress1, 0x12, 0xFF);
	writetoIO(expanderaddress2, 0x12, 0x00);
}
void ClearBus3(){
	writetoIO(expanderaddress1, 0x12, 0xFF);
	writetoIO(expanderaddress2, 0x13, 0x00);
}

