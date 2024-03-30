#include "lib/i2c.h"

#define LM75_ADDRESS		(0x48)

#define LM75_REG_TEMP		(0x00)
#define LM75_REG_CONFIG		(0x01)
#define LM75_REG_THYST		(0x02)
#define LM75_REG_TOS		(0x03)
#define LM75_REG_NONE		(0x04)

static uint8_t last_reg = LM75_REG_NONE;


// int  lm75_read_temp(int *temp)
// {
// 	uint8_t buffer [2];
// 	int st;

//     if (last_reg!=LM75_REG_TEMP) {
// 		buffer [0] = last_reg = LM75_REG_TEMP;
// 		//i2c_write_read(I2C_t *i2c, uint8_t addr, uint8_t* buf, uint32_t nwr, uint32_t nrd)
// 		st = i2c_write_read(_I2C1, LM75_ADDRESS, buffer, 1, 2);
// 	} else {
// 		st = i2c_read(_I2C1, LM75_ADDRESS, buffer, 2);
// 	}
	
// 	if (st==I2C_OK) {
// 		//  Sign extend negative numbers
		
// 		/* A COMPLETER */

// 	} else {
// 		*temp=0;
// 	}
// 	return st;
// }

