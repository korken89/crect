
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
 * @brief Stack definition.
 */
extern uint32_t __stack;


#define __STACK_END         &__stack
#define __MCU_NUM_VECTORS   (SPI5_IRQn+1)
#define __F_CPU             100000000

/****************************************************************************
 * End MCU definitions.
 ****************************************************************************/
