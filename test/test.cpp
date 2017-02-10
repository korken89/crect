
#include "rtfm/rtfm_srp.hpp"
//#include "util/print_types.hpp"

/* TODO: Implement "get vector table" from job list. */


//void test_rtfm()
//{
//  rtfm::srp::lock< R4 > lock;
//  /* Lock */
//
//  asm volatile("nop");
//  asm volatile("nop");
//  asm volatile("nop");
//  asm volatile("nop");
//
//  /* Automatic unlock via RAII */
//}



int main()
{
  //print_list<rtfm::system_job_list>("System Jobs");

  //print_list<rtfm::details::resource_tree>("Resource tree");

  //using isr = find_job_isr< find_job_isr_impl< brigand::integral_constant<int, 10> >::type >::isr;
  //std::cout << "\nisr: " << type_name< isr >() << std::endl;

  return 0;
}
