//          Copyright Emil Fresk 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.md or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "kvasir/mpl/mpl.hpp"
#include "crect/details/job_resource_definitions.hpp"
#include "crect/details/job_resource_comparisons.hpp"
#include "crect/details/job_resource_transformations.hpp"
#include "crect/details/resource_tree.hpp"

namespace crect
{
namespace details
{

/**
 * @brief Finds a non-unique resource in the resource tree (implementation).
 *
 * @tparam  JobList   The system's job list.
 * @tparam  Resource  Resource to find.
 */
template <typename JobList, typename Resource>
struct find_resource_impl
{
  /* Searches the resource tree for the same Resource ID. */
  using f = kvasir::mpl::eager::find_if<resource_tree<JobList>,
                                    _same_resource_id<Resource>::template f >;

  static_assert(!std::is_same< f, kvasir::mpl::list<> >::value,
                "The resource in not registered in crect");

  static_assert(kvasir::mpl::eager::pop_front<f>::front::is_unique::value == false,
                "Found unique resource when normal was requested");
};

/**
 * @brief Finds a unique resource in the resource tree (implementation).
 *
 * @tparam  JobList   The system's job list.
 * @tparam  Resource  Resource to find.
 */
template <typename JobList, typename Resource>
struct find_unique_resource_impl
{
  /* Searches the resource tree for the same Resource ID. */
  using f = kvasir::mpl::eager::find_if<resource_tree<JobList>,
                                    _same_resource_id<Resource>::template f >;

  static_assert(!std::is_same< f, kvasir::mpl::list<> >::value,
                "The resource in not registered in crect");

  static_assert(kvasir::mpl::eager::pop_front<f>::front::is_unique::value == true,
                "Found normal resource when unique was requested");
};

/**
 * @brief Finds a non-unique resource in the resource tree (alias).
 *
 * @tparam  JobList   The system's job list.
 * @tparam Resource   Resource to find.
 */
template <typename JobList, typename Resource>
using find_resource = typename kvasir::mpl::eager::pop_front<
                        typename find_resource_impl<JobList, Resource>::f
                      >::front;

/**
 * @brief Finds a unique resource in the resource tree (alias).
 *
 * @tparam  JobList   The system's job list.
 * @tparam Resource   Resource to find.
 */
template <typename JobList, typename Resource>
using find_unique_resource = typename kvasir::mpl::eager::pop_front<
                        typename find_unique_resource_impl<JobList, Resource>::f
                      >::front;

} /* END namespace details */
} /* END namespace crect */
