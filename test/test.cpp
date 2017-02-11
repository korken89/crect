
#ifdef __clang__
#include "util/print_types.hpp"
#endif

#include "rtfm/rtfm_srp.hpp"



void test_rtfm()
{
  using namespace rtfm::srp;

  //rtfm::srp::lock< R4 > lock;
  /* Lock */


  async<J1>( 10ms );


  // asm volatile("nop");
  // asm volatile("nop");
  // asm volatile("nop");
  // asm volatile("nop");

  /* Automatic unlock via RAII */
}

int main()
{
  //print_list<rtfm::system_job_list>("System Jobs");

  //print_list<rtfm::details::resource_tree>("Resource tree");


  //std::cout << "Ticks: " << ticks << std::endl;

  return 0;
}

extern "C" {
void Reset_Handler()
{


}
}
