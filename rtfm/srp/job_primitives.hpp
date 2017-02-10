
#pragma once

#include "rtfm/details/job_resource.hpp"

namespace rtfm
{
namespace srp
{

/**
 * @brief Pend an RTFM job.
 *
 * @tparam Job  The job to pend.
 */
template <typename Job>
constexpr void pend()
{
  using ISRn = typename Job::ISR::index;
  NVIC->ISPR[ISRn::value >> 5UL] = (1UL << (ISRn::value & 0x1FUL));
}

/**
 * @brief Clear an pending RTFM job.
 *
 * @tparam Job  The job to clear.
 */
template <typename Job>
constexpr void clear()
{
  using ISRn = typename Job::ISR::index;
  NVIC->ICPR[ISRn::value >> 5UL] = (1UL << (ISRn::value & 0x1FUL));
}

}
}
