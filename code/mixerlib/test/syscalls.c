#include <stdint.h>

__attribute__((weak)) uint8_t _estack = 0x20000000 + 32768; // This seems to work for testing purposes. DO NOT INCLUDE THIS FILE IN PRODUCTION !!!!!!!!!!!!!!!!!!!!!!
__attribute__((weak)) uint32_t _Min_Stack_Size = 0x800; /* required amount of stack */
