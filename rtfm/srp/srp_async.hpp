
#pragma once

#include "rtfm/details/job_resource.hpp"
#include <chrono>

#ifndef __RTFM_ASYNC_QUEUE_SIZE
#   define __RTFM_ASYNC_QUEUE_SIZE 10
#endif

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

  struct async_queue_element {
    int32_t baseline;
    uint32_t id;
    struct async_queue_element* next;
  };

  async_queue_element _queue[__RTFM_ASYNC_QUEUE_SIZE];
  async_queue_element* _queue_head;    // head pointer
  async_queue_element* _queue_free;    // free pointer
  async_queue_element* _queue_new;     // new
  async_queue_element* _queue_current; // current

  /**
  * @brief Initialization function for the queue.
  */
  void init_memory()
  {
    for (int i = 0; i < __RTFM_ASYNC_QUEUE_SIZE - 1; i++)
      _queue[i].next = &_queue[i + 1];

    _queue[__RTFM_ASYNC_QUEUE_SIZE - 1].next = nullptr;

    _queue_head = nullptr;
    _queue_free = _queue;
  }

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

  static void initialize()
  {
    auto& queue = async_queue::get();
    queue.init_memory();
  }

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

/* TODO: Write custom clock, to use std::chrono::time_point and then a
         specialization taking duration, converting to time_point */ 
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
