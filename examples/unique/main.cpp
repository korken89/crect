#include <type_traits>

#include "rtfm/rtfm_srp.hpp"

/* Start a memory transaction */
void job_start()
{
  // Fill queue for processing
}

/* Memory transaction finished */
void job_process()
{
  // Has unique access to memory, can serialize access

}


int main()
{

  /* Initialization code */
  rtfm::srp::initialize();

  /*
   * Convoluted way to blink a LED
   */
  rtfm::srp::pend<J1>();


  while(1)
  {
    __WFI();
  }
}
