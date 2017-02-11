
#include "rtfm/rtfm_srp.hpp"

extern "C" {

void NMI_Handler(void)
{
  while(1)
  {
    __BKPT();
  }
}
void HardFault_Handler(void)
{
  while(1)
  {
    __BKPT();
  }
}
void MemManage_Handler(void)
{
  while(1)
  {
    __BKPT();
  }
}
void BusFault_Handler(void)
{
  while(1)
  {
    __BKPT();
  }
}
void UsageFault_Handler(void)
{
  while(1)
  {
    __BKPT();
  }
}
void SVCall_Handler(void)
{
  while(1)
  {
    __BKPT();
  }
}
void DebugMon_Handler(void)
{
  while(1)
  {
    __BKPT();
  }
}
void PendSV_Handler(void)
{
  while(1)
  {
    __BKPT();
  }
}
void SysTick_Handler(void)
{
  while(1)
  {
    __BKPT();
  }
}
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
    vptr from_f = (vptr)from;
    from_f();
    from++;
  }

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

  __enable_irq();
  __ISB();

  int main();

  main();

  // Runc ctors
  // my_exec_array(&__fini_array_start, &__fini_array_end);

}


}
