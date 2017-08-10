//          Copyright Emil Fresk 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.md or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include "kvasir/mpl/mpl.hpp"
#include "crect/details/job_resource_definitions.hpp"
#include "crect/details/job_resource_comparisons.hpp"
#include "crect/details/job_resource_transformations.hpp"

namespace crect
{
namespace details
{

/**************************************************************************/
/* Resource tree generation (impl)                                        */
/**************************************************************************/

/**
 * @brief Creates a list of resources where each job associated to a resource
 *        is bundled with it.
 *
 * @tparam ResList   List of resources.
 */
template < typename ResList >
struct make_resource_tree_impl
{
  using _current = kvasir::mpl::eager::pop_front< kvasir::mpl::eager::remove_if<
      ResList, _different_resource_id< typename kvasir::mpl::eager::pop_front<
                   ResList >::front >::template f > >;

  using _next = kvasir::mpl::eager::remove_if<
      ResList, _same_resource_id< typename kvasir::mpl::eager::pop_front<
                   ResList >::front >::template f >;

  using f = kvasir::mpl::eager::join<
      kvasir::mpl::list< kvasir::mpl::eager::fold_right<
          typename _current::rest, typename _current::front,
          merge_resources > >,
      typename make_resource_tree_impl< _next >::f >;
};

/**
 * @brief Creates a list of resources where each job associated do a resource
 *        is bundled with it.
 *
 * @note  End of recursion.
 */
template <>
struct make_resource_tree_impl< kvasir::mpl::list<> >
{
  using f = kvasir::mpl::list<>;
};

} /* END namespace details */


/**
 * @brief Interface to generate the resource tree.
 *
 * @tparam JobList   List of Jobs.
 */
template <typename JobList>
struct make_resource_tree
{
  /* Take all the job<resources> and transform to resource<jobs> */
  using j2r = kvasir::mpl::eager::flatten<
                kvasir::mpl::eager::transform<
                  JobList,
                  job_to_resource
                >
              >;

  using result = typename details::make_resource_tree_impl<j2r>::f;
};

/**
 * @brief Alias for the entire system's resource tree.
 *
 * @tparam  JobList   The system's job list.
 */
template <typename JobList>
using resource_tree = typename make_resource_tree<JobList>::result;


} /* END namespace crect */
