//          Copyright Emil Fresk 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.md or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <initializer_list>
#include <type_traits>
#include <cstdint>

#include "kvasir/mpl/mpl.hpp"
#include "crect/utils.hpp"

namespace crect
{
namespace details
{

/**
 * @brief Type wrapper for the for-each implementation.
 */
template <typename T>
struct _wrapper {};

/**
 * @brief For-each implementation, executes over every type in a list.
 *
 * @tparam Fun  Functor for which each type will be executed in.
 * @tparam Ts   Pack of types in the list.
 */
template <typename Fun, typename... Ts>
constexpr void for_each_impl(kvasir::mpl::list<Ts...>)
{
  std::initializer_list<int> {
    ( Fun{}( details::_wrapper<Ts>{} ), 0)...
  };
}

}

/**
 * @brief For-each interface, executes over every type in a list.
 *
 * @tparam Fun    Functor for which each type will be executed in.
 * @tparam List   List of types.
 */
template <typename Fun, typename List>
constexpr void for_each(void)
{
  details::for_each_impl<Fun>(List{});
}

/**
 * @brief Code generator for the NVIC initialization code.
 */
struct job_to_nvic_printer
{
  template <typename Job>
  void operator()(details::_wrapper<Job>) const
  {
    using ISRn = typename Job::isr::index;
    using Prio = typename Job::prio;


    if (ISRn::value < 0)
    {
      constexpr auto index = (static_cast<uint32_t>(ISRn::value) & 0xFUL) - 4UL;

      /* Enable system interrupt. */
#if __CORTEX_M == 7U
      SCB->SHPR[index] = util::priority_to_NVIC_priority(Prio::value);
#else // Not Cortex-M7
      SCB->SHP[index] = util::priority_to_NVIC_priority(Prio::value);
#endif
    }
    else
    {
      /* Enable interrupt. */
      NVIC->ISER[ISRn::value >> 5UL] = (1UL << (ISRn::value & 0x1FUL));

      /* Set priority */
      NVIC->IP[ISRn::value] = util::priority_to_NVIC_priority(Prio::value);
    }
  }
};

/**
 * @brief Initialize the NVIC based on a list of crect jobs.
 *
 * @tparam JobList    The system's job list.
 */
template <typename JobList>
constexpr void initialize_jobs_impl()
{
  /* Enable DWT */
  CoreDebug->DEMCR |= (1UL << CoreDebug_DEMCR_TRCENA_Pos);
  //DWT->LAR = 0xC5ACCE55;
  DWT->CYCCNT = 0;
  DWT->CTRL |= (1UL << DWT_CTRL_CYCCNTENA_Pos);

  /* Fill the ISR settings. */
  for_each<job_to_nvic_printer, JobList>();
}

} /* END namespace crect */
