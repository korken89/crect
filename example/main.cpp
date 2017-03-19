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

  static_assert( kvasir::mpl::size< f >::value <= 1,
      "A job may only have one unique resource in its resource claim.");
};

template <typename Job>
using job_to_unique_resource = typename job_to_unique_resource_impl<Job>::f;


template <typename JobList>
using jobs_to_unique_resource = kvasir::mpl::flatten<
    kvasir::mpl::transform<
        JobList,
        job_to_unique_resource > >;



template <typename SortedList>
using is_unique_list = kvasir::mpl::invert<
    kvasir::mpl::fold_right<
        typename kvasir::mpl::zip_with<
            std::is_same,
            SortedList,
            kvasir::mpl::rotate<SortedList, 1> >,
        kvasir::mpl::bool_<false>,
        kvasir::mpl::logical_or > >;

template <typename R1, typename R2>
using different_resource_id = kvasir::mpl::bool_<( !std::is_same<typename R1::id,
                                                        typename R2::id>::value )>;

template <typename JobList>
using is_unique_job_list = is_unique_list<
    kvasir::mpl::sort<
        jobs_to_unique_resource<JobList>,
        different_resource_id > >;


int main()
{
  //using ad = is_unique_list<a>;
  using ad = is_unique_job_list<rtfm::system_job_list>;
  //using ad = jobs_to_unique_resource< rtfm::system_job_list >;
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
