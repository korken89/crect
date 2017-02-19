
#pragma once

/****************************************************************************
 * User Job and Resource defines here.
 ****************************************************************************/

// #include "util/fake_hw.hpp"
#include "util/string_hash.hpp"

void job1(void);
void job2(void);

using ISR1 = rtfm::MakeISR<&job1, 1>;
using ISR2 = rtfm::MakeISR<&job2, 2>;

using R1 = rtfm::Resource<char>;

using J1 = rtfm::Job<1, 1, ISR1, R1>;
using J2 = rtfm::Job<2, 2, ISR2, R1>;

/****************************************************************************
 * End User Job and Resource defines.
 ****************************************************************************/


namespace rtfm
{

/****************************************************************************
 * Add user defined jobs to the list.
 ****************************************************************************/

using system_job_list = brigand::list<J1, J2>;

/****************************************************************************
 * Add user defined jobs to the list.
 ****************************************************************************/

} /* END namespace rtfm */
