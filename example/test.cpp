#include <functional>
#include <iostream>
#include "../../mpl_debug/print_types.hpp"
#include "kvasir/mpl/mpl.hpp"

namespace details
{

/**
 * @brief Get the type of an mpl::integral_constant, base case
 *        that always will fail.
 *
 * @tparam Ts   Parameter pack.
 */
template <typename... Ts>
struct get_integral_type_impl
{
  static_assert(kvasir::mpl::always_false<Ts...>{},
                "The input is not an integral_constant.");
};

/**
 * @brief Get the type of an mpl::integral_constant, general case.
 *
 * @tparam T    Type of the integral_constant.
 * @tparam val  Value of the integral_constant.
 */
template <typename T, T val>
struct get_integral_type_impl<kvasir::mpl::integral_constant<T, val>>
{
  using f = T;
};

/**
 * @brief Get the type of an mpl::integral_constant, special case for nullptr.
 */
template <>
struct get_integral_type_impl<
    kvasir::mpl::integral_constant<decltype(nullptr), nullptr>>
{
  using f = decltype(nullptr);
};


/**
 * @brief Checks if an integral_constant contains a nullptr, base case that
 *        will always fail.
 *
 * @tparam Ts   Parameter pack.
 */
template <typename... Ts>
struct is_nullptr_impl
{
  static_assert(kvasir::mpl::always_false<Ts...>{},
                "The input is not an integral_constant.");
};

/**
 * @brief Checks if an integral_constant contains a nullptr, general case.
 *
 * @tparam T    Type of the integral_constant.
 * @tparam val  Value of the integral_constant.
 */
template <typename T, T val>
struct is_nullptr_impl<kvasir::mpl::integral_constant<T, val>>
{
  using f = kvasir::mpl::bool_<false>;
};

/**
 * @brief Checks if an integral_constant contains a nullptr, first special case.
 *
 * @tparam T    Type of the integral_constant.
 */
template <typename T>
struct is_nullptr_impl<kvasir::mpl::integral_constant<T, nullptr>>
{
  using f = kvasir::mpl::bool_<true>;
};

/**
 * @brief Checks if an integral_constant contains a nullptr, second special
 *        case.
 *
 * @tparam T    Type of the integral_constant.
 */
template <>
struct is_nullptr_impl<
    kvasir::mpl::integral_constant<decltype(nullptr), nullptr>>
{
  using f = kvasir::mpl::bool_<true>;
};



/**
 * @brief Selector functors for @p claim, based on if the pointer is nullptr or
 *        not, base case.
 *
 * @tparam HasObject  True if the Object contains an valid pointer.
 * @tparam Fun        Function to evaluate.
 * @tparam Object     Integral_constant containing the data pointer.
 */
template <bool HasObject, typename Fun, typename Object>
struct claim_impl;

/**
 * @brief Selector functors for @p claim, based on if the pointer is nullptr or
 *        not - Object contains valid pointer.
 *
 * @tparam Fun        Function to evaluate.
 * @tparam Object     Integral_constant containing the data pointer.
 */
template <typename Fun, typename Object>
struct claim_impl<true, Fun, Object>
{
  auto operator()(Fun &&f) const noexcept
  {
    return f( std::ref(*Object{}) ); // Pass by reference
  }
};

/**
 * @brief Selector functors for @p claim, based on if the pointer is nullptr or
 *        not - Object does not contain a valid pointer.
 *
 * @tparam Fun        Function to evaluate.
 * @tparam Object     Integral_constant containing the data pointer.
 */
template <typename Fun, typename Object>
struct claim_impl<false, Fun, Object>
{
  auto operator()(Fun &&f) const noexcept
  {
    return f();
  }
};
}

/**
 * @brief Get the type of an mpl::integral_constant.
 *
 * @tparam T  Integral_constant to get type from.
 */
template <typename T>
using get_integral_type = typename details::get_integral_type_impl<T>::f;

/**
 * @brief Checks if an integral_constant contains a nullptr.
 *
 * @tparam T    Integral_constant to check.
 */
template <typename T>
using is_nullptr = typename details::is_nullptr_impl<T>::f;

template <typename ID_, typename Object, bool Unique, typename... Jobs>
struct Resource
{
  static_assert(kvasir::mpl::is_integral<Object>{},
                "Object must be an integral constant.");

  static_assert((std::is_pointer<get_integral_type<Object>>::value ||
                 is_nullptr<Object>{}),
                "The type of the object must be a pointer.");

  using ID = ID_;
  using object = Object;
  using has_object = typename kvasir::mpl::bool_<!is_nullptr<Object>{}>;
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

  using nr1 =
      Resource<typename R1::ID, typename R1::object, typename R1::is_unique{}>;
  using nr2 =
      Resource<typename R2::ID, typename R2::object, typename R2::is_unique{}>;
  cout << endl << "R1 : " << type_name<R1>() << endl;
  cout << "nr1: " << type_name<nr1>() << endl;
  cout << "R2 : " << type_name<R2>() << endl;
  cout << "nr2: " << type_name<nr2>() << endl << endl;

  using i1 = kvasir::mpl::integral_constant<decltype(nullptr), nullptr>;
  using i2 = kvasir::mpl::integral_constant<decltype(nullptr), nullptr>;


  cout << "i1: " << type_name<std::is_same<i1, i2>::type>() << endl << endl;


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
