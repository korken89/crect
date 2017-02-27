
#ifdef PC_DEBUG
#include "util/print_types.hpp"
#endif

#include "rtfm/rtfm_srp.hpp"
#include "led.hpp"

// using namespace std::chrono_literals;

/* Lower priority job */
void job1()
{
  using namespace std::chrono_literals;

  [](){
    rtfm::srp::lock< R1 > lock;
    EnableLED();
  }();

  // Disable led in 1000ms
  rtfm::srp::async<J2>(1000ms);
}

/* Higher priority job */
void job2()
{
  using namespace std::chrono_literals;

  [](){
    rtfm::srp::lock< R1 > lock;
    DisableLED();
  }();

  // Enable led in 1000ms
  rtfm::srp::async<J1>(1000ms);
}


int main()
{

  InitLED();

  /* Initialization code */
  rtfm::srp::initialize();

  /*
   * Convoluted way to blink a LED
   */
  rtfm::srp::pend<J1>();


  while(1)
  {
    __WFI();
  }
}
