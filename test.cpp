
#include <Eigen/Dense>
#include "rtfm/interrupts.hpp"
#include "rtfm/job_resource.hpp"
#include "rtfm/locks.hpp"

#include "util/fake_hw.hpp"
//#include "util/print_types.hpp"
#include "util/string_hash.hpp"


using testISR = MakeISR<&DefaultISR::UnusedISR, 34>;

using R1 = Resource<SPI1>;
using R2 = Resource<SPI2>;
using R3 = Resource<I2C1>;
using R4 = Resource<I2C2>;

using J1 = Job<1, 1, testISR, R2, R1, R4>;
using J2 = Job<2, 2, testISR, R2, R3, R4>;
using J3 = Job<3, 3, testISR, R1, R3, R4>;

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
  rtfm::srp::lock< brigand::integral_constant<unsigned, 1> > lock;
  /* Lock */

  asm volatile("nop");
  asm volatile("nop");
  asm volatile("nop");
  asm volatile("nop");

  /* Automatic unlock via RAII */
}

int main()
{
  //using Jlist = brigand::list<J1, J2, J3>;
  //print_list<Jlist>("Jobs");

  //using Rft = make_resource_tree<Jlist>::result;
  //print_list<Rft>("Full transform");

  return 0;
};
