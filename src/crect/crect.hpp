//          Copyright Emil Fresk 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.md or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include "kvasir/mpl/mpl.hpp"

/* crect system/MCU configuration. */
#include "crect_system_config.hpp"

#include "crect/async/async_queue.hpp"
#include "crect/details/interrupts.hpp"
#include "crect/details/job_resource_definitions.hpp"
#include "crect/details/resource_tree.hpp"
#include "crect/clock.hpp"
#include "crect/timer.hpp"
#include "crect/srp/init.hpp"
#include "crect/srp/locks.hpp"
#include "crect/srp/pend_clear.hpp"
#include "crect/srp/prioirty_ceiling.hpp"
#include "crect/srp/source_masking.hpp"
#include "crect/srp/unique.hpp"

/* Async resource definition for now... */
extern crect::async_queue<__CRECT_ASYNC_QUEUE_SIZE> crect_async_queue;

namespace crect
{
using Rasync =
    resource<kvasir::mpl::integral_constant<decltype(&crect_async_queue),
                                            &crect_async_queue>,
             false>;

using Jasync =
    job<0,                                // Priority
        make_system_isr<SysTick_IRQn>,    // ISR connection and location
        Rasync, Rsystem_clock             // Possible resource claims
       >;
}

/* crect job and user_job_list configuration. */
#include "crect_user_config.hpp"

namespace crect
{

/** Create the system job list. */
using system_job_list =
    kvasir::mpl::eager::flatten<kvasir::mpl::list<Jasync, user_job_list>>;

/** Check the system job list for unique resources. */
static_assert(is_unique_job_list< system_job_list >::value == true,
              "A unique resource is claimed by multiple jobs.");

/**
 * @brief  A convenience definition of a lock.
 *
 * @tparam Resource   The resource to lock.
 */
template <typename Resource>
#if __CORTEX_M  >= 3
/* BASEPRI based for Cortex-M3 and above */
using lock = lock_impl_basepri<get_priority_ceiling<system_job_list,
                                                    Resource>>;
#else
/* Source masking based for Cortex-M0(+) */
using lock = lock_impl_basepri<get_source_masking<system_job_list,
                                                  Resource>>;
#endif

/**
 * @brief  A convenience definition of the initialization of crect.
 */
inline void initialize()
{
  __disable_irq();

  /* Initialize the NVIC settings, enable ISRs, set priorities. */
  initialize_jobs_impl<system_job_list>();

  /* Initialize SysTick */
  timer::initialize();

  __enable_irq();
}

} /* END namespace crect */

#include "crect/srp/unique_locks.hpp"
#include "crect/srp/claim.hpp"
#include "crect/async.hpp"

