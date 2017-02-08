
#include <Eigen/Dense>
#include "rtfm/interrupts.hpp"
#include "rtfm/job_resource.hpp"
#include "rtfm/job_resource_methods.hpp"
#include "rtfm/srp/srp_locks.hpp"

#include "util/fake_hw.hpp"
//#include "util/print_types.hpp"
#include "util/string_hash.hpp"


using ISR1 = rtfm::MakeISR<&rtfm::DefaultISR::UnusedISR, 2>;
using ISR2 = rtfm::MakeISR<&rtfm::DefaultISR::UnusedISR, 1>;
using ISR3 = rtfm::MakeISR<&rtfm::DefaultISR::UnusedISR, 3>;

using R1 = rtfm::Resource<SPI1>;
using R2 = rtfm::Resource<SPI2>;
using R3 = rtfm::Resource<I2C1>;
using R4 = rtfm::Resource<I2C2>;

using J1 = rtfm::Job<1, 1, ISR1, R2, R1, R4>;
using J2 = rtfm::Job<2, 2, ISR2, R2, R3, R4>;
using J3 = rtfm::Job<3, 3, ISR3, R1, R3, R4>;



namespace rtfm
{
using system_job_list = brigand::list<J1, J2, J3>;
}

/* Needs to be included after the definition of the rtfm::system_job_list. */
#include "rtfm/srp/srp_prioirty_ceiling.hpp"

namespace rtfm
{
namespace srp
{

/**
 * @brief  A convenience definition of a lock in the SRP version of RTFM++.
 *
 * @tparam Resource   The resource to lock.
 */
template <typename Resource>
using lock = lock_impl< get_priority_ceiling< Resource > >;

}
}


/* TODO: Implement "get vector table" from job list. */





EIGEN_DONT_INLINE
double test_eigen(const double a[2])
{
  using namespace Eigen;

  const auto v = (Vector2d() << 2, 1).finished();
  Map<const Vector2d> m(a);
  //const Vector2f m = (Vector2f() << a[0], a[1]).finished();
  //const auto m = Vector2d(a[0], a[1]);
  double ret = v.dot( m.asDiagonal() * v );

  return ret;
}

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
