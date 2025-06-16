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

static uint16_t frame_buffer0[SCREEN_H*SCREEN_W] = {0x7FFF};
static uint16_t frame_buffer1[SCREEN_H*SCREEN_W] = {0x7FFF};
static bool which_bufer = false;

int main(){
    stdio_init_all();
    sleep_ms(10000);
    WrenConfiguration config;
    WrenVM* vm = start_wren(config);
    run_wren(vm);


    wrenFreeVM(vm);
    scanvideo_setup(&VGA_MODE);
    scanvideo_timing_enable(true);
    Point p0 = {
      -200,-250
    };
    Point p1 = {
      200,50
    };
    Point p2 = {
      20, 250
    };
    //draw_line(p0,p1,0xFFFF, frame_buffer0);
    draw_wireframe_tri(p0,p1,p2, 0xFFFF, frame_buffer0);
    for(;;){
      scanvideo_wait_for_vblank();
      if (which_bufer)
      {
        for (int y = 0;y<VGA_MODE.height;y++){
          scanvideo_scanline_buffer_t *scanline_buffer = scanvideo_begin_scanline_generation(true);
          uint16_t *p = (uint16_t *) scanline_buffer->data;
          *p++ = COMPOSABLE_RAW_RUN;
          *p++ = frame_buffer1[y* SCREEN_W];
          *p++ = VGA_MODE.width;
          memcpy(p, &frame_buffer1[y*SCREEN_W], sizeof(frame_buffer1[y])*SCREEN_W);
          p[VGA_MODE.width+3]  = COMPOSABLE_EOL_SKIP_ALIGN;
          p[VGA_MODE.width+4]  = 0;
          scanline_buffer->data_used = (VGA_MODE.width + 6)>>1;
          assert(scanline_buffer->data_used < scanline_buffer->data_max);
          scanline_buffer->status = SCANLINE_OK;
          scanvideo_end_scanline_generation(scanline_buffer);
        }
      }else{
        for (int y = 0;y<VGA_MODE.height;y++){
        scanvideo_scanline_buffer_t *scanline_buffer = scanvideo_begin_scanline_generation(true);
        uint16_t *p = (uint16_t *) scanline_buffer->data;
        *p++ = COMPOSABLE_RAW_RUN;
        *p++ = frame_buffer0[y* SCREEN_W];
        *p++ = VGA_MODE.width;
        memcpy(p, &frame_buffer0[y*SCREEN_W], sizeof(frame_buffer0[y])*SCREEN_W);
        p[VGA_MODE.width+3]  = COMPOSABLE_EOL_SKIP_ALIGN;
        p[VGA_MODE.width+4]  = 0;
        scanline_buffer->data_used = (VGA_MODE.width + 6)>>1;
        assert(scanline_buffer->data_used < scanline_buffer->data_max);
        scanline_buffer->status = SCANLINE_OK;
        scanvideo_end_scanline_generation(scanline_buffer);
        }
      }
      
      
      /*
      struct scanvideo_scanline_buffer *scanline_buffer = scanvideo_begin_scanline_generation(true);
      render_scanline(scanline_buffer);
      scanvideo_end_scanline_generation(scanline_buffer);
      */
    }
    /*while (true) {
        printf("Hello, world!\n");
        sleep_ms(1000);
    }*/
}
