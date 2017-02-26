
#pragma once

#include "brigand/brigand.hpp"

/* RTFM system/MCU configuration. */
#include "rtfm_system_config.hpp"

#include "rtfm/details/interrupts.hpp"
#include "rtfm/details/job_resource.hpp"
#include "rtfm/details/job_resource_methods.hpp"
#include "rtfm/srp/srp_locks.hpp"
#include "rtfm/srp/srp_pend_clear.hpp"
#include "rtfm/rtfm_clock.hpp"
#include "rtfm/srp/srp_init.hpp"
#include "rtfm/rtfm_timer.hpp"

/* RTFM job/system_job_list configuration. */
#include "rtfm_user_config.hpp"

/* Needs to be included after the definition of the rtfm::system_job_list. */
#include "rtfm/srp/srp_prioirty_ceiling.hpp"


namespace rtfm
{
namespace srp
{

/**
 * @brief  A convenience definition of a lock in the SRP version of RTFM++.
 *
 * @tparam Resource   The resource to lock.
 */
template <typename Resource>
using lock = lock_impl< get_priority_ceiling< system_job_list, Resource > >;

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

#include "rtfm/srp/srp_async.hpp"
