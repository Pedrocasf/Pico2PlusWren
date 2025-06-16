#include "psram.h"
void psram_init(){
    size_t psram_sz = sfe_setup_psram(SFE_RP2350_XIP_CSI_PIN);
    if(psram_sz == 0){
        for(;;){
            printf("PSRAM not detected");
        };
    }
    if(sfe_pico_alloc_init()){
        printf("PSRAM Initialized\n");
    };
}