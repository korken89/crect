
#include "rtfm/rtfm_srp.hpp"

extern "C" {

void NMI_Handler(void)
{
  while(1);
}
void HardFault_Handler(void)
{
  while(1);
}
void MemManage_Handler(void)
{
  while(1);
}
void BusFault_Handler(void)
{
  while(1);
}
void UsageFault_Handler(void)
{
  while(1);
}
void SVCall_Handler(void)
{
  while(1);
}
void DebugMon_Handler(void)
{
  while(1);
}
void PendSV_Handler(void)
{
  while(1);
}
void SysTick_Handler(void)
{
  while(1);
}

__attribute__ ((noinline))
void my_memset(uint32_t *from, uint32_t *to, uint32_t val)
{
  while (from < to)
    *(from++) = val;
}

__attribute__ ((noinline))
void my_memcpy(uint32_t *to, const uint32_t *from, uint32_t size)
{
  size /= 4;
  while (size--)
    *(to++) = *(from++);
}

__attribute__ ((noinline))
void my_exec_array(uint32_t *from, uint32_t *to)
{
  typedef void(*vptr)(void);

  while (from < to)
  {
    vptr from_f = (vptr)(*from);
    from_f();
    from++;
  }

}

__attribute__ ((noinline))
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

__attribute__((naked))
void Reset_Handler()
{

  __disable_irq();

  __set_PSP((uint32_t)&__stack);

  // Enable FPU ( Set bits 20-23 to enable CP10 and CP11 coprocessors)
  SCB->CPACR |= (0xF << 20);
  __ISB();

  //Copy data from Flash to SRAM
  my_memcpy(&__data_start, &__text_end, (uint32_t)&__data_end - (uint32_t)&__data_start);

  // Clear the BSS segment
  my_memset(&__bss_start, &__bss_end, 0);

  // Runc ctors
  my_exec_array(&__init_array_start, &__init_array_end);

  __DSB();

  // Setup clocks
  InitClocks();

  __enable_irq();
  __ISB();

  int main();

  main();

  // Runc ctors
  // my_exec_array(&__fini_array_start, &__fini_array_end);

}


}
