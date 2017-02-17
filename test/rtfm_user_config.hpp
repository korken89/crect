
#pragma once

/****************************************************************************
 * User Job and Resource defines here.
 ****************************************************************************/

// #include "util/fake_hw.hpp"
#include "util/string_hash.hpp"

static void Used1()
{
}
static void Used2()
{
}
static void Used3()
{
}

using ISR1 = rtfm::MakeISR<&Used1, 2>;
using ISR2 = rtfm::MakeISR<&Used2, 1>;
using ISR3 = rtfm::MakeISR<&Used3, 32>;

using R1 = rtfm::Resource<char>;
using R2 = rtfm::Resource<float>;
using R3 = rtfm::Resource<double>;
using R4 = rtfm::Resource<unsigned>;
using Rn = rtfm::Resource<int>;

using J1 = rtfm::Job<1, 1, ISR1, R2, R1, R4>;
using J2 = rtfm::Job<2, 2, ISR2, R2, R3, R4>;
using J3 = rtfm::Job<3, 3, ISR3, R1, R3, R4>;

/****************************************************************************
 * End User Job and Resource defines.
 ****************************************************************************/


namespace rtfm
{

/****************************************************************************
 * Add user defined jobs to the list.
 ****************************************************************************/

using system_job_list = brigand::list<J1, J2, J3>;

/****************************************************************************
 * Add user defined jobs to the list.
 ****************************************************************************/

} /* END namespace rtfm */
