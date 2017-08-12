//          Copyright Emil Fresk 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.md or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <cstdint>
#include "kvasir/mpl/mpl.hpp"
#include "crect/details/arm_intrinsics.hpp"

namespace crect
{
namespace error_handling
{

/**
 * @brief     Halt the system and disables ISRs, has optional hook.
 *
 * @param[in] reason    The reason for the halt.
 */
static void halt(const char *reason)
{
  arm_intrinsics::disable_interrupts();

#ifdef CRECT_HALT_HOOK
  CRECT_HALT_HOOK(reason);
#endif

  (void)reason;

  while(1);
}

} /* END namespace error_handling */
} /* END namespace crect */
