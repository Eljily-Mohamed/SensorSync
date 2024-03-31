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
    float temperature, humidity;

    uart_init(_USART2, 9600, UART_8N1, on_rx_cb);
    i2c_master_init(_I2C1);
    TH02_begin(); // Initialize TH02 sensor

    while (1)
    {
        //temperature = TH02_ReadTemperature();
        //humidity = TH02_ReadHumidity();

        uart_printf(_USART2, "Temperature: %.2f C\r\n", temperature);
        //uart_printf(_USART2, "Humidity: %.2f %%\r\n", humidity);

        //delay_ms(1000); // Delay for 1 second
    }

    return 0;
}
