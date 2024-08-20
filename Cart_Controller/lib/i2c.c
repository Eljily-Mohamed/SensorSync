#include "i2c.h"
#include "io.h"

// I2C Pin config
#define I2C_PIN_CONFIG	(PIN_MODE_ALTFUNC | PIN_OPT_OUTPUT_OPENDRAIN | \
                             PIN_OPT_OUTPUT_SPEED_LOW | \
                             PIN_OPT_RESISTOR_NONE | PIN_OPT_AF4)

// I2C Operation
#define I2C_WRITE		0
#define I2C_READ		1

// I2C status register SR1 bits
#define I2C_IT_ERR		(1<<8)
#define I2C_IT_EVT		(1<<9)
#define I2C_IT_BUF		(1<<10)

#define I2C_SR1_RxNE	(1<<6)
#define I2C_SR1_TxE		(1<<7)

/* context of the I2C transfert */
int nb_irqs_i2c=0;

typedef struct _I2CContext {
	I2C_t *			i2c;				// I2C interface
	volatile int	status;				// driver status
	uint8_t			op;					// read/write operation
	uint8_t			address;			// slave device address
	uint8_t * 		buffer;				// buffer to write or to store read data
	uint32_t		n_to_read;			// how many bytes to be read?
	uint32_t		n_to_write;			// how many bytes to write?
	uint32_t		n_wr;				// how many data actually written?
	uint32_t		n_rd;				// how many data actually read?
} I2CContext;

static I2CContext i2c_ctx[3] = {
	{
		.i2c = I2C1
	},
	{
		.i2c = I2C2
	},
	{
		.i2c = I2C3
	}
};

static I2CContext * i2c_get_context(I2C_t *i2c) {
	if (i2c == _I2C1) return &(i2c_ctx[0]);
	else if (i2c == _I2C2) return &(i2c_ctx[1]);
	else if (i2c == _I2C3) return &(i2c_ctx[2]);
	
	return NULL;
}

void event_handler(I2CContext *ctx) {
	uint32_t reg32 __attribute__((unused));
	
	I2C_t *i2c = ctx->i2c;
	uint32_t sr1 = i2c->SR1;
	
	nb_irqs_i2c++;
	
	if (sr1 & I2C_SR1_SB) {					
		// Start or Repeated Start sent
		i2c->DR = (ctx->address<<1) | ctx->op;	//   send address + mode (R/_W)
	} else if (sr1 & I2C_SR1_ADDR) {		
		// A slave acknowledged the address
		if (ctx->op == I2C_WRITE) {				//   write the first data
			reg32 = i2c->SR2;
			i2c->DR = ctx->buffer[ctx->n_wr++];
			if (ctx->n_wr == ctx->n_to_write) {
				i2c->CR2 &= ~I2C_IT_BUF;
			}
	    } else if (ctx->op==I2C_READ) {			// Some data to read?
	    	if (ctx->n_to_read == 1) {			//   special case: 1 data to read
				i2c->CR1 &= ~I2C_CR1_ACK;
				reg32 = i2c->SR2;				//   irq acknowledge
//				i2c->CR1 |= I2C_CR1_STOP;
			} else {							//   general case
				reg32 = i2c->SR2;				//   irq acknowledge
			}
		}
	} else if (sr1 & I2C_SR1_TxE) {				// some more data to write
		if (ctx->n_wr < ctx->n_to_write) {
			i2c->DR = ctx->buffer[ctx->n_wr++];
			if (ctx->n_wr == ctx->n_to_write) {
				i2c->CR2 &= ~I2C_IT_BUF;		// when no more data disable BUF irq to disallow TxE events
			} 
		} else if ((ctx->n_wr == ctx->n_to_write) && (sr1 & I2C_SR1_BTF)) {	// no more data to write and last written byte transmitted, ack received
			if (ctx->n_to_read) {				//   anything to read?
				ctx->op=I2C_READ;
				ctx->n_rd=0;
				i2c->CR2 |= I2C_IT_BUF;
				i2c->CR1 |= I2C_CR1_START | I2C_CR1_ACK;	// send repeated start
			} else {							// no more data to write: stop
				i2c->CR1 |= I2C_CR1_STOP;		// send stop condition
				i2c->CR2 &= ~I2C_IT_EVT;
				ctx->status = I2C_OK;
			}
		}
	} else if (sr1 & I2C_SR1_RxNE) {			// a data was received
//		if (ctx->n_rd < ctx->n_to_read) {
			ctx->buffer[ctx->n_rd++] = (uint8_t)i2c->DR;
			if (ctx->n_rd == ctx->n_to_read-1) {
				i2c->CR1 &= ~I2C_CR1_ACK;
			} else if (ctx->n_rd == ctx->n_to_read) {
				i2c->CR1 |= I2C_CR1_STOP;
				ctx->status = I2C_OK;
			}
//		}
	}
}

