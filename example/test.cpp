#include <functional>
#include <iostream>
#include "../../mpl_debug/print_types.hpp"
#include "kvasir/mpl/mpl.hpp"

template <typename... T>
struct GetIntegralType {};

template <typename T, T val>
struct GetIntegralType<kvasir::mpl::integral_constant<T, val>>
{
  using f = T;
};

template <typename ID_, typename Object, bool Unique, typename... Jobs>
struct Resource
{
  static_assert(kvasir::mpl::is_integral<Object>{},
                "Object must be an integral constant.");

  static_assert(std::is_pointer<typename GetIntegralType<Object>::f>::value,
                "The type of the object must be a pointer.");

  using ID = ID_;
  using type = ID_;
  using object = Object;
  using is_unique = kvasir::mpl::bool_<Unique>;
  using jobs = kvasir::mpl::flatten<kvasir::mpl::list<Jobs...>>;
};

template <typename ID, typename... Jobs>
using MakeVirtualResource =
    Resource<ID, kvasir::mpl::integral_constant<decltype(nullptr), nullptr>,
             false, Jobs...>;

template <typename ID, typename Object, typename... Jobs>
using MakeResource = Resource<ID, Object, false, Jobs...>;

template <typename ID, typename Object, typename... Jobs>
using MakeUniqueResource = Resource<ID, Object, true, Jobs...>;

template <typename... Resources>
using MakeResourceAlias = kvasir::mpl::list<Resources...>;

template <bool IsNullPtr, typename Fun, typename Object>
struct claim_impl
{
  auto operator()(Fun &&f) const noexcept
  {
    return f(std::ref(*Object{}));
  }
};

template <typename Fun, typename Object>
struct claim_impl<true, Fun, Object>
{
  auto operator()(Fun &&f) const noexcept
  {
    return f();
  }
};

template <typename Resource, typename Fun>
constexpr auto claim(Fun &&f)
{
  using obj = typename Resource::object;

  // rtfm::lock<Resource> lock;

  return claim_impl<obj{} == nullptr, Fun, obj>{}(std::forward<Fun>(f));
}

int i = 0;

int main()
{
  using namespace std;

  using tr =
      MakeResource<int, kvasir::mpl::integral_constant<decltype(&i), &i>>;

  cout << "i is now 1: " << i << endl << endl;

  auto ret = claim<tr>([](int &j) {
    j++;
    cout << "Hello from lambda... j = " << j << endl << endl;

    return j + 100;
  });

  cout << "claim return: " << ret << endl << endl;
  cout << "i is now 2: " << i << endl << endl;
}
