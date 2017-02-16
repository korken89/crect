
#pragma once

/*
 * Place MCU definition file here.
 *
 * Needs to define:
 * - __NVIC_PRIO_BITS     Number of priority bits
 * - __STACK_END          Address for the end of the stack
 * - __MCU_NUM_VECTORS    Number of peripheral ISR vectors (not including ARM's)
 * - __F_CPU              Clock speed of the MCU (needed for time conversions)
 */
#include "stm32f411xe.h"

/**
 * @brief Symbols exported by the linker script:
 */
extern uint32_t __preinit_array_start, __preinit_array_end, __init_array_start;
extern uint32_t __init_array_end, __fini_array_start, __fini_array_end;
extern uint32_t __text_end, __data_start, __data_end, __bss_start, __bss_end;
extern uint32_t __all_end, __stack;


#define __STACK_END         &__stack
#define __MCU_NUM_VECTORS   (SPI5_IRQn+1)
#define __F_CPU             100000000

/****************************************************************************
 * End MCU definitions.
 ****************************************************************************/
