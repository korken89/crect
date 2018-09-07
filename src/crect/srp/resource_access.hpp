//          Copyright Emil Fresk 2017-2018.
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
 * @brief  A resource access wrapper to be able to check that jobs only take
 *         the resources they are allowed to.
 *
 * @tparam Job   The job to enforce resource access for.
 */
template < typename Job >
struct resource_access
{
  template < typename Resource >
  inline static auto claim()
  {
    using namespace kvasir;

    static_assert(
        kvasir::mpl::call< mpl::unpack<
                             mpl::any< details::_same_resource_id< Resource > >
                           >,
                           typename Job::resources >::value,
        "This job does not have the requested resource in its resource claim.");

    lock< Resource > lock{};

    return std::forward< Fun >(f)(Resource::as_object());
  }
};
}  // namespace crect
