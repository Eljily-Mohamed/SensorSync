#include <stdlib.h>
#include <stdarg.h>
#include "uart.h"
#include "io.h"
#include "util.h"
                             
#ifdef USE_USART1
static OnUartRx usart1_cb=0;

void USART1_IRQHandler(void)
{
	uint32_t sr = _USART1->SR;
	
	if (sr & (1<<5)) {			// Read data register not empty interrupt
		if (!((sr & (1<<2)) || (sr & (1<<2)))) {
			if (usart1_cb) usart1_cb((char)_USART1->DR);
		} else {				// Noise or framing error or break detected
			_USART1->DR;
		}
	} else if (sr & (1<<4)) {	// idle interrupt
		_USART1->DR;
	} else if (sr & (1<<3)) {	// overrun interrupt
		_USART1->DR;
	} else if (sr & (1<<0)) {	// parity error interrupt
		_USART1->DR;
	}
}
#endif

#ifdef USE_USART2
static OnUartRx usart2_cb=0;

void USART2_IRQHandler(void)
{
	uint32_t sr = _USART2->SR;
	
	if (sr & (1<<5)) {			// Read data register not empty interrupt
		if (!((sr & (1<<2)) || (sr & (1<<2)))) {
			if (usart2_cb) usart2_cb((char)_USART2->DR);
		} else {				// Noise or framing error or break detected
			_USART2->DR;
		}
	} else if (sr & (1<<4)) {	// idle interrupt
		_USART2->DR;
	} else if (sr & (1<<3)) {	// overrun interrupt
		_USART2->DR;
	} else if (sr & (1<<0)) {	// parity error interrupt
		_USART2->DR;
	}
}
#endif

#ifdef USE_USART6
static OnUartRx usart6_cb=0;

void USART6_IRQHandler(void)
{
	uint32_t sr = _USART6->SR;
	
	if (sr & (1<<5)) {			// Read data register not empty interrupt
		if (!((sr & (1<<2)) || (sr & (1<<2)))) {
			if (usart2_cb) usart2_cb((char)_USART6->DR);
		} else {				// Noise or framing error or break detected
			_USART6->DR;
		}
	} else if (sr & (1<<4)) {	// idle interrupt
		_USART6->DR;
	} else if (sr & (1<<3)) {	// overrun interrupt
		_USART6->DR;
	} else if (sr & (1<<0)) {	// parity error interrupt
		_USART6->DR;
	}
}
#endif

/*
 * uart_init : polling Tx and IRQ Rx
 */
