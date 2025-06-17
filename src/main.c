#include <stdio.h>
#include <string.h>
#include "pico/scanvideo.h"
#include "pico/scanvideo/composable_scanline.h"
#include "sparkfun_pico/sfe_pico.h"
#include "pico/stdlib.h"
#include "hardware/dma.h"
#include "hardware/uart.h"
#include "wren/src/include/wren.h"
#include "wren_interface.h"
#include "gfx.h"
#define VGA_MODE vga_mode_160x120_60

extern const struct scanvideo_pio_program video_24mhz_composable;

// UART defines
// By default the stdout UART is `uart0`, so we will use the second one
#define UART_ID uart1
#define BAUD_RATE 115200

// Use pins 4 and 5 for UART1
// Pins can be changed, see the GPIO function select table in the datasheet for information on GPIO assignments
#define UART_TX_PIN 4
#define UART_RX_PIN 5

static uint16_t frame_buffer0[SCREEN_H*SCREEN_W] = {0xFFFF};
//static uint16_t frame_buffer1[SCREEN_H*SCREEN_W] = {0x7FFF};
static bool which_bufer = false;
void fill_scanline_buffer(struct scanvideo_scanline_buffer *buffer){
        static uint32_t postamble[] = {
            0x0000u | (COMPOSABLE_EOL_ALIGN << 16)
        };  
      
        buffer->data[0] = 4;
        buffer->data[1] = host_safe_hw_ptr(buffer->data+8);
        buffer->data[2] = (SCREEN_W - 4)/2;
        uint16_t *pixels = frame_buffer0 + (scanvideo_scanline_number(buffer->scanline_id) * SCREEN_W);
        buffer->data[3] = host_safe_hw_ptr(pixels+4);  
        buffer->data[4] = count_of(postamble);
        buffer->data[5] = host_safe_hw_ptr(postamble);
        buffer->data[6] = 0;
        buffer->data[7] = 0;
        buffer->data_used = 8;
        buffer->data[8] = (pixels[0] << 16u) | COMPOSABLE_RAW_RUN;
        buffer->data[9] = (pixels[1] << 16u) | 0;
        buffer->data[10] = (COMPOSABLE_RAW_RUN << 16u) | pixels[2];
        buffer->data[11] = (((SCREEN_W-3) + 1 - 3) << 16u) | pixels[3];
        buffer->status = SCANLINE_OK;
        //scanvideo_end_scanline_generation(scanline_buffer);
};
int main(){
    stdio_init_all();
    sleep_ms(10000);
    /*
    WrenConfiguration config;
    WrenVM* vm = start_wren(config);
    run_wren(vm);
    wrenFreeVM(vm);
    */
    scanvideo_setup(&VGA_MODE);
    scanvideo_timing_enable(true);
    Point p0 = {
      1, 1, 1.0f
    };
    Point p1 = {
      50,50, 1.0f
    };
    Point p2 = {
      20, 90, 1.0f
    };
    Color c = {
      255,255,255
    };
    //draw_line(p0,p1,0x000F, frame_buffer0);
    //draw_line(p1, p2, 0x03E0, frame_buffer0);
    draw_shaded_tri(p0,p1,p2, c, frame_buffer0);
    //draw_wireframe_tri(p0,p1,p2, c, frame_buffer0);
    for(;;){
      scanvideo_wait_for_vblank();
      struct scanvideo_scanline_buffer *buffer = scanvideo_begin_scanline_generation(false);
      while(buffer){
        fill_scanline_buffer(buffer);
        scanvideo_end_scanline_generation(buffer);
        buffer = scanvideo_begin_scanline_generation(true);
      } 
    }
}
