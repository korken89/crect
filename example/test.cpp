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



int main()
{
  using namespace std;

  //using t = rtfm::details::resource_tree<rtfm::system_job_list>;
  //print_list<t>("tree");

  using j2r = rtfm::details::job_to_resource_impl< J1 >;
  cout << endl << "j2r: " << type_name<j2r>() << endl << endl;


  using trans = mpl::transform<rtfm::job_to_resource, rtfm::system_job_list >;
  cout << endl << "trans: " << type_name<trans>() << endl << endl;

  using rtree = rtfm::make_resource_tree< rtfm::system_job_list >::j2r;
   cout << endl << "rtree: " << type_name<rtree>() << endl << endl;
  // print_list<rtree>("rtree");
}
