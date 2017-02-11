
#pragma once

#include "brigand/brigand.hpp"

#include "stm32f411xe.h"


/*
 * Place MCU definition file here.
 *
 * Needs to define:
 * - __NVIC_PRIO_BITS     Number of priority bits
 * - __STACK_END          Address for the end of the stack
 * - __MCU_NUM_VECTORS    Number of peripheral ISR vectors (not including ARM's)
 * - __F_CPU              Clock speed of the MCU (needed for time conversions)
 */

#define __STACK_END         (1024*8)
#define __MCU_NUM_VECTORS   SPI5_IRQn
#define __F_CPU             168000000

#include "rtfm/details/interrupts.hpp"
#include "rtfm/details/job_resource.hpp"
#include "rtfm/details/job_resource_methods.hpp"
#include "rtfm/srp/srp_locks.hpp"
#include "rtfm/srp/job_primitives.hpp"


/****************************************************************************
 * User defines here.
 ****************************************************************************/

//#include "util/fake_hw.hpp"
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
