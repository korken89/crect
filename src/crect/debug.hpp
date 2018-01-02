//          Copyright Emil Fresk 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.md or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <cstdint>
#include <initializer_list>
#include "kvasir/mpl/mpl.hpp"
#include "crect/unsafe_flag.hpp"

//
// Checker + lookup table holder
//
struct resource_lock_checker
{
  // Lookup table (92 different IRQs, 100 resources)
  crect::unsafe_flag<92> storage_[100];

  // Checking function to see if job_id can take r_id resource
  constexpr bool exists(std::uint8_t r_id,
                        std::uint8_t job_id) const noexcept
  {
    return storage_[r_id][job_id];
  }

  // Helper function to generate some flags
  template <std::size_t... Is>
  constexpr static auto make_flags() noexcept
  {
    crect::unsafe_flag<92> flags;
    flags.reset();

    for (auto idx : {Is...})
      flags.set(idx);

    return flags;
  }
};
