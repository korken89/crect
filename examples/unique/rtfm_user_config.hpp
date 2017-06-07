
#pragma once

#include "rtfm/rtfm_srp.hpp"

/****************************************************************************
 * User Job and Resource defines here.
 ****************************************************************************/

#include "rtfm/rtfm_utils.hpp"

void job_start(void);
void job_process(void);

/* TODO: Check that a "normal" resource cannot share the address of the unique
 *       resource
 *
 * TODO: Change the Runique to something meaningful
 */
using Runique = rtfm::make_unique_resource<
    RTFM_OBJECT_LINK(job_process)
  >;

using J1 = rtfm::job<
              1,                               // Priority
              rtfm::make_isr<job_start, 1>,    // ISR connection and location
              rtfm::Rsystem_clock,             // Possible resource claims
              rtfm::Rasync
            >;

using J2 = rtfm::job<
              2,                              // Priority
              rtfm::make_isr<job_process, 2>, // ISR connection and location
              Runique,                        // Possible resource claims
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

