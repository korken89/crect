#include <initializer_list>

template <typename T>
struct _wrapper
{
};

struct code_printer
{
  template <typename T>
  void operator()(_wrapper<T>) const
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

void test()
{
  for_each<code_printer, int, int, int, int>();
}
