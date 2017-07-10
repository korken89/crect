//          Copyright Emil Fresk 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.md or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "crect/crect.hpp"
#include "crect/async.hpp"
#include "crect/timer.hpp"


crect::async_queue<__CRECT_ASYNC_QUEUE_SIZE> crect_async_queue;

namespace crect
{
namespace timer
{

/**
 * @brief   Sets the reload of SysTick to a specific time.
 *
 * @param[in] time    The time to set the SysTick to execute.
 */
inline void set(time::system_clock::time_point time)
{
  auto current_time = crect::claim<crect::Rsystem_clock>([](auto &now){
    return now();
  });

  auto diff = time - current_time;

  if (diff > timer::max_systick_duration)
  {
    /* Set load to maximum time. */
    SysTick->LOAD = timer::max_systick_duration.count();
  }
  else
  {
    /* If the difference is in the past, trigger directly. */
    if (diff <= time::system_clock::duration{0})
      timer::pend();

    SysTick->LOAD = diff.count();
  }

  /* Write to force reload. */
  SysTick->VAL = 0;

}

/**
 * @brief   Sets the reload of SysTick to its max value.
 */
inline void set_max()
{
  /* Set load to maximum time. */
  SysTick->LOAD = timer::max_systick_duration.count();
  SysTick->VAL = 0;
}


} /* END namespace timer */
} /* END namespace crect */


/**
 * @brief   The SysTick handler, implements the Async queue.
 */
extern "C" void SysTick_Handler()
{
  /* Always get the current time. */
  auto current_time = crect::claim<crect::Rsystem_clock>([](auto &now){
    return now();
  });

  /* Access the async queue through the resource. */
  crect::claim<crect::Rasync>([&](auto &async_queue)
  {
    /* Check if there is any job in the async queue. */
    if (async_queue.front() != nullptr)
    {
      /* For every job where the time has expired, pend the job and remove from
       * the job from the queue.
       */
      while (current_time >= async_queue.front()->baseline)
      {
        crect::pend(async_queue.front()->job_isr_id);

        if (async_queue.pop() == nullptr)
        {
          /* The queue is empty, exit and set the timer to its max time.
             is to adhere to the requirements of @p system_clock.
           */
          crect::timer::set_max();
          return;
        }
      }

      /* Set up the timer to call for the next job in the queue. */
      crect::timer::set(async_queue.front()->baseline);
    }
    else
    {
      crect::timer::set_max();
    }
  });
}
