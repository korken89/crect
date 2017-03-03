#include <initializer_list>
#include <type_traits>
#include <iostream>
#include "../../mpl_debug/print_types.hpp"

//#include "rtfm/details/interrupts.hpp"
//#include "rtfm/details/job_resource_details.hpp"
#include "rtfm/details/job_resource_methods.hpp"

// template <typename T>
// struct _wrapper
// {
// };
//
// struct code_printer
// {
//   template <typename T>
//   void operator()(_wrapper<T>) const
//   {
//     asm volatile("nop");
//     *((volatile unsigned *)0x12345678) = 3;
//   }
// };
//
//
// template <typename Fun, typename... Ts>
// void for_each()
// {
//   std::initializer_list<int> {
//     ( Fun{}( _wrapper<Ts>{} ), 0)...
//   };
// }
//
// void test()
// {
//   for_each<code_printer, int, int, int, int>();
// }

struct sR1;
struct sR2;
struct sR3;

using R1 = rtfm::Resource< sR1 >;
using R2 = rtfm::Resource< sR2 >;
using R3 = rtfm::Resource< sR3 >;

void job1(void){

}
void job2(void){

}
void job3(void){

}

using J1 = rtfm::Job<
              rtfm::util::hashit("Job1"), // Unique ID
              8,                          // Priority
              rtfm::MakeISR<job1, 1>,     // ISR connection and location
              R1,                         // Possible resouce claims
              R2
            >;

using J2 = rtfm::Job<
              rtfm::util::hashit("Job2"), // Unique ID
              7,                          // Priority
              rtfm::MakeISR<job2, 2>,     // ISR connection and location
              R1,                         // Possible resouce claims
              R3
            >;

using J3 = rtfm::Job<
              rtfm::util::hashit("Job3"), // Unique ID
              6,                          // Priority
              rtfm::MakeISR<job3, 3>,     // ISR connection and location
              R3                         // Possible resouce claims
            >;


namespace rtfm
{


using system_job_list = kvasir::mpl::list<J3, J1, J2>;


} /* END namespace rtfm */

//#include "rtfm/srp/srp_prioirty_ceiling.hpp"
using namespace kvasir;


/**
 * @brief Compare two Job IDs.
 *
 * @tparam A   Left hand side.
 * @tparam B   Right hand side.
 */
template <typename A>
struct _same_job_id {
  template <typename B>
  using f = std::is_same<typename A::UID, typename B::UID>;
};

/**
 * @brief Checks if Resource ID is same.
 *
 * @tparam A   Left hand side.
 * @tparam B   Right hand side.
 */
template <typename A>
struct _same_resource_id {
  template <typename B>
  using f = std::is_same<typename A::ID, typename B::ID>;
};

/**
 * @brief Checks if Resource ID is different.
 *
 * @tparam A   Left hand side.
 * @tparam B   Right hand side.
 */
template <typename A>
struct _different_resource_id {
  template <typename B>
  using f = kvasir::mpl::invert< std::is_same<typename A::ID, typename B::ID> >;
};






template <typename... Ts>
struct merge_resources_impl
{
  static_assert(rtfm::util::always_false<Ts...>::value,
                "Merging different resources are not allowed");
};

/**
 * @brief Merges resources of same ID.
 *
 * @tparam ID    Type of the ID.
 * @tparam Jobs1 Left hand side pack of jobs.
 * @tparam Job     Right hand side last job.
 */
template <typename ID, typename... Jobs1, typename Job>
struct merge_resources_impl< rtfm::Resource<ID, Jobs1...>, rtfm::Resource<ID, Job> >
{
  using f = rtfm::Resource<ID, Jobs1..., Job>;
};


/**
 * @brief Work alias that merges resources of same ID.
 *
 * @tparam Ts    Parameters to be forwarded to merge_resources_impl.
 */
template <typename... Ts>
using merge_resources = typename merge_resources_impl<Ts...>::f;



/**
 * @brief Creates a list of resources where each job associated do a resource
 *        is bundled with it.
 *
 * @tparam ResList   List of resources.
 */
template <typename ResList>
struct make_resource_tree_impl
{

  using _current =  kvasir::mpl::pop_front< kvasir::mpl::remove_if<
                      _same_resource_id<
                        typename kvasir::mpl::pop_front<ResList>::front
                      >::template f,
                      ResList
                    > >;

  using _next =     kvasir::mpl::remove_if<
                      _different_resource_id<
                        typename kvasir::mpl::pop_front<ResList>::front
                      >::template f,
                      ResList
                    >;

  using f = kvasir::mpl::flatten< kvasir::mpl::list< kvasir::mpl::fold_right<
                    merge_resources,
                    typename _current::front,
                    typename _current::rest
                  >,
                  typename make_resource_tree_impl< _next >::f
            > >;
};

/**
 * @brief Creates a list of resources where each job associated do a resource
 *        is bundled with it.
 *
 * @note  End of recursion.
 */
template <>
struct make_resource_tree_impl< kvasir::mpl::list<> >
{
  using f = kvasir::mpl::list<>;
};


/**
 * @brief Interface to generate the resource tree.
 *
 * @tparam JobList   List of Jobs.
 */
template <typename JobList>
struct make_resource_tree
{
  /* Take all the job<resources> and transform to resource<jobs> */
  using j2r = kvasir::mpl::flatten<
                kvasir::mpl::transform<
                  rtfm::job_to_resource,
                  kvasir::mpl::flatten< JobList >
                > >;


  using result = typename make_resource_tree_impl<j2r>::f;
};

template <typename T>
using comp = std::is_same<int, T>;

// struct asfasd{};
//
// template <typename T, T& a>
// struct ptest
// {
//     using ic = kvasir::mpl::integral_constant<T&, a>;
// };
//
// asfasd tt{};

int main()
{
  using namespace std;

  // using tst = ptest<asfasd, tt>;
  // cout << "tst: " << type_name<tst::ic>() << endl << endl;

  using an = mpl::any< comp, mpl::list<int, char, float> >;

  static_assert(bool(an{}), "noes");
  //cout << "an: " << bool(an{}) << endl << endl;

  //cout << "an: " << type_name<an>() << endl << endl;

  using sr = _same_resource_id<R1>::f<R1>;
  cout << "sr: " << type_name<sr>() << endl << endl;



  using rtree = make_resource_tree< rtfm::system_job_list >;
  //cout << endl << "rtree::j2r: " << type_name<rtree>() << endl << endl;
  print_list<rtree::j2r>("rtree::j2r");
  print_list<rtree::result>("rtree::result");

}
