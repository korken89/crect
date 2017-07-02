#include <type_traits>

#include "crect/crect.hpp"
#include "led.hpp"

/* Shared LED object. */
ManageLED led_resource;

/* Lower priority job */
void job1()
{
  using namespace std::chrono_literals;

  /* Access the LED resource through the claim following a monitor pattern. */
  crect::claim<Rled>([](auto &led){
    led.enable();
  });

  // Disable led in 200ms
  crect::async<J2>(200ms);
}

/* Higher priority job */
void job2()
{
  using namespace std::chrono_literals;

  /* Access the LED resource through the claim following a monitor pattern. */
  crect::claim<Rled>([](auto &led){
    led.disable();
  });

  // Enable led in 200ms
  crect::async<J1>(200ms);
}


int main()
{

  /* Initialization code */
  crect::initialize();

  /*
   * Convoluted way to blink a LED
   */
  crect::pend<J1>();


  while(1)
  {
    __WFI();
  }
}
