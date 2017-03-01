
#include "rtfm/rtfm_srp.hpp"

/**
 * @brief   Init the BSS segment.
 */
inline void bss_init()
{
  extern uint32_t __bss_start, __bss_end;

  uint32_t *from = &__bss_start;
  uint32_t *to = &__bss_end;

  while (from < to)
    *(from++) = 0;
}

/**
 * @brief   Init the data segment.
 */
inline void data_init()
{
  extern uint32_t __text_end, __data_start, __data_end;

  uint32_t *to = &__data_start;
  const uint32_t *from = &__text_end;
  uint32_t size = ((uint32_t)&__data_end - (uint32_t)&__data_start) / 4;

  while (size--)
    *(to++) = *(from++);
}

/**
 * @brief   Run global constructors.
 */
inline void constructor_init()
{
  /* Start and end points of the constructor list. */
  extern void (*__init_array_start)();
  extern void (*__init_array_end)();

  /* Call each function in the list. */
  for (void (**p)() = &__init_array_start; p < &__init_array_end; ++p) {
    (*p)();
  }
}

/**
 * @brief   Initialize the system clock and start the PLL.
 */
inline void InitClocks()
{
  RCC->APB1ENR |= RCC_APB1ENR_PWREN;  // Enable power control clocks
  PWR->CR |= (3 << PWR_CR_VOS_Pos);   // Voltage Scale 1 (<= 100 MHz)

  /* Set HSE configuration to BYPASS ( 8 MHz from STlink ) */
  RCC->CR |= RCC_CR_HSEBYP;
  RCC->CR |= RCC_CR_HSEON;

  /* Wait till HSE is ready */
  while((RCC->CR & RCC_CR_HSERDY) == 0);

  /*
   * PLL startup sequence
   */

  /* Disable PLL */
  RCC->CR &= ~(1 << RCC_CR_PLLON_Pos);

  /* Wait till PLL is ready */
  while((RCC->CR & RCC_CR_PLLRDY) != 0);

  /* Configure the main PLL clock source, multiplication and division factors. */
  RCC->PLLCFGR = ((1 << RCC_PLLCFGR_PLLSRC_Pos) |  // HSE
                  (4 << RCC_PLLCFGR_PLLM_Pos)   |  // PLLM
                  (200 << RCC_PLLCFGR_PLLN_Pos) |  // PLLN
                  (1 << RCC_PLLCFGR_PLLP_Pos)   |  // PLLP div / 4
                  (4 << RCC_PLLCFGR_PLLQ_Pos));    // PLLQ

  /* Enable the main PLL. */
  RCC->CR |= (1 << RCC_CR_PLLON_Pos);

  /* Wait till PLL is ready */
  while((RCC->CR & RCC_CR_PLLRDY) == 0);


  /*
   * Clock configuration sequence
   */

  /* Set flash latency (3 WS, taken from STM32CubeMX) */
  FLASH->ACR |= (3 << FLASH_ACR_LATENCY_Pos);

  /* Check to it took hold */
  while ((FLASH->ACR & FLASH_ACR_LATENCY) != 3);


  /* HCLK Configuration */
  RCC->CFGR &= ~RCC_CFGR_HPRE;  // DIV1


  /* SYSCLK Configuration (select PLL) */
  RCC->CFGR |= RCC_CFGR_SW_PLL;

  /* Wait for PLL */
  while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);


  /* PCLK1 Configuration */
  RCC->CFGR |= RCC_CFGR_PPRE1_DIV2;


  /* PCLK2 Configuration */
  RCC->CFGR |= RCC_CFGR_PPRE2_DIV1;

}

extern "C" {

void Reset_Handler()
{

  __disable_irq();

  /* Set the stack pointer. */
  __set_PSP((uint32_t)&__stack);

  /* Enable FPU ( Set bits 20-23 to enable CP10 and CP11 coprocessors) */
  SCB->CPACR |= (0xF << 20);
  __ISB();

  /* Copy data from Flash to SRAM, assumes 4 byte alignment of DATA must be
     correct in the link file. */
  data_init();

  /* Clear the BSS segment, assumes 4 byte alignment of BSS must be correct in
     the link file. */
  bss_init();

  /* Runc ctors */
  constructor_init();

  __DSB();

  /* Setup clocks */
  InitClocks();

  /* Enable prefetch / cache */
  FLASH->ACR |= FLASH_ACR_PRFTEN | FLASH_ACR_ICEN | FLASH_ACR_DCEN;

  __enable_irq();
  __ISB();

  int main(void);

  main();


  /* Runc ctors */
  // my_exec_array(&__fini_array_start, &__fini_array_end);

}


}
