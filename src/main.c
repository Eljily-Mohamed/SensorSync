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

int32_t buffer[4], moy[4]={0,0,0,0}, tmp;
int k=0;

int main(void)
{
	int temp,PE,PF;

	uart_init(_USART2, 115200, UART_8N1, on_rx_cb);
	i2c_master_init(_I2C1);
	th02_begin();

	while(1) {
#ifdef MMA0
		uart_printf(_USART2,"\r\nEntrez une commande : ");
#endif
		while (!cmd) ;
		switch (cmd) {
		case 't': // get current temperature
			th02_read_temp(&temp);
			//cmd=0;
			PE=temp>>3;
			PF=(temp&0x7)*1000/8;
			uart_printf(_USART2,"\n\rLa température est %d.%d°C",PE,PF); //temp,temp
			break;
		default:
			cmd=0;
			break;
		}
	}
	return 0;
}