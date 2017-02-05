
#pragma once

namespace rtfm
{
namespace core
{

/**
 * @brief  Start of a instruction and data barrier, used to guarantee the
 *         memory order for critical operations.
 */
__attribute__((always_inline)) inline void barrier_entry()
{
  asm volatile("dsb 0xF\nisb 0xF\n" ::: "memory");
}

/**
 * @brief  End of a instruction and data barrier, used to guarantee the
 *         memory order for critical operations.
 */
__attribute__((always_inline)) inline void barrier_exit()
{
  asm volatile("" ::: "memory");
}

} /* END namespace core */
} /* END namespace rtfm */
