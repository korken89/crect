
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
  using ISRn = typename Job::ISR::index;
#ifndef __clang__
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
  using ISRn = typename Job::ISR::index;
#ifndef __clang__
  NVIC->ICPR[ISRn::value >> 5UL] = (1UL << (ISRn::value & 0x1FUL));
#endif
}

} /* END namespace srp */
} /* END namespace rtfm */
