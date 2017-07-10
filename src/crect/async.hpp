//          Copyright Emil Fresk 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.md or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include "crect/crect.hpp"
#include "crect/async/async_queue.hpp"

#include <chrono>

extern crect::async_queue<__CRECT_ASYNC_QUEUE_SIZE> crect_async_queue;

namespace crect
{
namespace details
{

/*
 * @brief Asynchronous pend of a job with duration (implementation).
 *
 * @param[in] duration  The time until the job shall be executed.
 * @param[in] isr       The Job's ISR value.
 */
static void async_impl_dur(time::system_clock::duration dur, unsigned isr)
{
  /* Always get the current time. */
  auto current_time = claim<Rsystem_clock>([](auto &now){
    return now();
  });

  /* Claim the async queue and manipulate. */
  claim<Rasync>([&](auto &async_queue){
    async_queue.push(current_time + dur, isr);
  });
}

/*
 * @brief Asynchronous pend of a job at specific time (implementation).
 *
 * @param[in] time      The absolute time the job shall be executed.
 * @param[in] isr       The Job's ISR value.
 */
static void async_impl_time(time::system_clock::time_point time, unsigned isr)
{
  /* Claim the async queue and manipulate. */
  claim<Rasync>([&](auto &async_queue){
    async_queue.push(time, isr);
  });
}

} /* END namespace details */

/**
 * @brief Asynchronous pend of a job.
 *
 * @tparam Job          The job to pend in the future.
 * @tparam Rep          Representation of the duration.
 * @tparam Period       Period of the duration.
 *
 * @param[in] duration  The time until the job shall be executed.
 */

template <typename Job, typename Rep, typename Period>
constexpr void async(std::chrono::duration<Rep, Period> duration)
{
  using namespace std::chrono;
  using idx = typename Job::isr::index;

  auto dur = duration_cast<time::system_clock::duration>(duration);

  details::async_impl_dur(dur, idx::value);
}

/**
 * @brief Asynchronous pend of a job, manually specifying the ISR ID.
 *
 * @tparam Rep          Representation of the duration.
 * @tparam Period       Period of the duration.
 *
 * @param[in] duration  The time until the job shall be executed.
 * @param[in] isr       The ISR ID to pend.
 */

template <typename Rep, typename Period>
constexpr void async(std::chrono::duration<Rep, Period> duration, unsigned isr)
{
  using namespace std::chrono;

  auto dur = duration_cast<time::system_clock::duration>(duration);

  details::async_impl_dur(dur, isr);
}

/**
 * @brief Asynchronous pend of a job.
 *
 * @tparam Job          The job to pend in the future.
 *
 * @param[in] time      The time the job shall be executed.
 */

template <typename Job>
constexpr void async(time::system_clock::time_point time)
{
  using idx = typename Job::isr::index;
  details::async_impl_time(time, idx::value);
}

/**
 * @brief Asynchronous pend of a job, manually specifying the ISR ID.
 *
 * @tparam Job          The job to pend in the future.
 *
 * @param[in] time      The time the job shall be executed.
 * @param[in] isr       The ISR ID to pend.
 */

inline void async(time::system_clock::time_point time, unsigned isr)
{
  details::async_impl_time(time, isr);
}

} /* END namespace crect */
