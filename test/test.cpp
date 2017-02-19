
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

  /* Hold the lock for a long time to force long priority inversion */
  for (uint32_t i = 0; i < 5000000; i++)
    asm volatile("nop");
}

/* Higher priority job */
void job2()
{
  rtfm::srp::lock< R1 > lock;

  DisableLED();
}

void test_rtfm()
{
  /* Lock */
  //rtfm::srp::lock< R4 > lock;

  /* Automatic unlock via RAII */
}


__attribute__ ((noinline))
int main()
{
#ifndef PC_DEBUG
  InitLED();
#endif

  /* Initialization code */
  rtfm::srp::initialize();

  /* This will block the higher prio job */
  rtfm::srp::pend<J1>();

  /* Blink a LED! */
  while(1)
  {
#ifndef PC_DEBUG
    // ToggleLED();

    // for (uint32_t i = 0; i < 5000000; i++)
    //   asm volatile("nop");
#endif
  }

  return 0;
}
