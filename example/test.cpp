#include "brigand/brigand.hpp"
#include <initializer_list>

template <typename T>
struct _wrapper
{
};

struct code_printer
{
  template <typename T>
  void operator()(_wrapper<T>)
  {
    asm volatile("nop");
    *((volatile unsigned *)0x12345678) = 3;
  }
};


template <typename Fun, typename... Ts>
void for_each()
{
  std::initializer_list<int> {
    ( Fun{}( _wrapper<Ts>{} ), 0)...
  };
}

using t = brigand::list<int, int, int, int>;

void test()
{
  for_each<code_printer, int, int, int, int>();
}
