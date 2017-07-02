
#pragma once


namespace crect
{
namespace timer
{

/**
 * @brief   Convenience definition of the max SysTick duration.
 */
static const constexpr auto max_systick_duration =
  crect::time::system_clock::duration{SysTick_LOAD_RELOAD_Msk};

/**
 * @brief   Convenience definition to pend SysTick.
 */
inline void pend()
{
  SCB->ICSR |= SCB_ICSR_PENDSTSET_Msk;
}

/**
 * @brief   Convenience definition to set up SysTick.
 */
inline void initialize()
{
  SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk;
  SysTick->LOAD = timer::max_systick_duration.count();
  SysTick->VAL = 0;
  SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
}

} /* END namespace timer */
} /* END namespace crect */
