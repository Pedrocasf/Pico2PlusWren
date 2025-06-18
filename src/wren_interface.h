#ifndef WREN_INTERFACE_H
#define WREN_INTERFACE_H
#include <stdio.h>
#include "sparkfun_pico/sfe_pico.h"
#include "wren.h"
WrenVM* start_wren(WrenConfiguration config);
void run_wren(WrenVM* vm);
#endif