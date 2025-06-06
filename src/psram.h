#ifndef PSRAM_H
#define PSRAM_H
#define PSRAM_BASE (0x11000000)
#define PSRAM_SIZE (8 * 1024 * 1024)
extern void __no_inline_not_in_flash_func(setup_psram)();
#endif