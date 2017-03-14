
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
  using ISRn = typename Job::isr::index;
  NVIC->ISPR[ISRn::value >> 5UL] = (1UL << (ISRn::value & 0x1FUL));
}

/**
 * @brief Synchronous pend of an RTFM job.
 *
 * @param[in] id    The interrupt ID to pend.
 */
inline void pend(unsigned id)
{
  NVIC->ISPR[id >> 5UL] = (1UL << (id & 0x1FUL));
}

/**
 * @brief Clear an pending RTFM job.
 *
 * @tparam Job  The job to clear.
 */
template <typename Job>
constexpr void clear()
{
  using ISRn = typename Job::isr::index;
  NVIC->ICPR[ISRn::value >> 5UL] = (1UL << (ISRn::value & 0x1FUL));
}

/**
 * @brief Clear an pending RTFM job.
 *
 * @param[in] id    The interrupt ID to clear.
 */
inline void clear(unsigned id)
{
  NVIC->ICPR[id >> 5UL] = (1UL << (id & 0x1FUL));
}

} /* END namespace srp */
} /* END namespace rtfm */
