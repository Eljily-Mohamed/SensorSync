#include "include/board.h"
#include "lib/io.h"
#include "lib/uart.h"
#include "lib/util.h"
#include "lib/i2c.h"
#include "libshield/th02.h"

volatile char cmd;
char buf[10];

static void on_rx_cb(char c)
{
	cmd = c;
}

int main(void)
{
	uart_init(_USART2, 115200, UART_8N1, on_rx_cb);
	i2c_master_init(_I2C1);
    if (TH02_begin()) {
        uart_printf(_USART2, "TH02 sensor configured successfully!\n");
    } else {
        uart_printf(_USART2, "TH02 sensor configuration failed!\n");
       
    }
}