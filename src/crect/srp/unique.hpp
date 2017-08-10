//          Copyright Emil Fresk 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.md or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include "kvasir/mpl/mpl.hpp"
#include "crect/srp/prioirty_ceiling.hpp"

namespace crect
{
namespace details
{

/**
 * @brief Checks if a resource does not have its unique flag set.
 *
 * @tparam  Resource  Resource to check.
 */
template <typename Resource>
using is_not_unique_resource = kvasir::mpl::bool_< !Resource::is_unique::value >;

/**
 * @brief Implementation to extract resources that have the unique flag set.
 *
 * @tparam  Job   Job to extract unique resources from.
 */
template <typename Job>
struct job_to_unique_resource_impl
{
  using f = kvasir::mpl::eager::remove_if< typename Job::resources,
                                    is_not_unique_resource >;

  static_assert( kvasir::mpl::eager::size< f >::value <= 1,
      "A job may only have one unique resource in its resource claim.");
};

/**
 * @brief Extract resources that have the unique flag set from a job.
 *
 * @tparam  Job   Job to extract unique resources from.
 */
template <typename Job>
using job_to_unique_resource = typename job_to_unique_resource_impl<Job>::f;

/**
 * @brief Takes a list of jobs and converts it to a list of corresponding
 *        unique resources.
 *
 * @tparam  JobList   Job to extract unique resources from.
 */
template <typename JobList>
using jobs_to_unique_resource = kvasir::mpl::eager::flatten<
    kvasir::mpl::eager::transform<
        JobList,
        job_to_unique_resource > >;

/**
 * @brief Takes a sorted resource list and checks if any unique resource is
 *        used more than once.
 *
 * @tparam  SortedResourceList    Sorted list of resources to check.
 */
template <typename SortedResourceList>
using is_unique_list = kvasir::mpl::eager::invert<
    kvasir::mpl::eager::fold_right<
        typename kvasir::mpl::eager::zip_with<
            std::is_same,
            SortedResourceList,
            kvasir::mpl::eager::rotate<SortedResourceList, 1> >,
        kvasir::mpl::bool_<false>,
        kvasir::mpl::eager::logical_or > >;

/**
 * @brief Takes a job list and converts it to sorted unique resources.
 *
 * @tparam  SortedResourceList    Sorted list of resources to check.
 */
template <typename JobList>
using jobs_to_unique_sorted = kvasir::mpl::eager::sort<
      details::jobs_to_unique_resource<JobList>,
      details::_different_resource_id_2r >;

} /* END namespace details */

/**
 * @brief Takes a list of jobs and checks if any unique resource is used more
 *        than once across and within jobs.
 *
 * @tparam  JobList    List of jobs to check.
 */
template <typename JobList>
using is_unique_job_list = kvasir::mpl::bool_<
  details::is_unique_list< details::jobs_to_unique_sorted<JobList> >::value ||
  (kvasir::mpl::eager::size< details::jobs_to_unique_sorted<JobList> >::value <= 1) >;
/**
 * @brief Takes a list of jobs and finds the corresponding job to the unique
 *        resource.
 *
 * @tparam  JobList    List of jobs search.
 * @tparam  Resource   Resource to find the job of.
 */
template <typename JobList, typename Resource>
using get_unique_job_from_resource = typename kvasir::mpl::eager::pop_front<
    typename details::find_unique_resource<JobList, Resource>::jobs
  >::front;


} /* END namespace crect */



