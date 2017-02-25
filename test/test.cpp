
#ifdef PC_DEBUG
#include "util/print_types.hpp"
#endif

#include "rtfm/rtfm_srp.hpp"
#include "led.hpp"

// using namespace std::chrono_literals;

/* Lower priority job */
void job1()
{
  rtfm::srp::lock< R1 > lock;

  EnableLED();
}

/* Higher priority job */
void job2()
{
  rtfm::srp::lock< R1 > lock;

  DisableLED();
}


int main()
{
  InitLED();

  /* Initialization code */
  rtfm::srp::initialize();

  /* Convoluted way to blink a LED. */
  rtfm::srp::pend<J1>();

  for (uint32_t i = 0; i < 15000000; i++)
     asm volatile("nop");

  /* Convoluted way to blink a LED. */
  rtfm::srp::pend<J2>();


  /* Blink a LED! */
  while(1)
  {


    // ToggleLED();

    for (uint32_t i = 0; i < 15000000; i++)
       asm volatile("nop");


  }

  return 0;
}
