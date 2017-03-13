
#include "rtfm/rtfm_srp.hpp"
#include "rtfm/rtfm_clock.hpp"


namespace rtfm
{
namespace time
{

system_clock::time_point system_clock::now() noexcept
{
  /* Holds the current offset from start time due to DWT overflows. */
  static uint32_t base = 0;

  /* Holds the old DWT value to check for overflows. */
  static uint32_t old_dwt = 0;

  uint32_t dwt = DWT->CYCCNT;

  /* If the DWT has overflowed, update the base. */
  if (old_dwt > dwt)
    base += 1;

  /* Save old DWT. */
  old_dwt = dwt;

  return time_point(duration(
    (static_cast<uint64_t>(base) << 32U) + static_cast<uint64_t>(dwt)
  ));
}

} /* END namespace time */
} /* END namespace rtfm */
