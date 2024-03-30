#include "include/board.h"
#include "lib/io.h"
#include "lib/uart.h"
#include "lib/util.h"
#include "lib/i2c.h"
#include "libshield/lm75.h"
#include "libshield/mma7660.h"

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
	int temp;

	uart_init(_USART2, 115200, UART_8N1, on_rx_cb);
	i2c_master_init(_I2C1);
	return 0;
}


