
#ifdef PC_DEBUG
#include "util/print_types.hpp"
#endif

#include "rtfm/rtfm_srp.hpp"
#include "led.hpp"

using namespace std::chrono_literals;


void test_rtfm()
{
  using namespace rtfm::srp;

  /* Lock */
  //lock< R4 > lock;

  //async<J1>( 10ms );

  // asm volatile("nop");
  // asm volatile("nop");
  // asm volatile("nop");
  // asm volatile("nop");

  /* Automatic unlock via RAII */
}


__attribute__ ((noinline))
int main()
{
#ifndef PC_DEBUG
  InitLED();
#endif

  rtfm::srp::initialize();
  test_rtfm();

  //print_list<rtfm::system_job_list>("System Jobs");
  //print_list<rtfm::details::resource_tree>("Resource tree");

  //async<J1>( 10ms );

  /* Blink a LED! */
  while(1)
  {
#ifndef PC_DEBUG
    ToggleLED();

    for (uint32_t i = 0; i < 5000000; i++)
      asm volatile("nop");
#endif
  }

  return 0;
}
