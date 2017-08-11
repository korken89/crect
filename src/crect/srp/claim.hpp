//          Copyright Emil Fresk 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.md or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <type_traits>
#include "kvasir/mpl/mpl.hpp"
#include "crect/utils.hpp"

namespace crect
{
/**
 * @brief Will lock a resource and pass the resource's object by reference
 *        (if it has an object) to the specified lambda for safe usage.
 *
 * @details   Follows the monitor pattern idea.
 *
 * @tparam Resource   Resource to lock and access.
 * @tparam Fun        Function to evaluate.
 */
template < typename Resource, typename Fun >
constexpr auto claim(Fun &&f)
{
  lock< Resource > lock{};

  return std::forward< Fun >(f)(Resource::as_object());
}

} /* END namespace crect */
