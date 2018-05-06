//          Copyright Emil Fresk 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.md or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include "kvasir/mpl/mpl.hpp"
#include "crect/details/arm_intrinsics.hpp"
#include "crect/utils.hpp"
#include "crect/srp/critical_sections.hpp"

namespace crect
{
/**
 * @brief  The definition of a lock in SRP using BASEPRI in Cortex-M3 and above.
 *
 * @tparam ResourcePriority   The integral_constant which has the priority
 *                            value to lock.
 */
template < typename ResourcePriority >
class lock_impl_basepri
{
private:
  /**
   * @brief  Alias of the type of the BASEPRI register.
   */
  using basepri_t = decltype(arm_intrinsics::get_BASEPRI());

  /**
   * @brief  Holder of old BASEPRI while the lock is active.
   */
  const basepri_t _old_basepri;

public:
  /**
   * @brief  The constructor locks the resource by manipulating BASEPRI.
   */
  lock_impl_basepri() : _old_basepri(arm_intrinsics::get_BASEPRI())
  /* Save old BASEPRI before execution of constructor body as per
   * C++ Standard §12.6.2 */
  {
#if __CORTEX_M == 7U
    // Fix for the Cortex-M7 Errata: setting of BASEPRI must be done with all
    // ISRs disabled
    critical_lock lock{};
#endif

    /* Lock the resource. */
    arm_intrinsics::set_BASEPRI_MAX< util::priority_to_NVIC_priority(
        ResourcePriority::value) >();

    /* Barriers to guarantee the instruction took hold before continuing. */
    arm_intrinsics::barrier_entry();
  }

  /**
   * @brief  The destructor releases the resource by restoring BASEPRI.
   */
  ~lock_impl_basepri()
  {
    /* Barriers to guarantee no reordering before continuing. */
    arm_intrinsics::barrier_exit();

    /* Unlock the resource. */
    arm_intrinsics::set_BASEPRI(_old_basepri);
  }
};

/**
 * @brief  The definition of a lock in SRP using source masking for
 *         Cortex-M0(+).
 *
 * @tparam ResourceMask   The integral_constant which has the source mask to
 *                        lock.
 */
template < typename ResourceMask >
class lock_impl_source_mask
{
private:
  /**
   * @brief  Alias of the type of the mask.
   */
  using mask_t = decltype(NVIC->ISER[0]);

  /**
   * @brief  Holder of old mask while the lock is active.
   */
  const mask_t _old_mask;

public:
  /**
   * @brief  The constructor locks the resource by manipulating the NVIC
   *         interrupt clear enable mask.
   */
  lock_impl_source_mask() : _old_mask(NVIC->ICER[0])
  {
    /* Lock the resource by clearing the mask. */
    NVIC->ICER[0] = ResourceMask::value;

    /* Barriers to guarantee the instruction took hold before continuing. */
    arm_intrinsics::barrier_entry();
  }

  /**
   * @brief  The destructor releases the resource by restoring the mask.
   */
  ~lock_impl_source_mask()
  {
    /* Barriers to guarantee no reordering before continuing. */
    arm_intrinsics::barrier_exit();

    /* Unlock the resource by restoring the mask. */
    NVIC->ISER[0] = _old_mask;
  }
};

} /* END namespace crect */
