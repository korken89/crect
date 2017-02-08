
#pragma once

namespace rtfm
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
 * @param [in] value    Base Priority value to set
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
 * @param [in]    basePri  Base Priority value to set
 */
__attribute__((always_inline))
static inline void set_BASEPRI_MAX(uint32_t value)
{
  asm volatile ("MSR basepri_max, %0" : : "r" (value) : "memory");
}


/**
 * @brief  Start of a instruction and data barrier, used to guarantee the
 *         memory order for critical operations.
 */
__attribute__((always_inline)) static inline void barrier_entry()
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
} /* END namespace rtfm */
