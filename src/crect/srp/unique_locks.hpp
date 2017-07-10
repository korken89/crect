//          Copyright Emil Fresk 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.md or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include "kvasir/mpl/mpl.hpp"
#include "crect/details/arm_intrinsics.hpp"
#include "crect/utils.hpp"
#include "crect/srp/unique.hpp"

namespace crect
{
/**
 * @brief  The definition of a unique lock.
 *
 * @tparam ISRindex   The integral_constant which has the ISR index to lock.
 */
template <typename Resource>
inline void unique_lock()
{
  using Job = get_unique_job_from_resource<crect::system_job_list, Resource>;
  using ISRn = typename Job::isr::index;

  /* Lock the resource by disabling the ISR. */
  NVIC->ICER[ISRn::value >> 5UL] = (1UL << (ISRn::value & 0x1FUL));

  /* Barriers to guarantee the instruction took hold before continuing. */
  arm_intrinsics::barrier_entry_with_sync();
}

/**
 * @brief  The definition of a unique unlock.
 *
 * @tparam ISRindex   The integral_constant which has the ISR index to unlock.
 */
template <typename Resource>
inline void unique_unlock()
{
  using Job = get_unique_job_from_resource<crect::system_job_list, Resource>;
  using ISRn = typename Job::isr::index;

  /* Barriers to guarantee no reordering before continuing. */
  arm_intrinsics::barrier_exit();

  /* Unlock the resource. */
  NVIC->ISER[ISRn::value >> 5UL] = (1UL << (ISRn::value & 0x1FUL));
}

} /* END namespace crect */
