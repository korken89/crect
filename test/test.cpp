
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

__attribute__ ((noinline))
int main()
{
  //print_list<rtfm::system_job_list>("System Jobs");

  //print_list<rtfm::details::resource_tree>("Resource tree");

  rtfm::srp::async_queue::initialize();

  //async<J1>( 10ms );

  //std::cout << "Ticks: " << ticks << std::endl;

  return 0;
}


extern "C" {

void my_memset(uint32_t *p, uint32_t val, uint32_t size)
{
  size /= 4;
  while (size--)
    *(p++) = val;
}

void my_memcpy(uint32_t *to, const uint32_t *from, uint32_t size)
{
  size /= 4;
  while (size--)
    *(to++) = *(from++);
}

// __attribute__ ((noinline))
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

void Reset_Handler()
{
  __disable_irq();
  __ISB();

  // Enable FPU ( Set bits 20-23 to enable CP10 and CP11 coprocessors)
  SCB->CPACR |= (0xF << 20);
  __ISB();

  //Copy data from Flash to SRAM
  my_memcpy(&__data_start, &__text_end, &__data_end - &__data_start);

  // Clear the BSS segment
  my_memset(&__bss_start, 0, &__bss_end - &__bss_start);

  // Runc ctors
  my_exec_array(&__init_array_start, &__init_array_end);

  __DSB();

  __enable_irq();
  __ISB();

  main();

  // Runc ctors
  // my_exec_array(&__fini_array_start, &__fini_array_end);

}
}
