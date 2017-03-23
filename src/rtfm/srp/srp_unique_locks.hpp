
#pragma once

#include "kvasir/mpl/mpl.hpp"
#include "rtfm/details/arm_intrinsics.hpp"
#include "rtfm/rtfm_utils.hpp"

namespace rtfm
{
namespace srp
{
/**
 * @brief  The definition of a unique lock.
 *
 * @tparam ISRindex   The integral_constant which has the ISR index to lock.
 */
template <typename ISRindex>
inline void unique_lock_impl()
{
  /* Lock the resource by disabling the ISR. */


  /* Barriers to guarantee the instruction took hold before continuing. */
  arm_intrinsics::barrier_entry_with_sync();
}

/**
 * @brief  The definition of a unique unlock.
 *
 * @tparam ISRindex   The integral_constant which has the ISR index to lock.
 */
template <typename ISRindex>
inline void unique_unlock_impl()
{
  /* Barriers to guarantee no reordering before continuing. */
  arm_intrinsics::barrier_exit();

  /* Unlock the resource. */

}
};

} /* END namespace srp */
} /* END namespace rtfm */
