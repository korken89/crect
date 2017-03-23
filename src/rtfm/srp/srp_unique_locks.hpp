
#pragma once

#include "kvasir/mpl/mpl.hpp"
#include "rtfm/details/arm_intrinsics.hpp"
#include "rtfm/rtfm_utils.hpp"
#include "rtfm/srp/srp_unique.hpp"

namespace rtfm
{
namespace srp
{
/**
 * @brief  The definition of a unique lock.
 *
 * @tparam ISRindex   The integral_constant which has the ISR index to lock.
 */
template <typename Resource>
inline void unique_lock()
{
  using Job = get_unique_job_from_resource<rtfm::system_job_list, Resource>;
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
  using Job = get_unique_job_from_resource<rtfm::system_job_list, Resource>;
  using ISRn = typename Job::isr::index;

  /* Barriers to guarantee no reordering before continuing. */
  arm_intrinsics::barrier_exit();

  /* Unlock the resource. */
  NVIC->ISER[ISRn::value >> 5UL] = (1UL << (ISRn::value & 0x1FUL));

}

} /* END namespace srp */
} /* END namespace rtfm */
