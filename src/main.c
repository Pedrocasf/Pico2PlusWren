#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/dma.h"
#include "pico/cyw43_arch.h"
#include "hardware/uart.h"
#include "psram.h"
// Data will be copied from src to dst
const char src[] = "Hello, world! (from DMA)";
char dst[count_of(src)];


// UART defines
// By default the stdout UART is `uart0`, so we will use the second one
#define UART_ID uart1
#define BAUD_RATE 115200

// Use pins 4 and 5 for UART1
// Pins can be changed, see the GPIO function select table in the datasheet for information on GPIO assignments
#define UART_TX_PIN 4
#define UART_RX_PIN 5



int main()
{
    stdio_init_all();
    setup_psram();
    while (true) {
        printf("Hello, world!\n");
        sleep_ms(1000);
    }
}
