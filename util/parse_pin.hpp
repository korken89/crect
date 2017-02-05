
#pragma once

#include <brigand/brigand.hpp>

namespace details
{
template <int V, int I, int... Is>
struct ints_to_val_impl
    : brigand::integral_constant<int, ints_to_val_impl<V * 10 + I, Is...>::value>
{
};

template <int V, int I>
struct ints_to_val_impl<V, I> : brigand::integral_constant<int, V * 10 + I>
{
};

template <int... Is>
struct ints_to_val
    : brigand::integral_constant<int, ints_to_val_impl<0, Is...>::value>
{
};

template <typename C>
struct ic_to_int : brigand::integral_constant<int, C::value - '0'>
{
  static_assert(C::value >= '0' && C::value <= '9', "Not a number");
};

template <typename... Ts>
struct parse_pin
{
};

template <template <typename...> class Container, typename... Ts1,
          typename... Ts2>
struct parse_pin<Container<Ts1...>, Container<Ts2...>>
{
  using port =
      brigand::integral_constant<int, ints_to_val<ic_to_int<Ts1>::value...>::value>;
  using pin =
      brigand::integral_constant<int, ints_to_val<ic_to_int<Ts2>::value...>::value>;
};
}

template <char... C>
constexpr auto operator"" _pin()
{
  /* Create a list of the characters and define the delimiter character. */
  using str = brigand::integral_list<int, C...>;
  using del = brigand::integral_constant<int, '.'>;

  /* Check so the format is correct. */
  using cf = brigand::count_if<str, std::is_same<brigand::_1, del>>;
  static_assert(cf::value == 1, "The pin syntax is [port].[pin]_pin");

  using bl = brigand::split<str, del>;
  static_assert(brigand::size<bl>::value == 2,
                "The pin syntax is [port].[pin]_pin");

  /* Extract and parse the port and pin number. */
  using h = brigand::front<bl>;
  using t = brigand::back<bl>;
  using parsed = details::parse_pin<h, t>;

  return parsed{};
}
