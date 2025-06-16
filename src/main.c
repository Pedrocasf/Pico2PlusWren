#define SFE_RP2350_XIP_CSI_PIN 47
#include <stdio.h>
#include "pico/scanvideo.h"
#include "pico/scanvideo/composable_scanline.h"
#include "sparkfun_pico/sfe_pico.h"
#include "pico/stdlib.h"
#include "hardware/dma.h"
#include "hardware/uart.h"
#include "wren/src/include/wren.h"
#define VGA_MODE vga_mode_640x480_60
extern const struct scanvideo_pio_program video_24mhz_composable;

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
#define MIN_COLOR_RUN 3
int32_t single_color_scanline(uint32_t *buf, size_t buf_length, int width, uint32_t color16) {
    assert(buf_length >= 2);

    assert(width >= MIN_COLOR_RUN);
    // | jmp color_run | color | count-3 |  buf[0] =
    buf[0] = COMPOSABLE_COLOR_RUN | (color16 << 16);
    buf[1] = (width - MIN_COLOR_RUN) | (COMPOSABLE_RAW_1P << 16);
    // note we must end with a black pixel
    buf[2] = 0 | (COMPOSABLE_EOL_ALIGN << 16);

    return 3;
}

void render_scanline(struct scanvideo_scanline_buffer *dest) {
    uint32_t *buf = dest->data;
    size_t buf_length = dest->data_max;

    int l = scanvideo_scanline_number(dest->scanline_id);
    uint16_t bgcolour = (uint16_t) l << 2;
    dest->data_used = single_color_scanline(buf, buf_length, VGA_MODE.width, bgcolour);
    dest->status = SCANLINE_OK;
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
    scanvideo_setup(&VGA_MODE);
    scanvideo_timing_enable(true);
    for(;;){
      struct scanvideo_scanline_buffer *scanline_buffer = scanvideo_begin_scanline_generation(true);
      render_scanline(scanline_buffer);
      scanvideo_end_scanline_generation(scanline_buffer);
    }
    /*while (true) {
        printf("Hello, world!\n");
        sleep_ms(1000);
    }*/
}
