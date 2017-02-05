
#pragma once

#include<cstdint>

constexpr uint32_t hashit(const char *txt)
{
  uint32_t h = 0;
  while (*txt != '\0')
    h = h * 1572869 + *txt++;

  return h;
}

