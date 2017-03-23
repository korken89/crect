
#pragma once

#include "kvasir/mpl/mpl.hpp"
#include "rtfm/details/job_resource.hpp"
#include "rtfm/details/job_resource_details.hpp"

namespace rtfm
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
  using f = kvasir::mpl::remove_if< typename Job::resources,
                                    is_not_unique_resource >;

  static_assert( kvasir::mpl::size< f >::value <= 1,
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
using jobs_to_unique_resource = kvasir::mpl::flatten<
    kvasir::mpl::transform<
        JobList,
        job_to_unique_resource > >;

/**
 * @brief Takes a sorted resource list and checks if any unique resource is
 *        used more than once.
 *
 * @tparam  SortedResourceList    Sorted list of resources to check.
 */
template <typename SortedResourceList>
using is_unique_list = kvasir::mpl::invert<
    kvasir::mpl::fold_right<
        typename kvasir::mpl::zip_with<
            std::is_same,
            SortedResourceList,
            kvasir::mpl::rotate<SortedResourceList, 1> >,
        kvasir::mpl::bool_<false>,
        kvasir::mpl::logical_or > >;

} /* END namespace details */

/**
 * @brief Takes a list of jobs and checks if any unique resource is used more
 *        than once across and within jobs.
 *
 * @tparam  JobList    List of jobs to check.
 */
template <typename JobList>
using is_unique_job_list = details::is_unique_list<
    kvasir::mpl::sort<
        details::jobs_to_unique_resource<JobList>,
        details::_different_resource_id_2r > >;

} /* END namespace rtfm */



