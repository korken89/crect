
#pragma once

#include "kvasir/mpl/mpl.hpp"

/* RTFM system/MCU configuration. */
#include "rtfm_system_config.hpp"

#include "rtfm/async/async_queue.hpp"
#include "rtfm/details/interrupts.hpp"
#include "rtfm/details/job_resource.hpp"
#include "rtfm/details/job_resource_methods.hpp"
#include "rtfm/rtfm_clock.hpp"
#include "rtfm/rtfm_timer.hpp"
#include "rtfm/srp/srp_init.hpp"
#include "rtfm/srp/srp_locks.hpp"
#include "rtfm/srp/srp_pend_clear.hpp"
#include "rtfm/srp/srp_prioirty_ceiling.hpp"

/* Async resource definition for now... */
extern rtfm::async_queue<__RTFM_ASYNC_QUEUE_SIZE> rtfm_async_queue;
namespace rtfm
{
struct async_resource;
using Rasync =
    resource<async_resource,
             kvasir::mpl::integral_constant<decltype(&rtfm_async_queue),
                                            &rtfm_async_queue>,
             false>;

using Jasync =
    rtfm::job<rtfm::util::hashit("JobAsync"),     // Unique ID
              0,                                  // Priority
              rtfm::MakeSystemISR<SysTick_IRQn>,  // ISR connection and location
              rtfm::Rasync, rtfm::Rsystem_clock   // Possible resource claims
              >;
}

/* RTFM job and user_job_list configuration. */
#include "rtfm_user_config.hpp"

namespace rtfm
{
/** Create the system job list. */
using system_job_list =
    kvasir::mpl::flatten<kvasir::mpl::list<Jasync, user_job_list>>;

namespace srp
{
/**
 * @brief  A convenience definition of a lock.
 *
 * @tparam Resource   The resource to lock.
 */
template <typename Resource>
using lock = lock_impl<get_priority_ceiling<system_job_list, Resource>>;

/**
 * @brief  A convenience definition of the initialization of RTFM.
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

} /* END namespace srp */
} /* END namespace rtfm */

#include "rtfm/srp/srp_claim.hpp"
#include "rtfm/srp/srp_async.hpp"
