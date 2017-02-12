
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

void InitClocks()
{
  RCC->APB1ENR |= RCC_APB1ENR_PWREN;  // Enable power control clocks
  PWR->CR |= (3 << PWR_CR_VOS_Pos);   // Volate Scale 1 (<= 100 MHz)

  // Set HSE configuration to BYPASS ( 8 MHz from STlink )
  RCC->CR |= RCC_CR_HSEBYP;
  RCC->CR |= RCC_CR_HSEON;

  // Wait till HSE is ready
  while((RCC->CR & RCC_CR_HSERDY) == 0);

  //
  // PLL startup sequence
  //

  // Disable PLL
  RCC->CR &= ~(1 << RCC_CR_PLLON_Pos);

  // Wait till PLL is ready
  while((RCC->CR & RCC_CR_PLLRDY) != 0);

  // Configure the main PLL clock source, multiplication and division factors.
  RCC->PLLCFGR = ((1 << RCC_PLLCFGR_PLLSRC_Pos) |  // HSE
                  (4 << RCC_PLLCFGR_PLLM_Pos)   |  // PLLM
                  (200 << RCC_PLLCFGR_PLLN_Pos) |  // PLLN
                  (1 << RCC_PLLCFGR_PLLP_Pos)   |  // PLLP div / 4
                  (4 << RCC_PLLCFGR_PLLQ_Pos));    // PLLQ

  // Enable the main PLL.
  RCC->CR |= (1 << RCC_CR_PLLON_Pos);

  // Wait till PLL is ready
  while((RCC->CR & RCC_CR_PLLRDY) == 0);


  //
  // Clock configuration sequence
  //

  // Set flash latency (3 WS, taken from STM32CubeMX)
  FLASH->ACR |= (3 << FLASH_ACR_LATENCY_Pos);

  // Check to it took hold
  while ((FLASH->ACR & FLASH_ACR_LATENCY) != 3);


  // HCLK Configuration
  RCC->CFGR &= ~RCC_CFGR_HPRE;  // DIV1


  // SYSCLK Configuration (select PLL)
  RCC->CFGR |= RCC_CFGR_SW_PLL;

  // Wait for PLL
  while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);


  // PCLK1 Configuration
  RCC->CFGR |= RCC_CFGR_PPRE1_DIV2;


  // PCLK2 Configuration
  RCC->CFGR |= RCC_CFGR_PPRE2_DIV1;

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
  InitClocks();

  //print_list<rtfm::system_job_list>("System Jobs");

  //print_list<rtfm::details::resource_tree>("Resource tree");

  asm volatile("nop");
  asm volatile("nop");

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
