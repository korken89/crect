//          Copyright Emil Fresk 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.md or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "crect/crect.hpp"
#include "crect/debug.hpp"

template <typename Resource>
using get_jobs = typename Resource::jobs;

template <typename Job>
using get_isr = typename Job::isr::index;



template <typename... Ts>
struct printer
{
};

template <int... Is>
struct int_list
{
};

template <typename T>
auto make_ints()
{
  return int_list< >{};
}

template <typename... Ts>
auto make_ints(kvasir::mpl::list< Ts... >)
{
  return int_list< Ts::value... >{};
}

void test()
{
  using ints = kvasir::mpl::eager::transform<
      crect::resource_tree< crect::system_job_list >, get_jobs >;

  auto a = printer< ints >{};
  //decltype(make_ints(ints{}))::f;
  //decltype(a)::f;
}
