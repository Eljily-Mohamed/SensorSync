#include "lib/i2c.h"

#define LM75_ADDRESS		(0x48)

#define LM75_REG_TEMP		(0x00)
#define LM75_REG_CONFIG		(0x01)
#define LM75_REG_THYST		(0x02)
#define LM75_REG_TOS		(0x03)
#define LM75_REG_NONE		(0x04)


static uint8_t last_reg = LM75_REG_NONE;
//lire le registre de température,renvoie val signée codée en virgule fixe au format [29:3].
int  lm75_read_temp(int *temp)
{
	uint8_t buffer [2];
	int st;

	if (last_reg!=LM75_REG_TEMP) {
		buffer [0] = last_reg = LM75_REG_TEMP;
		st = i2c_write(_I2C1, LM75_ADDRESS, buffer, 1);
		if(st==I2C_ERROR) return -1;
	} else {
		st = i2c_read(_I2C1,LM75_ADDRESS,buffer,2);
		if(st==I2C_ERROR) return -1;
	}
	if (st==I2C_OK) {
		lcd_reset();
		cls();
		lcd_printf("OK");
		//  Sign extend negative numbers
		if (buffer[0] & 0x80)
		{
			buffer[0] |=0xFFFFFF00;
		}
		*temp = ((buffer[0] << 3) | (buffer[1] >> 5))/(1<<3);
		

	} else {
		*temp=0;
	}
	return st;
}
