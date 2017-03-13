
#pragma once

#include <type_traits>
#include "kvasir/mpl/mpl.hpp"
#include "rtfm/rtfm_utils.hpp"


namespace rtfm
{
namespace srp
{
namespace details
{
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
    /* Use the function traits to check the number of parameters, and that
     * arguments are passed by reference.
     */
    using rtype = rtfm::util::function_traits<Fun>;
    static_assert(typename rtype::n_args{} == 1,
                  "The claim lambda may only take one argument" );
    static_assert(std::is_reference< typename rtype::template arg<0> >::value,
                  "The claim lambda must take arguments by reference." );

    /* Pass by reference. */
    return f( *Object{} );
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
} /* END namespace details */

/**
 * @brief Will lock a resource and pass the resource's object by reference
 *        (if it has an object) to the specified lambda for safe usage.
 *
 * @details   Follows the monitor pattern idea.
 *
 * @tparam Resource   Resource to lock and access.
 * @tparam Fun        Function to evaluate.
 */
template <typename Resource, typename Fun>
constexpr auto claim(Fun &&f)
{
  using object = typename Resource::object;
  using has_object = typename Resource::has_object;


  rtfm::srp::lock<Resource> lock{};

  return details::claim_impl<has_object{}, Fun, object>{}(std::forward<Fun>(f));
}

} /* END namespace srp */
} /* END namespace rtfm */
