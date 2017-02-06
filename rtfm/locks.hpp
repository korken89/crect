
#pragma once

#include <cstdint>
#include "CMSIS/Core/Include/cmsis_gcc.h"
#include "rtfm/barriers.hpp"


namespace rtfm
{
namespace srp
{

template <typename Resource>
class lock
{
private:

  uint32_t _old_basepri;

public:
  lock()
  {
    /* Lock the resource. */
    _old_basepri = __get_BASEPRI();
    __set_BASEPRI_MAX( 5 /* getSRPResourceCeiling<Resource>::value */ );

    core::barrier_entry();
  }

  ~lock()
  {
    core::barrier_exit();

    /* Unlock the resource. */
    __set_BASEPRI(_old_basepri);
  }
};

} /* END namespace srp */
} /* END namespace rtfm */
