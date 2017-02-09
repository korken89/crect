
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


constexpr unsigned apply_test(unsigned i)
{
  return i + 16;
}

struct vector_table
{
  unsigned vectors[10];
};

template <typename T, T... Is>
constexpr vector_table table_test(std::integer_sequence<T, Is...>)
{
  return {{ apply_test(Is)... }};
  //return {{ apply_test(1), apply_test(2), apply_test(3) }};
}

template <unsigned N>
using make_indexs = std::make_integer_sequence<unsigned, N>;

__attribute__((used))
const constexpr vector_table system_vectors __attribute__((section("isr_vectors"))) =
  table_test(make_indexs<10>{});

int main()
{
  //print_list<rtfm::system_job_list>("System Jobs");

  //print_list<rtfm::details::resource_tree>("Resource tree");

  ////std::cout << "\nresult: " << type_name< Rft >() << std::endl;

  //using s = gen_seq<10>;
  //std::cout << "\nseq: " << type_name< s >() << std::endl;

  //using is = std::make_integer_sequence<unsigned, 10>;
  //std::cout << "\nint seq: " << type_name< is >() << std::endl;

  return 0;
}
