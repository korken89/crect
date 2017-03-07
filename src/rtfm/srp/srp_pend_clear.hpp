
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
  NVIC->ISPR[ISRn{} >> 5UL] = (1UL << (ISRn{} & 0x1FUL));
#endif
}

/**
 * @brief Synchronous pend of an RTFM job.
 *
 * @param[in] id    The interrupt ID to pend.
 */
inline void pend(unsigned id)
{
#ifndef PC_DEBUG
  NVIC->ISPR[id >> 5UL] = (1UL << (id & 0x1FUL));
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
  NVIC->ICPR[ISRn{} >> 5UL] = (1UL << (ISRn{} & 0x1FUL));
#endif
}

/**
 * @brief Clear an pending RTFM job.
 *
 * @param[in] id    The interrupt ID to clear.
 */
inline void clear(unsigned id)
{
#ifndef PC_DEBUG
  NVIC->ICPR[id >> 5UL] = (1UL << (id & 0x1FUL));
#endif
}

} /* END namespace srp */
} /* END namespace rtfm */
