
#pragma once

#include "rtfm/details/job_resource.hpp"
#include <chrono>
using namespace std::chrono_literals;

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

/** @brief  Definition of timer ticks running at MCU clock. */
using timer_ticks = std::chrono::duration<uint64_t, std::ratio<1, __F_CPU>>;

class async_queue final
{
private:
  std::array<int, 100> queue;

public:

  static auto& Get()
  {
    static async_queue queue;
    return queue;
  }

  int test()
  {
    return 5;
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
  auto queue = async_queue::Get();
}

}
}
