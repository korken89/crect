
#include "rtfm/rtfm_srp.hpp"
//#include "util/print_types.hpp"

/* TODO: Implement "get vector table" from job list. */


void test_rtfm()
{
  rtfm::srp::lock< R4 > lock;
  /* Lock */

  asm volatile("nop");
  asm volatile("nop");
  asm volatile("nop");
  asm volatile("nop");

  /* Automatic unlock via RAII */
}

int main()
{
  //print_list<rtfm::system_job_list>("System Jobs");

  //print_list<rtfm::details::resource_tree>("Resource tree");

  ////std::cout << "\nresult: " << type_name< Rft >() << std::endl;

  //using ceiling = rtfm::get_priority_ceiling< R4 >;
  //std::cout << "\nceiling: " << type_name< ceiling >() << std::endl;

  return 0;
};
