#include "include/board.h"
#include "lib/io.h"
#include "lib/uart.h"
#include "lib/util.h"
#include "lib/i2c.h"
#include "libshield/th02.h"

#define MMA0

volatile char cmd;
char buf[10];

static void on_rx_cb(char c)
{
	cmd = c;
}

int main(void)
{
	int temp,PE,PF;
	int ret;
	uart_init(_USART2, 115200, UART_8N1, on_rx_cb);
	i2c_master_init(I2C1);
  	// Initialize I2C and read temperature
  	ret = th02_read_temp(&temp);
  	if (ret == I2C_OK) {
    	uart_printf(_USART2,"\n\rTemperature: %d °C\n", temp);
  	} else {
    	uart_printf(_USART2,"\n\rError reading temperature: %d\n", ret);
  	}
	//while(1) {
		    // uart_printf(_USART2,"\n\rStart");
			// th02_read_temp(&temp);
			// //PE=temp>>3;
			// //PF=(temp&0x7)*1000/8;
			// uart_printf(_USART2,"\n\rLa température est %d.%d°C",temp,temp); //temp,temp
	//}
	return 0;
}