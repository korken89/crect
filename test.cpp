
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

template <int I>
struct get_vector_from_position : brigand::integral_constant<int, I>
{
  //return reinterpret_cast<void (*)(void)>(2 + I);
  //static I;
};

struct vector_table
{
  const int vectors[10];
};

template <typename T, T... Is>
const constexpr vector_table table_test(std::integer_sequence<T, Is...>)
{
  return {{ get_vector_from_position<Is>::value... }};
  //return {{ apply_test(1), apply_test(2), apply_test(3) }};
}

template <unsigned N>
using make_indexs = std::make_integer_sequence<unsigned, N>;

__attribute__(( used, section(".isr_vectors") ))
const constexpr vector_table system_vectors = table_test(make_indexs<10>{});

int main()
{
  //print_list<rtfm::system_job_list>("System Jobs");

  //print_list<rtfm::details::resource_tree>("Resource tree");

  //std::cout << "\nresult: " << type_name< Rft >() << std::endl;

  return 0;
}
