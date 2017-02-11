
#pragma once

#include "rtfm/details/job_resource.hpp"
#include <chrono>
using namespace std::chrono_literals;

namespace rtfm
{
namespace srp
{

/** @brief  Definition of timer ticks running at MCU clock. */
using timer_ticks = std::chrono::duration<uint64_t, std::ratio<1, __F_CPU>>;

/**
 * @brief Timer queue class for async, implemented as a Singleton.
 */
class async_queue final
{
private:
  std::array<int, 100> _queue;

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
};

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

  auto ticks = duration_cast<timer_ticks>(duration);
  auto queue = async_queue::get();
}

} /* END namespace srp */
} /* END namespace rtfm */
