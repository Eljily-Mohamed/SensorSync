#include "include/board.h"
#include "lib/io.h"
#include "lib/uart.h"
#include "lib/util.h"
#include "lib/i2c.h"
#include "lib/timer.h"

#define TEMP_HUMIDITYSENSOR


volatile char cmd;

#ifdef TEMP_HUMIDITYSENSOR

// #define delay_us(us)        timer_wait_us(_TIM3,us,NULL)

static void on_rx_cb(char c) {
    cmd = c;
}

int main(void) {
    int temp,PE,PF;
    // Initialize UART and I2C
    uart_init(_USART2, 115200, UART_8N1, on_rx_cb);
    // Initialize TH02 sensor
    TH02_dev_begin();
    while(1){
		   // Read temperature from TH02 sensor
        temp = TH02_dev_ReadTemperature();
        PE=temp>>3;
		PF=(temp&0x7)*1000/8;
		uart_printf(_USART2,"\n\rLa température est %d.%d°C",PE,PF); //temp,temp
		
        // delay_us(100);
	}
    
    return 0;
}

#endif

#ifdef COLORSENSOR
#include "libshield/tcs3414cs.h"
static void on_rx_cb(char c)
{
    cmd = c;
}

int main(void)
{
	// Variables pour stocker les valeurs de couleur
    uint16_t red, green, blue, clear;
	uart_init(_USART2, 115200, UART_8N1, on_rx_cb);
	i2c_master_init(_I2C1);
	tcs3414cs_init();

	
	// Lecture des valeurs de couleur
	tcs3414cs_read_color(&red, &green, &blue, &clear);
	uart_printf(_USART2,"\n\rValeur rouge: %u\n", red);
	uart_printf(_USART2,"\n\rValeur verte: %u\n", green);
	uart_printf(_USART2,"\n\rValeur bleue: %u\n", blue);
	uart_printf(_USART2,"\n\rValeur claire: %u\n", clear);
	while(1) {}
	return 0;
}
#endif