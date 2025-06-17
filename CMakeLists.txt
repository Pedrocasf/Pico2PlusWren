# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)

# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.1.1)
set(toolchainVersion 14_2_Rel1)
set(picotoolVersion 2.1.1)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
set(PICO_BOARD vgaboard CACHE STRING "Board type")
set(PICO_PLATFORM rp2350)
set(PICO_CYW43_SUPPORTED 1)
set(PICO_EXTRAS_FETCH_FROM_GIT 1)
add_compile_definitions(SFE_RP2350_XIP_CSI_PIN=47)
# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)
include(pico_extras_import.cmake)

project(Pico2PlusWren C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1

add_executable(Pico2PlusWren 
            src/main.c
            src/psram.c
            src/wren_interface.c
            src/gfx.c
)

pico_set_program_name(Pico2PlusWren "Pico2PlusWren")
pico_set_program_version(Pico2PlusWren "0.1")

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(Pico2PlusWren 1)
pico_enable_stdio_usb(Pico2PlusWren 1)
add_subdirectory(sparkfun_pico)
include_directories(
    ./wren/src/include
)
add_subdirectory(./wren)

# Add the standard library to the build
target_link_libraries(Pico2PlusWren
        pico_multicore
        pico_stdlib
        pico_scanvideo_dpi
        sparkfun_pico
        WrenVM)

# Add the standard include files to the build
target_include_directories(Pico2PlusWren PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
)

# Add any user requested libraries
target_link_libraries(Pico2PlusWren 
        hardware_dma
        hardware_spi
        hardware_flash
        )

pico_add_extra_outputs(Pico2PlusWren)

target_compile_definitions(Pico2PlusWren PRIVATE
                PICO_USE_SW_SPIN_LOCKS=0
                PICO_SCANVIDEO_PLANE1_VARIABLE_FRAGMENT_DMA=1
        )