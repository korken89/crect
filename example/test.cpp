#include <functional>
#include <iostream>
#include "../../mpl_debug/print_types.hpp"
#include "kvasir/mpl/mpl.hpp"
#include "rtfm/rtfm_utils.hpp"


template <typename Fun>
constexpr auto forward_function(Fun &&f)
{
  using namespace std;
  using rtype = rtfm::util::function_traits<Fun>;

  static_assert(typename rtype::n_args{} == 1,
                "The lambda may only take one argument" );

  using argtype = typename rtype::template arg<0>;

  static_assert(std::is_reference<argtype>::value,
                "The lambda may take arguments by reference." );

  cout << "Function's type:           " << type_name< rtype >() << "\n";
  cout << "Function's no. arguments:  " << typename rtype::n_args{} << "\n";
  cout << "Function's first argument: " << type_name< argtype >() << "\n";
}

int i = 0;

#define link_object(obj)    kvasir::mpl::integral_constant<decltype(obj), (obj)>

template <typename Fun>
struct get_type
{
  using f = decltype(&Fun::operator());
};

int main()
{
  using namespace std;

  forward_function([](int &i) { return i + 3; });

  cout << "link_object: " << type_name< link_object(&i) >() << "\n";
}
