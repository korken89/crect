//          Copyright Emil Fresk 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.md or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once


namespace crect
{

/**
 * @brief Synchronous pend of an crect job.
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
 * @brief Synchronous pend of an crect job.
 *
 * @param[in] id    The interrupt ID to pend.
 */
inline void pend(unsigned id)
{
  NVIC->ISPR[id >> 5UL] = (1UL << (id & 0x1FUL));
}

/**
 * @brief Clear an pending crect job.
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
 * @brief Clear an pending crect job.
 *
 * @param[in] id    The interrupt ID to clear.
 */
inline void clear(unsigned id)
{
  NVIC->ICPR[id >> 5UL] = (1UL << (id & 0x1FUL));
}

} /* END namespace crect */
