
#pragma once

#include "rtfm/details/interrupts.hpp"
#include "rtfm/details/job_resource.hpp"
#include "rtfm/details/job_resource_methods.hpp"
#include "rtfm/srp/srp_locks.hpp"


/****************************************************************************
 * User defines here.
 ****************************************************************************/

#include "util/fake_hw.hpp"
//#include "util/print_types.hpp"
#include "util/string_hash.hpp"


using ISR1 = rtfm::MakeISR<&rtfm::DefaultISR::UnusedISR, 2>;
using ISR2 = rtfm::MakeISR<&rtfm::DefaultISR::UnusedISR, 1>;
using ISR3 = rtfm::MakeISR<&rtfm::DefaultISR::UnusedISR, 3>;

using R1 = rtfm::Resource<SPI1>;
using R2 = rtfm::Resource<SPI2>;
using R3 = rtfm::Resource<I2C1>;
using R4 = rtfm::Resource<I2C2>;

using J1 = rtfm::Job<1, 1, ISR1, R2, R1, R4>;
using J2 = rtfm::Job<2, 2, ISR2, R2, R3, R4>;
using J3 = rtfm::Job<3, 3, ISR3, R1, R3, R4>;

/****************************************************************************
 * End User defines.
 ****************************************************************************/


namespace rtfm
{

/****************************************************************************
 * Add user defined jobs to the list.
 ****************************************************************************/

using system_job_list = brigand::list<J1, J2, J3>;

}

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
using lock = lock_impl< get_priority_ceiling< Resource > >;

}
}
