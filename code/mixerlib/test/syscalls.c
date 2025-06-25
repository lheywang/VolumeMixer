#include <stdint.h>

/**
 * This file provide overhide for basic stuff, needed to compile.
 *
 * They're aren't included in the build.
 */
__attribute__((weak)) uint8_t _estack = 0x00000000 + 0xFF; // This seems to work for testing purposes. DO NOT INCLUDE THIS FILE IN PRODUCTION !!!!!!!!!!!!!!!!!!!!!!
__attribute__((weak)) uint32_t _Min_Stack_Size = 0x800; /* required amount of stack */
