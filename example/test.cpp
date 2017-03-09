#include <functional>
#include <iostream>
#include "../../mpl_debug/print_types.hpp"
#include "kvasir/mpl/mpl.hpp"

template <typename... T>
struct GetIntegralType
{
};

template <typename T, T val>
struct GetIntegralType<kvasir::mpl::integral_constant<T, val>>
{
  using f = T;
};

template <typename ID_, typename OT, OT *Object, bool Unique, typename... Jobs>
struct Resource
{
  using ID = ID_;

  using object_type = OT;
  using object = kvasir::mpl::integral_constant<OT *, Object>;
  using has_object = kvasir::mpl::bool_<Object != nullptr>;
  using is_unique = kvasir::mpl::bool_<Unique>;
  using jobs = kvasir::mpl::flatten<kvasir::mpl::list<Jobs...>>;

  // static_assert(kvasir::mpl::is_integral<Object>{},
  //               "Object must be an integral constant.");

  static_assert(std::is_pointer<decltype(Object)>::value,
                "The type of the object must be a pointer.");
};

template <typename ID, typename OT, OT *Object>
using MakeResource = Resource<ID, OT, Object, false>;

template <typename ID, typename OT, OT *Object>
using MakeUniqueResource = Resource<ID, OT, Object, true>;

template <typename ID>
using MakeVirtualResource = Resource<ID, decltype(nullptr), nullptr, false>;

template <typename ID>
using MakeUniqueVirtualResource =
    Resource<ID, decltype(nullptr), nullptr, true>;

template <typename... Resources>
using MakeResourceAlias = kvasir::mpl::list<Resources...>;

template <bool HasObject, typename Fun, typename Object>
struct claim_impl
{
  auto operator()(Fun &&f) const noexcept
  {
    return f(std::ref(*Object{}));
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

template <typename Resource, typename Fun>
constexpr auto claim(Fun &&f)
{
  using obj = typename Resource::object;

  // rtfm::lock<Resource> lock;

  return claim_impl<(typename Resource::has_object){}, Fun, obj>{}(
      std::forward<Fun>(f));
}

int i = 0;

int main()
{
  using namespace std;

  using tr = MakeResource<int, decltype(i), &i>;
  using tu = MakeVirtualResource<float>;

  cout << "tu: " << type_name<tu>() << endl << endl;

  cout << "i is now 1: " << i << endl << endl;

  auto ret = claim<tr>([](int &j) {
    j++;
    cout << "Hello from lambda... j = " << j << endl << endl;

    return j + 100;
  });

  cout << "claim return: " << ret << endl << endl;
  cout << "i is now 2: " << i << endl << endl;
}
