
#include "rtfm/rtfm_srp.hpp"
#include "rtfm/srp/srp_async.hpp"
#include "rtfm/rtfm_timer.hpp"


rtfm::async_queue<__RTFM_ASYNC_QUEUE_SIZE> rtfm_async_queue;

namespace rtfm
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
  auto current_time = [](){
    rtfm::srp::lock<rtfm::Rsystem_clock> lock;
    return rtfm::time::system_clock::now();
  }();

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
} /* END namespace rtfm */


/**
 * @brief   The SysTick handler, implements the Async queue.
 */
extern "C" void SysTick_Handler()
{
  /* Always get the current time. */
  auto current_time = [](){
    rtfm::srp::lock<rtfm::Rsystem_clock> lock;
    return rtfm::time::system_clock::now();
  }();

  /* Access the async queue. */
  rtfm::srp::lock<rtfm::Rasync> lock;

  /* Check if there is any job in the async queue. */
  if (rtfm_async_queue.front() != nullptr)
  {
    /* For every job where the time has expired, pend the job and remove from
     * the job from the queue.
     */
    while (current_time >= rtfm_async_queue.front()->baseline)
    {
      rtfm::srp::pend(rtfm_async_queue.front()->job_isr_id);

      if (rtfm_async_queue.pop() == nullptr)
      {
        /* The queue is empty, exit and set the timer to its max time.
         j is to adhere to the requirements of @p system_clock.
         */
        rtfm::timer::set_max();
        return;
      }
    }

    /* Set up the timer to call for the next job in the queue. */
    rtfm::timer::set(rtfm_async_queue.front()->baseline);
  }
  else
    rtfm::timer::set_max();

}
