#include "mma7660.h"
#include "lib/i2c.h"

int mma7660_setup(uint32_t mode)
{
	int res;
	uint8_t buf[3];
	
	/* reset mode register */
	buf[0] = 7;			// Mode Register index
	buf[1] = 0;			// Mode Register value
		
	if ((res=i2c_write(_I2C1, 0x4C, buf, 3)) != I2C_OK) {
		return -1;
	}
	
	/* configure mode and sample rate registers */
	buf[0] = 7;						// Mode Register index
	buf[1] = (uint8_t)(mode>>8);	// Mode Register value
	buf[2] = (uint8_t)mode;			// Sample Rate Register value
	
	if ((res=i2c_write(_I2C1, 0x4C, buf, 3)) != I2C_OK) {
		return -1;
	}
	
	/* read mode register */
	buf[0] = 7;
	if ((res = i2c_write_read(_I2C1, 0x4C, buf, 1, 1)) != I2C_OK) {
		return -1;
	}
	
	return 0;
}

/* read X, Y, Z and Tilt registers */
int mma7660_read_XYZT(int32_t *data)
{
	int res;
	uint8_t buf[4];
	
	/* A COMPLETER */
		
	return -1;
}
