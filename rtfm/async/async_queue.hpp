
#pragma once

#include <chrono>
#include "rtfm/rtfm_clock.hpp"
#include "rtfm/rtfm_timer.hpp"

extern "C" void _unhandled_exception(void);

namespace rtfm
{

template <unsigned N>
class async_queue
{
private:
  struct queue_element {
    time::system_clock::time_point baseline;
    uint32_t job_isr_id;
    queue_element *next;
  };

  std::array<queue_element, N> _queue;
  queue_element *_head;
  queue_element *_free;

public:

  async_queue()
  {
    /**/
    for (unsigned i = 0; i < N - 1; i++)
      _queue[i].next = &_queue[i + 1];

    _queue[N - 1].next = nullptr;

    _head = nullptr;
    _free = _queue.data();
  }

  void push(time::system_clock::time_point t, uint32_t id)
  {
    auto _new = _free;


    /* Out of elements, panic. */
    if (_free == nullptr)
      _unhandled_exception();


    /* Fill in the new node. */
    _new->baseline = t;
    _new->job_isr_id = id;
    _free = _free->next;

    if (_head == nullptr || _head->baseline >= t)
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

      while ((_current->next != nullptr) && (_current->next->baseline < t))
        _current = _current->next;

      _new->next = _current->next;
      _current->next = _new;
    }
  }

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

  auto front()
  {
    return _head;
  }

};

} /* END namespace rtfm */
