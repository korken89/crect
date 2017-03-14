
#pragma once

#include "kvasir/mpl/mpl.hpp"
#include "rtfm/details/arm_intrinsics.hpp"
#include "rtfm/rtfm_utils.hpp"

namespace rtfm
{
namespace srp
{

/**
 * @brief  The definition of a lock in the SRP version of RTFM++.
 *
 * @tparam ResourcePriority   The integral_constant which has the priority
 *                            value to lock.
 */
template <typename ResourcePriority>
class lock_impl
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
  lock_impl() : _old_basepri( arm_intrinsics::get_BASEPRI() )
    /* Save old BASEPRI before execution of constructor body as per
     * C++ Standard §12.6.2 */
  {
    /* Lock the resource. */
    arm_intrinsics::set_BASEPRI_MAX<
      util::priority_to_NVIC_priority( ResourcePriority::value )
    >();

    /* Barriers to guarantee the instruction took hold before continuing. */
    arm_intrinsics::barrier_entry();
  }

  /**
   * @brief  The destructor releases the resource by restoring BASEPRI.
   */
  ~lock_impl()
  {
    /* Barriers to guarantee no reordering before continuing. */
    arm_intrinsics::barrier_exit();

    /* Unlock the resource. */
    arm_intrinsics::set_BASEPRI(_old_basepri);
  }
};

} /* END namespace srp */
} /* END namespace rtfm */
