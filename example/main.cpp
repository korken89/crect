#include <type_traits>

#include "rtfm/rtfm_srp.hpp"
#include "led.hpp"

/* Shared LED object. */
ManageLED led_resource;

/* Lower priority job */
void job1()
{
  using namespace std::chrono_literals;

  /* Access the LED resource through the claim following a monitor pattern. */
  rtfm::srp::claim<Rled>([](auto &led){
    led.enable();
  });

  // Disable led in 200ms
  rtfm::srp::async<J2>(200ms);
}

/* Higher priority job */
void job2()
{
  using namespace std::chrono_literals;

  /* Access the LED resource through the claim following a monitor pattern. */
  rtfm::srp::claim<Rled>([](auto &led){
    led.disable();
  });

  // Enable led in 200ms
  rtfm::srp::async<J1>(200ms);
}


int main()
{

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