void err_handler(I2CContext *ctx) {
	I2C_t * i2c = ctx->i2c;
	uint32_t sr1 = i2c->SR1;
	
	if (sr1 & I2C_SR1_BERR) {
		i2c->SR1 &= ~I2C_SR1_BERR;
		ctx->status = I2C_ERROR;
	} else if (sr1 & I2C_SR1_ARLO) {
		i2c->SR1 &= ~I2C_SR1_ARLO;
		ctx->status = I2C_ARBITRATION_LOST;
	} else if (sr1 & I2C_SR1_AF) {
		i2c->SR1 &= ~I2C_SR1_AF;
		ctx->status = I2C_SLAVE_ERROR;
	} else {
		i2c->SR1 &= ~0xDF00;
		ctx->status = I2C_ERROR;
	}
	i2c->CR1 |= I2C_CR1_STOP;

}

void I2C1_EV_IRQHandler(void) {                /* I2C1 Event */
	event_handler(&(i2c_ctx[0]));
}

void I2C1_ER_IRQHandler(void) {                /* I2C1 Error */
	err_handler(&(i2c_ctx[0]));
}

void I2C2_EV_IRQHandler(void) {                /* I2C2 Event */
	event_handler(&(i2c_ctx[1]));
}

void I2C2_ER_IRQHandler(void) {                /* I2C2 Error */
	err_handler(&(i2c_ctx[1]));
}

void I2C3_EV_IRQHandler(void) {                /* I2C3 event */
	event_handler(&(i2c_ctx[2]));
}

void I2C3_ER_IRQHandler(void) {                /* I2C3 error */
	err_handler(&(i2c_ctx[2]));
}


/* i2c_master_init
 *
 * i2c interface initialized in master mode
 */
int i2c_master_init(I2C_t *i2c)
{
	IRQn_t	irqn, irqn_err;
	uint32_t irq_priority, irqerr_priority;
	
	if (i2c == _I2C1) {
#ifdef USE_I2C1
		// enable I2C clocking
		_RCC->APB1ENR |= 1<<21;
		
		io_configure(I2C1_GPIO_PORT, I2C1_GPIO_PINS, I2C1_GPIO_CFG, NULL);
		irqn            = I2C1_EV_IRQn;
		irqn_err        = I2C1_ER_IRQn;
		irq_priority    = I2C1_IRQ_PRIORITY;
		irqerr_priority = I2C1_IRQERR_PRIORITY;
#else
		return -1;
#endif
	} else if (i2c == _I2C2) {
#ifdef USE_I2C2
		// enable I2C clocking
		_RCC->APB1ENR |= 1<<22;
		
		io_configure(I2C2_GPIO_PORT, I2C2_GPIO_PINS, I2C2_GPIO_CFG, NULL);
		irqn            = I2C2_EV_IRQn;
		irqn_err        = I2C2_ER_IRQn;
		irq_priority    = I2C2_IRQ_PRIORITY;
		irqerr_priority = I2C2_IRQERR_PRIORITY;
#else
		return -1;
#endif
	} else if (i2c == _I2C3) {
#ifdef USE_I2C3
		// enable I2C clocking
		_RCC->APB1ENR |= 1<<23;
		
		io_configure(I2C3_GPIO_PORT, I2C3_GPIO_PINS, I2C3_GPIO_CFG, NULL);
		irqn            = I2C3_EV_IRQn;
		irqn_err        = I2C3_ER_IRQn;
		irq_priority    = I2C3_IRQ_PRIORITY;
		irqerr_priority = I2C3_IRQERR_PRIORITY;
#else
		return -1;
#endif
	} else {
		return I2C_ERROR;
	}

	// make a software reset to deal with spurious busy states
	// SR2[1]==1 ...
	i2c->CR1|=1<<15;
	
	// disable the peripheral before changing any configuration
	i2c->CR1 = 0;
	
	// peripheral input clock frequency + Event & Buffer & Error Interrupt Enable
	i2c->CR2 = (sysclks.apb1_freq/1000000);
	
	// clock control register config : Fast Mode (400kHz), duty=0 (duty cycle 2/3)
	// Ti2c = (2+1)*CCR*Tapb1 ==> CCR = (apb1_freq/(3*400e3))
	i2c->CCR = (1<<15) | (sysclks.apb1_freq/1200000);
	
	//Configure the rise time register (from ST Cube library)
	// rise time:
	//  standard mode: 1000ns
	//  fast mode    : 300ns
	// TRISE = risetime / Tapb1 = 300e-9 * apb1_freq = 300 * apb1_freq_MHz * 1e6 / 1e9
	//  =(((i2c_freq) <= 100000) ? ((apb1_freq/10e6) + 1) : ((((apb1_freq/10e6) * 300) / 1000) + 1))
	i2c->TRISE =(((sysclks.apb1_freq/1000000) * 300) / 1000) + 1;
	
	// analog noise filter on, digital filter off
	i2c->FLTR = i2c->FLTR & (~0x1F);
	
	// enable event and error irq handlers
	NVIC_SetPriority(irqn_err, irqerr_priority);
	NVIC_SetPriority(irqn, irq_priority);
	NVIC_EnableIRQ(irqn_err);
	NVIC_EnableIRQ(irqn);
	
	// enable the peripheral
	i2c->CR1 |= 1;
		
	return I2C_OK;
}

