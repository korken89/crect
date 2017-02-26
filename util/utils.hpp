

#pragma once

#include "brigand/brigand.hpp"
#include "util/string_hash.hpp"

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
constexpr auto priority_to_NVIC_priority(unsigned priority)
{
  /* Max priority */
  auto N = (1U << __NVIC_PRIO_BITS) - 1U;
  return ((N - priority) << (8U - __NVIC_PRIO_BITS)) & 0xFFU;
}

} /* END namespace util */
} /* END namespace rtfm */
