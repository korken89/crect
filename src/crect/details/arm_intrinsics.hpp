//          Copyright Emil Fresk 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.md or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <cstdint>

namespace crect
{

/**
 * @brief   Namespace for the ARM intrinsics needed. The CMSIS library changes
 *          too much and does not seem to be stable, this is to cut the
 *          dependency on it.
 */
namespace arm_intrinsics
{

/**
 * @brief     Get Base Priority
 * @details   Returns the current value of the Base Priority register.
 *
 * @return    Base Priority register value
 */
__attribute__((always_inline))
static inline uint32_t get_BASEPRI(void)
{
  uint32_t result;

  asm volatile ("MRS %0, basepri" : "=r" (result) );
  return(result);
}


/**
 * @brief     Set Base Priority
 * @details   Assigns the given value to the Base Priority register.
 *
 * @param[in] value     Base Priority value to set.
 */
__attribute__((always_inline))
static inline void set_BASEPRI(uint32_t value)
{
  asm volatile ("MSR basepri, %0" : : "r" (value) : "memory");
}


/**
 * @brief     Set Base Priority with condition
 * @details   Assigns the given value to the Base Priority register only if
 *            BASEPRI masking is disabled, or the new value increases the
 *            BASEPRI priority level.
 *
 * @param[in] value     Base Priority value to set, if above current value.
 */
__attribute__((always_inline))
static inline void set_BASEPRI_MAX(uint32_t value)
{
  asm volatile ("MSR basepri_max, %0" : : "r" (value) : "memory");
}

/**
 * @brief     Set Base Priority
 * @details   Assigns the given value to the Base Priority register.
 *
 * @tparam I    Base Priority value to set.
 */
template <unsigned I>
__attribute__((always_inline))
static inline void set_BASEPRI(void)
{
  asm volatile ("MSR basepri, %0" : : "r" (I) : "memory");
}


/**
 * @brief     Set Base Priority with condition
 * @details   Assigns the given value to the Base Priority register only if
 *            BASEPRI masking is disabled, or the new value increases the
 *            BASEPRI priority level.
 *
 * @tparam I    Base Priority value to set, if above current value.
 */
template <unsigned I>
__attribute__((always_inline))
static inline void set_BASEPRI_MAX(void)
{
  asm volatile ("MSR basepri_max, %0" :: "r" (I) : "memory");
}

/**
 * @brief     Get PRIMASK
 * @details   Returns the current value of the PRIMASK register.
 *
 * @return    PRIMASK register value
 */
__attribute__((always_inline)) static inline uint32_t get_PRIMASK(void)
{
  uint32_t result;
  asm volatile("MRS %0, primask" : "=r"(result) : : "memory");
  return result;
}

/**
 * @brief     Set PRIMASK
 * @details   Assigns the given value to the PRIMASK register.
 *
 * @param primask    PRIMASK value to set.
 */
__attribute__((always_inline)) static inline void set_PRIMASK(uint32_t primask)
{
  asm volatile("MSR primask, %0" : : "r"(primask) : "memory");
}

/**
 * @brief     Get IPSR
 * @details   Returns the current value of the IPSR register.
 *
 * @return    IPSR register value
 */
__attribute__((always_inline))
static inline uint32_t get_IPSR(void)
{
  uint32_t result;

  asm volatile ("MRS %0, ipsr" : "=r" (result) );
  return(result);
}

/**
 * @brief   Enable IRQ Interrupts
 */
__attribute__((always_inline))
static inline void enable_interrupts(void)
{
  asm volatile ("cpsie i" ::: "memory");
}


/**
 * @brief   Disable IRQ Interrupts
 */
__attribute__((always_inline))
static inline void disable_interrupts(void)
{
  asm volatile ("cpsid i" ::: "memory");
}

/**
 * @brief  Start of a instruction and data barrier, used to guarantee the
 *         memory order for critical operations.
 */
__attribute__((always_inline)) static inline void barrier_entry()
{
  /* According to the "ARM Cortex-M Programming Guide to Memory Barrier
   * Instructions - Application Note 321", paragraph 4.8, the barrier
   * instructions are not needed for Cortex-M >= M3.
   */
#if __CORTEX_M  >= 3
  /* No synchronization as >= Cortex-M3 uses the MSR instruction to manipulate
   * priorities.
   */
  asm volatile("" ::: "memory");
#else
  /* Synchronization instructions are only needed with the Cortex-M0, as it is
   * using source masking in the NVIC to manipulate priorities.
   */
  asm volatile("dsb 0xF\nisb 0xF\n" ::: "memory");
#endif
}

/**
 * @brief  Start of a instruction and data barrier with forced synchronization,
 *         used to guarantee the memory order for critical operations.
 */
__attribute__((always_inline)) static inline void barrier_entry_with_sync()
{
  asm volatile("dsb 0xF\nisb 0xF\n" ::: "memory");
}

/**
 * @brief  End of a instruction and data barrier, used to guarantee the
 *         memory order for critical operations.
 */
__attribute__((always_inline)) static inline void barrier_exit()
{
  asm volatile("" ::: "memory");
}

} /* END namespace arm_intrinsics */
} /* END namespace crect */
