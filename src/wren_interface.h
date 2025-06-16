#ifndef WREN_INTERFACE_H
#define WREN_INTERFACE_H
#include <stdio.h>
#include "sparkfun_pico/sfe_pico.h"
#include "wren.h"
void* reallocate_psram(void* ptr, size_t new_size, void* _);
WrenVM* start_wren(WrenConfiguration config);
void run_wren(WrenVM* vm);
#endif