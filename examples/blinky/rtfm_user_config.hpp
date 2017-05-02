
#pragma once

#include "rtfm/rtfm_srp.hpp"

/****************************************************************************
 * User Job and Resource defines here.
 ****************************************************************************/

#include "rtfm/rtfm_utils.hpp"
#include "led.hpp"

void job1(void);
void job2(void);

using Rled = rtfm::make_resource<
    RTFM_OBJECT_LINK(led_resource)  // Pointer to some object to be protected
  >;

using J1 = rtfm::job<
              1,                          // Priority
              rtfm::make_isr<job1, 1>,    // ISR connection and location
              Rled,                       // Possible resource claims
              rtfm::Rsystem_clock,
              rtfm::Rasync
            >;

using J2 = rtfm::job<
              2,                          // Priority
              rtfm::make_isr<job2, 2>,    // ISR connection and location
              Rled,                       // Possible resource claims
              rtfm::Rsystem_clock,
              rtfm::Rasync
            >;


/****************************************************************************
 * End User Job and Resource defines.
 ****************************************************************************/


/****************************************************************************
 * Add user defined jobs to the list.
 ****************************************************************************/

using user_job_list = kvasir::mpl::list<J1, J2>;

/****************************************************************************
 * Add user defined jobs to the list.
 ****************************************************************************/

