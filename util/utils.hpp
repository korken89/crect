

#pragma once

#include "brigand/brigand.hpp"

namespace rtfm
{
namespace util
{

template <typename... T>
struct always_false : brigand::integral_constant<bool, false>
{
};

/**
 * @brief   Takes a user priority (0 = lowest, increasing numbers means higher
 *          priority) and transforms to NVIC priority (0 = highest, increasing
 *          numbers means lower priority).
 *
 * @return  The transformed priority.
 */
template <typename T>
constexpr T priority_to_NVIC_priority(T priority)
{
  /* Max priority */
  auto N = (1 << __NVIC_PRIO_BITS) - 1;
  return ((N - priority) << (8 - __NVIC_PRIO_BITS)) & 0xFF;
}

} /* END namespace util */
} /* END namespace rtfm */
