

#pragma once

#include "kvasir/mpl/mpl.hpp"

namespace rtfm
{
namespace util
{

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

/**
 * @brief  A convenience function for transforming strings to a hashed value.
 *
 * @param[in] txt   A string with '\0' termination.
 */
constexpr uint32_t hashit(const char *txt)
{
  uint32_t h = 0;
  while (*txt != '\0')
    h = h * 1572869 + *txt++;

  return h;
}

} /* END namespace util */
} /* END namespace rtfm */
