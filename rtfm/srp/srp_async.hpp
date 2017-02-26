
#pragma once

#include "rtfm/rtfm_srp.hpp"
#include "rtfm/async/async_queue.hpp"

#include <chrono>

#ifndef __RTFM_ASYNC_QUEUE_SIZE
#   define __RTFM_ASYNC_QUEUE_SIZE 10
#endif

extern rtfm::async_queue<__RTFM_ASYNC_QUEUE_SIZE> rtfm_async_queue;

namespace rtfm
{
namespace srp
{
namespace details
{

/*
 * @brief Asynchronous pend of an RTFM job with duration (implementation).
 *
 * @param[in] duration  The time until the job shall be executed.
 * @param[in] isr       The Job's ISR value.
 */
static void async_impl_dur(rtfm::time::system_clock::duration dur, unsigned isr)
{
  /* Always get the current time. */
  auto current_time = [](){
    rtfm::srp::lock<rtfm::Rsystem_clock> lock;
    return rtfm::time::system_clock::now();
  }();

  rtfm_async_queue.push(current_time + dur, isr);
}

/*
 * @brief Asynchronous pend of an RTFM job at specific time (implementation).
 *
 * @param[in] time      The absoulute time the job shall be executed.
 * @param[in] isr       The Job's ISR value.
 */
static void async_impl_time(rtfm::time::system_clock::time_point time, unsigned isr)
{
  /* Always get the current time. */
  rtfm_async_queue.push(time, isr);
}

} /* END namespace details */

/**
 * @brief Asynchronous pend of an RTFM job.
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

  auto dur = duration_cast<rtfm::time::system_clock::duration>(duration);

  details::async_impl_dur(dur, Job::ISR::index::value);
}

/**
 * @brief Asynchronous pend of an RTFM job, manually specifying the ISR ID.
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

  auto dur = duration_cast<rtfm::time::system_clock::duration>(duration);

  details::async_impl_dur(dur, isr);
}

/**
 * @brief Asynchronous pend of an RTFM job.
 *
 * @tparam Job          The job to pend in the future.
 *
 * @param[in] time      The time the job shall be executed.
 */

template <typename Job>
constexpr void async(rtfm::time::system_clock::time_point time)
{
  details::async_impl_time(time, Job::ISR::index::value);
}

/**
 * @brief Asynchronous pend of an RTFM job, manually specifying the ISR ID.
 *
 * @tparam Job          The job to pend in the future.
 *
 * @param[in] time      The time the job shall be executed.
 * @param[in] isr       The ISR ID to pend.
 */

inline void async(rtfm::time::system_clock::time_point time, unsigned isr)
{
  details::async_impl_time(time, isr);
}

} /* END namespace srp */
} /* END namespace rtfm */
