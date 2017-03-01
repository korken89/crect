
#pragma once

#include<cstdint>

namespace rtfm
{
namespace util
{

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
