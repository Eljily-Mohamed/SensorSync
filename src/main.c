#include "include/board.h"
#include "lib/io.h"
#include "lib/uart.h"
#include "lib/util.h"
#include "lib/i2c.h"
#include "libshield/th02.h"
#include "libshield/mma7660.h"

volatile char cmd;
char buf[10];

static void on_rx_cb(char c)
{
    cmd = c;
}

int main(void)
{
    float temperature, humidity;

    uart_init(_USART2, 9600, UART_8N1, on_rx_cb);
    i2c_master_init(_I2C1);
    //TH02_begin(); // Initialize TH02 sensor
    return 0;
}
