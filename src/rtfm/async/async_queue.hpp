
#pragma once

#include <chrono>
#include <array>
#include "rtfm/rtfm_clock.hpp"
#include "rtfm/rtfm_timer.hpp"

extern "C" void _unhandled_exception(void);

namespace rtfm
{

/**
 * @brief   The linked list that holds future job executions.
 *
 * @tparam N  The size of the async queue.
 */
template <unsigned N>
class async_queue
{
private:
  /** @brief Definition of each queue element. */
  struct queue_element {
    time::system_clock::time_point baseline;  /**< Time for exection. */
    uint32_t job_isr_id;                      /**< The job's ISR ID to pend. */
    queue_element *next;                      /**< The next element. */
  };

  /** @brief The queue. */
  std::array<queue_element, N> _queue;

  /** @brief Pointer to the head element. */
  queue_element *_head;

  /** @brief Ponter to the free element. */
  queue_element *_free;

public:

  /**
  * @brief   Constructor, initializes the queue.
  */
  async_queue()
  {
    /**/
    for (unsigned i = 0; i < N - 1; i++)
      _queue[i].next = &_queue[i + 1];

    _queue[N - 1].next = nullptr;

    _head = nullptr;
    _free = _queue.data();
  }

  /**
  * @brief   Push an element onto the queue.
  *
  * @param[in] time    The time for the ISR to execute.
  * @param[in] isr_id  The ISR to execute at said time.
  */
  void push(time::system_clock::time_point time, uint32_t isr_id)
  {
    auto _new = _free;


    /* Out of elements, panic. */
    if (_free == nullptr)
      _unhandled_exception();


    /* Fill in the new node. */
    _new->baseline = time;
    _new->job_isr_id = isr_id;
    _free = _free->next;

    if (_head == nullptr || _head->baseline >= time)
    {
      /* First in list, place and pend the handler. */
      _new->next = _head;
      _head = _new;

      timer::pend();
    }
    else
    {
      /* Queue is not emptly, sort the element into the queue. */
      auto _current = _head;

      while ((_current->next != nullptr) && (_current->next->baseline < time))
        _current = _current->next;

      _new->next = _current->next;
      _current->next = _new;
    }
  }

  /**
  * @brief   Pop the head of the queue.
  */
  auto pop()
  {
    auto _current = _head;

    if (_head != nullptr)
    {
      _head = _head->next;
      _current->next = _free;
      _free = _current;
    }

    return _head;
  }

  /**
  * @brief   Get the head of the queue.
  *
  * @return   Head of the queue.
  */
  auto front()
  {
    return _head;
  }

};

} /* END namespace rtfm */
