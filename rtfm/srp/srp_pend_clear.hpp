
#pragma once


namespace rtfm
{
namespace srp
{

/**
 * @brief Synchronous pend of an RTFM job.
 *
 * @tparam Job  The job to pend.
 */
template <typename Job>
constexpr void pend()
{
#ifndef PC_DEBUG
  using ISRn = typename Job::ISR::index;
  NVIC->ISPR[ISRn::value >> 5UL] = (1UL << (ISRn::value & 0x1FUL));
#endif
}

/**
 * @brief Clear an pending RTFM job.
 *
 * @tparam Job  The job to clear.
 */
template <typename Job>
constexpr void clear()
{
#ifndef PC_DEBUG
  using ISRn = typename Job::ISR::index;
  NVIC->ICPR[ISRn::value >> 5UL] = (1UL << (ISRn::value & 0x1FUL));
#endif
}

} /* END namespace srp */
} /* END namespace rtfm */
