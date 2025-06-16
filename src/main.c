#define SFE_RP2350_XIP_CSI_PIN 47
#include <stdio.h>
#include "sparkfun_pico/sfe_pico.h"
#include "pico/stdlib.h"
#include "hardware/dma.h"
#include "hardware/uart.h"
#include "wren/src/include/wren.h"
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
void* reallocate_psram(void* ptr, size_t new_size, void* _){
    if(new_size == 0){
        sfe_mem_free(ptr);
        return NULL;
    }
    return sfe_mem_realloc(ptr, new_size);
}
void write_fn(WrenVM *vm, const char* text){
    printf("%s", text);
}
void error_fn(WrenVM* vm, WrenErrorType errorType,
             const char* module, const int line,
             const char* msg)
{
  switch (errorType)
  {
    case WREN_ERROR_COMPILE:
    {
      printf("[%s line %d] [Error] %s\n", module, line, msg);
    } break;
    case WREN_ERROR_STACK_TRACE:
    {
      printf("[%s line %d] in %s\n", module, line, msg);
    } break;
    case WREN_ERROR_RUNTIME:
    {
      printf("[Runtime Error] %s\n", msg);
    } break;
  }
}
int main(){
    stdio_init_all();
    sleep_ms(10000);
    size_t psram_sz = sfe_setup_psram(SFE_RP2350_XIP_CSI_PIN);
    if(psram_sz == 0){
        for(;;){
            printf("PSRAM not detected");
        };
    }
    if(sfe_pico_alloc_init()){
        printf("PSRAM Initialized\n");
    };
    WrenConfiguration config;
    wrenInitConfiguration(&config);
    config.writeFn = &write_fn;
    config.errorFn = &error_fn;
    config.reallocateFn = &reallocate_psram;
    WrenVM* vm = wrenNewVM(&config);
    const char* module = "main";
    //const char* script = "while(true){System.print(\"I am running in a VM!\")}";
    const char* script = "System.print(\"I am running in a VM!\")";
    WrenInterpretResult result = wrenInterpret(vm, module, script);
    switch (result) {
        case WREN_RESULT_COMPILE_ERROR:
        { printf("Compile Error!\n"); } break;
        case WREN_RESULT_RUNTIME_ERROR:
        { printf("Runtime Error!\n"); } break;
        case WREN_RESULT_SUCCESS:
        { printf("Success!\n"); } break;
    }

    wrenFreeVM(vm);
    for(;;){}
    /*while (true) {
        printf("Hello, world!\n");
        sleep_ms(1000);
    }*/
}
