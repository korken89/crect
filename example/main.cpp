#include <type_traits>

#include "rtfm/rtfm_srp.hpp"
#include "led.hpp"

/* Shared LED object. */
ManageLED led_resource;

/* Lower priority job */
void job1()
{
  using namespace std::chrono_literals;

  /* Access the LED resource through the claim following a monitor pattern. */
  rtfm::srp::claim<Rled>([](auto &led){
    led.enable();
  });

  // Disable led in 200ms
  rtfm::srp::async<J2>(200ms);
}

/* Higher priority job */
void job2()
{
  using namespace std::chrono_literals;

  /* Access the LED resource through the claim following a monitor pattern. */
  rtfm::srp::claim<Rled>([](auto &led){
    led.disable();
  });

  // Enable led in 200ms
  rtfm::srp::async<J1>(200ms);
}

template <typename Resource>
using is_not_unique_resource = kvasir::mpl::bool_< !Resource::is_unique::value >;

template <typename Job>
struct job_to_unique_resource_impl
{
  using f = kvasir::mpl::remove_if< typename Job::resources,
                                    is_not_unique_resource >;

  //static_assert( kvasir::mpl::size< f >::value > 1,
  //    "A job may only have one unique resource in its resource claim.");
};

template <typename Job>
using job_to_unique_resource = typename job_to_unique_resource_impl<Job>::f;

using jobs_to_unique_resource = kvasir::mpl::flatten< kvasir::mpl::transform<rtfm::system_job_list,
                                                       job_to_unique_resource > >;

template <typename E1, typename E2>
using compare_func = kvasir::mpl::bool_<( !std::is_same<typename E1::id,
                                                        typename E2::id>::value )>;

template <typename E1, typename E2>
using compare_func2 = kvasir::mpl::bool_<( std::is_same<E1,
                                                        E2>::value )>;

using a = kvasir::mpl::list< int, char, float, double >;
using b = kvasir::mpl::rotate< a, 1 >;

template <template <typename...> class Pred, typename List>
using adjacent = typename kvasir::mpl::zip_with< Pred, List, kvasir::mpl::rotate<List, 1> >;

int main()
{
  using s = kvasir::mpl::sort<jobs_to_unique_resource, compare_func>;
  //s::g;
  //b::g;

  //using ad = adjacent<std::is_same, a>;
  using ad = kvasir::mpl::fold_right<adjacent<compare_func2, a>,
                                     kvasir::mpl::bool_<false>,
                                     kvasir::mpl::logical_or>;
  ad::g;

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
