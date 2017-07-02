#include <type_traits>

#include "crect/crect.hpp"

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
  crect::initialize();

  /*
   * Convoluted way to blink a LED
   */
  crect::pend<J1>();


  while(1)
  {
    __WFI();
  }
}
