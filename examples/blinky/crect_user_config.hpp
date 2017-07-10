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
#include "led.hpp"

void job1(void);
void job2(void);

using Rled = crect::make_resource<
    CRECT_OBJECT_LINK(led_resource)  // Pointer to some object to be protected
  >;

using J1 = crect::job<
              1,                          // Priority
              crect::make_isr<job1, 1>,   // ISR connection and location
              Rled,                       // Possible resource claims
              crect::Rsystem_clock,
              crect::Rasync
            >;

using J2 = crect::job<
              2,                          // Priority
              crect::make_isr<job2, 2>,   // ISR connection and location
              Rled,                       // Possible resource claims
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

