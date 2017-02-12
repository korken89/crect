
#ifdef __clang__
#include "util/print_types.hpp"
#endif

#include "rtfm/rtfm_srp.hpp"

using namespace std::chrono_literals;


void test_rtfm()
{
  using namespace rtfm::srp;

  /* Lock */
  //lock< R4 > lock;


  async<J1>( 10ms );

  // asm volatile("nop");
  // asm volatile("nop");
  // asm volatile("nop");
  // asm volatile("nop");

  /* Automatic unlock via RAII */
}


void InitLED()
{
  // Setup GPIO Port A, Pin 5 (LED)
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;          // Enable GPIOA clock
  GPIOA->MODER |= (1 << GPIO_MODER_MODE5_Pos);  // Output
}

inline void EnableLED()
{
  // Enable GPIO Port A, Pin 5 (LED)
  GPIOA->ODR |= (1 << GPIO_OTYPER_OT5_Pos);   // Enable LED
}

inline void DisableLED()
{
  // Disable GPIO Port A, Pin 5 (LED)
  GPIOA->ODR &= ~(1 << GPIO_OTYPER_OT5_Pos);  // Disable LED
}

inline void ToggleLED()
{
  // Toggle GPIO Port A, Pin 5 (LED)
  GPIOA->ODR ^= (1 << GPIO_OTYPER_OT5_Pos);   // Toggle LED
}

__attribute__ ((noinline))
int main()
{
  InitLED();
  

  //print_list<rtfm::system_job_list>("System Jobs");

  //print_list<rtfm::details::resource_tree>("Resource tree");

  rtfm::srp::async_queue::initialize();

  //async<J1>( 10ms );

  //std::cout << "Ticks: " << ticks << std::endl;

  /* Blink a LED! */
  while(1)
  {
    ToggleLED();

    for (uint32_t i = 0; i < 1000000; i++)
      asm volatile("nop");
  }

  return 0;
}
