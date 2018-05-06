//          Copyright Emil Fresk 2017-2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.md or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <utility>
#include <cstdint>
#include "crect/details/arm_intrinsics.hpp"

namespace crect
{
/**
 * @brief  Critical lock by disabling all interrupts
 */
class critical_lock
{
private:
  /**
   * @brief  Alias of the type of the PRIMASK register.
   */
  using primask_t = decltype(arm_intrinsics::get_PRIMASK());

  /**
   * @brief  Holder of old PRIMASK while the lock is active.
   */
  const primask_t _old_primask;

public:
  /**
   * @brief  The constructor locks by manipulating PRIMASK.
   */
  __attribute__((always_inline)) critical_lock()
      : _old_primask(arm_intrinsics::get_PRIMASK())
  {
    arm_intrinsics::disable_interrupts();
  }

  /**
   * @brief  The destructor releases the resource by restoring BASEPRI.
   */
  __attribute__((always_inline)) ~critical_lock()
  {
    arm_intrinsics::set_PRIMASK(_old_primask);
  }
};

// Wrap critical section functions with a lock to guarantee correct
// instantiation of instructions
template < typename Fun >
auto critical_section(Fun&& f)
{
  critical_lock lock{};
  return std::forward< Fun >(f)();
}

} /* END namespace crect */
