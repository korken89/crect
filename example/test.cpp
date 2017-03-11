#include <functional>
#include <iostream>
#include "../../mpl_debug/print_types.hpp"
#include "kvasir/mpl/mpl.hpp"

namespace details
{
template <typename... T>
struct GetIntegralType_impl;

template <typename T, T val>
struct GetIntegralType_impl<kvasir::mpl::integral_constant<T, val>>
{
  using f = T;
};

template <>
struct GetIntegralType_impl<
    kvasir::mpl::integral_constant<decltype(nullptr), nullptr>>
{
  using f = decltype(nullptr);
};



template <typename... T>
struct IsNullPtr_impl;

template <typename T, T val>
struct IsNullPtr_impl<kvasir::mpl::integral_constant<T, val>>
{
  using f = kvasir::mpl::bool_<false>;
};

template <typename T>
struct IsNullPtr_impl<kvasir::mpl::integral_constant<T, nullptr>>
{
  using f = kvasir::mpl::bool_<true>;
};

template <>
struct IsNullPtr_impl<
    kvasir::mpl::integral_constant<decltype(nullptr), nullptr>>
{
  using f = kvasir::mpl::bool_<true>;
};



template <bool HasObject, typename Fun, typename Object>
struct claim_impl;

template <typename Fun, typename Object>
struct claim_impl<true, Fun, Object>
{
  auto operator()(Fun &&f) const noexcept
  {
    return f( std::ref(*Object{}) ); // Pass by reference
  }
};

template <typename Fun, typename Object>
struct claim_impl<false, Fun, Object>
{
  auto operator()(Fun &&f) const noexcept
  {
    return f();
  }
};
}

template <typename T>
using GetIntegralType = typename details::GetIntegralType_impl<T>::f;

template <typename T>
using IsNullPtr = typename details::IsNullPtr_impl<T>::f;

template <typename ID_, typename Object, bool Unique, typename... Jobs>
struct Resource
{
  static_assert(kvasir::mpl::is_integral<Object>{},
                "Object must be an integral constant.");

  static_assert((std::is_pointer<GetIntegralType<Object>>::value ||
                 IsNullPtr<Object>{}),
                "The type of the object must be a pointer.");

  using ID = ID_;
  using object = Object;
  using has_object = typename kvasir::mpl::bool_<!IsNullPtr<Object>{}>;
  using is_unique = kvasir::mpl::bool_<Unique>;
  using jobs = kvasir::mpl::flatten<kvasir::mpl::list<Jobs...>>;
};



template <typename Resource, typename Fun>
constexpr auto claim(Fun &&f)
{
  using object = typename Resource::object;
  using has_object = typename Resource::has_object;

  // rtfm::lock<Resource> lock;

  return details::claim_impl<has_object{}, Fun, object>{}(std::forward<Fun>(f));
}



template <typename ID, typename Object, typename... Jobs>
using MakeResource = Resource<ID, Object, false, Jobs...>;

template <typename ID, typename Object, typename... Jobs>
using MakeUniqueResource = Resource<ID, Object, true, Jobs...>;

template <typename ID, typename... Jobs>
using MakeVirtualResource =
    Resource<ID, kvasir::mpl::integral_constant<decltype(nullptr), nullptr>,
             false, Jobs...>;

template <typename ID, typename... Jobs>
using MakeUniqueVirtualResource =
    Resource<ID, kvasir::mpl::integral_constant<decltype(nullptr), nullptr>,
             true, Jobs...>;

template <typename... Resources>
using MakeResourceAlias = kvasir::mpl::list<Resources...>;


int i = 0;

int main()
{
  using namespace std;

  using R1 =
      MakeResource<int, kvasir::mpl::integral_constant<decltype(&i), &i>>;
  using R2 = MakeVirtualResource<float>;

  using nr =
      Resource<typename R1::ID, typename R1::object, typename R1::is_unique{}>;
  cout << endl << "R1: " << type_name<R1>() << endl;
  cout << "nr: " << type_name<nr>() << endl << endl;


  cout << "i is now 1: " << i << endl;

  auto ret = claim<R1>([](int &j) {
    // int j = 0;
    j++;
    cout << "Hello from claimed resource lambda... j = " << j << endl;

    return j + 100;
  });

  cout << "claim return: " << ret << endl;
  cout << "i is now 2: " << i << endl << endl;

  auto ret2 = claim<R2>([]() {
    int j = 100;
    j++;
    cout << "Hello from virtual resource lambda... j = " << j << endl;

    return j + 100;
  });

  cout << "claim return: " << ret2 << endl;
  cout << "i is now 3: " << i << endl << endl;
}
