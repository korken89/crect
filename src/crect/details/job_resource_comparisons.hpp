//          Copyright Emil Fresk 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.md or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include "kvasir/mpl/mpl.hpp"
#include "crect/details/job_resource_definitions.hpp"

namespace crect
{
namespace details
{

/**************************************************************************/
/* Comparisons                                                            */
/**************************************************************************/

/**
 * @brief Compare two Job IDs.
 *
 * @tparam A   Left hand side.
 * @tparam B   Right hand side.
 */
template < typename A >
struct _same_job_id
{
  template < typename B >
  using f = std::is_same< typename A::isr, typename B::isr >;
};

/**
 * @brief Checks if resource ID is same.
 *
 * @tparam A   Left hand side.
 * @tparam B   Right hand side.
 */
template < typename A >
struct _same_resource_id
{
  template < typename B >
  using f = std::is_same< typename A::object, typename B::object >;
};

/**
 * @brief Checks if resource ID is different.
 *
 * @tparam A   Left hand side.
 * @tparam B   Right hand side.
 */
template < typename A >
struct _different_resource_id
{
  template < typename B >
  using f = kvasir::mpl::eager::invert<
      typename _same_resource_id< A >::template f< B > >;
};

/**
 * @brief Checks if resource ID is different.
 *
 * @tparam A   Left hand side.
 * @tparam B   Right hand side.
 */
template < typename A, typename B >
using _different_resource_id_2r =
    typename _different_resource_id< A >::template f< B >;

} /* END namespace details */
} /* END namespace crect */
