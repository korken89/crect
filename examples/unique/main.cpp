#include <type_traits>

#include "rtfm/rtfm_srp.hpp"

/* Start a memory transaction */
void job_start()
{
  rtfm::srp::unique_lock<Runique>();
}

/* Memory transaction finished */
void job_done()
{
  rtfm::srp::unique_unlock<Runique>();
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
