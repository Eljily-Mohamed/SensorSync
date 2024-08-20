#ifndef _I2C_H_
#define _I2C_H_

#ifdef __cplusplus
extern "C" {
#endif 

#include "include/board.h"

/**** Status Errors ****/
#define I2C_BUSY				1  		// transfer busy
#define I2C_OK					0		// transfer ended No Errors
#define I2C_ERROR				-1  	// err: general error
#define I2C_DEVICE_NOT_PRESENT	-5  	// err: Device not present
#define I2C_ARBITRATION_LOST	-6  	// err: Arbitration lost
#define I2C_TIME_OUT			-7  	// err: Time out occurred
#define I2C_SLAVE_ERROR			-8  	// err: Slave mode error

/* i2c_master_init
 *
 * i2c interface initialized in master mode
 */
int i2c_master_init(I2C_t *i2c);

/* i2c_write : write n bytes from buf to slave identified by addr
 *
 */
int i2c_write(I2C_t *i2c, uint8_t addr, uint8_t* buf, uint32_t n);

/* i2c_read : read n bytes from slave identified by addr to buf
 *
 */
int i2c_read(I2C_t *i2c, uint8_t addr, uint8_t* buf, uint32_t n);

/* i2c_write_read : write nwr bytes from buf to slave identified by addr,
 *                  then read nrd bytes to buf
 *
 */
int i2c_write_read(I2C_t *i2c, uint8_t addr, uint8_t* buf, uint32_t nwr, uint32_t nrd);

#ifdef __cplusplus
}
#endif
#endif
