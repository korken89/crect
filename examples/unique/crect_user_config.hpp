//          Copyright Emil Fresk 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.md or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include "crect/crect.hpp"

/****************************************************************************
 * User Job and Resource defines here.
 ****************************************************************************/

#include "crect/utils.hpp"

void job_start(void);
void job_process(void);

/* TODO: Check that a "normal" resource cannot share the address of the unique
 *       resource
 *
 * TODO: Change the Runique to something meaningful
 */
using Runique = crect::make_unique_resource<
    CRECT_OBJECT_LINK(job_process)
  >;

using J1 = crect::job<
              1,                               // Priority
              crect::make_isr<job_start, 1>,   // ISR connection and location
              crect::Rsystem_clock,            // Possible resource claims
              crect::Rasync
            >;

using J2 = crect::job<
              2,                               // Priority
              crect::make_isr<job_process, 2>, // ISR connection and location
              Runique,                         // Possible resource claims
              crect::Rsystem_clock,
              crect::Rasync
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

