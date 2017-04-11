
#pragma once

#include "rtfm/rtfm_srp.hpp"

/****************************************************************************
 * User Job and Resource defines here.
 ****************************************************************************/

#include "rtfm/rtfm_utils.hpp"

void job_start(void);
void job_done(void);

using Runique = rtfm::make_unique_resource<char>;

using J1 = rtfm::job<
              rtfm::util::hashit("Job start"), // Unique ID
              1,                               // Priority
              rtfm::make_isr<job_start, 1>,    // ISR connection and location
                                               // Possible resource claims
              rtfm::Rsystem_clock,
              rtfm::Rasync
            >;

using J2 = rtfm::job<
              rtfm::util::hashit("Job done"), // Unique ID
              2,                              // Priority
              rtfm::make_isr<job_done, 2>,    // ISR connection and location
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