int uart_init(USART_t *u, uint32_t baud, uint32_t mode, OnUartRx cb)
{
	IRQn_t	irq_number;
	uint32_t irq_priority;
	
	if (u == _USART1) {
#ifdef USE_USART1
		// enable USART clocking
		_RCC->APB2ENR |= (1<<4); 
		// configure Tx/Rx pins : Tx -->, Rx --> 
		io_configure(USART1_GPIO_PORT, USART1_GPIO_PINS, USART1_GPIO_CFG, NULL);		
        usart1_cb = cb;
		irq_number = USART1_IRQn;
		irq_priority = USART1_IRQ_PRIORITY;
		// configure USART speed
		u->BRR = sysclks.apb2_freq/baud;	
#else
		return -1;
#endif
	 } else if (u == _USART2) {
#ifdef USE_USART2
		// enable USART clocking
	    _RCC->APB1ENR |=(1<<17);
		// configure Tx/Rx pins : Tx --> PA2, Rx --> PA3
		io_configure(USART2_GPIO_PORT, USART2_GPIO_PINS, USART2_GPIO_CFG, NULL);
		// equivalent to io_configure(_GPIOA, PIN_2|PIN_3, USART_PIN_CONFIG);
		usart2_cb = cb;
		irq_number = USART2_IRQn;
		irq_priority = USART2_IRQ_PRIORITY;
		// configure USART speed
		u->BRR = sysclks.apb1_freq/baud;
#else
		return -1;
#endif
	} else if (u == _USART6) {
#ifdef USE_USART6
		// enable USART clocking
		_RCC->APB2ENR | = (1<<5); 
		// configure Tx/Rx pins
		io_configure(USART6_GPIO_PORT, USART6_GPIO_PINS, USART6_GPIO_CFG, NULL);

		//usart6_cb
		//irq_number
		//irq_priority
        usart6_cb = cb;
		irq_number = USART6_IRQn;
		irq_priority = USART6_IRQ_PRIORITY; 
		// configure USART speed
		u->BRR = sysclks.apb2_freq/baud;
#else
		return -1;
#endif
	} else {
		return -1;
	}
	
/*Reset usart conf */
    u->CR1&=~(1<<12); //nb de bit par data raz
    u->CR1&=~(7<<8);  //parité raz
    u->CR2&=~(3<<12); //bit stop

	u->GTPR = 0;
	u->CR3 = 0;
/*conf à partir mode */
/*  mode = b10-b9-b8-x-x-b5-b4-x-x-x-b0
    b0 = 8/9 bits dans CR1 b12
    b5/b4 = nb bit stop dans CR2 b13/b12
    b10/b9/b8 = parité , 10 controle parité (1 activé/0 désactivé), 9 type parité (Odd/Even) , 8 PEIE parity Error interrupt enable
*/
	u->CR1 |=((mode & 0x1)<<12) | (mode & 0x700) | (3<<2) | (1<<13);  //conf + tx/rx activé + usart enable
	u->CR2 |=((mode & 0x30)<<8); //bit stop positionné en b12/b13 de CR2
			 
	// Setup NVIC
	if (cb) {
		/* A COMPLETER */
        NVIC_SetPriority(irq_number, irq_priority);
		NVIC_EnableIRQ(irq_number);
		u->CR1 |= (1<<5);		// Receiver Not Empty Interrupt Enable

	}
	
    return 1;
}

/*
 * uart_getc : get a char from the serial link (blocking)
 */
char uart_getc(USART_t *u)
{
					// wait for Data Register full
    while ((u->SR & (1<<5)) == 0) ;		// wait for Data Register full
	return (char)(u->DR);				// read the received char

	return 0;							// read the received char
}

/*
 * uart_getchar : check if a char has been received from the serial link
 * (non-blocking)
 */
int uart_getchar(USART_t *u, char *c)
{
	/* A COMPLETER */
    if (u->SR & (1<<5)) {
		*c = (char)(u->DR);
	}
	return 0;
}

/*
 * uart_putc : send a char over the serial link (polling)
 */
void uart_putc(USART_t *u, char c)
{
	/* A COMPLETER */	
    while ((u->SR & (1<<7)) == 0);		// wait for Data Register empty
     u->DR = c;							// write char to send
				
}

/*
 * uart_puts : send a string over the serial link (polling)
 */
void uart_puts(USART_t *u, const char *s)
{
	/* A COMPLETER */
    if (s)
        while (*s) uart_putc(u, *s++);
}

/*
 * uart_printf : print formatted text to serial link
 */
void uart_printf(USART_t * u, const char* fmt, ...)
{
	va_list        ap;
	char          *p;
	char           ch;
	unsigned long  ul;
	char           s[34];
	
	va_start(ap, fmt);
	while (*fmt != '\0') {
		if (*fmt =='%') {
			switch (*++fmt) {
				case '%':
					uart_putc(u,'%');
					break;
				case 'c':
					ch = (char)va_arg(ap, int);
					uart_putc(u, ch);
					break;
				case 's':
					p = va_arg(ap, char *);
					uart_puts(u, p);
					break;
				case 'd':
					ul = va_arg(ap, long);
					if ((long)ul < 0) {
						uart_putc(u, '-');
						ul = -(long)ul;
					}
					num2str(s, ul, 10);
					uart_puts(u, s);
					break;
				case 'u':
					ul = va_arg(ap, unsigned int);
					num2str(s, ul, 10);
					uart_puts(u, s);
					break;
				case 'x':
					ul = va_arg(ap, unsigned int);
					num2str(s, ul, 16);
					uart_puts(u, s);
					break;
				default:
				    uart_putc(u, *fmt);
			}
		} else uart_putc(u, *fmt);
		fmt++;
	}
	va_end(ap);
}