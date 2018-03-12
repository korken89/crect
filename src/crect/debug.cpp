//          Copyright Emil Fresk 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.md or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cstdint>
#include <initializer_list>
#include "kvasir/mpl/mpl.hpp"
#include "crect/debug.hpp"
#include "crect/details/resource_tree.hpp"
#include "crect_system_config.hpp"
#include "crect/unsafe_flag.hpp"
#include "crect/crect.hpp"

template < typename Fun, std::size_t... Is >
constexpr void for_each(Fun&& fun)
{
  (void)std::initializer_list< int >{
      (fun(std::integral_constant< std::size_t, Is >{}), 0)...};
}

//
// Checker + lookup table holder
//
struct resource_lock_checker
{
  // Get the number of resources in the system
  using num_resources = kvasir::mpl::eager::size<
      crect::resource_tree< crect::system_job_list > >;

  // Lookup table (92 different IRQs, 100 resources)
  crect::unsafe_flag< __MCU_NUM_VECTORS > storage_[num_resources::value];

  // Checking function to see if job_id can take r_id resource
  constexpr bool exists(std::uint8_t r_id, std::uint8_t job_id) const noexcept
  {
    return storage_[r_id][job_id];
  }

  // Helper function to generate some flags
  template < std::size_t... Is >
  constexpr static auto make_flags() noexcept
  {
    crect::unsafe_flag< __MCU_NUM_VECTORS > flags;
    flags.reset();

    for_each<>([&](auto idx) { flags.set(idx); });

    return flags;
  }
};

template < typename Resource >
using get_jobs = typename Resource::jobs;

template < typename Job >
using get_isr = typename Job::isr::index;

template < typename... Ts >
struct printer
{
};

template < int... Is >
struct int_list
{
};

template < typename >
struct make_int_list
{
};

template < int... Is >
struct make_int_list<
    kvasir::mpl::list< kvasir::mpl::integral_constant< int, Is >... > >
{
	using f = int_list<Is...>;
};


template < typename List >
struct lists;

template < typename... Lists >
struct lists< kvasir::mpl::list< Lists... > >
{
  using f =
      kvasir::mpl::list< typename make_int_list< kvasir::mpl::eager::transform< Lists, get_isr > >::f ... >;
};

template < typename List >
using get_lists = typename lists< List >::f;

void test()
{
  using tree = crect::resource_tree< crect::system_job_list >;
  using jobs = kvasir::mpl::eager::transform< tree, get_jobs >;
  using ints = get_lists< jobs >;

  auto a = printer< ints >{};
  // auto a = printer< crect::system_job_list >{};
  decltype(a)::f;
}