/* i2c_write : write n bytes from buf to slave identified by addr
 *
 */
int i2c_write(I2C_t *i2c, uint8_t addr, uint8_t* buf, uint32_t n)
{
	I2CContext *ctx = i2c_get_context(i2c);
	
	if (ctx) {
		ctx->status = I2C_BUSY;
		
		ctx->address    = addr;
		ctx->buffer     = buf;
		ctx->n_to_read  = 0;
		ctx->n_to_write = n;
		ctx->n_wr = ctx->n_rd = 0;
		ctx->op = I2C_WRITE;
		
		i2c->CR2 |= I2C_IT_ERR | I2C_IT_EVT | I2C_IT_BUF;
		i2c->CR1 |= I2C_CR1_START;					// send start condition
	
		while (ctx->status == I2C_BUSY) ;			// wait for the transaction to be done
	
		for (int i=0;i<100;i++)
			__asm volatile("nop");
			
		return ctx->status;
	}
	
	return I2C_ERROR;
}

/* i2c_read : read n bytes from slave identified by addr to buf
 *
 */
int i2c_read(I2C_t *i2c, uint8_t addr, uint8_t* buf, uint32_t n)
{
	I2CContext *ctx = i2c_get_context(i2c);
	
	if (ctx) {
		ctx->status = I2C_BUSY;
		
		ctx->address    = addr;
		ctx->buffer     = buf;
		ctx->n_to_read  = n;
		ctx->n_to_write = 0;
		ctx->n_wr = ctx->n_rd = 0;
		ctx->op = I2C_READ;
	
		i2c->CR2 |= I2C_IT_ERR | I2C_IT_EVT | I2C_IT_BUF;
		i2c->CR1 |= I2C_CR1_START | I2C_CR1_ACK;	// send start condition
	
		while (ctx->status == I2C_BUSY) ;			// wait for the transaction to be done
		
		for (int i=0;i<100;i++)
			__asm volatile("nop");
			
		return ctx->status;
	}
	
	return I2C_ERROR;
}

/* i2c_write_read : write nwr bytes from buf to slave identified by addr,
 *                  then read nrd bytes to buf
 *
 */
int i2c_write_read(I2C_t *i2c, uint8_t addr, uint8_t* buf, uint32_t nwr, uint32_t nrd)
{
	I2CContext *ctx = i2c_get_context(i2c);
	
	if (ctx) {
		ctx->status = I2C_BUSY;
		
		ctx->address    = addr;
		ctx->buffer     = buf;
		ctx->n_to_read  = nrd;
		ctx->n_to_write = nwr;
		ctx->n_wr = ctx->n_rd = 0;
		ctx->op = I2C_WRITE;
		
		i2c->CR2 |= I2C_IT_ERR | I2C_IT_EVT | I2C_IT_BUF;
		i2c->CR1 |= I2C_CR1_START;					// send start condition
	
		while (ctx->status == I2C_BUSY) ;			// wait for the transaction to be done
		
		for (int i=0;i<100;i++)
			__asm volatile("nop");
			
		return ctx->status;
	}
	
	return I2C_ERROR;
}
