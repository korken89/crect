

#include <iostream>
#include "../../mpl_debug/print_types.hpp"
#include "kvasir/mpl/mpl.hpp"

template <typename ID_, typename T, T *Object,
          bool Unique, typename... Jobs>
struct Resource
{
  using ID = ID_;
  using type = ID_;
  using object = kvasir::mpl::integral_constant<T *, Object>;
  using is_unique = kvasir::mpl::bool_<Unique>;
  using jobs = kvasir::mpl::flatten<kvasir::mpl::list<Jobs...>>;
};

template <typename ID, typename... Jobs>
using MakeResource = Resource<ID, void, nullptr, false, Jobs...>;

template <typename ID, typename T, T *Object, typename... Jobs>
using MakeLinkedResource = Resource<ID, T, Object, false, Jobs...>;

template <bool IsNullPtr, typename Fun, typename Object>
struct claim_impl
{
  auto operator()(Fun f) const
  {
    return f( *Object{} );
  }
};

template <typename Fun, typename Object>
struct claim_impl<true, Fun, Object>
{
  auto operator()(Fun f) const
  {
    return f( );
  }
};

template <typename Resource, typename Fun>
constexpr auto claim(Fun f)
{
  using obj = typename Resource::object;

  return claim_impl<obj{} == nullptr, Fun, obj>{}(f);
}

int i = 23;

int main()
{
  using namespace std;

  using tr = MakeLinkedResource<int, decltype(i), &i>;

  auto ret = claim<tr>([](auto &i) {
    i++;
    cout << "Hello from lambda... i = " << i << endl << endl;

    return i + 1;
  });

  cout << "ret type: " << type_name<decltype(ret)>() << endl << endl;
  cout << "claim return: " << ret << endl << endl;
  cout << "i is now 3: " << i << endl << endl;
}
