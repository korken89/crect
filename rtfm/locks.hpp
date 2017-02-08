
#pragma once

#include <cstdint>
#include "brigand/brigand.hpp"
#include "rtfm/arm_intrinsics.hpp"

/* TODO: Only for testing, shall be removed. */
#ifndef __NVIC_PRIO_BITS
# define __NVIC_PRIO_BITS 3
#endif

namespace rtfm
{
namespace srp
{

/**
 * @brief   Takes a user priority (0 = lowest, increasing numbers means higher
 *          priority) and transforms to NVIC priority (0 = highest, increasing
 *          numbers means lower priority).
 *
 * @return  The transformed priority.
 */
template <typename T>
constexpr T priority_to_NVIC_priority(T priority)
{
  auto N = (1 << __NVIC_PRIO_BITS) - 1;
  return ((N - priority) << (8 - __NVIC_PRIO_BITS)) & 0xFF;
}

/**
 * @brief  The definition of a lock in the SRP version of RTFM++.
 *
 * @tparam ResourcePriority   The integral_constant which has the priority
 *                            value to lock.
 */
template <typename ResourcePriority>
class lock
{
private:
  /**
   * @brief  Alias of the type of the BASEPRI register.
   */
  using basepri_t = decltype( arm_intrinsics::get_BASEPRI() );

  /**
   * @brief  Holder of old BASEPRI while the lock is active.
   */
  const basepri_t _old_basepri;

public:
  /**
   * @brief  The constructor locks the resource by manipulating BASEPRI.
   */
  lock() : _old_basepri( arm_intrinsics::get_BASEPRI() )
    /* Save old BASEPRI before execution of constructor body as per
     * C++ Standard §12.6.2 */
  {
    /* Lock the resource. */
    arm_intrinsics::set_BASEPRI_MAX(
      priority_to_NVIC_priority( ResourcePriority::value )
    );

    /* Barriers to guarantee the instruction took hold before continuing. */
    arm_intrinsics::barrier_entry();
  }

  /**
   * @brief  The destructor releases the resource by restoring BASEPRI.
   */
  ~lock()
  {
    /* Barriers to guarantee no reordering before continuing. */
    arm_intrinsics::barrier_exit();

    /* Unlock the resource. */
    arm_intrinsics::set_BASEPRI(_old_basepri);
  }
};

} /* END namespace srp */
} /* END namespace rtfm */
