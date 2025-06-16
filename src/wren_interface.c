#include "wren_interface.h"
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
WrenVM* start_wren(WrenConfiguration config){
    wrenInitConfiguration(&config);
    config.writeFn = &write_fn;
    config.errorFn = &error_fn;
    config.reallocateFn = &reallocate_psram;
    WrenVM* vm = wrenNewVM(&config);
    return vm;
}
void run_wren(WrenVM* vm){
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
}
void* reallocate_psram(void* ptr, size_t new_size, void* _){
    if(new_size == 0){
        sfe_mem_free(ptr);
        return NULL;
    }
    return sfe_mem_realloc(ptr, new_size);
}