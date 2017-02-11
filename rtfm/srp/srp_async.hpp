
#pragma once

#include "rtfm/details/job_resource.hpp"
#include <chrono>

namespace rtfm
{
namespace srp
{

/** @brief  Definition of timer ticks running at MCU clock. */
using timer_ticks = std::chrono::duration<uint64_t, std::ratio<1, __F_CPU>>;

/**
 * @brief Timer queue class for async, implemented as a Singleton.
 * @note  Remember to add the -fno-threadsafe-statics to g++ flags so the
 *        __cxa_guard_... are not added.
 */
class async_queue final
{
private:
  std::array<int, 100> _queue;

  /**
  * @brief Empty constructor.
  */
  async_queue() {}

public:

  /**
  * @brief Static getter to the async queue.
  *
  * @return   Reference to the queue.
  */
  static auto& get()
  {
    static async_queue queue;
    return queue;
  }

  /**
  * @brief Make sure no copies can be made.
  */
  async_queue(async_queue const&)    = delete;
  void operator=(async_queue const&) = delete;
};

/**
 * @brief Asynchronous pend of an RTFM job.
 *
 * @tparam Job          The job to pend in the future.
 * @tparam Rep          Representation of the duration.
 * @tparam Period       Period of the duration.
 * @tparam TimerTicks   Definition of the clock rate of the system timer,
 *                      can be overridden if needed (if the timer changes rate).
 *
 * @param[in] duration  The time until the job shall be executed.
 */
template <typename Job, typename Rep, typename Period,
          typename TimerTicks = timer_ticks>
constexpr void async(std::chrono::duration<Rep, Period> duration)
{
  using namespace std::chrono;

  auto ticks = duration_cast<TimerTicks>(duration);
  auto& queue = async_queue::get();
}

} /* END namespace srp */
} /* END namespace rtfm */
